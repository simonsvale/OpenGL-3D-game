#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <string>

// File reading and writing library
#include <fstream>

#include "renderer.h"
#include "spriteHandler.h"

using namespace std;


unsigned char Sprite::LoadImageTexture()
{
    unsigned char TEST;


    return TEST;
}

// Open the sprite file.
void Sprite::LoadSpriteFile(std::string FilePath)
{
    string AtrisFileContent;

    // Open File
    ifstream ReadSpriteFile(FilePath);

    // extract file content
    while(getline(ReadSpriteFile, AtrisFileContent))
    {   
        cout << AtrisFileContent;
    }
    
    // Close file again
    ReadSpriteFile.close();

    // Set values
    HasAnimation = 1; // test



}