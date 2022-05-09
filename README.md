# Quickrender
An idiot-proof object-oriented abstraction layer for OpenGL.

I found that OpenGL can be a mite clunky to work with, so I sought to make myself a simple 3D renderer with the API. 
I designed it with the intention of it being as simple as possible to import and use. 
Most code here is adapted from LearnOpenGL.com, with optimizations wherever I thought were necessary.
Not recommended for production use in its current state. I barely even have textures working, and much of the architecture of the renderer still needs to be laid out.

# Usage
Place quickrender.h in your project.
Requires GLFW3, GLAD, and GLM's folders to be pointed to directly in the linker.
This renderer can be configured in quickrender.h's QR_SETTINGS preprocessor block.
