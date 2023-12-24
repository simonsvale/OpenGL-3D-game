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
#include <memory> 

#include <STB/stb_image_write.h>

#include "shaderHandler.h"
#include "gameElementHandler.h"

// Cubemap super class
class Cubemap
{
    public:
        void load_cubemap(array<string, 6> CubemapSidesPath);

        // Sets the active opengl texture, int is the GL_TEXTUREX
        void bind_active_texture(GLuint GLTextureSpace);

        GLuint CubemapTexture;
        
        // Texture size of each face.
        GLuint CUBEMAP_RES_W = 0;
        GLuint CUBEMAP_RES_H = 0;
        
        // Position of the cubemap.
        glm::vec3 CubePos;
};


// Skybox class
class Skybox: public Cubemap
{
    public:
        // Functions
        void render_skybox(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);

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


class ShadowMap: public Cubemap
{
    public:
        /** Renders a depth map, using scene geometry, currently only from a single light source.
         *  Sets a framebuffer containing the depth map to a variable `GLuint DepthMapFBO` by refrence.
         * @param std::vector<std::unique_ptr<GameElement>> &GameElementVector, Models in the scene.
         *  
        **/
        void render_depthmap(vector<unique_ptr<GameElement> > &GameElementVector);

        /** Create a framebuffer and a texture, and set the framebuffer to the 
         *  variable `GLuint DepthMapFBO` and the texture to the variable `GLuint CubemapTexture`, by reference. 
        **/
        void set_depth_FBO(void);

        Shader DepthMapShader;
        GLuint DepthMapFBO;

        ShadowMap(GLuint Width = 1024, GLuint Height = 1024): DepthMapShader("source/shaders/simpleDepthVert.glsl", "source/shaders/simpleDepthFrag.glsl", "source/shaders/simpleDepthGeom.glsl")
        {
            CUBEMAP_RES_W = Width;
            CUBEMAP_RES_H = Height;
            set_depth_FBO();
        }

};


class ReflectionProbe: public Cubemap
{
    public:
        // Should be classes for themselves
        void set_reflection_FBO(void);
        void render_reflection_framebuffer(Shader ReflectionShader);
        void framebuffer_to_texture(void);

        GLuint ReflectionMapFBO;
        GLuint RenderBuffer;

};



#endif