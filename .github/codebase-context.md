# Codebase Context — LightGraphicsLibraryEngine (LGL)

A C++23 OpenGL graphics and rigid-body physics engine built with CMake and MSVC.
Target platform: Windows x64, MSVC (Visual Studio 2022 / vc143).

---

## Repository Layout

```
LglEngine/
├── dep/                    # Third-party dependency headers + prebuilt libs
│   └── alldeps/            # glew, glfw, glm, imgui, assimp, stb, xxhash, eigen, osqp
├── src/
│   ├── mod/                # C++23 module interface units (.ixx)
│   ├── util/               # Low-level OpenGL / GLFW / math wrappers
│   │   ├── gl/             # GPU object wrappers (VAO, VBO, FBO, Shader, Texture)
│   │   ├── glfw/           # Input and timing (keyin, Mouse, Time)
│   │   ├── math/           # Camera, Transformation, RigidBody, Quaternion, Dnum
│   │   └── model/          # Assimp mesh loader
│   ├── core/               # Engine-core entities and rendering primitives
│   │   ├── sceneobject/    # SceneObject hierarchy (Box, Sphere, Sun, Terrain)
│   │   ├── collision/      # Collider hierarchy + ContactData + TerrainData
│   │   └── visual/         # Mesh hierarchy, Material, Light, Rect
│   └── eng/eng/            # High-level engine (WindowManager, Scene, UserInterface, CollisionHandler)
├── lgl_demo/               # Concrete application (SimulationScene, SponzaScene, their UIs, main)
├── res/                    # Runtime assets (shaders, textures, models, imgui.ini)
└── .github/
    └── codebase-context.md # This file
```

---

## Build Targets & Dependency Chain

```
Demo  (executable — lgl_demo/)
 └── LglEng   (static lib — src/eng/)
      └── LglCore  (static lib — src/core/)
           └── LglUtil  (static lib — src/util/)
                └── LglMod   (static lib — src/mod/, C++23 module wrappers)
                     └── LglDeps  (static lib — dep/, third-party)
```

All targets use **C++23**, `/permissive-` strict conformance, and MSVC `/W4`.

---

## Third-party Dependencies (LglDeps)

| Library       | Purpose                                          |
|---------------|--------------------------------------------------|
| GLEW (static) | OpenGL extension loading                         |
| GLFW          | Window creation, OpenGL context, raw input       |
| GLM           | GLSL-compatible math (vec2/3/4, mat3/4)          |
| ImGui         | Immediate-mode debug/editor UI                   |
| Assimp        | 3D model file import                             |
| stb_image     | Image/texture decoding                           |
| xxhash64      | Fast hashing for shader hot-reload detection     |
| Eigen         | Dense and sparse linear algebra                  |
| OSQP          | Quadratic programming (resting contact solver)   |
| OsqpEigen     | Eigen-friendly OSQP wrapper                      |
| QDLDL         | Direct linear solver (OSQP dependency)           |

Compile definitions required: `GLEW_STATIC`, `OSQP_EIGEN_OSQP_IS_V1`, `OSQP_EIGEN_OSQP_IS_V1_FINAL`.

---

## C++23 Module Wrappers (LglMod — `src/mod/*.ixx`)

Third-party headers are not included directly across the engine. Instead they are
wrapped in named module interface units and consumed via `import`:

| Module        | Wraps / exports                                  |
|---------------|--------------------------------------------------|
| `lglmodule`   | `lgl::utl::*` typedefs, `lgl::Logger`, helpers   |
| `glmmodule`   | `glm::vec*`, `glm::mat*`, math functions         |
| `asimmodule`  | Assimp scene types                               |
| `qpmodule`    | OSQP quadratic programming interface             |

Every engine source file starts with one or more of:
```cpp
import lglmodule;
import glmmodule;
```
followed by `#include` of local `.hxx` headers.

---

## `lgl::utl` — Type Alias Conventions (`lglmodule.ixx`)

All STL types are aliased inside `lgl::utl`. Use these aliases throughout the
engine; never use the raw STL names in engine code.

