#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>

#include "renderer.h"
#include "helperFunctions.h"
#include "mapHandler.h"
#include "shaderHandler.h"

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


void Renderer::LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj, Shader *RedShader, GLuint *TexturePtr, GLuint *VAOPtr)
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
	HelperObjRenderer.SplitByDelimiter(NoSpacesArrmap, &ArrmapInfoVector, ';', -1);


    // Get player spawnpoint, from its 3D coordinates.
    int SpawnpointArraySize = 3;
    HelperObjRenderer.GetKeyValue_floatarray("SPAWNPOINT", ArrmapInfoVector, ArrmapObj->SpawnpointArr, &SpawnpointArraySize, ArrmapFilePath);


    vector<string> MapGeometry;
    HelperObjRenderer.GetKeyValue_strvector("MAP_GEOMETRY", ArrmapInfoVector, &MapGeometry);

    // Remove outer braces.
    string GeometryInfo = MapGeometry[0].substr(1, MapGeometry[0].size()-2);


    // Vector for holding all geometry in the MAP_GEOMETRY.
    vector<string> GeometryVector;

    // Split the Map_Geometry of the map file into seperate Geometry.
    HelperObjRenderer.SplitByBraces(GeometryInfo, &GeometryVector, '{', '}');


    vector<string> SingleGeometryVector;
    vector<string> ArrmapAttributeVector;

    // Size should be: GeometryVector.size()
    Graphics *GraphicsObjs = new Graphics[1];


    vector<float> VertexVec;


    // Go through each vector index, and extract information.
    for(int Index = 0; Index < GeometryVector.size();)
    {
        HelperObjRenderer.SplitByDelimiter(GeometryVector[Index].substr(1, GeometryVector[Index].size()-1), &SingleGeometryVector, ',', 4);


        /*
        for(int ArrmapAttributeNumber = 0; ArrmapAttributeNumber < SingleGeometryVector.size();)
        {   
            cout << SingleGeometryVector[ArrmapAttributeNumber] << endl;
            ArrmapAttributeNumber++;
        }
        */

        // Do data processing !

        HelperObjRenderer.GetKeyValue_floatvector("VERTECIES", SingleGeometryVector, &VertexVec, ArrmapFilePath);

        // Load vertecies into VBO and set VAO.
        GraphicsObjs[Index].SetVBO(&VertexVec[0], VertexVec.size());
        GraphicsObjs[Index].SetVAO();
        
        GraphicsObjs[Index].LoadTexture(TexturePtr, &RedShader->ShaderProgram, "source/textures/debug3.png");

        *VAOPtr = GraphicsObjs[Index].VAO;

        
        break;

        // Generate the float arrays.

        // Clear vector
        SingleGeometryVector.clear();

        Index++;

        // Perhaps a loading bar on another thread.
    }

    // Delete all instances of the Graphics class.
    //delete[] GraphicsObjs;
    
    
    /*
    // DEBUG !!!
    for(int test = 0; test < ArrmapInfoVector.size();)
    {
        cout << ArrmapInfoVector[test] << endl;
        test++;
    }
    */
}














