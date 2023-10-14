#include <string>
#include <array>
#include <iostream>

#include <glad/glad.h>

#include "graphicsHandler.h"

using namespace std;

// WIP
void Graphics::SetVBO(array<float, 32> Vertecies)
{
    // Initialize VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);

    // Bind the VBO to an OpenGL array Buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data that the VBO should contain
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Vertecies), &Vertecies[0], GL_STATIC_DRAW); // x * sizeof(Vertex_list), where x is the total amount of floats, and vertex_list is a list containing them.
}


void Graphics::SetVAO()
{
    // Initialize VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // All of these should be parameters:
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
}


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



    // ------------------------------- The rest should brob. be new mathod, in the shader class.

    // Needs to be a parameter "tex0"
    GLuint tex0Uni = glGetUniformLocation(*ShaderProgramPtr, "tex0");
    glLinkProgram(*ShaderProgramPtr);

    glUniform1i(tex0Uni, 0);
}
