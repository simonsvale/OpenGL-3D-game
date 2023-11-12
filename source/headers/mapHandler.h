#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>

#include "renderer.h"
#include "helperFunctions.h"
#include "mapHandler.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;


class ArrayLevelMap
{	
	public:
		float SpawnpointArr[3];

        // Loads an .arrmap file.
        void LoadArrmapFile(string ArrmapFilePath, vector< unique_ptr<Shader> > *ShaderObjectVector, vector<unique_ptr<GameElement> > *GameElementVector);

        // Unloads an .arrmap file.
        void UnloadArrmapFile(vector< unique_ptr<Shader> > *ShaderObjectVector, vector<unique_ptr<GameElement> > *GameElementVector);

        // Required a possible fragment and vertex shader pair and returns an index to the compiled shaderprogram.
        int CompileRequiredShaders(vector< unique_ptr<Shader> > *ShaderObjectVector, vector< array<string, 2> > &UniquePairVector, array<string, 2> VertexFragmentPair);
};


#endif