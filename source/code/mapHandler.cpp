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

#include <algorithm>

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
    vector<float> NormalsVec;
    vector<float> TexVec;
    vector<unsigned int> IndicesVec;

    // Path of a texture and shaders
    string TexturePath;
    string SpecularPath;
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

        // Get values contained in the .arrmap file.
        GetKeyValue_str("DIFFUSE_TEXTURE_PATH", SingleGeometryVector, &TexturePath, ArrmapFilePath);
        GetKeyValue_str("VERTEX_SHADER_PATH", SingleGeometryVector, &VertexShaderPath, ArrmapFilePath);
        GetKeyValue_str("FRAGMENT_SHADER_PATH", SingleGeometryVector, &FragmentShaderPath, ArrmapFilePath);

        GetKeyValue_int32("TYPE", SingleGeometryVector, &GameElementVector[0][Index]->GameElementType, ArrmapFilePath);

        // The geometry information.
        GetKeyValue_floatvector("VERTICES", SingleGeometryVector, &VertexVec, ArrmapFilePath);
        GetKeyValue_floatvector("NORMALS", SingleGeometryVector, &NormalsVec, ArrmapFilePath);
        GetKeyValue_floatvector("TEXTURE_COORDS", SingleGeometryVector, &TexVec, ArrmapFilePath);
        GetKeyValue_uintvector("INDICES", SingleGeometryVector, &IndicesVec, ArrmapFilePath);

        GetKeyValue_floatarray("WORLD_POSITION", SingleGeometryVector, GameElementVector[0][Index]->WorldPosition, &PositionArrSize, ArrmapFilePath);
        GetKeyValue_floatarray("ROTATION", SingleGeometryVector, GameElementVector[0][Index]->Rotation, &PositionArrSize, ArrmapFilePath);
        GetKeyValue_floatarray("SCALE", SingleGeometryVector, GameElementVector[0][Index]->Scale, &PositionArrSize, ArrmapFilePath);


        // Load vertecies into VBO and set VAO.
        GameElementVector[0][Index]->SetVBOSubData(
            &VertexVec[0], VertexVec.size(), 
            &NormalsVec[0], NormalsVec.size(), 
            &TexVec[0], TexVec.size(),
            &IndicesVec[0], IndicesVec.size()
        );

        if(GameElementVector[0][Index]->GameElementType >= 1)
        {
            // Materials.
            GetKeyValue_float("SHINE_VALUE", SingleGeometryVector, &GameElementVector[0][Index]->Material.ShineValue, ArrmapFilePath);

            GameElementVector[0][Index]->SetLightVAO(VertexVec.size(), NormalsVec.size());
        }
        else
        {
            GameElementVector[0][Index]->SetVAO(VertexVec.size(), NormalsVec.size(), TexVec.size());
        }
     
        //GameElementVector[0][Index]->SetVBO(&VertexVec[0], VertexVec.size());
        //GameElementVector[0][Index]->SetVAO();
        //GameElementVector[0][Index]->SetEBO(&IndicesVec[0], IndicesVec.size());

        // Set Indices array size!
        GameElementVector[0][Index]->IndicesSize = IndicesVec.size();

        array<string, 2> VertFragPair = {VertexShaderPath, FragmentShaderPath};

        // Compile shaders
        GameElementVector[0][Index]->ShaderProgramIndex = CompileRequiredShaders(ShaderObjectVector, VertexFragmentVector, VertFragPair);

        // Take the texture path extracted from the .arrmap file and load the texture into the gameElement Class
   
        GameElementVector[0][Index]->LoadTexture(
            &GameElementVector[0][Index]->DiffuseTexture, 
            TexturePath.c_str()
        );

        // Load specular map if set.
        if(GameElementVector[0][Index]->GameElementType == 2)
        {
            GetKeyValue_str("SPECULAR_TEXTURE_PATH", SingleGeometryVector, &SpecularPath, ArrmapFilePath);

            GameElementVector[0][Index]->LoadTexture(
                &GameElementVector[0][Index]->SpecularTexture,  
                SpecularPath.c_str()
            );
        }
        
        // Clear vectors
        SingleGeometryVector.clear();

        VertexVec.clear();
        NormalsVec.clear();
        TexVec.clear();
        IndicesVec.clear();

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