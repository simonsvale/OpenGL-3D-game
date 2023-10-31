#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <string>
#include <vector>

#include <glad/glad.h>
#include <iostream>

#include <STB/stb_image.h>

using namespace std;

class Graphics
{
    public:

        GLuint VAO;     // Vertex Array Object

        // Since C style arrays decays to pointers, we need to pass the size in too.
        void SetVBO(float Vertecies[], int VertSize);
        void SetVAO();

        void LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath);

};



#endif