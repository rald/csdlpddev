#ifndef BITMAP_H
#define BITMAP_H



#include <SDL/SDL.h>

#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

#define UTIL_IMPLEMENTATION
#include "util.h"

typedef struct Bitmap Bitmap;

struct Bitmap {
    int w,h;
    int *p;
};



Bitmap *Bitmap_New(int w,int h);
Bitmap *Bitmap_CreateFromSurface(SDL_Surface *srf,int np,Uint32 *p);
Bitmap *Bitmap_Load(char *path,int np,Uint32 p[]);
int Bitmap_Save(Bitmap *b,char *path,int n,Uint32 *p,SDL_Surface *screen);

void Bitmap_DrawPoint(Bitmap *b, int x, int y, int c );
void Bitmap_DrawLine(Bitmap *b,int x0,int y0,int x1,int y1,int c);
#ifdef BITMAP_IMPLEMENTATION 



Bitmap *Bitmap_New(int w,int h) {
    Bitmap *b=malloc(sizeof(Bitmap));
    if(b) {
        b->w=w;
        b->h=h;
        b->p=malloc(sizeof(int)*(w*h));
        for(int i=0;i<w*h;i++) b->p[i]=12;
    }	
    return b;
}

void Bitmap_Free(void *bitmap) {
	free(((Bitmap*)bitmap)->p);
	free(((Bitmap*)bitmap));
	bitmap=NULL;
}


Bitmap *Bitmap_CreateFromSurface(SDL_Surface *srf,int np,Uint32 *p) {
    Bitmap *b=malloc(sizeof(Bitmap));
    if(b) {
        b->w=srf->w;
        b->h=srf->h;
        b->p=malloc(sizeof(*(b->p))*(b->w*b->h));
        if(b->p) {
            for(int j=0;j<b->h;j++) {
                for(int i=0;i<b->w;i++) {
                    Uint32 k=mycolor(Graphics_GetPixel(srf,i,j),np,p);
                    b->p[j*b->w+i]=k;
                }
            }
        }
    }	
    return b;
}

Bitmap *Bitmap_Load(char *path,int np,Uint32 p[]) {
    SDL_Surface *srf=Graphics_LoadImage(path);
    return Bitmap_CreateFromSurface(srf,np,p);
}

int Bitmap_Save(Bitmap *b,char *path,int n,Uint32 *p,SDL_Surface *screen) {
    int res=0; 
    SDL_Surface *srf=Graphics_CreateSurface(b->w,b->h,screen);
    if(srf) {
        for(int j=0;j<b->h;j++)
            for(int i=0;i<b->w;i++)
                Graphics_SetPixel(srf,i,j,p[b->p[j*b->w+i]]);
        res=SDL_SaveBMP(srf,path);
        SDL_FreeSurface(srf);
        srf=NULL;
    }
    return res;		
}

void Bitmap_DrawPoint(Bitmap *b, int x, int y, int c ) {
    if(x>=0 && x<b->w && y>=0 && y<b->h) {
        b->p[y*b->w+x]=c;
    }
}

void Bitmap_DrawLine(Bitmap *b,int x0,int y0,int x1,int y1,int c) {
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
            Bitmap_DrawPoint(b, x, y, c);
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
            Bitmap_DrawPoint(b, x, y, c);
            err -= dx;
            if(err < 0) {
                x += sx;
                err += dy;
            }
            y += sy;
        }    
       }
    Bitmap_DrawPoint(b, x, y, c);
}



#endif /* BITMAP_IMPLEMENTATION */



#endif /* BITMAP_H */
