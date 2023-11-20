#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>

#include <algorithm>

#include "obj_helperfunctions.h"

using namespace std;

void LoadObjFile(string ObjFilePath, string ObjOutputFilePath)
{
    struct ObjModel ModelRef;

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

    // Write to arrobj file.
    ofstream ArrmapModelFile(ObjOutputFilePath);

    ArrmapModelFile << "VERTICES = {";
    for(int i = 0; i < FinalVertices.size();)
    {
        ArrmapModelFile << FinalVertices[i];
        if(i < (FinalVertices.size()-1))
        {
            ArrmapModelFile << ", ";
        }
        i++;
    }
    ArrmapModelFile << "}," << endl;


    ArrmapModelFile << "NORMALS = {";
    for(int i = 0; i < FinalNormals.size();)
    {
        ArrmapModelFile << FinalNormals[i];
        if(i < (FinalNormals.size()-1))
        {
            ArrmapModelFile << ", ";
        }
        i++;
    }
    ArrmapModelFile << "}," << endl;


    ArrmapModelFile << "TEXTURE_COORDS = {";
    for(int i = 0; i < FinalTexCoords.size();)
    {
        ArrmapModelFile << FinalTexCoords[i];
        if(i < (FinalTexCoords.size()-1))
        {
            ArrmapModelFile << ", ";
        }
        i++;
    }
    ArrmapModelFile << "}," << endl;


    ArrmapModelFile << "INDICES = {";
    for(int i = 0; i < FinalIndices.size();)
    {
        ArrmapModelFile << FinalIndices[i];
        if(i < (FinalIndices.size() - 1))
        {
            ArrmapModelFile << ", ";
        }
        i++;
    }
    ArrmapModelFile << "}" << endl;

    ArrmapModelFile.close();

    cout << "Finished writing to ARROBJ file, " << ObjOutputFilePath << "!" << endl;
    cout << "Vsize:" << FinalVertices.size() << endl;
    cout << "Nsize:" << FinalNormals.size() << endl;
    cout << "Tsize:" << FinalTexCoords.size() << endl;
    cout << "Isize:" << ModelRef.Indices.size() << endl;
    cout << "Triangle amount: " << ModelRef.Indices.size()/3 << endl;
}


void tool_usage(string argv)
{
    cerr << "Options:\n"
         << "\t-i, -input INPUT FILE PATH\tThe path of the .obj wavefront file.\n"
         << "\t-o, --output OUTPUT FILE PATH\tThe path of converted .obj wavefront file."
         << endl;
}


int main(int argc, char *argv[]) 
{
    string Argument;

    string InputPath = " ";
    string OutputPath = " ";

    if (argc < 2) 
    {
        tool_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Load arguments
    for(int Num = 0; Num < argc;)
    {   
        Argument = argv[Num];

        // Argument 1: -i, input path
        if( (Argument == "-i") || (Argument == "--input") )
        {
            if (Num + 1 < argc)
            {
                // Get the actual argument
                InputPath = argv[Num+1];
            }
            else
            {
                cerr << "No argument for --input given!" << endl;
                return EXIT_FAILURE;
            }
        }
        // Argument 1: -i, input path
        else if( (Argument == "-o") || (Argument == "--output") )
        {
            if (Num + 1 < argc)
            {
                OutputPath = argv[Num+1];
            }
            else
            {
                cerr << "No argument for --ouput given!" << endl;
                return EXIT_FAILURE;
            }
        }

        Num++;
    }

    // Set default
    if( OutputPath == " " )
    {
        cout << "No --output specified, setting default!" << endl;
        OutputPath = "C:\\Users\\srisb\\OneDrive\\Skrivebord\\Output.arrobj";
    }

    // If no input!
    if( (InputPath == " ") )
    {
        cout << "Option --input mssing!" << endl;
        return EXIT_FAILURE;
    }

    LoadObjFile(InputPath, OutputPath);
    return EXIT_SUCCESS;
}