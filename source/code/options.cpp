// Include SDL2 headers
#include <SDL2/SDL.h>

// Include Opengl headers
#include <glad/glad.h>


class Options
{
    public:

    void ToggleRenderMode(int Mode, SDL_Event windowEvent)
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
    }





};