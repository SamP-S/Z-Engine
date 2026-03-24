# Zen Architecture

## Layered Structure

### Layer 1: Core (zen::core)
- Memory management
- Containers & data structures
- Math (vectors, matrices, quaternions)
- UUID generation
- Timer & time management
- Logging interface
- Linear algebra interface

### Layer 2: Modules (zen::modules::<module_name>)
- Graphics: Rendering, shaders, textures, meshes
- Audio: Sound playback, mixing, spatial audio
- Input: Keyboard, mouse, gamepad, touch
- Window: Cross-platform window management
- Physics: Collision detection & response
- Networking: Client-server communication

### Layer 3: Engine (zen::engine::<system>)
- Runtime: Game loop systems
- Editor: Content creation tools
- DevTools: Asset pipeline, compilers
- Shared: Engine-only utilities

### Layer 4: Projects
- Sandbox: Testing environment
- Sample Game: Complete example
- User Project Template: Starter template
