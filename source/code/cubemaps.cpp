#include "cubemaps.h"

using namespace std;

void Cubemap::create_reflection_cubemap(void)
{
    // Generate the framebuffer and the cubemap texture.
    glGenTextures(1, &Texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

    // Assign a 2d texture to each side of the cubemap.
    for (unsigned int i = 0; i < 6;)
    {
        // This is possible due to GL_TEXTURE_CUBE_MAP_POSITIVE_X's hex value being 1 int from every other side.
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Bind the cubemap texture.
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Texture, 0);

    // Unbind the cubemap texture 
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::render_reflection_framebuffer(Shader ReflectionShader)
{   
    // Set the viewport size the framebuffer should render to.
    glViewport(0, 0, CUBEMAP_RES_H, CUBEMAP_RES_W);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ReflectionShader.ShaderProgram);

    vector<glm::mat4> CubeSides;
    CubeSides.reserve(6);

    float CloseReflection = 0.1;
    float FarReflection = 25.0;
    glm::mat4 Cubeprojection = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, CloseReflection, FarReflection);

    // Look at all four cardinal directions, and up, down. Basically just the sides of the cube the cubemap consists of.
    CubeSides.push_back(Cubeprojection * glm::lookAt(CubePos, CubePos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    CubeSides.push_back(Cubeprojection * glm::lookAt(CubePos, CubePos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    CubeSides.push_back(Cubeprojection * glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    CubeSides.push_back(Cubeprojection * glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    CubeSides.push_back(Cubeprojection * glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    CubeSides.push_back(Cubeprojection * glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    string ReflectionMatrix;
    for (unsigned int i = 0; i < 6;)
    {
        ReflectionMatrix = "CubeSides[" + std::to_string(i) + "]";
        glUniformMatrix4fv( glGetUniformLocation(ReflectionShader.ShaderProgram, ReflectionMatrix.c_str()), 1, GL_FALSE, &CubeSides[i][0][0]);
        i++;
    }

    // Steps:

    // Precompute cubemaps as textures and store them somewhere.


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void Cubemap::load_cubemap(void)
{
    // Generate the framebuffer and the cubemap texture.
    glGenTextures(1, &Texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

    int ImageWidth;
    int ImageHeight;
    int Channels;

    // Assign a 2d texture to each side of the cubemap.
    for (unsigned int i = 0; i < 6;)
    {   
        unsigned char* ImageData = stbi_load(CubemapPath[i].c_str(), &ImageWidth, &ImageHeight, &Channels, 0);

        // This is possible due to GL_TEXTURE_CUBE_MAP_POSITIVE_X's hex value being 1 int from every other side.
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);

        stbi_image_free(ImageData);
        i++;
    }
    
    // Setup parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}