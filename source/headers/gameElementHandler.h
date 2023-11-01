#ifndef GAME_ELEMENT_HANDLER_H
#define GAME_ELEMENT_HANDLER_H

#include <string>

#include <graphicsHandler.h>


class GameElement: public Graphics
{
    public:
        // All variables that need to be set using the .arrmap file.
        int Type;

        float WorldPosition[3];

        // Only here to allow change of textures on the fly.
        string VertexShaderPath = "NULL";
        string FragmentShaderPath = "NULL";

        // Because its a Graphics Object, it also includes VAO.

};

#endif