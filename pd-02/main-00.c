#include "SDL/SDL.h"

#include <stdbool.h>

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

int drag=0;
int dx=0,dy=0;

bool inrect(int x,int y,int rx,int ry,int rw,int rh) {
	return x>=rx && x<rx+rw && y>=ry && y<ry+rh;
}

Uint32 get_pixel( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void set_pixel( SDL_Surface *srf, int x, int y, Uint32 pixel )
{
	if(x>=0 && x<srf->w && y>=0 && y<srf->h) {
	    //Convert the pixels to 32 bit
	    Uint32 *pixels = (Uint32 *)srf->pixels;

	    //Set the pixel
	    pixels[ ( y * srf->w ) + x ] = pixel;
    }
}

void drawRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {

	for(int i=0;i<w;i++) {
		set_pixel(srf,x+i,y,    c);
		set_pixel(srf,x+i,y+h-1,c);
	}

	for(int j=0;j<h;j++) {
		set_pixel(srf,x,    y+j,c);
		set_pixel(srf,x+w-1,y+j,c);
	}

}

void fillRect(SDL_Surface *srf,int x,int y,int w,int h,Uint32 c) {
	for(int j=0;j<h;j++) {
		for(int i=0;i<w;i++) {
			set_pixel(srf,x+i,y+j,c);
		}
	}
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

void palette(SDL_Surface *srf,Uint32 *pal,int *x,int *y,int *c,bool *b,int s) {
	int w=8*s;
	int h=2*s;

	for(int k=0;k<16;k++) {
		int i=k%8;
		int j=k/8;

		if(k==*c) {
			*b=true;
		} else {
			*b=false;
		}

		if(*b) {
			fillRect(srf,*x+i*s,*y+j*s,s,s,pal[12]);
			fillRect(srf,*x+i*s+2,*y+j*s+2,s-4,s-4,pal[k]);
			drawRect(srf,*x+i*s+2,*y+j*s+2,s-4,s-4,pal[0]);
		} else {
			fillRect(srf,*x+i*s,*y+j*s,s,s,pal[k]);
			drawRect(srf,*x+i*s,*y+j*s,s,s,pal[0]);
		}

		if(mb==1) {
			if(inrect(mx,my,*x+i*s,*y+j*s,s,s)) {
				*c=k;
			}
		}

		if(mb==4) {
			if(drag==0) {
				if(inrect(mx,my,*x,*y,w,h)) {
					dx=*x-mx;
					dy=*y-my;
					drag=1;
				} 
			} else {
				*x=mx+dx;
				*y=my+dy;
				if(*x<0) *x=0;
				if(*y<0) *y=0;
				if(*x>sw-w) *x=sw-w;
				if(*y>sh-h) *y=sh-h;		
			}
		} else {
			if(mb==0) {
				drag=0;
			}
		}

	} 
}


int main( int argc, char* args[] )
{
    SDL_Surface *screen = NULL;
    SDL_Event *ev = NULL;

	int px=0,py=0,pi=0;
	bool ps=false;
    
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
		drawRect(screen,0,0,sw,sh,pal[3]);

		palette(screen,pal,&px,&py,&pi,&ps,16);
	
		SDL_Flip(screen);
	}	

    SDL_Quit();

    return 0;
}


