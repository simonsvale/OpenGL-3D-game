// terminal command for running program: ../../main


// Include build-in libraries
#include <iostream>
#include <cmath>
#include <array>

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

#include "controls.h"

using namespace std;

// Window width and height
const int WIDTH = 1080, HEIGHT = 720;

float GroundLayer[] = {
    // Verticies                // Colors               // Texture
  -0.5f, 0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 1.0f,       
  -0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
   0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
   0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
   0.5f, 0.5f,-0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
  -0.5f, 0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 1.0f
};

int main(int argc, char **argv) 
{   
    // Object for options and controls
    Controls Controls;

    int Mode = 0 ;

    // !!!
    Renderer RenderObj;

    ArrayLevelMap MapObj;

    // !!!
    RenderObj.LoadArrmapFile("source/maps/myFirstMap.arrmap", &MapObj);

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

    // Do graphics
    Graphics GraphicsObj_1(&RenderObj.VertexVec[0], RenderObj.VertexVec.size());
    Graphics GraphicsObj_2(GroundLayer, sizeof(GroundLayer)/sizeof(GroundLayer[0]));

    // !!!
    Shader RedShader("source/shaders/basicVertexShader.GLSL", "source/shaders/redShader.GLSL");
    Shader RainbowShader("source/shaders/basicVertexShader.GLSL", "source/shaders/rainbowShader.GLSL");

    GLuint Texture;
    GLuint *TexturePtr = &Texture;
    GraphicsObj_1.LoadTexture(TexturePtr, &RedShader.ShaderProgram, "source/textures/debug3.png");

    GLuint Texture2;
    GLuint *TexturePtr2 = &Texture2;
    GraphicsObj_2.LoadTexture(TexturePtr2, &RainbowShader.ShaderProgram, "source/textures/mars_sand.png");

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
        glBindTexture(GL_TEXTURE_2D, Texture2);
        glUseProgram(RainbowShader.ShaderProgram);

        glm::vec3 scale2 = glm::vec3(30.0f, 30.0f, 30.0f);

        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, -13.0f, 0.0f));
        model2 = glm::scale(model2, scale2);

        // Assign new values to vertex shader.
        int modelLoc2 = glGetUniformLocation(RainbowShader.ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
        int viewLoc2 = glGetUniformLocation(RainbowShader.ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc2 = glGetUniformLocation(RainbowShader.ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw elements for obj_1
        glBindVertexArray(GraphicsObj_2.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // New element
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUseProgram(RedShader.ShaderProgram);

        // Rotate the model
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f));

        // The rotation of the cube.
        model = glm::rotate(model, float(SDL_GetTicks64()/2000.0) * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f)); 

        // Assign new values to vertex shader.
        int modelLoc = glGetUniformLocation(RedShader.ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(RedShader.ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(RedShader.ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        

        // Draw elements for obj_1
        glBindVertexArray(GraphicsObj_1.VAO);

        // Draw cube
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

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}