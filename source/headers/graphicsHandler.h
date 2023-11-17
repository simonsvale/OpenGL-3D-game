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
        GLuint EBO;

        // Since C style arrays decays to pointers, we need to pass the size in too.
        void SetVBO(float Vertices[], int VertSize);
        void SetVAO();

        // For creating EBO from indices.
        void SetEBO(unsigned int indices[], int IndiSize);

        void LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath);

};



#endif