#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <fstream>

#include "renderer.h"
#include "helperFunctions.h"
#include "mapHandler.h"

using namespace std;

HelperFunctions HelperObjRenderer;

/** 
  *  Renders all elements in the scene
  * 
  *  @param vector<GLuint> ShaderProgramArray, a vector of all Shaderprogrammes
  *  @param 
  *  @param 
  *
  *  @return void
*/
void Renderer::RenderEverything(vector<Sprite> SpriteArray)
{

}


void Renderer::LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj)
{
    // Setup Variables
    string ArrmapFileLine;
    string NoSpacesArrmap;

	string AuxLine;

    vector<string> ArrmapInfoVector;

    // Open File
    ifstream ReadSpriteFile(ArrmapFilePath);

    // extract file content
    while(getline(ReadSpriteFile, ArrmapFileLine))
    { 	
		// Remove empty spaces in .arrmap line.
		AuxLine = HelperObjRenderer.RemoveChar(ArrmapFileLine, ' ');

		// Remove comments, defined by "//" in .arrmap line.
		if('/' != AuxLine[0] && '/' != AuxLine[1])
		{
			// Add line to new string.
			NoSpacesArrmap += HelperObjRenderer.RemoveChar(ArrmapFileLine, ' ');
		}
    }
		
    // Close file
    ReadSpriteFile.close();

	// Split the string by the ';' delimiter
	HelperObjRenderer.SplitByDelimiter(NoSpacesArrmap, &ArrmapInfoVector, ';');


	// Get player spawnpoint and put it in the pointer to the Arrmapobj:
	//ArrmapObj->Spawnpoint = HelperObjRenderer.GetKeyValue_vector("SPAWNPOINT", ArrmapInfoVector, &ArrmapInfoVector, ArrmapFilePath);



	float MapArray[] = {0.0f};

	HelperObjRenderer.GetArrayFromStr("{1,2,3,4,5,6,7,8,9}", MapArray);

	cout << sizeof(MapArray)/sizeof(float) << endl;

	for(int test1 = 0; test1 < sizeof(MapArray)*sizeof(MapArray[0]);)
    {
        cout << ArrmapInfoVector[test1] << endl;
        test1++;
    }

    // DEBUG !!!
    for(int test = 0; test < ArrmapInfoVector.size();)
    {
        cout << ArrmapInfoVector[test] << endl;
        test++;
    }

}














