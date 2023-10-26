#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <glad/glad.h>

#include <spriteHandler.h>

#include "mapHandler.h"

using namespace std;

// Does everything related to the graphics
class Renderer
{
    public:

        vector<float> VertexVec;

        // A method for rendering everything
        void RenderEverything(vector<Sprite> SpriteArray);

        void LoadArrmapFile(string ArrmapFilePath, ArrayLevelMap *ArrmapObj);
};

#endif