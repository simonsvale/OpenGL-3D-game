#ifndef GAME_ELEMENT_HANDLER_H
#define GAME_ELEMENT_HANDLER_H

#include <string>

#include <graphicsHandler.h>


struct GameElementMaterial
{
    float SpecularStrength[3];
    float ShineValue;
};


class GameElement: public Graphics
{
    public:
        // All variables that need to be set using the .arrmap file.
        int GameElementType = -1;
        int ShaderProgramIndex;
        int GLArraySize;
        int IndicesSize;

        float WorldPosition[3];
        float Scale[3];
        float Rotation[3];

        // Material values.
        GameElementMaterial Material;


        // Only here to allow change of shaders on the fly.
        string VertexShaderPath = "NULL";
        string FragmentShaderPath = "NULL";

        // Texture
        GLuint Texture;
};

#endif