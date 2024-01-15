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
        unsigned char* ImageBytes = stbi_load(CubemapSidesPath[i].c_str(), &CUBEMAP_RES_W, &CUBEMAP_RES_H, &Channels, 0);

        if (ImageBytes)
        {   
            // Depending on if the .png image has an alpha channel, include that channel in the load.
            switch (Channels)
            {
                case 3:
                {
                    // This is possible due to GL_TEXTURE_CUBE_MAP_POSITIVE_X's hex value being 1 int from every other side.
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageBytes);
                    break;
                }
                case 4:
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageBytes);
                    break;
                }
            }
        }
        else
        {
            cout << "Could not load texture!" << endl;
        }

        stbi_image_free(ImageBytes);
        i++;
    }
    
    // Setup parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::create_cubemap_texture(void)
{
    glGenTextures(1, &CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);

    // Create all six faces of the cubemap.
    for (unsigned int i = 0; i < 6;)
    { 
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, CUBEMAP_RES_W, CUBEMAP_RES_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        i++;
    }

    // Setup parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind cubemap texture.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void Cubemap::cubemap_to_images(void)
{   
    // Allocate memory for the png data.
    unsigned char *CubemapFacePixels = new unsigned char[CUBEMAP_RES_W * CUBEMAP_RES_H * 3];

    // Bind to the cubemap's texture.
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int FaceNumber = 0; FaceNumber < 6;)
    {   
        // Since a cubemap cannot contain an alpha channel when its extracted, only extract the RGB channels.
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + FaceNumber, 0, GL_RGB, GL_UNSIGNED_BYTE, CubemapFacePixels);

        // Create a variable and set the different face names using sprintf, since cout did not work.
        char Cube2DSide[34];
        sprintf(Cube2DSide, "source/textures/cubemaps/Face%i.png", FaceNumber);

        // Write to texture to image.
        int WriteStatus = stbi_write_png(Cube2DSide, CUBEMAP_RES_W, CUBEMAP_RES_H, 3, CubemapFacePixels, CUBEMAP_RES_W * 3);
        if (WriteStatus == 0)
        {
            cout << "Could not write 2D texture, for face: " << FaceNumber << endl;
        }

        FaceNumber++;
    }

    // Unbind the cubemap.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Free memory
    delete[] CubemapFacePixels;
}

void Cubemap::bind_active_texture(GLuint GLTextureSpace)
{   
    // Works because GL_TEXTUREX is hex and 1 apart.
    glActiveTexture(GL_TEXTURE0 + GLTextureSpace);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
}


// WIP
void ReflectionProbe::render_reflection_map(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, ShadowMap DepthMap, Skybox Sky, SDL_Window *window)
{   
    // Set viewport
    glViewport(0, 0, CUBEMAP_RES_W, CUBEMAP_RES_H);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)CUBEMAP_RES_W / (float)CUBEMAP_RES_H, 1.0f, 25.0f);
    vector<glm::mat4> CubemapTransforms;

    // Look at all four cardinal directions, and up, down. Basically just the sides of the cube the cubemap consists of.
    CubemapTransforms.push_back(glm::lookAt(CubePos, CubePos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    CubemapTransforms.push_back(glm::lookAt(CubePos, CubePos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    CubemapTransforms.push_back(glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    CubemapTransforms.push_back(glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    CubemapTransforms.push_back(glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    CubemapTransforms.push_back(glm::lookAt(CubePos, CubePos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, ReflectionMapFBO);

    for(int FaceNumber = 0; FaceNumber < 6;)
    {       
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + FaceNumber, CubemapTexture, 0);

        // Render skybox
        Sky.render_skybox(CubemapTransforms[FaceNumber], ProjectionMatrix);

        // Send the diffuse and specular map to the fragment shader.
        glUseProgram(ShaderObjectVector[0]->ShaderProgram);

        DepthMap.bind_active_texture(1);

        glUniformMatrix4fv( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(CubemapTransforms[FaceNumber]));
        glUniformMatrix4fv( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

        // set lighting uniforms
        glUniform3f( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "lightPos"), 3.7f, 7.0f, 2.0f);
        glUniform3f( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "viewPos"), CubePos.x, CubePos.y, CubePos.z);
        float far_plane  = 25.0f;
        glUniform1f( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "far_plane"), far_plane);

        for(int GameElementNumber = 0; GameElementNumber < GameElementVector.size();)
        {   
            int ShaderIndex = GameElementVector[GameElementNumber]->ShaderProgramIndex;
            
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

            // Assign new values to vertex shader.
            int modelLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, GameElementVector[GameElementNumber]->DiffuseTexture);

            // Bind GameElement VAO.
            glBindVertexArray(GameElementVector[GameElementNumber]->VAO);

            // Bind IBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GameElementVector[GameElementNumber]->IBO);

            // Draw the VBO stored in the VAO, by using the IBO.
            glDrawElements(GL_TRIANGLES, GameElementVector[GameElementNumber]->IndicesSize, GL_UNSIGNED_INT, 0);

            GameElementNumber++;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);

        FaceNumber++;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Set normal screen viewport ! rework later so it does not use constants.
    glViewport(0, 0, 1080, 720);
}


void ReflectionProbe::set_reflection_FBO(void)
{   
    glGenFramebuffers(1, &ReflectionMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ReflectionMapFBO);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenRenderbuffers(1, &RenderBuffer);
    glBindRenderbuffer(1, RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, CUBEMAP_RES_W, CUBEMAP_RES_H);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, ReflectionMapFBO);
}


void Skybox::render_skybox(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix)
{
    glDepthFunc(GL_LEQUAL);
    glUseProgram(SkyboxShader.ShaderProgram);

    // Bind to the skybox texture.
    bind_active_texture(0);

    ViewMatrix = glm::mat4( glm::mat3(ViewMatrix) );

    // Set view matrix and projection matrix for skybox.
    glUniformMatrix4fv( glGetUniformLocation(SkyboxShader.ShaderProgram, "view"), 1, GL_FALSE, &ViewMatrix[0][0] );
    glUniformMatrix4fv( glGetUniformLocation(SkyboxShader.ShaderProgram, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0] );

    glBindVertexArray(SkyboxVAO);

    // Draw the skybox and unbind the skybox VAO.
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
   
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
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, CubemapTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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
