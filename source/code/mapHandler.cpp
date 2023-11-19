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

        // The important information:
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

        GameElementVector[0][Index]->SetVAO(VertexVec.size(), NormalsVec.size(), TexVec.size());  

        //GameElementVector[0][Index]->SetVBO(&VertexVec[0], VertexVec.size());
        //GameElementVector[0][Index]->SetVAO();
        //GameElementVector[0][Index]->SetEBO(&IndicesVec[0], IndicesVec.size());

        // Set Indices array size!
        GameElementVector[0][Index]->IndicesSize = IndicesVec.size();
        

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

// Load.obj wavefront file.
void ArrayLevelMap::LoadObjFile(string ObjFilePath, struct ObjModel &ModelRef)
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

    vector<string> SplitObjLineVector;
    vector<string> SplitIndicesVector;
    string AuxString;

    for(int Index = 0; Index < ObjLineVector.size();)
    {   
        // Check if it is vertices
        if((ObjLineVector[Index][0] == 'v'))
        {   
            if(ObjLineVector[Index][1] == 0x20) // Hex code for space.
            {
                SplitBySpace(Index, ObjLineVector, &SplitObjLineVector);

                // add the vertecies to the struct.
                for(int Index_3 = 1; Index_3 < SplitObjLineVector.size();)
                {
                    ModelRef.Vertices.push_back( atof(SplitObjLineVector[Index_3].c_str()) );
                    Index_3++;
                }
            }

            // Check if it is texture coordinates
            if(ObjLineVector[Index][1] == 't')
            {
                SplitBySpace(Index, ObjLineVector, &SplitObjLineVector);

                // add the vertecies to the struct.
                for(int Index_3 = 1; Index_3 < SplitObjLineVector.size();)
                {
                    ModelRef.TexCoords.push_back( atof(SplitObjLineVector[Index_3].c_str()) );
                    Index_3++;
                }
            }

            // Check if it is normals
            if(ObjLineVector[Index][1] == 'n')
            {
                SplitBySpace(Index, ObjLineVector, &SplitObjLineVector);

                // add the vertecies to the struct.
                for(int Index_3 = 1; Index_3 < SplitObjLineVector.size();)
                {
                    ModelRef.Normals.push_back( atof(SplitObjLineVector[Index_3].c_str()) );
                    Index_3++;
                }
            }
        }

        // Check if it is indices.
        if(ObjLineVector[Index][0] == 'f')
        {
            SplitBySpace(Index, ObjLineVector, &SplitObjLineVector);

            // get float indices
            for(int Index_2 = 1; Index_2 < SplitObjLineVector.size();)
            {
                AuxString = SplitObjLineVector[Index_2];
                SplitByDelimiter(AuxString, &SplitIndicesVector, '/', -1);

                ModelRef.Indices.push_back( atoi(SplitIndicesVector[0].c_str()) - 1);
                ModelRef.TextureIndices.push_back( atoi(SplitIndicesVector[1].c_str()) - 1);
                ModelRef.NormalsIndices.push_back( atoi(SplitIndicesVector[2].c_str()) - 1);

                SplitIndicesVector.clear();
                Index_2++;
            }
        }

        SplitObjLineVector.clear();
        Index++;
    }

    // The correctly configured vectors.
    vector<float> FinalVertices;
    vector<float> FinalNormals;
    vector<float> FinalTexCoords;

    vector<int> IndicesTest;

    vector<unsigned int> FinalIndices;

    vector<int>::iterator FoundIndex;

    int counter = 0;


    cout << "test" << endl;
    // vertices indices / vertices texture coords / vertices normals
    for(int i = 0; i < ModelRef.Indices.size();)
    {   
        FoundIndex = find(IndicesTest.begin(), IndicesTest.end(), ModelRef.Indices[i]);

        if(FoundIndex == IndicesTest.end())
        {   
            // If not found
            FinalVertices.push_back( ModelRef.Vertices[ ModelRef.Indices[i] * 3 ] );
            FinalVertices.push_back( ModelRef.Vertices[ ModelRef.Indices[i] * 3 + 1 ] );
            FinalVertices.push_back( ModelRef.Vertices[ ModelRef.Indices[i] * 3 + 2 ] );

            FinalTexCoords.push_back( ModelRef.TexCoords[ ModelRef.TextureIndices[i] * 2 ] );
            FinalTexCoords.push_back( ModelRef.TexCoords[ ModelRef.TextureIndices[i] * 2 + 1 ] );

            FinalNormals.push_back( ModelRef.Normals[ ModelRef.NormalsIndices[i] * 3 ] );
            FinalNormals.push_back( ModelRef.Normals[ ModelRef.NormalsIndices[i] * 3 + 1 ]  );
            FinalNormals.push_back( ModelRef.Normals[ ModelRef.NormalsIndices[i] * 3 + 2 ]  );

            FinalIndices.push_back(counter);
            IndicesTest.push_back(ModelRef.Indices[i]);
            counter++;
        }
        else
        {
            // if found
            FinalIndices.push_back(FoundIndex - IndicesTest.begin());
        }
        i++;
    }


    cout << "VERTICES = {";
    for(int i = 0; i < FinalVertices.size();)
    {
        cout << FinalVertices[i];
        if(i < (FinalVertices.size()-1))
        {
            cout << ", ";
        }
        i++;
    }
    cout << "}," << endl;


    cout << "NORMALS = {";
    for(int i = 0; i < FinalNormals.size();)
    {
        cout << FinalNormals[i];
        if(i < (FinalNormals.size()-1))
        {
            cout << ", ";
        }
        i++;
    }
    cout << "}," << endl;


    cout << "TEXTURE_COORDS = {";
    for(int i = 0; i < FinalTexCoords.size();)
    {
        cout << FinalTexCoords[i];
        if(i < (FinalTexCoords.size()-1))
        {
            cout << ", ";
        }
        i++;
    }
    cout << "}," << endl;


    cout << "INDICES = {";
    for(int i = 0; i < FinalIndices.size();)
    {
        cout << FinalIndices[i];
        if(i < (FinalIndices.size() - 1))
        {
            cout << ", ";
        }
        i++;
    }
    cout << "}" << endl;

    cout << "Vsize:" << FinalVertices.size() << endl;
    cout << "Nsize:" << FinalNormals.size() << endl;
    cout << "Tsize:" << FinalTexCoords.size() << endl;
    cout << "Isize:" << ModelRef.Indices.size() << endl;


    /*
    cout << "\n" << endl;

    // TEST
    cout << "test" << endl;

    int n = 0;
    int y = 0;

    cout << "Indices: " << endl; 
    for(int i = 0; i < ModelRef.Indices.size();)
    {
        cout << ModelRef.Indices[i] << ", "; 
        i++;
    }
    cout << endl;


    cout << "TI size:" << ModelRef.TextureIndices.size() << endl; 
    cout << "TV:" << ModelRef.TextureVertices.size() << endl; 
    cout << "V:" << ModelRef.Vertices.size() << endl; 


    while(n < ModelRef.Indices.size())
    {
        cout << ModelRef.Vertices[ModelRef.Indices[n]*3] << ", " << ModelRef.Vertices[ModelRef.Indices[n]*3+1] << ", " << ModelRef.Vertices[ModelRef.Indices[n]*3+2] << ", ";
        cout << "1, 0, 1, ";
        
        if(y < ModelRef.TextureIndices.size())
        {
            cout << ModelRef.TextureVertices[ModelRef.TextureIndices[y]*2] << ", " << ModelRef.TextureVertices[ModelRef.TextureIndices[y]*2+1] << "," << endl;;
            y++;
        }   
        n++;;
    }

    cout << "\n" << endl;
    */


}