
#include <iostream>

#include <SDL2/SDL.h>


#include "controls.h"

using namespace std;

void Camera::ComputeMouseInput(SDL_Window *window)
{

	// is called only once, the first time this function is called
	static float lastTime = SDL_GetTicks64()/1000.0;

	// Compute time difference between current and last frame
	float currentTime = SDL_GetTicks64()/1000.0;
	deltaTime = currentTime - lastTime;

	SDL_GetMouseState(&MousePosX, &MousePosY);

	// Reset mouse position for next frame
	SDL_WarpMouseInWindow(window, 600/2, 600/2);

	//cout << currentTime << endl;
	//cout << deltaTime << endl;
    //cout << "mouse: " << MousePosX << ", " << MousePosY << endl;

	horizontalAngle += mouseSpeed * float(600/2 - MousePosX);
	verticalAngle   += mouseSpeed * float(600/2 - MousePosY);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	direction2 = direction;
	
	// Right vector
	right = glm::vec3(sin(horizontalAngle - 3.14f/2.0f), 0, cos(horizontalAngle - 3.14f/2.0f));
	
	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Camera::GetMovementInput(SDL_Event windowEvent)
{	
	if((windowEvent.key.keysym.sym == SDLK_w) && (SDL_KEYDOWN == windowEvent.type))
    {
        position += direction2 * deltaTime * speed;
    }

	if((windowEvent.key.keysym.sym == SDLK_s) && (SDL_KEYDOWN == windowEvent.type))
    {
        position -= direction2 * deltaTime * speed;
    }

	if((windowEvent.key.keysym.sym == SDLK_d) && (SDL_KEYDOWN == windowEvent.type))
    {
        position += right * deltaTime * speed;
    }

	if((windowEvent.key.keysym.sym == SDLK_a) && (SDL_KEYDOWN == windowEvent.type))
    {
        position -= right * deltaTime * speed;
    }
}