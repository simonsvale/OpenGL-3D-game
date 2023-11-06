#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <array>

#include "renderer.h"
#include "helperFunctions.h"
#include "mapHandler.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;


void Renderer::RenderEverything(vector<unique_ptr<GameElement> > *GameElementVector)
{

}


void Renderer::LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj, Shader *RedShader, Shader *RainbowShader, vector<unique_ptr<GameElement> > *GameElementVector)
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
		AuxLine = RemoveChar(ArrmapFileLine, ' ');

		// Remove comments, defined by "//" in .arrmap line.
		if('/' != AuxLine[0] && '/' != AuxLine[1])
		{
			// Add line to new string.
			NoSpacesArrmap += RemoveChar(ArrmapFileLine, ' ');
		}
    }
		
    // Close file
    ReadSpriteFile.close();

	// Split the string by the ';' delimiter
	SplitByDelimiter(NoSpacesArrmap, &ArrmapInfoVector, ';', -1);


    // Get player spawnpoint, from its 3D coordinates.
    int SpawnpointArraySize = 3;
    GetKeyValue_floatarray("SPAWNPOINT", ArrmapInfoVector, ArrmapObj->SpawnpointArr, &SpawnpointArraySize, ArrmapFilePath);


    vector<string> MapGeometry;
    GetKeyValue_strvector("MAP_GEOMETRY", ArrmapInfoVector, &MapGeometry);

    // Remove outer braces.
    string GeometryInfo = MapGeometry[0].substr(1, MapGeometry[0].size()-2);


    // Vector for holding all geometry in the MAP_GEOMETRY.
    vector<string> GeometryVector;

    // Split the Map_Geometry of the map file into seperate Geometry.
    SplitByBraces(GeometryInfo, &GeometryVector, '{', '}');

    vector<string> SingleGeometryVector;
    vector<string> ArrmapAttributeVector;

    vector<float> VertexVec;

    // Path of a texture and shaders
    string TexturePath;
    string VertexShaderPath;
    string FragmentShaderPath;

    int PositionArrSize = 3;


    // Go through each vector index, and extract information.
    for(int Index = 0; Index < GeometryVector.size();)      
    {                                                 
        // Add object to vector
        GameElementVector->push_back(make_unique<GameElement>());       

        SplitByDelimiterAndBraces(GeometryVector[Index].substr(1, GeometryVector[Index].size()-1), &SingleGeometryVector, ',', '{', '}');

        // !!!
        for(int ArrmapAttributeNumber = 0; ArrmapAttributeNumber < SingleGeometryVector.size();)
        {   
            cout << SingleGeometryVector[ArrmapAttributeNumber] << endl;
            ArrmapAttributeNumber++;
        }
        
        // Get values contained in the .arrmap file.
        GetKeyValue_str("TEXTURE_PATH", SingleGeometryVector, &TexturePath, ArrmapFilePath);
        GetKeyValue_str("VERTEX_SHADER_PATH", SingleGeometryVector, &VertexShaderPath, ArrmapFilePath);
        GetKeyValue_str("FRAGMENT_SHADER_PATH", SingleGeometryVector, &FragmentShaderPath, ArrmapFilePath);

        GetKeyValue_floatvector("VERTECIES", SingleGeometryVector, &VertexVec, ArrmapFilePath);
        GetKeyValue_floatarray("WORLD_POSITION", SingleGeometryVector, GameElementVector[0][Index]->WorldPosition, &PositionArrSize, ArrmapFilePath);
        GetKeyValue_floatarray("SCALE", SingleGeometryVector, GameElementVector[0][Index]->Scale, &PositionArrSize, ArrmapFilePath);
        GetKeyValue_floatarray("ROTATION", SingleGeometryVector, GameElementVector[0][Index]->Rotation, &PositionArrSize, ArrmapFilePath);


        // Load vertecies into VBO and set VAO.
        GameElementVector[0][Index]->SetVBO(&VertexVec[0], VertexVec.size());
        GameElementVector[0][Index]->SetVAO();
        
        // Take the texture path extracted from the .arrmap file and load the texture into the gameElement Class
        if(Index == 0)
        {
            GameElementVector[0][Index]->LoadTexture(&GameElementVector[0][Index]->Texture, &RedShader->ShaderProgram, TexturePath.c_str());
        }

        if(Index == 1)
        {
            GameElementVector[0][Index]->LoadTexture(&GameElementVector[0][Index]->Texture, &RainbowShader->ShaderProgram, TexturePath.c_str());
        }


        // !!!
        // break;
 

        // Clear vector
        SingleGeometryVector.clear();

        Index++;

        // Perhaps a loading bar on another thread.
    }

    // Delete all instances of the Graphics class.
    //delete[] GameElementObj;
    
    
    /*
    // DEBUG !!!
    for(int test = 0; test < ArrmapInfoVector.size();)
    {
        cout << ArrmapInfoVector[test] << endl;
        test++;
    }
    */
}

void CompileRequiredShaders(vector<GLuint> *ShaderProgramVector, vector< array<string, 2> > VertexFragmentPairVector)
{   
    vector< array<string, 2> > UniqueVertexFragmentPairVector;

    string VertexShaderPath;
    string FragmentShaderPath;

    // Run through outer index.
    for(int Index_1 = 0; Index_1 < VertexFragmentPairVector.size();)
    {

        VertexShaderPath = VertexFragmentPairVector[Index_1][0];   // Vertex Shader
        FragmentShaderPath = VertexFragmentPairVector[Index_1][1];   // Fragment Shader

        // !!!
        cout << VertexShaderPath << ", " << FragmentShaderPath << endl;

        // Check all compile ready shaders
        for(int Index_2 = 0; Index_2 < UniqueVertexFragmentPairVector.size()+1;)
        {
            if((VertexShaderPath != UniqueVertexFragmentPairVector[Index_2][0]) && (FragmentShaderPath != UniqueVertexFragmentPairVector[Index_2][1]))
            {
                UniqueVertexFragmentPairVector.push_back({VertexShaderPath, FragmentShaderPath});

                // Create shader program
                Shader NewShader(VertexShaderPath, FragmentShaderPath);

                // Push the shader program into the vector.
                ShaderProgramVector->push_back(NewShader.ShaderProgram);
            }
            Index_2++;
        }
        Index_1++;
    }
}
