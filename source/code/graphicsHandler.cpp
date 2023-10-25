#include <string>
#include <array>
#include <iostream>

#include <glad/glad.h>

#include "graphicsHandler.h"

using namespace std;

// WIP
void Graphics::SetVBO(float Vertecies[], int VertSize)
{
    // Initialize VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);

    // Bind the VBO to an OpenGL array Buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data that the VBO should contain
    // @Note: Parameter 2 is the Size of array * reference to the array that have decayed into a pointer i.e. &Vertecies[0]
    glBufferData(GL_ARRAY_BUFFER, VertSize*sizeof(&Vertecies[0]), Vertecies, GL_STATIC_DRAW);
}


void Graphics::SetVAO()
{
    // Initialize VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Tell OpenGL, how the buffer data, from the VBO is structured.

    // Tell OpenGL, that the first 3 indexes of a row is vertex positions, give it the ID 0 and enable it.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL, that the next 3 indexes of a row is RGB colors, give it the ID 1 and enable it.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Tell OpenGL, that the next 2 indexes of a row is the texture mapping, give it the ID 2 and enable it.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
}

// Deprecated
void Graphics::SetEBO(array<unsigned int, 6> indices)
{
    // Initialize EBO (Element Buffer Object)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // EBO's can be used to make complex structures from vertecies, here we use the indicies list to store a square, created from 2 triangles.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
}


// Currently a working, but unfinished function, may need to take more parameters.
void Graphics::LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath)
{
    // Set textures:
    int widthImg, heightImg, numColCh;

    // Should be placed in an init program function/method.
    stbi_set_flip_vertically_on_load(true);

    unsigned char* bytes = stbi_load(TexturePath, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, Texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);


    // ------------------------------- The rest should probably be new method, in the shader class.

    // Needs to be a parameter "tex0"
    GLuint tex0Uni = glGetUniformLocation(*ShaderProgramPtr, "tex0");
    glLinkProgram(*ShaderProgramPtr);

    glUniform1i(tex0Uni, 0);
}
