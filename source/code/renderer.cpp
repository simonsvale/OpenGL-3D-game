#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;


void Renderer::RenderEverything(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, glm::mat4 projection, glm::mat4 view, glm::vec3 CameraPosition, SDL_Window *window, ShadowMap DepthMap, Skybox Sky, ReflectionProbe Refl)
{   
    int ShaderIndex;

    // ################################################################################
    // Need a function call that sets static geometry, since most things do not need to get rotated, scaled and get a position each frame.

    // Send the diffuse and specular map to the fragment shader.
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    // Set texture location / the uniform sampler
    ShaderObjectVector[0]->set_shader_texture(0, "diffuseTexture");
    ShaderObjectVector[0]->set_shader_texture(1, "depthMap");
    ShaderObjectVector[0]->set_shader_texture(2, "skybox");

    DepthMap.render_depthmap(GameElementVector);

    // Take screen size instead.
    glViewport(0, 0, 1080, 720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // How tf did i miss this...
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    float far_plane  = 25.0f;

    DepthMap.bind_active_texture(1);

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

        // Set reflection texture
        Sky.bind_active_texture(2);
        //Refl.bind_active_texture(2);

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

    // Render skybox
    Sky.render_skybox(view, projection);

    // Draw everything onto the program.
    SDL_GL_SwapWindow(window);
}



void Renderer::RenderCubemaps(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, glm::mat4 projection, glm::mat4 view, glm::vec3 CameraPosition, SDL_Window *window, ShadowMap DepthMap, Skybox Sky, ReflectionProbe Refl)
{   
    int ShaderIndex;

    // Send the diffuse and specular map to the fragment shader.
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    // Set texture location / the uniform sampler
    ShaderObjectVector[0]->set_shader_texture(0, "diffuseTexture");
    ShaderObjectVector[0]->set_shader_texture(1, "depthMap");
    ShaderObjectVector[0]->set_shader_texture(2, "skybox");

    DepthMap.render_depthmap(GameElementVector);

    // Now create the cubemap ---------------------------------------------------------------
    Refl.render_reflection_framebuffer();

    // Set texture location / the uniform sampler
    Refl.ReflectionShader.set_shader_texture(0, "diffuseTexture");
    Refl.ReflectionShader.set_shader_texture(1, "depthMap");
    Refl.ReflectionShader.set_shader_texture(2, "skybox");

    DepthMap.bind_active_texture(1);

    int viewLoc = glGetUniformLocation(Refl.ReflectionShader.ShaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(Refl.ReflectionShader.ShaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


    // set lighting uniforms
    glUniform3f( glGetUniformLocation(Refl.ReflectionShader.ShaderProgram, "lightPos"), 3.7f, 7.0f, 2.0f);
    
    int PlayerPosLoc = glGetUniformLocation(Refl.ReflectionShader.ShaderProgram, "viewPos");
    glUniform3f(PlayerPosLoc, CameraPosition.x, CameraPosition.y, CameraPosition.z);
    
    float far_plane  = 25.0f;
    glUniform1f( glGetUniformLocation(Refl.ReflectionShader.ShaderProgram, "far_plane"), far_plane);


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

        // Assign new values to vertex shader.
        int modelLoc = glGetUniformLocation(Refl.ReflectionShader.ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GameElementVector[GameElementNumber]->DiffuseTexture);

        // Set reflection texture
        Sky.bind_active_texture(2);
        //Refl.bind_active_texture(2);

        // Bind GameElement VAO.
        glBindVertexArray(GameElementVector[GameElementNumber]->VAO);

        // Bind IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GameElementVector[GameElementNumber]->IBO);

        // Draw the VBO stored in the VAO, by using the IBO.
        glDrawElements(GL_TRIANGLES, GameElementVector[GameElementNumber]->IndicesSize, GL_UNSIGNED_INT, 0);

        GameElementNumber++;
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Refl.cubemap_to_images();

    SDL_GL_SwapWindow(window);
}