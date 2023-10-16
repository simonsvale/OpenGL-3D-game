// terminal command for running program: ../../main


// Include build-in libraries
#include <iostream>
#include <cmath>
#include <array>

// Include SDL2 and OpenGL headers
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Header file for 3D camera.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include own headers
#include "options.h"
#include "structures.h"
#include "spriteHandler.h"
#include "renderer.h"
#include "graphicsHandler.h"
#include "shaderHandler.h"

using namespace std;

// Window width and height
const int WIDTH = 800, HEIGHT = 800;

// Vertecies to be rendered by OpenGL.
array<float, 32> Square = {
    // Verticies                // Colors               // Texture
  -0.5f,-0.5f, 0.0f,       1.0f, 0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
   0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
   0.5f,-0.5f, 0.0f,       1.0f, 1.0f, 1.0f,         1.0f, 0.0f        
};

array<float, 288> Cube = {
    // Verticies                // Colors               // Texture
  -0.5f,-0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f,-0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
   0.5f, 0.5f,-0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
   0.5f, 0.5f,-0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
   0.5f,-0.5f,-0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 0.0f,
  -0.5f,-0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 0.0f,  

  -0.5f,-0.5f, 0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
   0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
   0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
   0.5f,-0.5f, 0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 0.0f,
  -0.5f,-0.5f, 0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 0.0f,  

  -0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,         1.0f, 0.0f,       
  -0.5f,-0.5f, 0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
  -0.5f,-0.5f,-0.5f,       0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
  -0.5f,-0.5f,-0.5f,       0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
  -0.5f, 0.5f,-0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,         1.0f, 0.0f,  

   0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,         1.0f, 0.0f,       
   0.5f,-0.5f, 0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
   0.5f,-0.5f,-0.5f,       0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
   0.5f,-0.5f,-0.5f,       0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
   0.5f, 0.5f,-0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
   0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 0.0f,         1.0f, 0.0f,  

  -0.5f,-0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 1.0f,       
  -0.5f,-0.5f, 0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
   0.5f,-0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
   0.5f,-0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
   0.5f,-0.5f,-0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
  -0.5f,-0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 1.0f,  

  -0.5f, 0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 1.0f,       
  -0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
   0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
   0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
   0.5f, 0.5f,-0.5f,       1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
  -0.5f, 0.5f,-0.5f,       1.0f, 0.0f, 0.0f,         0.0f, 1.0f  
};

// Indices creating a square from two triangle vertecies.
array<unsigned int, 6> indices = {
    0, 2, 1,   // first triangle
    0, 3, 2    // second triangle.
}; 

// !!!
array<float, 32> Square2 = {
    // Verticies                // Colors               // Texture
  -1.0f,-1.0f, 0.0f,       1.0f, 0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
   1.0f, 1.0f, 0.0f,       0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
   0.2f,-0.2f, 0.0f,       1.0f, 1.0f, 1.0f,         1.0f, 0.0f        
};

array<unsigned int, 6> indices2 = {
    0, 2, 1,   // first triangle
    0, 3, 2    // second triangle.
}; 


int main(int argc, char **argv) 
{
    int Mode = 0 ;

    // !!!
    Options OptionsObj;
    Renderer RenderObj;

    // !!!
    Sprite SpriteObj_1("source/textures/dummy.atris");
    Sprite SpriteObj_2("source/textures/dummy2.atris");


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
        std::cout << "Could not create window" << SDL_GetError() << std::endl;
        return 1;
    }

    // OpenGL context.
    SDL_GLContext GL_context;
    GL_context = SDL_GL_CreateContext(window);

    // OpenGL function pointers.
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    if (NULL == GL_context)
    {
        std::cout << "Could not create OpenGL context" << SDL_GetError() << std::endl;
        return 1; 
    }

    // Do graphics
    Graphics GraphicsObj_1(Cube, indices);
    //Graphics GraphicsObj_2(Square2, indices2);

    // !!!
    Shader RedShader("source/shaders/basicVertexShader.GLSL", "source/shaders/redShader.GLSL");
    Shader RainbowShader("source/shaders/basicVertexShader.GLSL", "source/shaders/rainbowShader.GLSL");

    GLuint Texture;
    GLuint *TexturePtr = &Texture;
    GraphicsObj_1.LoadTexture(TexturePtr, &RedShader.ShaderProgram, "source/textures/debug3.png");

    GLuint Texture2;
    GLuint *TexturePtr2 = &Texture2;
    //GraphicsObj_2.LoadTexture(TexturePtr2, &RainbowShader.ShaderProgram, "source/textures/debug.png");

    // !!!
    glEnable(GL_DEPTH_TEST);  


    // Setup variables for maintaining 60 fps
    int FrameTimeTotal;
    Uint64 FrameTimeStart;
    Uint64 FrameTimeEnd;

    const int FrameDelay = 1000 / 60;

    bool Running = true;
    SDL_Event windowEvent;

    // Window loop
    while(Running == true)
    {   
        // Set frame start
        FrameTimeStart = SDL_GetTicks64();
        
        while(SDL_PollEvent(&windowEvent) != 0)
        {   

            if (SDL_QUIT == windowEvent.type)
            {
                Running = false;
            }

            Mode = OptionsObj.ToggleRenderMode(Mode, windowEvent);

        }


        // Set viewport.
        glViewport(0, 0, WIDTH, HEIGHT);

        // background color
        glClearColor(0.03f, 0.1f, 0.24f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // !!!
        int vertexColorLocation = glGetUniformLocation(RainbowShader.ShaderProgram, "ourColor");

        /*
        glUseProgram(RainbowShader.ShaderProgram);

        glBindTexture(GL_TEXTURE_2D, Texture2);
        glBindVertexArray(GraphicsObj_2.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GraphicsObj_2.EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        */

        // 3D
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, Texture);
        glUseProgram(RedShader.ShaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;
        
        // The placement of the object
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f)); 

        // Rotate the model
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.5f, 0.0f, 0.0f));

        // The "camera", the first float is how close things will be rendered (Cannot be zero, because math), the last float is the max distance objects will be rendered.
        projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.0001f, 100.0f);

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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GraphicsObj_1.EBO);

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

