#include "shader.h"
#include <stdio.h>


struct colors modeStatic(struct colors col, int i, int *iMax){
  *iMax = 0;
  return col;
}

struct colors modeFade(struct colors col, int i, int *iMax){
  unsigned char colors[3];
  unsigned char origColours[3];

  colors[0]=0;
  colors[1]=0;
  colors[2]=0;

  origColours[0] = col.r;
  origColours[1] = col.g;
  origColours[2] = col.b;

  *iMax = 255*6;
  //trouver le plus gros pour setter le imax
  //*iMax = (col.r > (col.g > col.b ? col.g:col.b)? col.r : (col.g > col.b ? col.g:col.b)) *6;
  int j = i/ (*iMax/3);
   i = i % (*iMax/3);
  colors[j] = (i < (*iMax/6)) ? i * origColours[j]/ (*iMax/6): origColours[j] * (*iMax/3 -i) / (*iMax/6);
  col.r = colors[0];
  col.g = colors[1];
  col.b = colors[2];
  return col;
}

struct colors modeBifade(struct colors col, int i, int *iMax){
  unsigned char colors[3];
  unsigned char origColours[3];

  colors[0]=0;
  colors[1]=0;
  colors[2]=0;

  origColours[0] = col.r;
  origColours[1] = col.g;
  origColours[2] = col.b;

  *iMax = 255*6;
  //trouver le plus gros pour setter le imax
  //*iMax = (col.r > (col.g > col.b ? col.g:col.b)? col.r : (col.g > col.b ? col.g:col.b)) *6;
  int j = i/ (*iMax/3);
  int j2 = j + 1 > 2 ? 0:j+1;
   i = i % (*iMax/3);
  colors[j] = (i < (*iMax/6)) ? i * origColours[j]/ (*iMax/6): origColours[j] * (*iMax/3 -i) / (*iMax/6);
  colors[j2] = (i < (*iMax/6)) ? i * origColours[j2]/ (*iMax/6): origColours[j2] * (*iMax/3 -i) / (*iMax/6);
  col.r = colors[0];
  col.g = colors[1];
  col.b = colors[2];
  return col;
}

struct colors modeMonofade(struct colors col, int i, int *iMax){
  unsigned char colors[3];
  unsigned char origColours[3];

  colors[0]=0;
  colors[1]=0;
  colors[2]=0;

  origColours[0] = col.r;
  origColours[1] = col.g;
  origColours[2] = col.b;
  *iMax = 255*2;
  //trouver le plus gros pour setter le j
  int j = (col.r > (col.g > col.b ? col.g:col.b)? 0 : (col.g > col.b ? 1:2));
  i = i % *iMax;
  colors[j] = (i < (*iMax/2)) ? i * origColours[j]/ (*iMax/2): origColours[j] * (*iMax -i) / (*iMax/2);
  col.r = colors[0];
  col.g = colors[1];
  col.b = colors[2];
  return col;
}