| Alias                  | Underlying type                              |
|------------------------|----------------------------------------------|
| `utl::vec<T>`          | `std::vector<T>`                             |
| `utl::svec<T>`         | `std::vector<std::shared_ptr<T>>`            |
| `utl::smap<T>`         | `std::unordered_map<std::string, sptr<T>>`   |
| `utl::umap<U,V>`       | `std::unordered_map<U,V>`                    |
| `utl::imap<T>`         | `std::unordered_map<int,T>`                  |
| `utl::set<T>`          | `std::unordered_set<T>`                      |
| `utl::arr<T,N>`        | `std::array<T,N>`                            |
| `utl::opt<T>`          | `std::optional<T>`                           |
| `utl::tup<Args...>`    | Wrapper around `std::tuple<Args...>`         |
| `utl::sptr<T>`         | `std::shared_ptr<T>`                         |
| `utl::uptr<T>`         | `std::unique_ptr<T>`                         |
| `utl::wptr<T>`         | `std::weak_ptr<T>`                           |
| `utl::str`             | `std::string`                                |
| `utl::cstr`            | `const char*`                                |
| `utl::uint`            | `std::uint32_t`                              |
| `utl::ull`             | `std::uint64_t`                              |
| `utl::int32`           | `std::int32_t`                               |
| `utl::byte`            | `unsigned char`                              |
| `utl::except`          | `std::exception`                             |
| `utl::runtime`         | `std::runtime_error`                         |

Smart pointer factories with compile-time `static_assert` diagnostics:
- `utl::makeSptr<T>(args...)` — wraps `std::make_shared`
- `utl::makeUptr<T>(args...)` — wraps `std::make_unique`
- `utl::sptrCast<U>(sptr<V>)` — checked `dynamic_pointer_cast`, throws on failure

---

## Naming & File Conventions

| Element              | Convention                                              |
|----------------------|---------------------------------------------------------|
| Classes / structs    | `PascalCase`                                            |
| Member variables     | `m_camelCase` prefix                                    |
| Static members       | `s_camelCase` prefix (e.g. `Time::s_deltaTime`)         |
| Methods              | `camelCase`                                             |
| Namespaces           | `lowercase` (e.g. `lgl`, `utl`, `ribo`, `keyin`, `tex`)|
| Header files         | `.hxx`                                                  |
| Implementation files | `.cxx`                                                  |
| Module interface units | `.ixx`                                                |
| Template impl in headers | yes — templates are fully defined in `.hxx`       |
| Include guards       | `#pragma once` (no `#ifndef` guards)                    |

---

## Architecture Overview

### Layer Responsibilities

#### `LglUtil` — GPU & Math Primitives
- **`gl/GpuProgram`** — compiles and links GLSL shaders; uses xxhash64 to detect
  source changes for hot-reload; exposes typed uniform setters (`setVec3`, `setMat4`, etc.).
- **`gl/VertexArray` / `VertexBuffer` / `VertexBufferLayout`** — thin RAII wrappers
  around OpenGL VAO/VBO; `VertexArray::addBuffer()` binds a layout to a buffer.
- **`gl/Texture`** — 2D texture object loaded via stb_image.
- **`gl/FrameBuffer`** — FBO + RBO for offscreen rendering (shadow/depth maps).
  Auto-assigns texture unit IDs via a static counter.
- **`gl/DebugDrawer`** — fully-static immediate-mode line/point renderer used by
  the collision system to visualise contacts and colliders at runtime.
- **`glfw/keyin`** — typed event system: `KeyEvent<C,A>` binds a member-function
  pointer and a value argument to a GLFW key; `KeyEventHandler<C,A>` holds a list
  and fires them each frame.
- **`math/Camera`** — perspective + orthographic projection; WASD + mouse-drag
  control via `Quaternion`; exposes `getView()` / `getProjection()`.
- **`math/Transformation`** — stores T, R, S as separate `glm::mat4`; final model
  matrix is T·R·S.
- **`math/ribo::PhysicsSolver`** — holds `BodyData` (X, R, P, L, vel, omega,
  force, torque) and an Euler integrator (`updateState(dt)`). Keeps `Initial` and
  `Previous` snapshots for rollback.

#### `LglCore` — Scene Entities & Rendering Abstractions

- **`SceneObject`** — base entity. Owns one each of `Mesh`, `Collider`,
  `PhysicsSolver`, and `Transformation` via `sptr<>`. Key virtual interface:
  ```cpp
  virtual void draw(const Camera&) const;
  virtual void handleLighting(const Light&, const FrameBuffer&, const glm::mat4&);
  virtual void stepPhysicsBy(float dt);
  virtual ribo::BodyData initializePhysics();
  virtual void resetBodyState();
  virtual utl::sptr<SceneObject> clone();
  ```
  Concrete types: `Box`, `Sphere`, `Sun`, `Terrain`.

