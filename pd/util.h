#ifndef UTIL_H
#define UTIL_H

#include <SDL/SDL.h>

#include <stdbool.h>
#include <stddef.h>



int hexval(char c);
bool exists(char *path);
int mycolor(Uint32 c,int np,Uint32 p[]);

double distance(double x0,double y0,double x1,double y1);
bool inRect(int x,int y,int rx,int ry,int rw,int rh);
bool inCirc(int x,int y,int cx,int cy,int cr);

size_t loadFile(char *path,char **source,size_t *len);
int saveFile( char *path, char *source, size_t len );



#ifdef UTIL_IMPLEMENTATION



char *hexstr="0123456789ABCDEF";

int hexval(char c) {
    int j=-1;
    for(int i=0;i<16;i++) {
        if(c==hexstr[i]) {
            j=i;
            break;
        }
    }
    return j;
}



bool exists(char *path) {
    return access(path, F_OK) == 0;
}



int mycolor(Uint32 c,int np,Uint32 p[]) {
    int j=0;
    long k=labs((long)p[0]-c);
    for(int i=1;i<np;i++) {
        long l=labs((long)p[i]-c);
        if(k>=l) {
            j=i;
            k=l;
        }
    }
    return j;
}



double distance(double x0,double y0,double x1,double y1) {
    double dx=x1-x0;
    double dy=y1-y0;
    return sqrt((dx*dx)+(dy*dy));
}

bool inRect(int x,int y,int rx,int ry,int rw,int rh) {
    return x>=rx && x<rx+rw && y>=ry && y<ry+rh;
}

bool inCirc(int x,int y,int cx,int cy,int cr) {
    return distance(x,y,cx,cy)<=cr;
}



size_t loadFile(char *path,char **source,size_t *len) {
 
    FILE *fp = fopen(path, "rb");

    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            (*source) = malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            *len = fread(*source, sizeof(char), bufsize, fp);
            if ((*len) == 0) {
                fprintf(stderr,"Error: cannot read file %s", path);
                free(*source);
                (*source) = NULL;
            } else {
                (*source)[++(*len)] = '\0';
            }
        }
        fclose(fp);
    } else {
        perror(path);
    }
    
    return (*len);
}

int saveFile( char *path, char *source, size_t len ) {
    FILE *fp=fopen(path,"wb");

    if(fp==NULL) {
        perror(path);
        return 1;
    }

    if(fwrite(source,sizeof(char),len,fp)<len) {
        perror(path);
        return 2;
    }

    return 0;
}



#endif /* UTIL_IMPLEMENTATION */

#endif /* UTIL_H */
