#include "cubemaps.h"

using namespace std;


void Cubemap::load_cubemap(array<string, 6> CubemapSidesPath)
{
    // Generate the framebuffer and the cubemap texture.
    glGenTextures(1, &CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);

    int Channels;

    // Assign a 2d texture to each side of the cubemap.
    for (unsigned int i = 0; i < 6;)
    {   
        unsigned char* ImageData = stbi_load(CubemapSidesPath[i].c_str(), &CUBEMAP_RES_W, &CUBEMAP_RES_H, &Channels, 0);
        if (ImageData)
        {
            // This is possible due to GL_TEXTURE_CUBE_MAP_POSITIVE_X's hex value being 1 int from every other side.
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
        }
        else
        {
            cout << "Could not load texture!" << endl;
        }

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

void Cubemap::bind_active_texture(GLuint GLTextureSpace)
{   
    // Works because GL_TEXTUREX is hex and 1 apart.
    glActiveTexture(GL_TEXTURE0 + GLTextureSpace);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
}



// WIP
void ReflectionProbe::render_reflection_framebuffer()
{   
    // Set the viewport size the framebuffer should render to.
    glViewport(0, 0, CUBEMAP_RES_W, CUBEMAP_RES_H);
    glBindFramebuffer(GL_FRAMEBUFFER, ReflectionMapFBO);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ReflectionShader.ShaderProgram);

    vector<glm::mat4> CubeSides;
    CubeSides.reserve(6);

    float CloseReflection = 10.0;
    float FarReflection = 25.0;
    glm::mat4 Cubeprojection = glm::perspective(glm::radians(90.0f), (float)CUBEMAP_RES_W / (float)CUBEMAP_RES_H, CloseReflection, FarReflection);

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
}

void ReflectionProbe::cubemap_to_texture(void)
{   
    cout << "what1" << endl;
    // Allocate memory for the png data.
    uint8_t *CubemapImageTexture = new uint8_t[CUBEMAP_RES_W * CUBEMAP_RES_H * 3];

    cout << "what" << endl;

    // Save cubemap to a temporary location, possibly by using ID's and only generating the cubemap if it does not already exist?
    GLenum err;
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, CUBEMAP_RES_W, CUBEMAP_RES_H, GL_RGB, GL_UNSIGNED_BYTE, CubemapImageTexture);

    while ( (err = glGetError()) != GL_NO_ERROR)
    {
        cout << "OpenGL Error: " << err << endl;
    }

    // Write to image texture.
    int WriteStatus = stbi_write_png("result.png", CUBEMAP_RES_W, CUBEMAP_RES_H, 3, CubemapImageTexture, CUBEMAP_RES_W * 3);
    if (WriteStatus == 0)
    {
        cout << "Could not write to cubemap image with name: " << 1 << endl;
    }

    cout << "freeing" << endl;

    // Free memory
    delete[] CubemapImageTexture;
}


void ReflectionProbe::set_reflection_FBO(void)
{   
    glGenFramebuffers(1, &ReflectionMapFBO);

    // Generate the framebuffer and the cubemap texture.
    glGenTextures(1, &CubemapTexture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);

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

    // Bind FBO and set the texture
    glBindFramebuffer(GL_FRAMEBUFFER, ReflectionMapFBO);

    // create the uniform depth buffer
    glGenRenderbuffers(1, &RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, CUBEMAP_RES_W, CUBEMAP_RES_H);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ReflectionMapFBO);


    // Unbind the cubemap texture and FBO
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}



void Skybox::render_skybox(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix)
{
    glDepthFunc(GL_LEQUAL);
    glUseProgram(SkyboxShader.ShaderProgram);

    ViewMatrix = glm::mat4( glm::mat3(ViewMatrix) );

    // Set view matrix and projection matrix for skybox.
    glUniformMatrix4fv( glGetUniformLocation(SkyboxShader.ShaderProgram, "view"), 1, GL_FALSE, &ViewMatrix[0][0] );
    glUniformMatrix4fv( glGetUniformLocation(SkyboxShader.ShaderProgram, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0] );

    glBindVertexArray(SkyboxVAO);

    bind_active_texture(0);

    // Draw the skybox and unbind the skybox VAO.
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
   
    glDepthFunc(GL_LESS);
}


void ShadowMap::set_depth_FBO(void)
{
    glGenFramebuffers(1, &DepthMapFBO);
    // create depth cubemap texture

    glGenTextures(1, &CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
    for (unsigned int i = 0; i < 6;)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        i++;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, CubemapTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::render_depthmap(vector<unique_ptr<GameElement> > &GameElementVector)
{   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // A single light, should be a vector containing n light positions, and then the create n cubemaps from these positions.
    glm::vec3 lightPos(3.7f, 7.0f, 2.0f);

    // Distance
    float near_plane = 1.0f;
    float far_plane  = 25.0f;

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)CUBEMAP_RES_W / (float)CUBEMAP_RES_H, near_plane, far_plane);
    vector<glm::mat4> shadowTransforms;

    // Look at all four cardinal directions, and up, down. Basically just the sides of the cube the cubemap consists of.
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    // Create the cubemap(s).
    glViewport(0, 0, CUBEMAP_RES_W, CUBEMAP_RES_H);
    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(DepthMapShader.ShaderProgram);

    string matrix;
    for (unsigned int i = 0; i < 6;)
    {
        matrix = "shadowMatrices[" + std::to_string(i) + "]";
        glUniformMatrix4fv( glGetUniformLocation(DepthMapShader.ShaderProgram, matrix.c_str()), 1, GL_FALSE, &shadowTransforms[i][0][0]);
        i++;
    }

    glUniform1f( glGetUniformLocation(DepthMapShader.ShaderProgram, "far_plane"), far_plane );
    glUniform3f( glGetUniformLocation(DepthMapShader.ShaderProgram, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

    // Render the scene
    for(int GameElementNumber = 0; GameElementNumber < GameElementVector.size();)
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(
            GameElementVector[GameElementNumber]->WorldPosition[0], 
            GameElementVector[GameElementNumber]->WorldPosition[1], 
            GameElementVector[GameElementNumber]->WorldPosition[2]
        ));

        // Set GameElement model rotation around x, y, z, in degrees.
        model = glm::rotate(model, glm::degrees(GameElementVector[GameElementNumber]->Rotation[0]), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::degrees(GameElementVector[GameElementNumber]->Rotation[1]), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::degrees(GameElementVector[GameElementNumber]->Rotation[2]), glm::vec3(0, 0, 1));

        // Set GameElement model scale.
        model = glm::scale(model, glm::vec3(
            GameElementVector[GameElementNumber]->Scale[0], 
            GameElementVector[GameElementNumber]->Scale[1], 
            GameElementVector[GameElementNumber]->Scale[2]
        ));
        

        // Set cubemap shader.
        glUniformMatrix4fv( glGetUniformLocation(DepthMapShader.ShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // bind and draw
        glBindVertexArray(GameElementVector[GameElementNumber]->VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GameElementVector[GameElementNumber]->IBO);
        glDrawElements(GL_TRIANGLES, GameElementVector[GameElementNumber]->IndicesSize, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        GameElementNumber++;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
