#include "mapHandler.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <memory>

#include "renderer.h"
#include "helperFunctions.h"
#include "mapHandler.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"
#include "structures.h"


void ArrayLevelMap::LoadArrmapFile(string ArrmapFilePath, vector< unique_ptr<Shader> > *ShaderObjectVector, vector< unique_ptr<GameElement> > *GameElementVector)
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
    GetKeyValue_floatarray("SPAWNPOINT", ArrmapInfoVector, SpawnpointArr, &SpawnpointArraySize, ArrmapFilePath);


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

    // !!!
    vector<array<string, 2> > VertexFragmentVector;
    vector<GLuint> ProgramVector;


    // Go through each vector index, and extract information.
    for(int Index = 0; Index < GeometryVector.size();)      
    {                                                 
        // Add object to vector
        GameElementVector->push_back(make_unique<GameElement>());       

        SplitByDelimiterAndBraces(GeometryVector[Index].substr(1, GeometryVector[Index].size()-1), &SingleGeometryVector, ',', '{', '}');

        /*
        for(int ArrmapAttributeNumber = 0; ArrmapAttributeNumber < SingleGeometryVector.size();)
        {   
            cout << SingleGeometryVector[ArrmapAttributeNumber] << endl;
            ArrmapAttributeNumber++;
        }
        */
        
        // Get values contained in the .arrmap file.
        GetKeyValue_str("TEXTURE_PATH", SingleGeometryVector, &TexturePath, ArrmapFilePath);
        GetKeyValue_str("VERTEX_SHADER_PATH", SingleGeometryVector, &VertexShaderPath, ArrmapFilePath);
        GetKeyValue_str("FRAGMENT_SHADER_PATH", SingleGeometryVector, &FragmentShaderPath, ArrmapFilePath);

        GetKeyValue_floatvector("VERTECIES", SingleGeometryVector, &VertexVec, ArrmapFilePath);
        
        GetKeyValue_floatarray("WORLD_POSITION", SingleGeometryVector, GameElementVector[0][Index]->WorldPosition, &PositionArrSize, ArrmapFilePath);
        GetKeyValue_floatarray("ROTATION", SingleGeometryVector, GameElementVector[0][Index]->Rotation, &PositionArrSize, ArrmapFilePath);
        GetKeyValue_floatarray("SCALE", SingleGeometryVector, GameElementVector[0][Index]->Scale, &PositionArrSize, ArrmapFilePath);


        // Load vertecies into VBO and set VAO.
        GameElementVector[0][Index]->SetVBO(&VertexVec[0], VertexVec.size());
        GameElementVector[0][Index]->SetVAO();

        // Set drawarraysize
        GameElementVector[0][Index]->GLArraySize = VertexVec.size()/8;


        array<string, 2> VertFragPair = {VertexShaderPath, FragmentShaderPath};

        // Compile shaders
        GameElementVector[0][Index]->ShaderProgramIndex = CompileRequiredShaders(ShaderObjectVector, VertexFragmentVector, VertFragPair);

        // Take the texture path extracted from the .arrmap file and load the texture into the gameElement Class
   
        GameElementVector[0][Index]->LoadTexture
        (
            &GameElementVector[0][Index]->Texture, 
            &ShaderObjectVector[0][GameElementVector[0][Index]->ShaderProgramIndex]->ShaderProgram, 
            TexturePath.c_str()
        );

        // Clear vector
        SingleGeometryVector.clear();
        VertexVec.clear();

        Index++;
    }

    cout << "<[ENGINE]>" << " Assigned memory to:" << "\nGAME ELEMENTS: "<< GameElementVector[0].size() << "\nSHADER PROGRAMS: " << ShaderObjectVector[0].size() << endl;
}

void ArrayLevelMap::UnloadArrmapFile(vector< unique_ptr<Shader> > *ShaderObjectVector, vector< unique_ptr<GameElement> > *GameElementVector)
{
    // Free allocated memory of GameElement objects in the vector.
    for(int Index = 0; Index < GameElementVector[0].size();)
    {
        // Free the rest of the memory allocated to the GameElement objects assigned using smart pointers.
        GameElementVector[0][Index].reset();
        Index++;
    }

    // Free allocated memory of Shader objects, stored in the vector.
    for(int Index = 0; Index < ShaderObjectVector[0].size();)
    {
        ShaderObjectVector[0][Index].reset();
        Index++;
    }


    cout << "<[ENGINE]>" << " Freed memory of:" << "\nGAME ELEMENTS: "<< GameElementVector[0].size() << "\nSHADER PROGRAMS: " << ShaderObjectVector[0].size() << endl;

    // Clear the vectors to allow a new .arrmap file to be loaded.
    GameElementVector[0].clear();
    ShaderObjectVector[0].clear();
}

int ArrayLevelMap::CompileRequiredShaders(vector< unique_ptr<Shader> > *ShaderObjectVector, vector< array<string, 2> > &UniquePairVector, array<string, 2> VertexFragmentPair)
{   

    string VertexShaderPath = VertexFragmentPair[0];   // Vertex Shader
    string FragmentShaderPath = VertexFragmentPair[1];   // Fragment Shader

    // Check if the pair already exists.
    for(int Index = 0; Index < UniquePairVector.size();)
    {
        if((VertexShaderPath == UniquePairVector[Index][0]) && (FragmentShaderPath == UniquePairVector[Index][1]))
        {   
            // Return the index if the Shader already exists.
            return Index;
        }
        Index++;
    }

    // If the shader does not exist do the following:

    // Push the pair into the vector.
    UniquePairVector.push_back({VertexShaderPath, FragmentShaderPath});

    ShaderObjectVector->push_back(make_unique<Shader>(VertexShaderPath, FragmentShaderPath));  

    // Push the shader program into the vector.
    return ShaderObjectVector->size()-1;
}

// Load.obj wavefront file.
void ArrayLevelMap::LoadObjFile(string ObjFilePath, struct ObjModel *ModelPtr)
{
    // Wavefront .obj
    string ObjLine;

	vector<string> ObjLineVector;

    // Open File
    ifstream ReadSpriteFile(ObjFilePath);

    // extract file content
    while(getline(ReadSpriteFile, ObjLine))
    { 	
		// Add line.
		ObjLineVector.push_back(ObjLine);
    }

    // Close file
    ReadSpriteFile.close();


    for(int Index = 0; Index < ObjLineVector.size();)
    {   
        // Check if it is vertices
        if((ObjLineVector[Index][0] == 'v'))
        {   
            // Check if it is vertices coordinates.
            if((ObjLineVector[Index][1] == ' '))
            {
                cout << "vertices time:" << ObjLineVector[Index] << endl;


                
            }

            // Check if it is texture coordinates
            if((ObjLineVector[Index][1] == 't'))
            {
                cout << "vertices texture time:" << ObjLineVector[Index] << endl;



            }

        }
        // Check if it is indices.
        else if((ObjLineVector[Index][0] == 'f'))
        {
            cout << "float time:" << ObjLineVector[Index] << endl;



        }

        Index++;
    }


    // First find the vertices coordinates: v


    // Now look for vertices texture coordinates: vt


    // Now look for indices: f


}