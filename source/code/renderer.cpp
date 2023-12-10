#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;


void Renderer::RenderEverything(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, glm::mat4 projection, glm::mat4 view, glm::vec3 CameraPosition, SDL_Window *window, GameElement &DepthFBO, Shader &CubemapShader)
{   
    int ShaderIndex;

    // ################################################################################
    // Need a function call that sets static geometry, since most things do not need to get rotated, scaled and get a position each frame.

    // Send the diffuse and specular map to the fragment shader.
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    glUniform1i( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "diffuseTexture"), 0);
    glUniform1i( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "depthMap"), 1);

    RenderCubemaps(GameElementVector, CubemapShader, DepthFBO);

    
    // Take screen size instead.
    glViewport(0, 0, 1080, 720);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // How tf did i miss this...
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    float far_plane  = 25.0f;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, DepthFBO.depthCubemap);

    int viewLoc = glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


    // set lighting uniforms
    glUniform3f( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "lightPos"), 3.7f, 7.0f, 2.0f);
    
    int PlayerPosLoc = glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "viewPos");
    glUniform3f(PlayerPosLoc, CameraPosition.x, CameraPosition.y, CameraPosition.z);
    
    glUniform1f( glGetUniformLocation(ShaderObjectVector[0]->ShaderProgram, "far_plane"), far_plane);

    for(int GameElementNumber = 0; GameElementNumber < GameElementVector.size();)
    {   
        
        glm::mat4 model = glm::mat4(1.0f);

        // Get GameElement's shaderprogram index.
        ShaderIndex = GameElementVector[GameElementNumber]->ShaderProgramIndex;

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

    // Draw everything onto the program.
    SDL_GL_SwapWindow(window);
}


void Renderer::RenderCubemaps(vector<unique_ptr<GameElement> > &GameElementVector, Shader &Cubemap, GameElement &DepthFBO)
{   
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // A single light, should be a vector containing n light positions, and then the create n cubemaps from these positions.
    glm::vec3 lightPos(3.7f, 7.0f, 2.0f);

    // Distance
    float near_plane = 1.0f;
    float far_plane  = 25.0f;

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
    vector<glm::mat4> shadowTransforms;

    // Look at all four cardinal directions, and up, down. Basically just the sides of the cube the cubemap consists of.
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

    // Create the cubemap(s).
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO.FBO);

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(Cubemap.ShaderProgram);

    string matrix;
    for (unsigned int i = 0; i < 6;)
    {
        matrix = "shadowMatrices[" + std::to_string(i) + "]";
        glUniformMatrix4fv( glGetUniformLocation(Cubemap.ShaderProgram, matrix.c_str()), 1, GL_FALSE, &shadowTransforms[i][0][0]);
        i++;
    }

    glUniform1f( glGetUniformLocation(Cubemap.ShaderProgram, "far_plane"), far_plane );
    glUniform3f( glGetUniformLocation(Cubemap.ShaderProgram, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

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
        int modelLoc = glGetUniformLocation(Cubemap.ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // bind and draw
        glBindVertexArray(GameElementVector[GameElementNumber]->VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GameElementVector[GameElementNumber]->IBO);
        glDrawElements(GL_TRIANGLES, GameElementVector[GameElementNumber]->IndicesSize, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        GameElementNumber++;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}