#ifndef GRAPHICS_H
#define GRAPHICS_H



#include <SDL/SDL.h>



Uint32 Graphics_GetPixel( SDL_Surface *srf, int x, int y );
void Graphics_SetPixel( SDL_Surface *srf, int x, int y, Uint32 pixel );
void Graphics_DrawPoint( SDL_Surface *srf, int x, int y, Uint32 pixel );
void Graphics_DrawLine(SDL_Surface *srf,int x0,int y0,int x1,int y1,Uint32 c);
void Graphics_DrawRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c);
void Graphics_FillRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c);
void Graphics_DrawCircle(SDL_Surface *srf,int x0,int y0,int r,Uint32 c);
void Graphics_FillCircle(SDL_Surface *srf,int x0,int y0,int r,Uint32 c);


SDL_Surface *Graphics_LoadImage( char *filename );
SDL_Surface *Graphics_CreateSurface(int w,int h,SDL_Surface *screen);

void Graphics_ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);

#ifdef GRAPHICS_IMPLEMENTATION



Uint32 Graphics_GetPixel( SDL_Surface *srf, int x, int y ) {
    Uint32 r=0;
    if(x>=0 && x<srf->w && y>=0 && y<srf->h) {
        //Convert the pixels to 32 bit
        Uint32 *pixels = (Uint32 *)srf->pixels;

        //Get the requested pixel
        r=pixels[ ( y * srf->w ) + x ];
    }

    return r;
}



void Graphics_SetPixel( SDL_Surface *srf, int x, int y, Uint32 pixel ) {
    if(x>=0 && x<srf->w && y>=0 && y<srf->h) {
        //Convert the pixels to 32 bit
        Uint32 *pixels = (Uint32 *)srf->pixels;

        //Set the pixel
        pixels[ ( y * srf->w ) + x ] = pixel;
    }
}



void Graphics_DrawPoint( SDL_Surface *srf, int x, int y, Uint32 pixel ) {
    if(x>=1 && x<srf->w-1 && y>=1 && y<srf->h-1) {
        Graphics_SetPixel(srf,x,y,pixel);
    }
}

void Graphics_DrawLine(SDL_Surface *srf,int x0,int y0,int x1,int y1,Uint32 c) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x=x0;
    int y=y0;
    int sx=x0>x1?-1:1;
    int sy=y0>y1?-1:1;
    int err;
    if(dx > dy) {
        err = dx / 2.0;
        while(x != x1) {
            Graphics_DrawPoint(srf, x, y, c);
            err -= dy;
            if(err < 0) {
                y += sy;
                err += dx;
            }
            x += sx;
        }
    } else {
        err = dy / 2.0;
        while(y != y1) {
            Graphics_DrawPoint(srf, x, y, c);
            err -= dx;
            if(err < 0) {
                x += sx;
                err += dy;
            }
            y += sy;
        }    
    }
    Graphics_DrawPoint(srf, x, y, c);
}

void Graphics_DrawRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {

    for(int i=0;i<w;i++) {
        Graphics_DrawPoint(srf,x+i,y, c);
        Graphics_DrawPoint(srf,x+i,y+h-1,c);
    }

    for(int j=0;j<h;j++) {
        Graphics_DrawPoint(srf,x, y+j,c);
        Graphics_DrawPoint(srf,x+w-1,y+j,c);
    }

}

void Graphics_FillRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {
    for(int j=0;j<h;j++) {
        int y0=y+j;
        if(y0<0) continue;
        if(y0>=sh) break;
        for(int i=0;i<w;i++) {
            int x0=x+i;
            if(x0<0) continue;
            if(x0>=sw) break;
            Graphics_DrawPoint(srf,x0,y0,c);
        }
    }
}

void Graphics_DrawCircle(SDL_Surface *srf,int x0,int y0,int r,Uint32 c) {
    int f=1-r;
    int ddfx=0;
    int ddfy=-2*r;
    int x=0;
    int y=r;

    Graphics_DrawPoint(srf,x0,y0+r,c);
    Graphics_DrawPoint(srf,x0,y0-r,c);
    Graphics_DrawPoint(srf,x0+r,y0,c);
    Graphics_DrawPoint(srf,x0-r,y0,c);

    while(x<y) {
        if(f>=0) {
            y--;
            ddfy+=2;
            f+=ddfy;
        }
        x+=1;
        ddfx+=2;
        f+=ddfx+1;

        Graphics_DrawPoint(srf,x0+x,y0+y,c);
        Graphics_DrawPoint(srf,x0-x,y0+y,c);
        Graphics_DrawPoint(srf,x0+x,y0-y,c);
        Graphics_DrawPoint(srf,x0-x,y0-y,c);
        Graphics_DrawPoint(srf,x0+y,y0+x,c);
        Graphics_DrawPoint(srf,x0-y,y0+x,c);
        Graphics_DrawPoint(srf,x0+y,y0-x,c);
        Graphics_DrawPoint(srf,x0-y,y0-x,c);
    }
}

void Graphics_FillCircle(SDL_Surface *srf,int x0,int y0,int r,Uint32 c) {
    int f=1-r;
    int ddfx=0;
    int ddfy=-2*r;
    int x=0;
    int y=r;

    Graphics_DrawLine(srf,x0,y0+r,x0,y0-r,c);
    Graphics_DrawLine(srf,x0,y0-r,x0,y0+r,c);
    Graphics_DrawLine(srf,x0+r,y0,x0-r,y0,c);
    Graphics_DrawLine(srf,x0-r,y0,x0+r,y0,c);

    while(x<y) {
        if(f>=0) {
            y--;
            ddfy+=2;
            f+=ddfy;
        }
        x+=1;
        ddfx+=2;
        f+=ddfx+1;

        Graphics_DrawLine(srf,x0+x,y0+y,x0-x,y0+y,c);
        Graphics_DrawLine(srf,x0-x,y0+y,x0+x,y0+y,c);
        Graphics_DrawLine(srf,x0+x,y0-y,x0-x,y0-y,c);
        Graphics_DrawLine(srf,x0-x,y0-y,x0+x,y0-y,c);
        Graphics_DrawLine(srf,x0+y,y0+x,x0-y,y0+x,c);
        Graphics_DrawLine(srf,x0-y,y0+x,x0+y,y0+x,c);
        Graphics_DrawLine(srf,x0+y,y0-x,x0-y,y0-x,c);
        Graphics_DrawLine(srf,x0-y,y0-x,x0+y,y0-x,c);
    }
}

SDL_Surface *Graphics_LoadImage( char *filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = SDL_LoadBMP( filename );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );



        //If the surface was optimized
/*
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
*/
    }

    //Return the optimized surface
    return optimizedImage;
}


SDL_Surface *Graphics_CreateSurface(int w,int h,SDL_Surface *screen) {
    /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
       as expected by OpenGL for textures */
    SDL_Surface *surface=NULL;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, screen->format->BitsPerPixel,
                                   screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }

    return surface;
}

void Graphics_ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


#endif /* GRAPHICS_IMPLEMENTATION */




#endif /* GRAPHICS_H */
