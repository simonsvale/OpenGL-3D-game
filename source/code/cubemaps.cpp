#include "cubemaps.h"



void create_reflection_framebuffer(GLuint *FrameBufferObject, GLuint *Texture)
{
    unsigned int CUBEMAP_RES_W, CUBEMAP_RES_H = 1024;

    // Generate the framebuffer and the cubemap texture.
    glGenFramebuffers(1, FrameBufferObject);
    glGenTextures(1, Texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, *Texture);

    // Assign a 2d texture to each side of the cubemap.
    for (unsigned int i = 0; i < 6;)
    {
        // This is possible due to GL_TEXTURE_CUBE_MAP_POSITIVE_X's hex value being 1 int from every other side.
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Bind the framebuffer and cubemap texture.
    glBindFramebuffer(GL_FRAMEBUFFER, *FrameBufferObject);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *Texture, 0);

    // Unbind the framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}