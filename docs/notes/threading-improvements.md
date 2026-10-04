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

`CollisionHandler` and `DebugDrawer` use `inline static` state (see
architectural-limitations.md §1). Any parallelism introduced inside the handler must
be scoped to the contact-detection phase only, before results are written back to
the shared static vectors (`currentContacts`, `restingContacts`).
