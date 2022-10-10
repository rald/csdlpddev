#include <SDL2/SDL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(void) {

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );

	SDL_Rect rect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );		
	SDL_RenderFillRect( renderer, &rect );

  SDL_RenderPresent( renderer );

	SDL_Delay(10000);

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	
	window = NULL;
	renderer = NULL;

  SDL_Quit();


	return 0;
}
