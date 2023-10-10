// terminal command for running program: ../../main


// Include build-in libraries
#include <iostream>
#include <cmath>

// Include SDL2 and OpenGL headers
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Include own headers
#include "options.h"
#include "structures.h"
#include "spriteHandler.h"
#include "renderer.h"

using namespace std;

// Window width and height
const int WIDTH = 800, HEIGHT = 800;

// Vertecies to be rendered by OpenGL.
float Square[] = {
    // Verticies                // Colors               // Texture
  -0.5f,-0.5f, 0.0f,       1.0f,  0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.0f,       0.0f,  1.0f, 0.0f,         0.0f, 1.0f,
   0.5f, 0.5f, 0.0f,       0.0f,  0.0f, 1.0f,         1.0f, 1.0f,
   0.5f,-0.5f, 0.0f,       1.0f,  1.0f, 1.0f,         1.0f, 0.0f        
};

float Square2[] = {
    // Verticies                // Colors               // Texture
  -0.5f,-0.5f, 0.0f,       1.0f,  0.0f, 0.0f,         0.0f, 0.0f,       
  -0.5f, 0.5f, 0.0f,       0.0f,  1.0f, 0.0f,         0.0f, 1.0f,
   0.5f, 0.5f, 0.0f,       0.0f,  0.0f, 1.0f,         1.0f, 1.0f,
   0.5f,-0.5f, 0.0f,       1.0f,  1.0f, 1.0f,         1.0f, 0.0f        
};

// Indices creating a square from two triangle vertecies.
unsigned int indices[] = {
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

    // Initialize VBO (Vertex Buffer Object)
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);

    // Bind the VBO to an OpenGL array Buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Write the data that the VBO should contain
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Square), Square, GL_STATIC_DRAW); // x * sizeof(Vertex_list), where x is the total amount of floats, and vertex_list is a list containing them.


    // Initialize VAO (Vertex Array Object)
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    // Initialize EBO (Element Buffer Objects)
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // EBO's can be used to make complex structures from vertecies, here we use the indicies list to store a square, created from 2 triangles.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 


    // Initialize vertex shader
    GLuint basic_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(basic_vertex_shader, 1, &myVertexShader, NULL);
    glCompileShader(basic_vertex_shader);
    

    // Initialize fragment shader
    GLuint basic_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(basic_fragment_shader, 1, &myFragmentShader, NULL);
    glCompileShader(basic_fragment_shader);


    // Start shader programme with OpenGL, and attach the shaders and link the program
    GLuint shader_program = glCreateProgram();
    GLuint *ShaderProgramPtr = &shader_program;

    glAttachShader(shader_program, basic_fragment_shader);
    glAttachShader(shader_program, basic_vertex_shader);

    SDL_Event windowEvent;


    GLuint Texture;
    GLuint *TexturePtr = &Texture;

    // Load texture (Working, but unfinished function).
    RenderObj.LoadTexture(TexturePtr, ShaderProgramPtr, "source/textures/debug3.png");

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

        int vertexColorLocation = glGetUniformLocation(shader_program, "ourColor");

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
        glUseProgram(shader_program);

        glBindTexture(GL_TEXTURE_2D, Texture);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Draw the 6 vertecies of the 2 triangles that makes up the square.
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

