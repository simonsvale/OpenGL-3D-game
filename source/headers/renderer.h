#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <memory>   // For smart pointer (unique_ptr<>).

#include <glad/glad.h>

#include <spriteHandler.h>

#include "mapHandler.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;

// Does everything related to the graphics
class Renderer
{
    public:


        /** 
        *  Renders all elements in the scene
        * 
        *  @param vector<GLuint> ShaderProgramArray, a vector of all Shaderprogrammes
        *  @param 
        *  @param 
        *
        *  @return void
        */
        void RenderEverything(vector<unique_ptr<GameElement> > *GameElementVector);
        
        void LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj, Shader *RedShader, Shader *RainbowShader, vector<unique_ptr<GameElement> > *GameElementVector);

        // Required a possible fragment and vertex shader pair and returns an index to the compiled shaderprogram.
        int CompileRequiredShaders(vector<GLuint> *ShaderProgramVector, vector< array<string, 2> > &VertexFragmentPairVector, array<string, 2> VertexFragmentPair);
};

#endif