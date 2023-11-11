#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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


void Renderer::RenderEverything(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, glm::mat4 projection, glm::mat4 view, SDL_Window *window)
{   
    int ShaderIndex;

    for(int GameElementNumber = 0; GameElementNumber < GameElementVector.size();)
    {   
        glm::mat4 model = glm::mat4(1.0f);

        // Get GameElement's shaderprogram index.
        ShaderIndex = GameElementVector[GameElementNumber]->ShaderProgramIndex;

        // New element
        glBindTexture(GL_TEXTURE_2D, GameElementVector[GameElementNumber]->Texture);
        glUseProgram(ShaderObjectVector[GameElementVector[GameElementNumber]->ShaderProgramIndex]->ShaderProgram);

        // Set GameElement model world position.
        model = glm::translate(model, glm::vec3(
            GameElementVector[GameElementNumber]->WorldPosition[0], 
            GameElementVector[GameElementNumber]->WorldPosition[1], 
            GameElementVector[GameElementNumber]->WorldPosition[2]
        ));

        // Set GameElement model rotation around x, y, z, in degrees.
        model = glm::rotate(model, glm::degrees(GameElementVector[GameElementNumber]->Rotation[0]), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::degrees(GameElementVector[GameElementNumber]->Rotation[1]), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::degrees(GameElementVector[GameElementNumber]->Rotation[2]), glm::vec3(0, 0, 1));

        // Set GameElement model scale.
        model = glm::scale(model, glm::vec3(
            GameElementVector[GameElementNumber]->Scale[0], 
            GameElementVector[GameElementNumber]->Scale[1], 
            GameElementVector[GameElementNumber]->Scale[2]
        ));
        

        // Assign new values to vertex shader.
        int modelLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind GameElement VAO.
        glBindVertexArray(GameElementVector[GameElementNumber]->VAO);

        // Draw GameElement array.
        glDrawArrays(GL_TRIANGLES, 0, GameElementVector[GameElementNumber]->GLArraySize);

        GameElementNumber++;
    }

    // Draw everything onto the program.
    SDL_GL_SwapWindow(window);
}


void Renderer::LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj, vector< unique_ptr<Shader> > *ShaderObjectVector, vector<unique_ptr<GameElement> > *GameElementVector)
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

    cout << "<[ENGINE]>" << "\nGAME ELEMENTS: "<< GameElementVector[0].size() << "\nSHADER PROGRAMS: " << ShaderObjectVector[0].size() << endl;

}

int Renderer::CompileRequiredShaders(vector< unique_ptr<Shader> > *ShaderObjectVector, vector< array<string, 2> > &UniquePairVector, array<string, 2> VertexFragmentPair)
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
