# Split Impulse Interpenetration Resolution

## Overview

This documents the Split Impulse positional correction implementation for `CollisionHandler::resolveInterpenetrations`. The approach separates positional correction (penetration removal) from velocity correction (impulse response), so that `applyImpulses()` and `resolveInterpenetrations()` do not interfere with each other.

---

## Config fields to add to `CollisionHandler.hxx`

```cpp
inline static int   splitImpulseIterations       = 5;
inline static float splitImpulseCorrectionFactor = 0.4f;
```

---

## `resolveInterpenetrations` implementation

```cpp
void CollisionHandler::resolveInterpenetrations(utl::svec<SceneObject>& sceneObjects) {
	// Split Impulse positional correction:
	// A pseudo-impulse j_s corrects positions (X) and orientations (R) of both bodies
	// directly without touching the real momenta P or L.
	//
	// Key design choices:
	//  - Scalar penetration depth = dot(depth_vector, normal): projects the depth onto
	//    the contact normal so that terrain contacts (where the depth vector is a vertical
	//    lift, not necessarily parallel to the sloped normal) are handled correctly.
	//  - Static bodies (invMass == 0) are fully excluded from both the effective-mass
	//    denominator and the correction step.  The base SceneObject leaves Iinv as the
	//    identity matrix even for terrain, so blindly including it would corrupt the
	//    denominator and rotate immovable objects.
	//  - splitImpulseCorrectionFactor (beta < 1) applies only a fraction of the required
	//    correction per pass, preventing large one-shot teleportation when bisection
	//    leaves residual deep penetrations.  The iterative outer loop converges the rest.
	//  - Contacts are re-detected each iteration so stale depth values are not reused.

	for (int iter = 0; iter < splitImpulseIterations; ++iter) {
		utl::vec<CONTACT> contacts;
		resolveCollisions(contacts, sceneObjects);

		bool anyCorrection = false;

		for (const auto& contact : contacts) {
			const auto& contactData = contact.get<1>();
			if (!contactData.depth.has_value()) {
				continue;
			}

			// Project depth onto the contact normal to get the true scalar penetration.
			// This is robust for terrain where depth = (surface_point - contact_point)
			// is a vertical vector while the normal is slope-perpendicular.
			float depth = glm::dot(*contactData.depth, contactData.normal);
			if (depth <= depthBias) {
				continue;
			}

			anyCorrection = true;

			SceneObject* objA = contact.get<2>();
			SceneObject* objB = contact.get<3>();

			ribo::BodyData* A = &objA->getPhysicsSolver()->Body;
			ribo::BodyData* B = &objB->getPhysicsSolver()->Body;

			glm::vec3 n  = contactData.normal;
			glm::vec3 ra = contactData.point - A->X;
			glm::vec3 rb = contactData.point - B->X;

			// Only dynamic bodies (invMass > 0) contribute angular terms to the
			// effective mass.  Static bodies (terrain, fixed boxes) have invMass == 0
			// but may have a non-zero Iinv due to the default BodyData initialisation,
			// so they must be explicitly excluded here.
			float angular_a = 0.0f;
			if (A->invMass > 0.0f) {
				glm::vec3 term_an = A->Iinv * glm::cross(ra, n);
				angular_a = glm::dot(n, glm::cross(term_an, ra));
			}
			float angular_b = 0.0f;
			if (B->invMass > 0.0f) {
				glm::vec3 term_bn = B->Iinv * glm::cross(rb, n);
				angular_b = glm::dot(n, glm::cross(term_bn, rb));
			}

			float denom = A->invMass + B->invMass + angular_a + angular_b;
			if (denom < 1e-8f) {
				continue;
			}

			// Apply only a fraction of the required correction per iteration to avoid
			// one-shot overcorrection when residual penetration is large.
			float     j_s     = splitImpulseCorrectionFactor * depth / denom;
			glm::vec3 impulse = j_s * n;

			// Positional correction for A (moves along +n, away from B)
			if (A->invMass > 0.0f) {
				A->X += A->invMass * impulse;
				glm::vec3 deltaOmegaA = A->Iinv * glm::cross(ra, impulse);
				A->R += ribo::star(deltaOmegaA) * A->R;
				A->R  = ribo::orthonormalize(A->R);
				objA->updateTransformations();
			}

			// Positional correction for B (moves along -n, away from A)
			if (B->invMass > 0.0f) {
				B->X -= B->invMass * impulse;
				glm::vec3 deltaOmegaB = B->Iinv * glm::cross(rb, impulse);
				B->R -= ribo::star(deltaOmegaB) * B->R;
				B->R  = ribo::orthonormalize(B->R);
				objB->updateTransformations();
			}
		}

		if (!anyCorrection) {
			break;
		}
	}
}
```

---

## Why the previous implementation caused teleportation

Three root causes were identified by inspecting `CuboidCollider.cxx`, `TerrainCollider.cxx`, `SphereCollider.cxx`, `RigidBody.hxx/cxx`, and `SceneObject.cxx`:

### 1 — `Iinv` is non-zero for terrain even though it is static

The base `SceneObject` constructor initialises `Iinv = mat3(1)` (identity) in `BodyData`. Only `Box` sets `Ibodyinv = mat3(0)` for stationary instances; terrain scene objects never override this. Because `updateVelocities` recomputes `Iinv = R * Ibodyinv * R^T`, and terrain's `Ibodyinv` stays as identity, terrain ends up with full rotational inertia. This corrupted the denominator (too small → impulse too large) and caused the terrain itself to visibly rotate each frame.

**Fix:** Every angular contribution to the denominator and every angular/linear correction is gated on `invMass > 0`.

### 2 — `glm::length(depth)` gives the wrong scalar for sloped terrain

For terrain, `depth = terrainSurfacePoint − contactPoint` is a **vertical** lift vector. On flat ground it is parallel to `(0,1,0)` and the contact normal also happens to be `(0,1,0)`, so `length(depth)` coincidentally equals the penetration along the normal. On a slope the two directions diverge, so `length(depth)` over-estimates the true penetration along the normal and the correction overshoots.

For cuboid vertex-face contacts, `depth = -normal * faceDot`, so `length(depth) == dot(depth, normal)` — both formulas happen to be equivalent there.

**Fix:** Scalar depth is computed as `dot(depth_vector, contact_normal)`, which is the component of the depth vector along the separation axis regardless of collider type.

### 3 — 100% correction in a single pass against large residual penetration

When bisection reaches its iteration limit it falls back to the current-frame state, which can have significant penetration. Applying the full `depth / denom` in one shot moved the object by the entire gap at once, producing a visible teleport.

**Fix:** A Baumgarte-style factor `splitImpulseCorrectionFactor` (default `0.4`) scales down the impulse each pass. The iterative loop (up to `splitImpulseIterations`, default `5`) converges on the solution gradually.

---

## Tuning guide

| Parameter | Default | Effect |
|---|---|---|
| `splitImpulseIterations` | `5` | More iterations → tighter convergence per frame at higher cost. Increase for stacking scenarios. |
| `splitImpulseCorrectionFactor` | `0.4` | Lower → smoother but slower correction. Higher → faster but risks overshoot on deep penetrations. Range `(0, 1]`. |
| `depthBias` | `0.000001` | Contacts shallower than this are ignored. Raise slightly if micro-jitter occurs at rest. |
