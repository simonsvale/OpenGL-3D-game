#ifndef CUBEMAPS_H
#define CUBEMAPS_H

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

        void create_reflection_cubemap(void);

        void render_reflection_framebuffer(Shader ReflectionShader);

        void load_cubemap(void);




};



#endif