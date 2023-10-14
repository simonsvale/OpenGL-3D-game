#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <glad/glad.h>

#include <spriteHandler.h>

using namespace std;

// Does everything related to the graphics
class Renderer
{
    public:

        // A method for rendering everything
        void RenderEverything(vector<Sprite> SpriteArray);

};

#endif