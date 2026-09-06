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