- **`Mesh`** (abstract) — owns `VertexArray`, `VertexBuffer`, `VertexBufferLayout`,
  `GpuProgram`, `Material`, `Transformation`, and a texture list. Subclasses
  implement `create()` and `draw(const Camera&)`.
  Concrete types: `CuboidMesh`, `SphereMesh`, `Surface`, `PatchSurface`, `ModelMesh`.

- **`Collider`** (abstract) — double-dispatch collision detection via the visitor
  pattern. `collidesWith(const Collider&)` dispatches to the correct
  `collidesWith(const XxxCollider&)` override. Returns `vec<ContactData>`.
  Concrete types: `CuboidCollider`, `SphereCollider`, `TerrainCollider`, `NullCollider`.

- **`Material`** — plain struct: `ambient`, `diffuse`, `specular`, `shininess`.
- **`Light`** — plain struct: `powerDensity`, `position`, `direction`, `isDirectional`.

#### `LglEng` — High-level Engine

- **`WindowManager`** — creates the GLFW window, initialises GLEW and OpenGL state,
  owns `sptr<Scene>` and `sptr<UserInterface>`. The template method
  `defineLayout<SceneT, UIT>()` is the single injection point for application code.
  The main loop lives in `renderWindowLoop()`:
  1. `Time::calculateDeltaTime()`
  2. `glfwPollEvents()`
  3. `scene->update(window)`
  4. `glClear` + `scene->draw()`
  5. `renderUserInterface()` (ImGui panel on the right portion of the window)
  6. `glfwSwapBuffers()`

- **`Scene`** (abstract) — owns all GPU resources (programs, textures, materials,
  meshes, framebuffers) in `smap<>` tables, plus the `svec<SceneObject>` world and
  the `Camera`. Abstract interface: `create()`, `draw() const`, `update(GLFWwindow*)`.

- **`UserInterface`** (abstract) — wraps ImGui; renders the standard panels
  (object picker, primitive picker, FPS). Override `renderAdditionalUI()` to add
  application-specific panels.

- **`CollisionHandler`** — fully-static class. Called once per frame with the scene
  object list. Pipeline:
  1. `calculateContacts()` — pairwise collider dispatch.
  2. `getContactType()` — classifies each contact as COLLIDING, RESTING, or SEPARATING.
  3. `applyImpulses()` — impulse-based response for colliding contacts.
  4. `resolveRestingContacts()` — builds and solves a QP (OSQP) for resting contact forces.
  5. Optional bisection for time-of-impact refinement.
  6. `debugDrawCollisions()` — forwards to `DebugDrawer`.

#### `lgl_demo` — Application Layer

Concrete subclasses of `Scene` and `UserInterface` wired together through
`WindowManager::defineLayout<>()`.

- **`SimulationScene`** — rigid-body physics sandbox with terrain, sun (shadow map),
  and dynamic boxes/spheres.
- **`SponzaScene`** — static Sponza model showcase with lighting controls.
- **`SimulationInterface`** / **`LightingInterface`** — ImGui panels for their
  respective scenes; both downcast the base `sptr<Scene>` to access scene-specific API.

---

## Key Design Patterns

- **Template layout injection** — `WindowManager::defineLayout<S,U>()` decouples the
  engine from any concrete scene or UI. This is the only place application types
  enter the engine.
- **Double-dispatch collision** — `Collider` uses the visitor pattern so each collider
  pair has a dedicated narrowphase implementation without runtime type switches in
  calling code.
- **Shared ownership for components** — `Mesh`, `Collider`, `PhysicsSolver`, and
  `Transformation` are held as `sptr<>` so objects can share or clone components
  independently.
- **`KeyEventHandler<C,A>` typed callbacks** — input bindings are typed member-function
  pointers with a fixed argument; no string-keyed action maps or virtual dispatch.
- **xxhash shader hot-reload** — `GpuProgram::handleShaderSourceChange()` re-hashes
  source files each frame and recompiles only on change.
- **`smap<T>` resource tables** — all GPU resources in `Scene` are stored in
  string-keyed shared-pointer maps for easy lookup and lifetime management.

---

## Known Architectural Limitations

The following are recognised design debts, **do not work around them silently** —
address them explicitly or document the trade-off:

