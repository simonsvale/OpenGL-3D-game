
#include <iostream>

#include <SDL2/SDL.h>


#include "controls.h"

using namespace std;

void Camera::ComputeMouseInput(SDL_Window *window)
{
	// is called only once, the first time this function is called
	static double lastTime = SDL_GetTicks64()/1000.0;

	// Compute the time between the last frame.
	double currentTime = SDL_GetTicks64()/1000.0;
	deltaTime = currentTime - lastTime;

	// Get the mouse coordinates using SDL2.
	SDL_GetMouseState(&MousePosX, &MousePosY);

	// Reset mouse to the middle of the screen 
	// WIP
	SDL_WarpMouseInWindow(window, 600/2, 600/2);

	horizontalAngle += mouseSpeed * float(600/2 - MousePosX);
	verticalAngle   += mouseSpeed * float(600/2 - MousePosY);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	
	// Right vector
	right = glm::vec3(sin(horizontalAngle - 3.14f/2.0f), 0, cos(horizontalAngle - 3.14f/2.0f));
	
	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Camera::GetMovementInput(const Uint8 *keyArray)
{	

    if(keyArray[SDL_SCANCODE_W])
    {
        position += direction * deltaTime * speed;
    }

	if(keyArray[SDL_SCANCODE_S])
    {
        position -= direction * deltaTime * speed;
    }

	if(keyArray[SDL_SCANCODE_D])
    {
        position += right * deltaTime * speed;
    }
	
	if(keyArray[SDL_SCANCODE_A])
    {
        position -= right * deltaTime * speed;
    }

}