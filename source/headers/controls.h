#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

class Controls
{
    public:
    // Main control loop
        void RunControls();

        // Variable for handling input events. Only for inputs dependent on keyup!
        SDL_Event windowEvent;

    // Camera Controls
        // position
        glm::vec3 position = glm::vec3(0, 5, 0);

        float deltaTime;

        // Initial Field of View
        float FoV = 45.0f;

        float speed = 3.0f; // 3 units / second
        double mouseSpeed = 0.002;

        // Mouse position
        int MousePosX, MousePosY;

        // The view and projection matrices.
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;

        // Compute new orientation
    	double horizontalAngle;
    	double verticalAngle;

        glm::vec3 direction;
        glm::vec3 right;
    

        void ComputeMouseInput(SDL_Window *window);
        void GetPlayerMovementInput(const Uint8 *keyArray);


    // Control Options:

        // 0 = Full, 1 = Vertecies, 2 = points
        int RenderMode = 0;
        void ToggleRenderMode(SDL_Event windowEvent);

        // QUIT
        bool Running = true;
        void QuitGame(const Uint8 *keyArray);


};




#endif