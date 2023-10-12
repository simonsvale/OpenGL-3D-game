#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <string>

using namespace std;

class GraphicsHandler
{
    public:
        void LoadShader(const char *Shader, string FilePath);


        void SetVBO(float Vertecies[]);

        void SetVAO();

        void SetEBO(float indices[]);



};






#endif