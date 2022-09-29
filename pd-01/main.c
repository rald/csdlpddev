#include "SDL/SDL.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>



int sw=320;
int sh=240;
int sb=32;

Uint32 pal[]={
	0x001a1c2cL,
	0x005d275dL,
	0x00b13e53L,
	0x00ef7d57L,
	0x00ffcd75L,
	0x00a7f070L,
	0x0038b764L,
	0x00257179L,
	0x0029366fL,
	0x003b5dc9L,
	0x0041a6f6L,
	0x0073eff7L,
	0x00f4f4f4L,
	0x0094b0c2L,
	0x00566c86L,
	0x00333c57L,
};

int mx=0,my=0,mb=0;

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

typedef struct Bitmap Bitmap;

struct Bitmap {
	int w,h;
	int *p;
};

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

bool inRect(int x,int y,int rx,int ry,int rw,int rh) {
	return x>=rx && x<rx+rw && y>=ry && y<ry+rh;
}

Uint32 getPixel( SDL_Surface *srf, int x, int y )
{
	Uint32 r=0;
	if(x>=0 && x<srf->w && y>=0 && y<srf->h) {
	    //Convert the pixels to 32 bit
	    Uint32 *pixels = (Uint32 *)srf->pixels;

	    //Get the requested pixel
	    r=pixels[ ( y * srf->w ) + x ];
	}

	return r;
}

void setPixel( SDL_Surface *srf, int x, int y, Uint32 pixel )
{
	if(x>=0 && x<srf->w && y>=0 && y<srf->h) {
	    //Convert the pixels to 32 bit
	    Uint32 *pixels = (Uint32 *)srf->pixels;

	    //Set the pixel
	    pixels[ ( y * srf->w ) + x ] = pixel;
    }
}

void drawPoint( SDL_Surface *srf, int x, int y, Uint32 pixel ) {
	if(x>=1 && x<srf->w && y>=1 && y<srf->h) {
		setPixel(srf,x,y,pixel);
	}
}

void drawRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {

	for(int i=0;i<w;i++) {
		drawPoint(srf,x+i,y,    c);
		drawPoint(srf,x+i,y+h-1,c);
	}

	for(int j=0;j<h;j++) {
		drawPoint(srf,x,    y+j,c);
		drawPoint(srf,x+w-1,y+j,c);
	}

}

void fillRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {
	for(int j=0;j<h;j++) {
		for(int i=0;i<w;i++) {
			drawPoint(srf,x+i,y+j,c);
		}
	}
}

void Bitmap_DrawPoint(Bitmap *b, int x, int y, Uint32 c ) {
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

char *GFX_LoadFile(char *path) {
    char *source = NULL;

    FILE *fp = fopen(path, "r");

    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            source = malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if (newLen == 0) {
                fprintf(stderr,"Error: cannot read file %s", path);
                free(source);
                source = NULL;
            } else {
                source[++newLen] = '\0';
            }
        }
        fclose(fp);
    } else {
        perror(path);
    }
    
    return source;
}

int GFX_SaveFile( char *path, char *source, size_t length ) {
    FILE *fp=fopen(path,"w");

    if(fp==NULL) {
        perror(path);
        return 1;
    }

    if(fwrite(source,sizeof(char),length,fp)<length) {
        perror(path);
        return 2;
    }

    return 0;
}

int button(SDL_Surface *srf,int n,int x,int y,int w,int h,int c,bool b) {

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
		fillRect(srf,x,y,w,h,pal[12]);
		fillRect(srf,x+2,y+2,w-4,h-4,c);
		drawRect(srf,x+2,y+2,w-4,h-4,pal[0]);
	} else {
		fillRect(srf,x,y,w,h,c);
		drawRect(srf,x,y,w,h,pal[0]);
	}

	return r;

}

bool close_button(SDL_Surface *srf,int n,int x,int y,int w,int h) {

	static bool hold=false;

	bool res=false;

	button(srf,n,x,y,w,h,pal[2],false);

	if(!hold) {
		if(mb==1) {
			if(inRect(mx,my,x,y,w,h)) {
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

		r=button(srf,n+k,*x+i*s,*y+j*s,s,s,pal[k],*c==k);
	}


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
			if(k!=t) fillRect(srf,x0,y0,s,s,pal[k]);
		}
	}

	drawRect(srf,*x-1,*y-1,gw+2,gh+2,pal[c]);

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
			if(k!=t) fillRect(srf,*x+i*s,*y+j*s,s,s,pal[k]);
		}
	}

	drawRect(srf,*x-1,*y-1,gw+2,gh+2,pal[c]);

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


	int bw=16,bh=16,bt=-1;

	int cx=sw-16-4,cy=4,cw=16,ch=16;

	int ps=16,px=4,py=sh-ps*2-4,pi=0;

	int gs=8,gx=5,gy=ch+4+5;

	int ts=2,tx=sw-bw*ts-5,ty=ch+4+5;

	Bitmap *b=Bitmap_New(bw,bh);

    if(SDL_Init( SDL_INIT_VIDEO ) == -1) {
		printf("%s\n",SDL_GetError());
		return 1;
    }

    screen = SDL_SetVideoMode( sw, sh, sb, SDL_SWSURFACE );

	if(screen==NULL) {
		printf("%s\n",SDL_GetError());
		return 1;
	}

	bool quit=false;

	while(!quit) {

		SDL_PollEvent(ev);

		mb=SDL_GetMouseState(&mx,&my);
		
		fillRect(screen,0,0,sw,sh,pal[12]);

		if(inRect(mx,my,cx,cy,cw,ch)) active=SURFACE_CLOSE;
		else if(inRect(mx,my,px,py,ps*8,ps*2)) active=SURFACE_PALETTE;
		else if(inRect(mx,my,tx,ty,bw*ts,bh*ts)) active=SURFACE_THUMB;
		else if(inRect(mx,my,gx,gy,bw*gs,bh*gs)) active=SURFACE_GRID;
		
		grid(screen,b,4*100,&gx,&gy,gs,pi,bt,0);
		
		thumb(screen,b,3*100,&tx,&ty,ts,bt,0);

		palette(screen,2*100,&px,&py,ps,&pi);

		if(close_button(screen,1*100,cx,cy,cw,ch)) {
			quit=true;
		}

		drawRect(screen,0,0,sw,sh,pal[3]);

		SDL_Flip(screen);
	}	

	SDL_Delay(1000);

    SDL_Quit();

    return 0;
}


