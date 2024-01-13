
#include <iostream>

#include <SDL2/SDL.h>
#include <glad/glad.h>


#include "controls.h"

using namespace std;

glm::vec3 up;

void Controls::ComputeMouseInput(SDL_Window *window)
{
	// is called only once, the first time this function is called
	static double lastTime = SDL_GetTicks64()/1000.0;

	// Compute the time between the last frame.
	double currentTime = SDL_GetTicks64()/1000.0;
	deltaTime = currentTime - lastTime;

    //cout << "Delta: " << deltaTime << endl;

	// Get the mouse coordinates using SDL2.
	SDL_GetMouseState(&MousePosX, &MousePosY);

    //cout << MousePosX << ", " << MousePosY << endl;

	// Reset mouse to the middle of the screen 
	// WIP
	SDL_WarpMouseInWindow(window, 1080/2, 720/2);

	horizontalAngle += mouseSpeed * (1080/2 - MousePosX);
	verticalAngle   += mouseSpeed * (720/2 - MousePosY);

    //cout << "HozA: " << horizontalAngle << endl;
    //cout << "MusX: " << MousePosX << endl;


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	
	// Right vector
	right = glm::vec3(sin(horizontalAngle - 3.14f/2.0f), 0, cos(horizontalAngle - 3.14f/2.0f));

    //cout << direction.x << ", " << direction.y << ", " << direction.z << endl;
    //cout << right.x << ", " << right.y << ", " << right.z << endl;
	
	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Projection matrix : 45° Field of View,              Aspect ratio, "render distance" like, where 0.1 is close and 100.0 is far.
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 1080.0f / 720.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );


	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Controls::GetPlayerMovementInput(const Uint8 *keyArray)
{	
    if(keyArray[SDL_SCANCODE_LSHIFT])
    {
        // Triple speed
        speed = 15.0f;
    }
    else
    {
        // default speed
        speed = 3.0f;
    }

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


void Controls::ToggleRenderMode(SDL_Event windowEvent)
{
    if((windowEvent.key.keysym.sym == SDLK_p) && (SDL_KEYUP == windowEvent.type))
    {
        switch(RenderMode)
        {
            case 0:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                RenderMode++;
                break;
            } 
            case 1:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                RenderMode++;
                break;
            } 
            case 2:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                RenderMode = 0;
                break;
            } 
        }
    }
}


void Controls::QuitGame(const Uint8 *keyArray)
{
    // QUIT if escape
    if(keyArray[SDL_SCANCODE_ESCAPE])
    {
        Running = false;
    }
}

// Main control loop
void Controls::RunControls()
{	
	// Needed to get keystate
    SDL_PumpEvents();

    // Get keystate
    const Uint8 *keyArray = SDL_GetKeyboardState(NULL);

	// Do controls
	GetPlayerMovementInput(keyArray);

	QuitGame(keyArray);


    // To handle one keypress, SDL_PollEvent is used.
    while(SDL_PollEvent(&windowEvent) != 0)
    {   
        // All methods dependent on key up press.
        ToggleRenderMode(windowEvent);  
    }
 
}