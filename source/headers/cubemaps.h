#ifndef CUBEMAPS_H
#define CUBEMAPS_H

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <array>

#include "shaderHandler.h"

class Cubemap
{
    public:
        void load_cubemap(array<string, 6> CubemapSidesPath);

        // Sets the active opengl texture, int is the GL_TEXTUREX
        void set_active_texture(int GLTextureSpace);

        GLuint CubemapTexture;
        GLuint FBO;
        
        // Texture size of each face.
        GLuint CUBEMAP_RES_W = 0;
        GLuint CUBEMAP_RES_H = 0;
        
        // Position of the cubemap.
        glm::vec3 CubePos;

        // Should be classes for themselves
        void create_reflection_cubemap(void);
        void render_reflection_framebuffer(Shader ReflectionShader);

};


// Skybox class
class Skybox: public Cubemap
{
    public:
        // Functions
        void render_skybox(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
        void set_shader_texture(int texture);

        // Variables
        GLuint SkyboxVAO;
        Shader SkyboxShader;

        // Constructor
        Skybox(): SkyboxShader("source/shaders/game/skybox/SkyboxVertexShader.glsl", "source/shaders/game/skybox/SkyboxFragmentShader.glsl")
        {               
            GLuint SkyboxVBO;

            float SkyboxVertices[] = { 
               -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
               -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
               -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 
                1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 
               -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 
                1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, 
               -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 
                1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 
               -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, 
                1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f
            };

            glGenBuffers(1, &SkyboxVBO);
            glBindBuffer(GL_ARRAY_BUFFER, SkyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), &SkyboxVertices, GL_STATIC_DRAW);

            // Set the skybox's VAO.
            glGenVertexArrays(1, &SkyboxVAO);
            glBindVertexArray(SkyboxVAO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Unbind buffers and array.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            
            // Free the memory for the skybox VBO, since it is now stored in the skybox VAO.
            glDeleteBuffers(1, &SkyboxVBO);
        }
};



#endif