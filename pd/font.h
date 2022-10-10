#ifndef FONT_H
#define FONT_H



#include <SDL/SDL.h>


#include "common.h"
#include "palette.h"


typedef struct Font Font;

struct Font {
    int w,h,f,t;
    int *p; 
};



Font *Font_New(char *path);
void Font_DrawChar(SDL_Surface *srf,Font *fnt,int x,int y,int s,char f,Uint32 c);
void Font_DrawText(SDL_Surface *srf,Font *fnt,int x,int y,int s,char *t,Uint32 c);


#ifdef FONT_IMPLEMENTATION



Font *Font_New(char *path) {
    Font *font=malloc(sizeof(*font));
    if(font) {
        FILE *fin;

        if((fin=fopen(path,"r"))==NULL) {
            perror(path);
            exit(1);
        }

        if(fscanf(fin,"%d %d %d %d\n",&font->w,&font->h,&font->f,&font->t)!=4) {
            printf("Error: error reading font header %s\n",path);
            exit(2);
        }

        font->p=malloc(sizeof(*font->p)*(font->w*font->h*font->f));

        size_t i=0;
        int c;
        while((c=fgetc(fin))!=EOF) {
            int h=hexval(c);
            if(h!=-1) {
                if(i<(size_t)(font->h*font->w*font->f)) {
                    font->p[i++]=h;
                } else {
                    break;
                }
            }
        }

        fclose(fin);

    }

    return font;
}



void Font_DrawChar(SDL_Surface *srf,Font *fnt,int x,int y,int s,char f,Uint32 c) {
    for(int j=0;j<fnt->h;j++) {
        for(int i=0;i<fnt->w;i++) {
            int k=fnt->p[f*fnt->w*fnt->h+j*fnt->w+i];
            if(k!=fnt->t) {
                Graphics_FillRect(srf,x+i*s,y+j*s,s,s,c);
            }
        }   
    }   
}

void Font_DrawText(SDL_Surface *srf,Font *fnt,int x,int y,int s,char *t,Uint32 c) {
    int i=x;
    int j=y;
    for(int k=0;t[k];k++) {
        Font_DrawChar(srf,fnt,i,j,s,t[k],c);
        i+=fnt->w;
        if(i+fnt->w>=sw) {
            i=0;
            j+=fnt->h;
            if(j+fnt->h>=sh) break;
        }
    }   
}




#endif /* FONT_IMPLEMENTATION */



#endif /* FONT_H */

