# 3D-Game
> This is a C++ based 3D game. Currently the idea and gamplay of this project is yet to be decided, but some basic features have already been implemented including:

1. Input management - Mouse and keyboard including modifiers and all key states
2. Sound management - 3D and 2D audio. Including audio listeners, SFX and music
3. Simple graphics - Drawing an array of vertices
4. Shaders - Support for any type of GLSL shader

---

The following dependencies are used:

1. `GLFW` - For creating the window and handling OS-specific operations
2. `GLEW` - The OpenGL Extension Wrangler
3. `OpenGL` - Allows for drawing of graphics via the GPU
4. `AmBIERA irrKlang` - For 3D and 2D audio managment
5. `SOIL` - For loading images as textures
6. `libnoise` - For perlin noise

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
- [ ] Bullet's integrated physics for collision detection
- [ ] RigidBodies
- [ ] Simple lighting
- [ ] Lightmaps
- [x] UV Mapping
- [x] Textures
- [ ] Shadow Mapping
- [ ] Instancing
- [x] Math class
- [x] Perlin noise integration
