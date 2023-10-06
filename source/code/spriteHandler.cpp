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

// Library for loading images from files
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

using namespace std;


// Create helper object
HelperFunctions HelperObj;


// Class Functions
void Sprite::LoadImageTexture(unsigned int Texture)
{
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("source/textures/dummy.png", &width, &height, &nrChannels, 0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}


// Function for reading .atris files.
void Sprite::LoadSpriteFile(string FilePath)
{
    // Setup Variables
    string AtrisFileLine;
    string NoSpacesInfo;

    vector<string> AtrisInfoVector;

    // Assign a pointer to the vector
    vector<string> *AtrisPtr = &AtrisInfoVector;


    // Open File
    ifstream ReadSpriteFile(FilePath);

    // extract file content
    while(getline(ReadSpriteFile, AtrisFileLine))
    {   
        // Add line to variable
        NoSpacesInfo += HelperObj.RemoveChar(AtrisFileLine, ' ');
    }

    // Close file
    ReadSpriteFile.close();

    // Split the string by delimiter ;
    HelperObj.SplitByDelimiter(NoSpacesInfo, AtrisPtr, ';');

    // Set Sprite attributes for created object, pass in FilePath for easier debugging, when during map/level construction.
    HasAnimation = HelperObj.GetAtrisKeyValue_bool("HasAnimation", AtrisInfoVector, AtrisPtr, FilePath);

    // DEBUG
    cout << "HasAnimation: " << HasAnimation << endl;
    if(HasAnimation == true)
    {
        AnimationFrameAmount = HelperObj.GetAtrisKeyValue_uint8_t("AnimationFrameAmount", AtrisInfoVector, AtrisPtr, FilePath);

        // DEBUG
        cout << "AnimationFrameAmount: " << +AnimationFrameAmount << endl;
    }
   

    Collision = HelperObj.GetAtrisKeyValue_bool("Collision", AtrisInfoVector, AtrisPtr, FilePath);

    // DEBUG
    cout << "Collision: " << Collision << endl;
    if(Collision == true)
    {
        CollisionBox = HelperObj.GetAtrisKeyValue_vector("CollisionBox", AtrisInfoVector, AtrisPtr, FilePath);

        // DEBUG
        cout << "CollisionBox: " << CollisionBox[0] << "," << CollisionBox[1] << endl;
    }


    // DEBUG end info display
    cout << "\n" << endl;

    /*
    // DEBUG !!!
    for(int test = 0; test < AtrisInfoVector.size();)
    {
        cout << AtrisInfoVector[test] << endl;
        test++;
    }
    */
    
}