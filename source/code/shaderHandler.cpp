#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "shaderHandler.h"

void Shader::LoadShaderFile(const char *Shader, string FilePath)
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


void Shader::CreateFragmentShader(const char *ShaderCode)
{
    // Tell the variable that it is a fragment shader.
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Give the fragment shader a pointer to the GLSL code.
    glShaderSource(FragmentShader, 1, &ShaderCode, NULL);
    glCompileShader(FragmentShader);
}


void Shader::CreateVertexShader(const char *ShaderCode)
{
    // Tell the variable that it is a vertex shader.
    VertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Give the vertex shader a pointer to the GLSL code.
    glShaderSource(VertexShader, 1, &ShaderCode, NULL);
    glCompileShader(VertexShader);
}


void Shader::AttachShader()
{   
    // Tell the variable it is a shader program.
    ShaderProgram = glCreateProgram();

    // Attach the vertex and fragment shader to the shader program.
    glAttachShader(ShaderProgram, FragmentShader);
    glAttachShader(ShaderProgram, VertexShader);
}

