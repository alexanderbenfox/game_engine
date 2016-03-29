#include <SDL2/SDL.h>
#include "graphics.h"
#include "conf.h"

//constructor
Graphics::Graphics()
{
	//int SDL_CreateWindowAndRenderer(int            width,
        //                        int            height,
        //                        Uint32         window_flags,
        //pointer to window pointer          SDL_Window**   window,
        //pointer to renderer pointer        SDL_Renderer** renderer)
	SDL_CreateWindowAndRenderer(640, 480, 0, &this->_window, &this->_renderer);
	//void SDL_SetWindowTitle(SDL_Window* window, const char* title)
	SDL_SetWindowTitle(this->_window, "Game");
}

//destructor
Graphics::~Graphics()
{
	SDL_DestroyWindow(this->_window);
}
