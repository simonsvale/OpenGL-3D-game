#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    public:
        // position
        glm::vec3 position = glm::vec3(0, 0, 5);

        float deltaTime;

        // Initial Field of View
        float FoV = 45.0f;

        float speed = 3.0f; // 3 units / second
        float mouseSpeed = 0.005f;

        // Mouse position
        int MousePosX, MousePosY;

        // The view and projection matrices.
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;

        // Compute new orientation
    	float horizontalAngle;
    	float verticalAngle;

        glm::vec3 direction2;
        glm::vec3 right;

        void ComputeMouseInput(SDL_Window *window);
        void GetMovementInput(SDL_Event windowEvent);
};




#endif