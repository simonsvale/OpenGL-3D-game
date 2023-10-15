#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H

#include <string>
#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "graphicsHandler.h"

using namespace std;


class Shader
{
    public:
        // Constructor
        Shader(string VertexShaderFilePath, string FragmentShaderFilePath)
        {   
            string GLSLCode;

            // Load and create the Vertex shader, from a GLSL file.
            LoadShaderFile(VertexShaderFilePath, &GLSLCode);
            CreateVertexShader(GLSLCode.c_str());

            // Clear the string, so it can be used to read the Fragment shader.
            GLSLCode.clear();
            
            // Load and create the Fragment shader, from a GLSL file.
            LoadShaderFile(FragmentShaderFilePath, &GLSLCode);
            CreateFragmentShader(GLSLCode.c_str());
        }

        // Vertex and fragment shaders
        GLuint VertexShader; 
        GLuint FragmentShader;
        
        /* It is somewhat inefficient to have a shaderprogram for each shader, 
        however re-linking a shaderprogram by reattaching new shaders to it takes more time. */
        GLuint ShaderProgram; 

        // Load a .GLSL file.
        void LoadShaderFile(string GLSLFilePath, string *GLSLCode);

        // Create a vertex or a fragment shader, uses the attributes GLuint VertexShader and GLuint FragmentShader.
        void CreateVertexShader(const char *ShaderCode);
        void CreateFragmentShader(const char *ShaderCode);
        
        // Attach the shaders to a shaderprogram
        void AttachShader(GLuint *Program);
};


#endif