1. **`CollisionHandler` and `DebugDrawer` are global static singletons.** All state
   is `inline static`. There can only be one physics world and one debug draw context
   per process. Multithreaded use is unsafe.

2. **`Scene` is a God Object.** It conflates GPU resource management, world state,
   camera ownership, and input-event registration. A future split into
   `ResourceCache` + `Scene` + `RenderContext` is desirable.

3. **`SceneObject` is monolithic.** Every entity carries a mesh, collider, physics
   solver, and transformation regardless of whether it needs them (e.g. `Sun` needs
   none of the physics slots). A component/composition model is the long-term direction.

4. **`Scene::update(GLFWwindow*)` couples the abstract interface to GLFW.** Replacing
   the parameter with an `InputState` value type would make `Scene` backend-agnostic.

5. **`UserInterface` downcasts `Scene` to the concrete subtype.** `SimulationInterface`
   stores a `sptr<SimulationScene>` cast from the base `sptr<Scene>`. The base
   contract is effectively ignored. Templating `UserInterface` on the scene type, or
   adding an observer interface on `Scene`, is the preferred fix.

6. **Render passes are hand-coded per scene.** `castShadowsOnTerrain()` in
   `SimulationScene` manually swaps programs, binds framebuffers, and resets camera
   matrices. A `RenderPass` abstraction would make shadow, depth-prepass, and
   post-process passes composable and reusable.

7. **Object selection state (`m_objectIndex`) lives in `Scene`.** This is a UI
   concern that has leaked into the simulation layer.

---

## Threading Improvement Opportunities

No multithreading is currently used anywhere in the engine. The following locations
have been identified as viable candidates, ordered by expected impact.

### 1. `CollisionHandler::resolveCollisions` — highest priority

The O(N²) pair loop processes every `(s1, s2)` object combination sequentially.
Each pair test is a **pure read** — no pair shares mutable state with any other
during contact detection. All pairs can be enumerated upfront into a flat list and
dispatched with `std::for_each(std::execution::par_unseq, ...)`, writing results
into per-pair vectors and merging afterwards. The `contactIndex` counter must move
to a post-merge pass to stay correct.

**Constraint**: the downstream pipeline (`applyImpulses`, `resolveRestingContacts`)
must remain on the main thread and still runs sequentially after the parallel gather.

### 2. `GpuProgram::handleShaderSourceChange` — file I/O off the main thread

Up to five shader source files are read and hashed sequentially every frame.
The reads are fully independent and can be launched with `std::async(std::launch::async, ...)`,
one future per shader type. Hash comparison and the decision to recompile happen
after all futures are collected.

**Hard constraint**: `createProgram()` (and any `glXxx` call) must remain on the
main OpenGL thread — only the file reads and hash computation can be offloaded.

### 3. `CuboidCollider::edgeEdgeCollision` — medium priority

The nested loop runs 144 independent `calculateEdgeIntersection` calls per colliding
pair (12 edges × 12 edges). Each call is a pure function with no side effects.
The outer loop can be parallelised with `std::execution::par_unseq` and results
written into pre-allocated slots to avoid synchronisation overhead.

This is only worth the overhead when many pairs are actively colliding simultaneously;
for sparse scenes the thread management cost will dominate.

### 4. `CuboidCollider::updateTransformations` — subdivision loop

The subdivision point transform loop (`transData.subdivision[i] = vec3(vec4(...) * M)`)
is a trivially parallel map over an array of independent matrix-vector multiplications.
It is worth parallelising only when `subdivideInto` is large enough that the array
has hundreds of entries. `std::execution::par_unseq` on the index range is the
appropriate mechanism.

### What must stay serial

| Location | Reason |
|---|---|
| Bisection loop in `calculateContactsWithBisection` | Each iteration depends on the previous result — inherently sequential |
| `applyImpulses` while loop | Each impulse mutates shared `BodyData` read by the next contact in the same iteration |
| `resolveRestingContacts` QP solve | Single linear algebra solve; not decomposable |
| `calculateSideIntersection` / `vertexFaceCollision` inner loop | Only 48 dot-product checks per call; thread overhead exceeds arithmetic cost |

### Global constraint

`CollisionHandler` and `DebugDrawer` use `inline static` state (see Known
Architectural Limitations §1). Any parallelism introduced inside the handler must
be scoped to the contact-detection phase only, before results are written back to
the shared static vectors (`currentContacts`, `restingContacts`).
