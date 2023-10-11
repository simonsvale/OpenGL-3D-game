#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <glad/glad.h>

using namespace std;

// Does everything related to the graphics
class Renderer
{
    public:
        void RenderEverything();

        // Loads a shader from a seperate file.
        void LoadShader(const char *Shader, string FilePath);
};

#endif