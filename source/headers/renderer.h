#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <memory>   // For smart pointer (unique_ptr<>).

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderHandler.h"
#include "gameElementHandler.h"

#include "cubemaps.h"

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
        void RenderEverything(
            vector<unique_ptr<GameElement> > &GameElementVector, 
            vector< unique_ptr<Shader> > &ShaderObjectVector, 
            glm::mat4 projection, 
            glm::mat4 view,
            glm::vec3 CameraPosition,
            SDL_Window *window,
            GameElement &FBODummy,
            Shader &CubemapShader,
            Skybox Sky
        );

        // Method for rendering cubemaps for shadow mapping.
        void RenderCubemaps(vector<unique_ptr<GameElement> > &GameElementVector, Shader &CubemapShader, GameElement &FBODummy);
        
};

#endif