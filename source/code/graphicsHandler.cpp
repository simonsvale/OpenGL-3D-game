#include <string>
#include <fstream>

#include <glad/glad.h>

#include "graphicsHandler.h"

using namespace std;




void GraphicsHandler::LoadShader(const char *Shader, string FilePath)
{
    string ShaderFileLine;

    // Open File
    ifstream ReadSpriteFile(FilePath);

    // extract file content
    while(getline(ReadSpriteFile, ShaderFileLine))
    {   

    }

    // Close file
    ReadSpriteFile.close();
}

// WIP
void GraphicsHandler::SetVAO()
{
    // Initialize VAO (Vertex Array Object)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
}


void GraphicsHandler::SetVBO(float Vertecies[])
{
    // Initialize VBO (Vertex Buffer Object)
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Bind the VBO to an OpenGL array Buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data that the VBO should contain
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Vertecies), Vertecies, GL_STATIC_DRAW); // x * sizeof(Vertex_list), where x is the total amount of floats, and vertex_list is a list containing them.

}


void GraphicsHandler::SetEBO(float indices[])
{
    // Initialize EBO (Element Buffer Object)
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // EBO's can be used to make complex structures from vertecies, here we use the indicies list to store a square, created from 2 triangles.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}



