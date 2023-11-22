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

        GLuint IBO;
        GLuint VBO;

        // Since C style arrays decays to pointers, we need to pass the size in too.
        void SetVBO(float Vertices[], int VertSize);
        void SetVAO(int VertSize, int NormalSize, int TextCoSize);

        void SetLightVAO(int VertSize);

        void SetVBOSubData(float Vertices[], int VertSize, float Normals[], int NormalSize, float TextureCoords[], int TextCoSize, unsigned int Indices[], int IndiSize);

        // For creating EBO from indices.
        void SetEBO(unsigned int Indices[], int IndiSize);

        void LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath);

};



#endif