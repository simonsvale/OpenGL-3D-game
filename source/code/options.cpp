#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

// Include own headers
#include "options.h"

using namespace std;

// DEBUG function
int Options::ToggleRenderMode(int Mode, SDL_Event windowEvent)
{
    if((windowEvent.key.keysym.sym == SDLK_p) && (SDL_KEYUP == windowEvent.type))
    {
        switch(Mode)
        {
            case 0:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                Mode++;
                break;
            } 
            case 1:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                Mode++;
                break;
            } 
            case 2:
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                Mode = 0;
                break;
            } 
        }
    }

    return Mode;
}