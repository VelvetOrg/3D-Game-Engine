# 3D-Game
> This is a C++ based 3D game. Currently the idea and gamplay of this project is yet to be decided, but some features have already been implemented including:

1. Input management - Mouse and keyboard including modifiers and all key states
2. Sound management - 3D and 2D audio. Including audio listeners, SFX and music
3. Simple graphics - Drawing an array of vertices
4. Shaders - Support for any type of GLSL shader
5. OBJ File Loading - For custom 3D models
6. Camera management
7. GameObjects
8. UV Mapping
9. Textures
10. A Math class
11. Perlin Noise - For procedural generation
12. Physics
13. Collision Detection 
14. Rigidbodies

---

The following dependencies are used:

1. `GLFW` - For creating the window and handling OS-specific operations
2. `GLEW` - The OpenGL Extension Wrangler
3. `OpenGL` - Allows for drawing of graphics via the GPU
4. `AmBIERA irrKlang` - For 3D and 2D audio managment
5. `SOIL` - For loading images as textures
6. `libnoise` - For perlin noise
7. `Bullet` - For physics, collision detection and rigidbodies

---

This project's goals to be implemented:

- [x] Input manager
- [x] Sound system with AmBIERA irrKlang
- [x] Vertex based graphics support
- [x] Shader loader and compiler
- [x] Loading of OBJ files
- [ ] Loading of FBX files
- [x] Camera support
- [x] Rotation, projection and movement matricies
- [x] Support for multiple unique GameObjects
- [x] Bullet's integrated physics for collision detection
- [x] RigidBodies
- [ ] Simple lighting
- [ ] Lightmaps
- [x] UV Mapping
- [x] Textures
- [ ] Shadow Mapping
- [ ] Instanced rendering
- [x] Math class
- [x] Perlin noise integration
