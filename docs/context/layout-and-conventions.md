# Lgl layout and conventions

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

## Naming and File Conventions

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

## Build Targets and Dependency Chain

```
Demo  (executable — lgl_demo/)
 └── LglEng   (static lib — src/eng/)
      └── LglCore  (static lib — src/core/)
           └── LglUtil  (static lib — src/util/)
                └── LglMod   (static lib — src/mod/, C++23 module wrappers)
                     └── LglDeps  (static lib — dep/, third-party)
```

All targets use **C++23**, `/permissive-` strict conformance, and MSVC `/W4`.

## Build target descriptions

### LglDeps
LglDeps is the static dependency bundle that aggregates the project’s external libraries and platform support. It links GLFW, GLEW, Assimp, ImGui, stb, Eigen, OSQP, and related toolchain libraries, while exporting the required OpenGL and physics compile definitions. This target is the lowest-level foundation for the rest of the engine and is linked transitively by all higher-level libraries.

### LglMod
LglMod is the C++23 module layer that exposes reusable project interfaces as importable modules rather than raw headers. Its module units cover utility helpers, GLM access, Assimp integration, and quadratic-programming support, providing a modernized shared API for the engine. It sits above the dependency layer and packages foundational functionality into a cleaner compile-time interface.

### LglUtil
LglUtil is the low-level utility library that implements OpenGL wrappers, timing/input helpers, math primitives, physics integrator and model loading. It contains GPU resource abstractions such as textures, buffers, framebuffers, and shaders, along with GLFW utilities for keyboard, mouse, and delta-time management. This layer provides the reusable runtime building blocks that both rendering and simulation code depend on.

### LglCore
LglCore is the engine’s content layer, where actual scene objects, colliders, and visual primitives are defined. It includes box and terrain objects, physics colliders, meshes, materials, lights, and other world entities that represent the simulation domain. This target is responsible for the concrete data and behavior of the engine’s world objects.

### LglEng
LglEng is the orchestration layer that ties together scenes, windows, UI, collision handling, and resource management. It defines the engine’s high-level runtime systems, such as Scene, UserInterface, WindowManager, CollisionHandler, and ResourceRepository. This target is the main library consumers use to build and run a full application on top of the lower-level components.

### Demo
Demo is the executable sample application that demonstrates the engine in action. It assembles the simulation scene, resource repository, and user interface into a runnable program and links the full engine stack together. This target is the project’s primary end-to-end validation and showcase for rendering, interaction, and rigid-body simulation.

---