#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;


void Renderer::RenderEverything(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, glm::mat4 projection, glm::mat4 view, glm::vec3 CameraPosition, SDL_Window *window, ShadowMap DepthMap, Skybox Sky, vector<unique_ptr<ReflectionProbe> > &ReflectionProbeVector)
{   
    int ShaderIndex;

    // ################################################################################
    // Need a function call that sets static geometry, since most things do not need to get rotated, scaled and get a position each frame.

    // Send the diffuse and specular map to the fragment shader.
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    // Set texture location / the uniform sampler
    ShaderObjectVector[0]->set_shader_texture(0, "diffuseTexture");
    ShaderObjectVector[0]->set_shader_texture(1, "depthMap");
    ShaderObjectVector[0]->set_shader_texture(2, "reflectionMap");

    DepthMap.render_depthmap(GameElementVector);

    // Take screen size instead.
    glViewport(0, 0, 1080, 720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    float far_plane  = 25.0f;

    // Set shadowmap and reflectionmap textures
    DepthMap.bind_active_texture(1);


    // 1. Check distance to nearest reflection probe.
    // 2. Assign reflection probe to texture 2.

    // !!! TEST
    // Calc dist
    double dist_1 = sqrt( 
        pow(CameraPosition.x - ReflectionProbeVector[0]->CubePos.x, 2) +
        pow(CameraPosition.y - ReflectionProbeVector[0]->CubePos.y, 2) +
        pow(CameraPosition.z - ReflectionProbeVector[0]->CubePos.z, 2)
    );

    double dist_2 = sqrt( 
        pow(CameraPosition.x - ReflectionProbeVector[1]->CubePos.x, 2) +
        pow(CameraPosition.y - ReflectionProbeVector[1]->CubePos.y, 2) +
        pow(CameraPosition.z - ReflectionProbeVector[1]->CubePos.z, 2)
    );

    cout << "Distance 1: " << dist_1 << "\nDistance 2:" << dist_2 << endl;

    if(dist_1 < dist_2)
    {
        ReflectionProbeVector[0]->bind_active_texture(2);
    }
    else
    {
        ReflectionProbeVector[1]->bind_active_texture(2);
    }
    // !!! TEST END.


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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render skybox
    Sky.render_skybox(view, projection);

    // Draw everything onto the program.
    SDL_GL_SwapWindow(window);
}



void Renderer::RenderCubemaps(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, ShadowMap DepthMap, Skybox Sky, vector<unique_ptr<ReflectionProbe> > &ReflectionProbeVector, bool SaveCubemap = false)
{   
    glUseProgram(ShaderObjectVector[0]->ShaderProgram);

    // Set texture location / the uniform sampler
    ShaderObjectVector[0]->set_shader_texture(0, "diffuseTexture");
    ShaderObjectVector[0]->set_shader_texture(1, "depthMap");
    ShaderObjectVector[0]->set_shader_texture(2, "reflectionMap");

    glUseProgram(0);

    // Create depthmap
    DepthMap.render_depthmap(GameElementVector);

    // Create cubemap
    for(int i = 0; i < ReflectionProbeVector.size();)
    {
        ReflectionProbeVector[i]->render_reflection_map(GameElementVector, ShaderObjectVector, DepthMap, Sky);
        i++;
    }

    // Save cubemap
    if(SaveCubemap == true)
    {
        ReflectionProbeVector[0]->cubemap_to_images();
    }
}