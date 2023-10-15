
#include "shaderHandler.h"


void Shader::LoadShaderFile(string GLSLFilePath, string *GLSLCode)
{
    short LineNum = 0;
    string GLSLFileLine;

    // Open GLSL File
    ifstream ReadGLSLFile(GLSLFilePath);

    // extract file content
    while(getline(ReadGLSLFile, GLSLFileLine))
    {   
        if(GLSLFileLine != "")
        {
            // Add the line to the auxiliary variable.
            *GLSLCode += GLSLFileLine+"\n";
        }
    }

    // Close file
    ReadGLSLFile.close();
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

void Shader::AttachShader(GLuint *Program)
{   
    // Tell the GLuint that it is a shader program.
    *Program = glCreateProgram();

    // Attach the vertex and fragment shader to the shader program.
    glAttachShader(*Program, FragmentShader);
    glAttachShader(*Program, VertexShader);
}

