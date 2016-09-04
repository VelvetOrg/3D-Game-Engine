# 3D-Game
> This is a C++ based 3D game. Currently the idea and gamplay of this project is yet to be decided, but some basic features have already been implemented including:

1. Input management - Mouse and keyboard including modifiers and all key states
2. Sound management - 3D and 2D audio. Including audio listeners, SFX and music
3. Simple graphics - Drawing an array of vertices
4. Shaders - Support for any type of GLSL shader

---

To do this the following dependencies are used:

1. `GLFW` - For creating the window and handling OS-specific operations
2. `GLEW` - The OpenGL Extension Wrangler
3. `OpenGL` - Allows for drawing of graphics via the GPU
4. `OpenAL` - For 3D audio managment
5. `libogg` - Upacking .ogg file
6. `libvorbis` - Decoder of .ogg files

---

This project's goals to be implemented:

- [x] Input manager
- [ ] Sound system with Open AL
- [x] Vertex based graphics support
- [x] Shader loader and compiler
- [x] Loading of model files
- [x] Camera support
- [x] Rotation, projection and movement matricies
- [ ] Textures
- [ ] Shadow mapping
