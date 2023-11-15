#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

using namespace std;

// Contains all structures

// struct for the .obj wavefront format.
struct ObjModel
{
    vector<vector<float> > Vertices;
    vector<vector<int> > Indices;
    vector<float> TextureVertices;
    vector<int> TextureIndices;
};

#endif