#ifndef CUBEMAPS_H
#define CUBEMAPS_H

#include <glad/glad.h>
#include <SDL2/SDL.h>

class Cubemap
{
    public:
        GLuint Texture;
        GLuint FBO;

        void create_reflection_framebuffer(GLuint *FrameBufferObject, GLuint *Texture);



};



#endif