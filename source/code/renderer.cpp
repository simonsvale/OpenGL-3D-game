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

        // New element
        glBindTexture(GL_TEXTURE_2D, GameElementVector[GameElementNumber]->Texture);
        glUseProgram(ShaderObjectVector[GameElementVector[GameElementNumber]->ShaderProgramIndex]->ShaderProgram);

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
      
        // Assign new values to vertex shader.
        int modelLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Object base color, should be texture.
        float ObjectC[] = {1.0f, 0.5f, 0.31f};
        int ObjectColorLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "objectColor");
        glUniform3f(ObjectColorLoc, ObjectC[0], ObjectC[1], ObjectC[2]);

        // Light color
        float LightC[] = {1.0f, 0.5f, 0.31f};
        int LightColorLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "lightColor");
        glUniform3f(LightColorLoc, LightC[0], LightC[1], LightC[2]);

        // Light position.
        int LightPosLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "lightPos");
        glUniform3f(LightPosLoc, 3.5f, 4.0f, 4.3f);

        int PlayerPosLoc = glGetUniformLocation(ShaderObjectVector[ShaderIndex]->ShaderProgram, "viewPos");
        glUniform3f(PlayerPosLoc, CameraPosition.x, CameraPosition.y, CameraPosition.z);


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
