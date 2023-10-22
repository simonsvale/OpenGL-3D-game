#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    public:
        // position
        glm::vec3 position = glm::vec3( 0, 0, 5 );

        // horizontal angle : toward -Z
        float horizontalAngle = 3.14f;

        // vertical angle : 0, look at the horizon
        float verticalAngle = 0.0f;

        // Initial Field of View
        float FoV = 45.0f;

        float speed = 3.0f; // 3 units / second
        float mouseSpeed = 0.005f;

        int MousePosX, MousePosY;

        // The view and projection matrices.
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;

        void ComputeMouseInput(SDL_Window *window);
};




#endif