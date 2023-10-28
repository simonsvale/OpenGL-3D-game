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


// Function for reading .atris files.
void Sprite::LoadSpriteFile(string FilePath)
{
    // Setup Variables
    string AtrisFileLine;
    string NoSpacesInfo;

    vector<string> AtrisInfoVector;

    // Open File
    ifstream ReadSpriteFile(FilePath);

    // extract file content
    while(getline(ReadSpriteFile, AtrisFileLine))
    {   
        // Add line to variable
        NoSpacesInfo += RemoveChar(AtrisFileLine, ' ');
    }

    // Close file
    ReadSpriteFile.close();

    // Split the string by delimiter ;
    SplitByDelimiter(NoSpacesInfo, &AtrisInfoVector, ';', -1);

    // Set Sprite attributes for created object, pass in FilePath for easier debugging, when during map/level construction.
    HasAnimation = GetKeyValue_bool("HasAnimation", AtrisInfoVector, &AtrisInfoVector, FilePath);

    // DEBUG
    cout << "HasAnimation: " << HasAnimation << endl;
    if(HasAnimation == true)
    {
        AnimationFrameAmount = GetKeyValue_uint8_t("AnimationFrameAmount", AtrisInfoVector, &AtrisInfoVector, FilePath);

        // DEBUG
        cout << "AnimationFrameAmount: " << +AnimationFrameAmount << endl;
    }
   

    Collision = GetKeyValue_bool("Collision", AtrisInfoVector, &AtrisInfoVector, FilePath);

    // DEBUG
    cout << "Collision: " << Collision << endl;
    if(Collision == true)
    {
        CollisionBox = GetKeyValue_intvector("CollisionBox", AtrisInfoVector, &AtrisInfoVector, FilePath);

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