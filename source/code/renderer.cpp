#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "STB/stb_image.h"

#include <iostream>
#include <string>
#include <fstream>

#include "renderer.h"

using namespace std;


void Renderer::RenderEverything()
{

}


void Renderer::LoadShader(const char *Shader, string FilePath)
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

// Currently a working, but unfinished function, may need to take more parameters.
void Renderer::LoadTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath)
{
    // Set textures:
    int widthImg, heightImg, numColCh;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(TexturePath, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &*Texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(*ShaderProgramPtr, "tex0");
    glLinkProgram(*ShaderProgramPtr);

    glUniform1i(tex0Uni, 0);

    return;
}










