#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

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
    string NoSpacesInfo;

    vector<string> AtrisInfoVector;


    // Open File
    ifstream ReadSpriteFile(FilePath);

    // extract file content
    while(getline(ReadSpriteFile, AtrisFileLine))
    {   
        // Add line to variable
        NoSpacesInfo += HelperObj.RemoveSpaces(AtrisFileLine);
    }

    // Close file
    ReadSpriteFile.close();

    // Split file by delimiters 
    AtrisInfoVector = HelperObj.SplitByDelimiter(NoSpacesInfo, ';');


    // DEBUG !!!
    for(int test = 0; test < AtrisInfoVector.size();)
    {
        cout << AtrisInfoVector[test] << endl;
        test++;
    }

    // Set values
    HasAnimation = 1; // test

}