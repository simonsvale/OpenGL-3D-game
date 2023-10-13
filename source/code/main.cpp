// terminal command for running program: ../../main


// Include build-in libraries
#include <iostream>
#include <cmath>
#include <array>

// Include SDL2 and OpenGL headers
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Include own headers
#include "options.h"
#include "structures.h"
#include "spriteHandler.h"
#include "renderer.h"
#include "graphicsHandler.h"

using namespace std;

// Window width and height
const int WIDTH = 800, HEIGHT = 800;

// Vertecies to be rendered by OpenGL.
array<float, 32> Square = {
    // Verticies                // Colors               // Texture
  -0.5f,-0.5f, 0.0f,       1.0f,  0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.0f,       0.0f,  1.0f, 0.0f,         0.0f, 1.0f,
   0.5f, 0.5f, 0.0f,       0.0f,  0.0f, 1.0f,         1.0f, 1.0f,
   0.5f,-0.5f, 0.0f,       1.0f,  1.0f, 1.0f,         1.0f, 0.0f        
};

// Indices creating a square from two triangle vertecies.
array<unsigned int, 6> indices = {
    0, 2, 1,   // first triangle
    0, 3, 2    // second triangle.
}; 

// !!!
array<float, 32> Square2 = {
    // Verticies                // Colors               // Texture
  -1.0f,-1.0f, 0.0f,       1.0f,  0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.0f,       0.0f,  1.0f, 0.0f,         0.0f, 1.0f,
   1.0f, 1.0f, 0.0f,       0.0f,  0.0f, 1.0f,         1.0f, 1.0f,
   0.2f,-0.2f, 0.0f,       1.0f,  1.0f, 1.0f,         1.0f, 0.0f        
};

array<unsigned int, 6> indices2 = {
    0, 2, 1,   // first triangle
    0, 3, 2    // second triangle.
}; 

// Vertex shader
const char *myVertexShader = 
    "#version 410 core\n"
    "layout (location = 0) in vec3 aPos;\n"  
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTex;\n"
    "out vec3 ourColor;\n" 
    "out vec2 texCoord;\n"
    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos, 1.0);\n"
        "ourColor = aColor;\n"   
        "texCoord = aTex;"  
    "}\0";

// Fragment shader
const char *myFragmentShader = 
    "#version 410 core\n"
    "out vec4 FragColor;\n"  
    "in vec3 ourColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tex0;\n"
    "void main()\n"
    "{\n"
        "FragColor = texture(tex0, texCoord) * vec4(ourColor, 1.0);\n" // The string: "* vec4(ourColor, 1.0)" is applying the color shader ontop of the texture.
    "}\0";

int main(int argc, char **argv) 
{
    int Mode = 0 ;

    // !!!
    Options OptionsObj;
    Renderer RenderObj;

    // !!!
    Sprite SpriteObj_1("source/textures/dummy.atris");
    Sprite SpriteObj_2("source/textures/dummy2.atris");

    cout << "hello2" << endl;


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
    Graphics GraphicsObj_1(Square, indices);
    Graphics GraphicsObj_2(Square2, indices2);

    // Initialize vertex shader
    GLuint basic_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(basic_vertex_shader, 1, &myVertexShader, NULL);
    glCompileShader(basic_vertex_shader);
    

    // Initialize fragment shader
    GLuint basic_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(basic_fragment_shader, 1, &myFragmentShader, NULL);
    glCompileShader(basic_fragment_shader);


    // Start shader program with OpenGL, attach the shaders and link the program
    GLuint ShaderProgram = glCreateProgram();
    GLuint *ShaderProgramPtr = &ShaderProgram;

    glAttachShader(ShaderProgram, basic_fragment_shader);
    glAttachShader(ShaderProgram, basic_vertex_shader);

    SDL_Event windowEvent;

    GLuint Texture;
    GLuint *TexturePtr = &Texture;

    // Load texture (Working, but unfinished function).
    GraphicsObj_1.LoadTexture(TexturePtr, ShaderProgramPtr, "source/textures/debug3.png");

    GLuint Texture2;
    GLuint *TexturePtr2 = &Texture2;

    GraphicsObj_2.LoadTexture(TexturePtr2, ShaderProgramPtr, "source/textures/debug2.png");

    // Setup variables for maintaining 60 fps
    int FrameTimeTotal;
    Uint64 FrameTimeStart;
    Uint64 FrameTimeEnd;

    const int FrameDelay = 1000 / 60;

    bool Running = true;

    // Window loop
    while(Running == true)
    {   
        // Set frame start
        FrameTimeStart = SDL_GetTicks64();
        
        // Set viewport
        glViewport(0, 0, WIDTH, HEIGHT);

        int vertexColorLocation = glGetUniformLocation(ShaderProgram, "ourColor");

        while(SDL_PollEvent(&windowEvent) != 0)
        {   

            if (SDL_QUIT == windowEvent.type)
            {
                Running = false;
            }

            Mode = OptionsObj.ToggleRenderMode(Mode, windowEvent);

        }

        // background color
        glClearColor(0.03f, 0.1f, 0.24f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Use the previously specified shader program, bind the vertex array to VAO and bind the EBO to a OpenGL buffer.
        glUseProgram(ShaderProgram);
       

        //!!!
        glBindTexture(GL_TEXTURE_2D, Texture2);
        glBindVertexArray(GraphicsObj_2.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GraphicsObj_2.EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Draw elements for obj_1
        glBindTexture(GL_TEXTURE_2D, Texture);

        glBindVertexArray(GraphicsObj_1.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GraphicsObj_1.EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // Update the SDL OpenGL window.
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

