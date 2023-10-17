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

        // Constructor (Basic graphics setup for now)
        Graphics(float Vertecies[], int VertSize)
        {   
            // Set Vertex Buffer object of the given vertecies.
            SetVBO(Vertecies, VertSize);

            // Set the Vertex Array object, (configuration of how openGL elements are structured.)
            SetVAO();

            //SetEBO(indices);
        }

        // Since C style arrays decays to pointers, we need to pass the size in too.
        void SetVBO(float Vertecies[], int VertSize);
        void SetVAO();

        void LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath);


        // Deprecated for 3D graphics.
        GLuint EBO;      // Element Buffer Object
        void SetEBO(array<unsigned int, 6> indices);
};



#endif