#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <string>
#include <array>
#include <glad/glad.h>
#include <iostream>

#include <STB/stb_image.h>

using namespace std;

class Graphics
{
    public:

        GLuint VBO;     // Vertex Buffer Object
        GLuint VAO;     // Vertex Array Object
        GLuint EBO;      // Element Buffer Object

        // Constructor (Basic graphics setup for now)
        Graphics(array<float, 32> Vertecies, array<unsigned int, 6> indices)
        {   
            SetVBO(Vertecies);

            SetVAO();

            SetEBO(indices);
        }

        void SetVBO(array<float, 32> Vertecies);
        void SetVAO();
        void SetEBO(array<unsigned int, 6> indices);

        void LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath);


};



#endif