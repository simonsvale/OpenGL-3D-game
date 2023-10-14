#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H

#include <string>
#include <fstream>
#include <array>
#include <iostream>

#include <glad/glad.h>

#include "graphicsHandler.h"

using namespace std;


class Shader
{
    public:

        GLuint VertexShader;    // Vertex shader
        GLuint FragmentShader;  // Fragment shader
        
        GLuint ShaderProgram; // ShaderProgram

        // Methods:
        void LoadShaderFile(const char *Shader, string FilePath);

        void CreateVertexShader(const char *ShaderCode);
        void CreateFragmentShader(const char *ShaderCode);

        void AttachShader();
};




#endif