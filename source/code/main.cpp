// terminal command for running program: ../../main


// Include build-in libraries
#include <cmath>
#include <memory>
#include <windows.h>

// Include SDL2 and OpenGL headers
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Header file for 3D.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include own headers
#include "structures.h"
#include "spriteHandler.h"
#include "renderer.h"
#include "graphicsHandler.h"
#include "shaderHandler.h"
#include "mapHandler.h"
#include "structures.h"

#include "gameElementHandler.h"
#include "cubemaps.h"

#include "controls.h"

using namespace std;

// Window width and height
const int WIDTH = 1080, HEIGHT = 720;

int main(int argc, char **argv) 
{   
    
    // Create Class objects
    Controls Controls;
    ArrayLevelMap Arraymap;
    Renderer RenderObj;
    
    // Create a vector to contain GameElement objects.
    vector< unique_ptr<GameElement> > GameElementVector;

    // Vector containing Shaders. Used to save memory on reused shaders.
    vector< unique_ptr<Shader> > ShaderObjectVector;

    vector< unique_ptr<ReflectionProbe> > ReflectionVector;

    SDL_Init(SDL_INIT_EVERYTHING);

    // Set OpenGL window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window;
    window = SDL_CreateWindow(
        "Game window", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        WIDTH, HEIGHT, 
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL
    );

    if (NULL == window)
    {
        cout << "Could not create game window, error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    // OpenGL context.
    SDL_GLContext GLContext;
    GLContext = SDL_GL_CreateContext(window);

    // OpenGL function pointers.
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    
    if (NULL == GLContext)
    {
        cout << "Could not create OpenGL context, error:" << SDL_GetError() << endl;
        return EXIT_FAILURE; 
    }

    // !!! Load map and create all vertecies and textures.
    Arraymap.LoadArrmapFile("source/maps/myFirstMap.arrmap", &ShaderObjectVector, &GameElementVector);

    // Create shadowmap to render shadows.
    ShadowMap DepthMap;

    // Create skybox:
    Skybox Sky;
    Sky.load_cubemap({
        "source/textures/skybox/treatmentLF.png", 
        "source/textures/skybox/treatmentRT.png", 
        "source/textures/skybox/treatmentUP.png", 
        "source/textures/skybox/treatmentDN.png", 
        "source/textures/skybox/treatmentFT.png", 
        "source/textures/skybox/treatmentBK.png"
    });
    Sky.SkyboxShader.set_shader_texture(0, "skybox");


    // Enable depth test and backface culling.
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE);  

    // !!!
    // Allocate memory for new cubemap.
    ReflectionVector.push_back(make_unique<ReflectionProbe>(512, 512));

    // Create the allocated cubemap cubemap
    ReflectionVector[0]->set_reflection_FBO();
    ReflectionVector[0]->CubePos = {6.06258, 4.58507, -0.548955};

    RenderObj.RenderCubemaps(GameElementVector, ShaderObjectVector, DepthMap, Sky, ReflectionVector, false);


    glm::mat4 view;
    glm::mat4 projection;

    // Setup variables for maintaining 60 fps
    int FrameTime;
    Uint64 FrameTimeStart;

    const int FrameDelay = 1000 / 60;

    // Needed for mouse inputs to work correctly
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Game loop
    while(Controls.Running == true)
    {   
        // Set frame start
        FrameTimeStart = SDL_GetTicks64();
        

        // Run controls, does keystate and everything
        Controls.RunControls();

        // Camera movement
        Controls.ComputeMouseInput(window);
        projection = Controls.ProjectionMatrix;
        view = Controls.ViewMatrix;

        // Render Everything.
        RenderObj.RenderEverything(GameElementVector, ShaderObjectVector, projection, view, Controls.position, window, DepthMap, Sky, ReflectionVector);


        // Calculate the amount of time it took to run through 1 frame.
        FrameTime = SDL_GetTicks() - FrameTimeStart;

        // Set the delay accordingly
        if(FrameDelay > FrameTime)
        {
            SDL_Delay(FrameDelay - FrameTime);
        }
    }

    // Free memory by unloading the .arrmap file.
    Arraymap.UnloadArrmapFile(&ShaderObjectVector, &GameElementVector);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}