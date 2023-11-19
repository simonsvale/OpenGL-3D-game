#ifndef OBJ_HELPERFUNCTIONS_H
#define OBJ_HELPERFUNCTIONS_H

#include <string>
#include <vector>

using namespace std;

struct ObjModel
{
    vector<float> Vertices;
    vector<float> TexCoords;
    vector<float> Normals;

    vector<int> Indices;
    vector<int> TextureIndices;
    vector<int> NormalsIndices;
};

void SplitBySpace(int Index, vector<string> ObjLineVector, vector<string> *SplitObjLineVector);
void SplitByDelimiter(string String, vector<string> *SplitStrVecPtr, char Delimiter, int DelimiterAmount);

#endif