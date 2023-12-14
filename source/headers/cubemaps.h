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
        GLuint Texture;
        GLuint FBO;

        array<string, 6> CubemapPath;
        
        // Texture size of each face.
        GLuint CUBEMAP_RES_W = 1024;
        GLuint CUBEMAP_RES_H = 1024;
        
        // Position of the cubemap.
        glm::vec3 CubePos;

        // Should be classes for themselves
        void create_reflection_cubemap(void);
        void render_reflection_framebuffer(Shader ReflectionShader);

        void load_cubemap(void);
};


// Skybox class
class Skybox: public Cubemap
{
    public:
        Shader SkyboxShader;

        void render_skybox(glm::mat4 *ViewMatrix, glm::mat4 *ProjectionMatrix);

        Skybox(): SkyboxShader("source/shaders/game/skybox/SkyboxVertexShader.glsl", "source/shaders/game/skybox/SkyboxFragmentShader.glsl")
        {               
            // Rework into function
            glGenVertexArrays(1, &SkyboxVAO);
            glBindVertexArray(SkyboxVAO);
            
            // Vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
            glEnableVertexAttribArray(0);
            
            // VAO Texture coords attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(48*sizeof(float)) );
            glEnableVertexAttribArray(1);


            float SkyboxVertices[72] = {-1, 1, -1, 1, 1, 1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, -1, -1, 1, -1, -1, -1, 1, 1, -1, 1, -1, 1, 1, -1, -1, -1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1};
            float SkyboxTextureCoords[48] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            float SkyboxIndices[36] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 1, 3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16};


            // Initialize VBO and IBO.
            glGenBuffers(1, &SkyboxVBO);

            // Bind the VBO to an OpenGL array Buffer.
            glBindBuffer(GL_ARRAY_BUFFER, SkyboxVBO);

            glBufferData(GL_ARRAY_BUFFER, 72*sizeof(float) + 48*sizeof(float), 0, GL_STATIC_DRAW);

            // Buffer subdata.
            glBufferSubData(GL_ARRAY_BUFFER, 0, 72*sizeof(float), SkyboxVertices);
            glBufferSubData(GL_ARRAY_BUFFER, 72*sizeof(float), 48*sizeof(float), SkyboxTextureCoords);

            // IBO
            glGenBuffers(1, &SkyboxIBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SkyboxIBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(unsigned int), SkyboxIndices, GL_STATIC_DRAW);

        }

    private:
        GLuint SkyboxVAO;
        GLuint SkyboxVBO;
        GLuint SkyboxIBO;

};




#endif