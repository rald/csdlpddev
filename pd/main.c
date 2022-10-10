#include <SDL2/SDL.h>

#include <stdbool.h>



#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240



bool quit=false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;


SDL_Event event;

SDL_Rect rect;

double x[3],y[3];

double f=0;


int main(void) {

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT );

	SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );


  while(!quit) {

    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT: quit=true; break;
        case SDL_KEYDOWN: 
          switch(event.key.keysym.sym) {
            case SDLK_ESCAPE: quit=true; break;
          }
        break;
      }
    }

    x[0]=100*cos(f/7)+SCREEN_WIDTH/2;
    y[0]=100*cos(f/11)+SCREEN_HEIGHT/2;
    x[1]=100*cos(f/13)+SCREEN_WIDTH/2;
    y[1]=100*cos(f/17)+SCREEN_HEIGHT/2;
    x[2]=100*cos(f/19)+SCREEN_WIDTH/2;
    y[2]=100*cos(f/23)+SCREEN_HEIGHT/2;

	  SDL_SetRenderTarget( renderer, texture );

  	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x10 );		
  	rect = (SDL_Rect) {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
   	SDL_RenderFillRect( renderer, &rect );

  	SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );		
  	rect = (SDL_Rect) {x[0],y[0],16,16};
   	SDL_RenderFillRect( renderer, &rect );

  	SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );		
  	rect = (SDL_Rect) {x[1],y[1],16,16};
   	SDL_RenderFillRect( renderer, &rect );

  	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF );		
  	rect = (SDL_Rect) {x[2],y[2],16,16};
   	SDL_RenderFillRect( renderer, &rect );

		SDL_SetRenderTarget( renderer, NULL );

  	SDL_RenderCopy( renderer, texture, NULL, NULL );

    f++;

    SDL_RenderPresent( renderer );

    SDL_Delay(1000/60);

  }

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_DestroyTexture( texture );
	
	window = NULL;
	renderer = NULL;
	texture = NULL;

  SDL_Quit();



	return 0;
}
