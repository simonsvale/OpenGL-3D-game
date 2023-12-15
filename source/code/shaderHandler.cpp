
#include "shaderHandler.h"


void Shader::LoadShaderFile(string GLSLFilePath, string *GLSLCode)
{
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
    int status;
    char info[1024];

    // Tell the variable that it is a fragment shader.
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Give the fragment shader a pointer to the GLSL code.
    glShaderSource(FragmentShader, 1, &ShaderCode, NULL);
    glCompileShader(FragmentShader);

    // Check if the shader could compile.
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &status);
    if(status != 1)
    {
        glGetShaderInfoLog(FragmentShader, 1024, NULL, info);
        cout << "Fragment shader could not compile, error: " << info << endl;
    }
}


void Shader::CreateVertexShader(const char *ShaderCode)
{
    int status;
    char info[1024];
    // Tell the variable that it is a vertex shader.
    VertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Give the vertex shader a pointer to the GLSL code.
    glShaderSource(VertexShader, 1, &ShaderCode, NULL);
    glCompileShader(VertexShader);

    // Check if the shader could compile.
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &status);
    if(status != 1)
    {
        glGetShaderInfoLog(VertexShader, 1024, NULL, info);
        cout << "Vertex shader could not compile, error: " << info << endl;
    }
}


void Shader::CreateGeometryShader(const char *ShaderCode)
{
    int status;
    char info[1024];

    // Tell the variable that it is a vertex shader.
    GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    // Give the vertex shader a pointer to the GLSL code.
    glShaderSource(GeometryShader, 1, &ShaderCode, NULL);
    glCompileShader(GeometryShader);
    
    // Check if the shader could compile.
    glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &status);
    if(status != 1)
    {
        glGetShaderInfoLog(GeometryShader, 1024, NULL, info);
        cout << "Geometry shader could not compile, error: " << info << endl;
    }
}


void Shader::AttachShader(GLuint *Program)
{   
    int status;
    char info[1024];

    // Tell the GLuint that it is a shader program.
    *Program = glCreateProgram();

    // Attach the vertex and fragment shader to the shader program.
    glAttachShader(*Program, FragmentShader);
    glAttachShader(*Program, VertexShader);
    
    if(GeometryShader != -1)
    {   
        glAttachShader(*Program, GeometryShader);
    }

    // Link the shader program
    glLinkProgram(*Program);

    // check for fragment shader linking errors
    glGetProgramiv(FragmentShader, GL_LINK_STATUS, &status);
    if(status != 1)
    {
        glGetShaderInfoLog(FragmentShader, 1024, NULL, info);
        cout << "Fragment shader could not be linked, error: " << info << endl;
    }

    // check for vertex shader linking errors
    glGetProgramiv(VertexShader, GL_LINK_STATUS, &status);
    if(status != 1)
    {
        glGetShaderInfoLog(VertexShader, 1024, NULL, info);
        cout << "Fragment shader could not be linked, error: " << info << endl;
    }

    // check for geometry shader linking errors
    if(GeometryShader != -1)
    {   
        glGetProgramiv(GeometryShader, GL_LINK_STATUS, &status);
        if(status != 1)
        {
            glGetShaderInfoLog(GeometryShader, 1024, NULL, info);
            cout << "Fragment shader could not be linked, error: " << info << endl;
        }
    }
    
}

