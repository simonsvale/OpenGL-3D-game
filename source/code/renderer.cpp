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


    // Get player spawnpoint, from its 3D coordinates.
    int SpawnpointArraySize = 3;
    HelperObjRenderer.GetKeyValue_floatarray("SPAWNPOINT", ArrmapInfoVector, ArrmapObj->SpawnpointArr, &SpawnpointArraySize, ArrmapFilePath);


    vector<string> MapGeometry;
    HelperObjRenderer.GetKeyValue_strvector("MAP_GEOMETRY", ArrmapInfoVector, &MapGeometry);

    // !!!
    cout << MapGeometry[0] << endl;

    MapGeometry[0] = MapGeometry[0].substr(1, MapGeometry[0].size()-2);


    // Split the Map_Geometry of the map file into seperate Geometry.
    vector<string> GeometryVector;
    HelperObjRenderer.SplitByBraces(MapGeometry[0], &GeometryVector, '{', '}');

    // DEBUG !!!
    for(int test = 0; test < GeometryVector.size();)
    {
        cout << GeometryVector[test] << endl;
        cout << "end" << endl;
        test++;
    }
    

	float MapArray[2];
    int MapArrSize;
	HelperObjRenderer.GetFloatArrayFromStr("{0.1f,0.5f}", MapArray, &MapArrSize);

    /*
    // DEBUG !!!
    for(int test = 0; test < ArrmapInfoVector.size();)
    {
        cout << ArrmapInfoVector[test] << endl;
        test++;
    }
    */
}














