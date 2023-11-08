// terminal command for running program: ../../main


// Include build-in libraries
#include <iostream>
#include <cmath>
#include <array>
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

#include "gameElementHandler.h"

#include "controls.h"

using namespace std;

// Window width and height
const int WIDTH = 1080, HEIGHT = 720;

int main(int argc, char **argv) 
{   
    
    // Create Class objects
    Controls Controls;
    ArrayLevelMap MapObj;
    Renderer RenderObj;

    // Create a vector to contain GameElement objects.
    vector<unique_ptr<GameElement> > GameElementVector;


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
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

    if (NULL == window)
    {
        cout << "Could not create window" << SDL_GetError() << endl;
        return 1;
    }

    // OpenGL context.
    SDL_GLContext GL_context;
    GL_context = SDL_GL_CreateContext(window);

    // OpenGL function pointers.
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    
    if (NULL == GL_context)
    {
        cout << "Could not create OpenGL context" << SDL_GetError() << endl;
        return 1; 
    }


    // !!!
    //Shader RedShader("source/shaders/basicVertexShader.GLSL", "source/shaders/redShader.GLSL");
    //Shader RainbowShader("source/shaders/basicVertexShader.GLSL", "source/shaders/rainbowShader.GLSL");
    
    // Vector containing Shaders.
    vector< unique_ptr<Shader> > ShaderObjectVector;

    // !!! Load map and create all vertecies and textures.
    RenderObj.LoadArrmapFile("source/maps/myFirstMap.arrmap", &MapObj, &ShaderObjectVector, &GameElementVector);

    // !!!
    glEnable(GL_DEPTH_TEST);  


    // Setup variables for maintaining 60 fps
    int FrameTimeTotal;
    Uint64 FrameTimeStart;
    Uint64 FrameTimeEnd;

    const int FrameDelay = 1000 / 60;

    // Hide cursor
    // Is needed for mouse inputs to work correctly
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Window loop
    while(Controls.Running == true)
    {   
        // Set frame start
        FrameTimeStart = SDL_GetTicks64();
        
        // Run controls, does keystate and everthing
        Controls.RunControls();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;

        // Camera movement
        Controls.ComputeMouseInput(window);
        projection = Controls.ProjectionMatrix;
        view = Controls.ViewMatrix;

        
        // background color
        glClearColor(0.766f, 0.922f, 0.970f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // 3D
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // !!!  
        glBindTexture(GL_TEXTURE_2D, GameElementVector[1]->Texture);
        glUseProgram(ShaderObjectVector[GameElementVector[1]->ShaderProgramIndex]->ShaderProgram);

        glm::vec3 scale2 = glm::vec3(30.0f, 30.0f, 1.0f);

        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
        model2 = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model2 = glm::scale(model2, scale2);

        // Assign new values to vertex shader.
        int modelLoc2 = glGetUniformLocation(ShaderObjectVector[GameElementVector[1]->ShaderProgramIndex]->ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
        int viewLoc2 = glGetUniformLocation(ShaderObjectVector[GameElementVector[1]->ShaderProgramIndex]->ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc2 = glGetUniformLocation(ShaderObjectVector[GameElementVector[1]->ShaderProgramIndex]->ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw elements for obj_1
        glBindVertexArray(GameElementVector[1]->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        

        // New element
        glBindTexture(GL_TEXTURE_2D, GameElementVector[0]->Texture);
        glUseProgram(ShaderObjectVector[GameElementVector[0]->ShaderProgramIndex]->ShaderProgram);

        // Rotate the model
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));

        // The rotation of the cube.
        model = glm::rotate(model, float(SDL_GetTicks64()/2000.0) * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f)); 

        // Assign new values to vertex shader.
        int modelLoc = glGetUniformLocation(ShaderObjectVector[GameElementVector[0]->ShaderProgramIndex]->ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(ShaderObjectVector[GameElementVector[0]->ShaderProgramIndex]->ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ShaderObjectVector[GameElementVector[0]->ShaderProgramIndex]->ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        

        // Draw elements for obj_1
        glBindVertexArray(GameElementVector[0]->VAO);

        // Draw cube
        // Instead of calling this GL method each time maybe using glbuffersubdata, could reduce this call to a single each loop. !!!!!!!!
        glDrawArrays(GL_TRIANGLES, 0, 36);
    
        // Update the SDL OpenGL window with the drawn elements.
        SDL_GL_SwapWindow(window);


        // Get the end time of the frame
        FrameTimeEnd = SDL_GetTicks();

        // Calculate the amount of time it took to run through 1 frame.
        FrameTimeTotal = FrameTimeEnd- FrameTimeStart;

        // Set the delay accordingly
        if(FrameDelay > FrameTimeTotal)
        {
            SDL_Delay(FrameDelay - FrameTimeTotal);
        }
    }

    // Free allocated memory of GameElement objects in the vector.
    for(int Index = 0; Index < GameElementVector.size();)
    {
        // Free the rest of the memory allocated to the GameElement objects assigned using smart pointers.
        GameElementVector[Index].reset();
        Index++;
    }

    // !!!
    cout << "Freed memory" << endl;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}