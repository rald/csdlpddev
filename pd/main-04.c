#include <SDL/SDL.h>



#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "common.h"

#define UTIL_IMPLEMENTATION
#include "util.h"

#include "palette.h"

#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

#define BITMAP_IMPLEMENTATION
#include "bitmap.h"

#define FONT_IMPLEMENTATION
#include "font.h"



#define DEFAULT_FILE "default.bmp"
#define FONT_FILE "font.txt"



enum Surface {
    SURFACE_NONE=0,
    SURFACE_CLOSE,
    SURFACE_PALETTE,
    SURFACE_THUMB,
    SURFACE_GRID,
    SURFACE_MAX
};

enum Surface drag=SURFACE_NONE;
enum Surface active=SURFACE_NONE;

void drawFrame(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {

    for(int i=0;i<w;i++) {
        Graphics_SetPixel(srf,x+i,y, c);
        Graphics_SetPixel(srf,x+i,y+h-1,c);
    }

    for(int j=0;j<h;j++) {
        Graphics_SetPixel(srf,x, y+j,c);
        Graphics_SetPixel(srf,x+w-1,y+j,c);
    }

}

int rect_button(SDL_Surface *srf,int n,int x,int y,int w,int h,int c,bool b) {

    static int hold = -1;

    int r=-1;

    if(hold==-1) {
        if(mb==1) {
            if(inRect(mx,my,x,y,w,h)) {
                hold=n;	
            }
        }
    } else {
        if(mb==0) {
            r=hold;
            hold=-1;
        }
    }

    if(b || hold==n) {
        Graphics_FillRect(srf,x,y,w,h,pal[0]);
        Graphics_DrawRect(srf,x+2,y+2,w-4,h-4,pal[12]);
        Graphics_FillRect(srf,x+3,y+3,w-6,h-6,c);
    } else {
        Graphics_FillRect(srf,x,y,w,h,c);
        Graphics_DrawRect(srf,x,y,w,h,pal[12]);
    }

    return r;

}

int circ_button(SDL_Surface *srf,int n,int x,int y,int radius,int c,bool b) {

    static int hold = -1;

    int r=-1;

    if(hold==-1) {
        if(mb==1) {
            if(inCirc(mx,my,x,y,radius)) {
                hold=n;	
            }
        }
    } else {
        if(mb==0) {
            r=hold;
            hold=-1;
        }
    }

    if(b || hold==n) {
        Graphics_FillCircle(srf,x,y,radius,pal[0]);
        Graphics_FillCircle(srf,x,y,radius-4,c);
        Graphics_DrawCircle(srf,x,y,radius-2,pal[12]);
    } else {
        Graphics_FillCircle(srf,x,y,radius,pal[0]);
        Graphics_FillCircle(srf,x,y,radius-2,c);
        Graphics_DrawCircle(srf,x,y,radius,pal[12]);
    }

    return r;

}

bool close_button(SDL_Surface *srf,int n,int x,int y,int r) {

    static bool hold=false;

    bool res=false;

    circ_button(srf,n,x,y,r,pal[2],false);

    if(!hold) {
        if(mb==1) {
            if(inCirc(mx,my,x,y,r)) {
                hold=true;
            }
        }
    } else {
        if(mb==0) {
            res=true;
            hold=false;
        }
    }

    return res;
}

int palette(SDL_Surface *srf,int n,int *x,int *y,int s,int *c) {

    static int dx=0,dy=0;

    int r=-1;

    int w=8*s;
    int h=2*s;

    for(int k=0;k<16;k++) {
        int i=k%8;
        int j=k/8;

        if(mb==1 && inRect(mx,my,*x+i*s,*y+j*s,s,s)) {
            *c=k;	
        }

        r=rect_button(srf,n+k,*x+i*s,*y+j*s,s,s,pal[k],*c==k);
    }

    Graphics_DrawRect(srf,*x-1,*y-1,w+2,h+2,pal[0]);
    Graphics_DrawRect(srf,*x-2,*y-2,w+4,h+4,pal[12]);

    if(mb==4) {
        if(drag==SURFACE_NONE) {
            if(inRect(mx,my,*x,*y,w,h)) {
                dx=*x-mx;
                dy=*y-my;
                drag=active;
            }
        } else if(drag==SURFACE_PALETTE) {
            *x=mx+dx;
            *y=my+dy;

            if(*x<4) *x=4;
            if(*y<4) *y=4;
            if(*x>sw-w-4) *x=sw-w-4;
            if(*y>sh-h-4) *y=sh-h-4;
        }
    } else {
        if(mb==0) {
            drag=SURFACE_NONE;
        }
    }

    return r;
}

void grid(SDL_Surface *srf,Bitmap *b,int n,int *x,int *y,int s,int z,int t,int c) {

    static bool draw=false;
    static int dx=0,dy=0;
    static int px=0,py=0;

    int gw=b->w*s;
    int gh=b->h*s;

    for(int j=0;j<b->h;j++) {
        int y0=*y+j*s;
        if(y0<-s) continue;
        if(y0>=sh) break;								
        for(int i=0;i<b->w;i++) {
            int x0=*x+i*s;
            if(x0<-s) continue;
            if(x0>=sw) break;
            int k=b->p[j*b->w+i];
            if(k!=t) Graphics_FillRect(srf,x0,y0,s,s,pal[k]);
        }
    }

    Graphics_DrawRect(srf,*x-1,*y-1,gw+2,gh+2,pal[c]);
    Graphics_DrawRect(srf,*x-2,*y-2,gw+4,gh+4,pal[12]);

    if(active==SURFACE_GRID) {
        if(mb==1) {
            if(inRect(mx,my,*x,*y,gw,gh)) {
                int nx=(mx-*x)/s;
                int ny=(my-*y)/s;	
                if(!draw) {
                    draw=true;
                } else {
                    Bitmap_DrawLine(b,px,py,nx,ny,z);
                }
                px=nx;
                py=ny;
            }
        } else if(mb==0) {
            draw=false;
        }
    }
    
    if(mb==4) {
        if(drag==SURFACE_NONE) {
            if(inRect(mx,my,*x,*y,gw,gh)) {
                dx=*x-mx;
                dy=*y-my;
                drag=active;
            }
        } else if(drag==SURFACE_GRID) {
            *x=mx+dx;
            *y=my+dy;
        }
    } else {
        if(mb==0) {
            drag=SURFACE_NONE;
        }
    }
}

void thumb(SDL_Surface *srf,Bitmap *b,int n,int *x,int *y,int s,int t,int c) {

    static int dx=0,dy=0;

    int gw=b->w*s;
    int gh=b->h*s;

    for(int j=0;j<b->h;j++) {
        for(int i=0;i<b->w;i++) {
            int k=b->p[j*b->w+i];
            if(k!=t) Graphics_FillRect(srf,*x+i*s,*y+j*s,s,s,pal[k]);
        }
    }

    Graphics_DrawRect(srf,*x-1,*y-1,gw+2,gh+2,pal[c]);
    Graphics_DrawRect(srf,*x-2,*y-2,gw+4,gh+4,pal[12]);

    if(mb==4) {
        if(drag==SURFACE_NONE) {
            if(inRect(mx,my,*x,*y,gw,gh)) {
                dx=*x-mx;
                dy=*y-my;
                drag=active;
            }
        } else if(drag==SURFACE_THUMB) {
            *x=mx+dx;
            *y=my+dy;
        }
    } else {
        if(mb==0) {
            drag=SURFACE_NONE;
        }
    }
}

int main( int argc, char* args[] )
{
    SDL_Surface *screen = NULL;
    SDL_Event *ev = NULL;

    SDL_Surface *bitmap = NULL;

    Bitmap *b = NULL;

    int bw=16,bh=16,bt=-1;

    int cx=sw-8-6,cy=8+2,cr=8;

    int ps=16,px=5,py=sh-ps*2-6,pi=0;

    int gs=8,gx=5,gy=cr*2+8;

    int ts=1,tx=sw-bw*ts-5,ty=cr*2+8;

    Font *fnt=NULL;

    if(SDL_Init( SDL_INIT_VIDEO ) == -1) {
        printf("%s\n",SDL_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode( sw, sh, sb, SDL_SWSURFACE );

    if(screen==NULL) {
        printf("%s\n",SDL_GetError());
        return 1;
    }

    if(exists(DEFAULT_FILE)) {
        bitmap=Graphics_LoadImage(DEFAULT_FILE);
        b=Bitmap_CreateFromSurface(bitmap,npal,pal);
    } else {
        b=Bitmap_New(bw,bh);
    }

    if(exists(FONT_FILE)) {
        fnt=Font_New(FONT_FILE);
    }

    bool quit=false;

    while(!quit) {

        SDL_PollEvent(ev);

        mb=SDL_GetMouseState(&mx,&my);
        
        Graphics_FillRect(screen,0,0,sw-1,sh-1,pal[0]);

        if(inCirc(mx,my,cx,cy,cr)) active=SURFACE_CLOSE;
        else if(inRect(mx,my,px,py,ps*8,ps*2)) active=SURFACE_PALETTE;
        else if(inRect(mx,my,tx,ty,bw*ts,bh*ts)) active=SURFACE_THUMB;
        else if(inRect(mx,my,gx,gy,bw*gs,bh*gs)) active=SURFACE_GRID;
        
        grid(screen,b,4*100,&gx,&gy,gs,pi,bt,0);
        
        thumb(screen,b,3*100,&tx,&ty,ts,bt,0);

        palette(screen,2*100,&px,&py,ps,&pi);

        if(close_button(screen,1*100,cx,cy,cr)) {
            Bitmap_Save(b,DEFAULT_FILE,npal,pal,screen);
            quit=true;
        }

        Font_DrawText(screen,fnt,2,2,1,"Pixel Dancer",pal[12]);

        drawFrame(screen,0,0,sw-1,sh-1,pal[3]);

        SDL_Flip(screen);
    }	

    SDL_Delay(1000);

    SDL_Quit();

    return 0;
}


