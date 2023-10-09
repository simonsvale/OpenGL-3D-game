#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>

#include "renderer.h"

using namespace std;


void LoadShader(const char *Shader, string FilePath)
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





void Renderer::RenderEverything()
{

}








