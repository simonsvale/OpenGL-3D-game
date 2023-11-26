#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.h"
#include "shaderHandler.h"
#include "gameElementHandler.h"

using namespace std;


void Renderer::RenderEverything(vector<unique_ptr<GameElement> > &GameElementVector, vector< unique_ptr<Shader> > &ShaderObjectVector, glm::mat4 projection, glm::mat4 view, glm::vec3 CameraPosition, SDL_Window *window)
{   
    int ShaderIndex;

    // ################################################################################
    // Need a function call that sets static geometry, since most things do not need to get rotated, scaled and get a position each frame.
    // Use EBO again!

    for(int GameElementNumber = 0; GameElementNumber < GameElementVector.size();)
    {   
        glm::mat4 model = glm::mat4(1.0f);

        // Get GameElement's shaderprogram index.
        ShaderIndex = GameElementVector[GameElementNumber]->ShaderProgramIndex;

        // Bind diffuse and specular textures.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GameElementVector[GameElementNumber]->DiffuseTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GameElementVector[GameElementNumber]->SpecularTexture);

        // Set shader program
        glUseProgram(ShaderObjectVector[GameElementVector[GameElementNumber]->ShaderProgramIndex]->ShaderProgram);

        // Send the diffuse and specular map to the fragment shader.
        int DiffuseLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "material.DiffuseMap");
        glUniform1i(DiffuseLoc, 0);

        int SpecularLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "material.SpecularMap");
        glUniform1i(SpecularLoc, 1);

        // Set material shine value.
        int ShineLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "material.ShineValue");
        glUniform1f(ShineLoc, GameElementVector[GameElementNumber]->Material.ShineValue);

        // If(GameElementVector[GameElementNumber]->Type != STATIC)
        // #define STATIC 1
        
        // Set GameElement model world position.
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


        // Player position for calculating specular.
        int PlayerPosLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "viewPos");
        glUniform3f(PlayerPosLoc, CameraPosition.x, CameraPosition.y, CameraPosition.z);
        
        float time  = (SDL_GetTicks()/3500.0);

        // Light position.
        int LightDirLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.position");
        glUniform3f(LightDirLoc, 10.0f, 15.0f * sin(time), 20.0f * cos(time)); // The direction the light is pointing. (The sun)

        int ConstantLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.constant");
        int LinearLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.linear");
        int QuadraticLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.quadratic");

        glUniform1f(ConstantLoc, 1.0f);
        glUniform1f(LinearLoc, 0.022f);
        glUniform1f(QuadraticLoc, 0.0019f);

        // Light ambient color
        float LightC[] = {0.08, 0.04, 0.008};
        int LightAmbientLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.ambient");
        glUniform3f(LightAmbientLoc, LightC[0], LightC[1], LightC[2]);

        // Light Diffuse.
        int LightDiffuseLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.diffuse");
        glUniform3f(LightDiffuseLoc, 0.812, 0.404 * sin(time), 0.082 + 0.3 * sin(time));

        // Light specular.
        int LightSpecularLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "light.specular");
        glUniform3f(LightSpecularLoc, 0.812, 0.404, 0.082);


        // Assign new values to vertex shader.
        int modelLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


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
