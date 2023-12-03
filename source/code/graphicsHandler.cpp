#include <string>
#include <array>
#include <iostream>

#include <glad/glad.h>

#include "graphicsHandler.h"

using namespace std;

// WIP
void Graphics::SetVBO(float Vertices[], int VertSize)
{
    GLuint VBO;
    // Initialize VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);

    // Bind the VBO to an OpenGL array Buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data that the VBO should contain
    // @Note: Parameter 2 is the Size of array * reference to the array that have decayed into a pointer i.e. &Vertecies[0]
    glBufferData(GL_ARRAY_BUFFER, VertSize*sizeof( float ), Vertices, GL_STATIC_DRAW);
}

void Graphics::SetVBOSubData(float Vertices[], int VertSize, float Normals[], int NormalSize, float TextureCoords[], int TextCoSize, unsigned int Indices[], int IndiSize)
{
    // Initialize VBO and IBO.
    glGenBuffers(1, &VBO);

    // Bind the VBO to an OpenGL array Buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, VertSize*sizeof(float) + NormalSize*sizeof(float) + TextCoSize*sizeof(float), 0, GL_STATIC_DRAW);

    // Buffer subdata.
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertSize*sizeof(float), Vertices);
    glBufferSubData(GL_ARRAY_BUFFER, VertSize*sizeof(float), NormalSize*sizeof(float), Normals);
    glBufferSubData(GL_ARRAY_BUFFER, VertSize*sizeof(float) + NormalSize*sizeof(float), TextCoSize*sizeof(float), TextureCoords);

    // IBO
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiSize*sizeof(unsigned int), Indices, GL_STATIC_DRAW);
}


void Graphics::SetVAO(int VertSize, int NormalSize, int TextCoSize)
{
    // Initialize VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    glEnableVertexAttribArray(0);

    // VAO Normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(VertSize*sizeof(float)) );
    glEnableVertexAttribArray(1);

    // VAO Texture coords attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(VertSize*sizeof(float) + NormalSize*sizeof(float)) );
    glEnableVertexAttribArray(2);
}

void Graphics::SetFBO()
{

    // Create the frame buffer object.
    glGenFramebuffers(1, &FBO);

    // create depth texture
    unsigned int depthCubemap;

    glGenTextures(1, &depthCubemap);
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    // Assign the depthmap texture to all 6 faces of the cubemap.
    for (unsigned int i = 0; i < 6;)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 
        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  


    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void Graphics::SetLightVAO(int VertSize, int NormalSize)
{   
    // Generate a VAO used for lighting.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind to object vao.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // pass vbo

    // Set attribute pointer for normals
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(VertSize*sizeof(float)) );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(VertSize*sizeof(float) + NormalSize*sizeof(float)) );
    glEnableVertexAttribArray(2);
}


// Deprecated (again)
void Graphics::SetEBO(unsigned int Indices[], int IndiSize)
{
    // Initialize EBO (Element Buffer Object)
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // EBO's can be used to make complex structures from vertecies, here we use the indicies list to store a square, created from 2 triangles.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiSize*sizeof( unsigned int ), Indices, GL_STATIC_DRAW); // DrawFlag
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
    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glLinkProgram(*ShaderProgramPtr);
}
