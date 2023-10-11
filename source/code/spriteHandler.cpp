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


// Currently a working, but unfinished function, may need to take more parameters.
void Sprite::LoadImageTexture(GLuint *Texture, GLuint *ShaderProgramPtr, const char *TexturePath)
{
    // Set textures:
    int widthImg, heightImg, numColCh;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(TexturePath, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, Texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Needs to be a parameter "tex0"
    GLuint tex0Uni = glGetUniformLocation(*ShaderProgramPtr, "tex0");
    glLinkProgram(*ShaderProgramPtr);

    // Needs to be a parameter
    glUniform1i(tex0Uni, 0);
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