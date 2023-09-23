#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <string>

// File reading and writing library
#include <fstream>

#include "renderer.h"
#include "spriteHandler.h"
#include "helperFunctions.h"

using namespace std;


// Create helper object
HelperFunctions HelperObj;


// Class Functions
unsigned char Sprite::LoadImageTexture()
{
    unsigned char TEST;


    return TEST;
}


// Function for reading .atris files.
void Sprite::LoadSpriteFile(string FilePath)
{
    string AtrisFileLine;
    string NoSpacesLine;

    // Open File
    ifstream ReadSpriteFile(FilePath);

    // extract file content
    while(getline(ReadSpriteFile, AtrisFileLine))
    {   
        NoSpacesLine = HelperObj.RemoveSpaces(AtrisFileLine);

        // Do checks
    }

    // Close file again
    ReadSpriteFile.close();

    // Set values
    HasAnimation = 1; // test



}