#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <memory>

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
        void RenderEverything(vector<Sprite> SpriteArray);
        
        void LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj, Shader *RedShader, vector<unique_ptr<GameElement> > *GameElementVector);
};

#endif