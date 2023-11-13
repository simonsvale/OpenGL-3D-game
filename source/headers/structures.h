#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

using namespace std;

// Contains all structures

// struct for the .obj wavefront format.
struct ObjModel
{
    vector<float> Vertices;
    vector<float> Indices;
};

#endif