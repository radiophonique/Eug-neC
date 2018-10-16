#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "led.h"
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "shader/shader.h"
#include "../status.h"
#include "tty.h"
#define NBRMODES 4

static struct timespec delay;



void ledLoop(){
   int i = 0;
   int iMax = 0;
  statusUpdate =0;
  delay.tv_sec = 0;
  delay.tv_nsec = 25000000L;
  funcPtr funcArr[NBRMODES];
  funcArr[0] = &modeStatic;
  funcArr[1] = &modeFade;
  funcArr[2] = &modeBifade;
  funcArr[3] = &modeMonofade;
  funcPtr currentFunc = funcArr[0];
  struct colors preColors;
  struct colors postColors;

  preColors.r=0;
  preColors.g=0;
  preColors.b=0;

  while(running == 1){
    pthread_mutex_lock(&ledStatusMutex);
    if (statusUpdate == 1) {
      i = 0;
      if (currentStatus->mode <= NBRMODES) {
        currentFunc = funcArr[currentStatus->mode];
      }
      preColors.r=currentStatus->r;
      preColors.g=currentStatus->g;
      preColors.b=currentStatus->b;
    }
    statusUpdate = 0;
    pthread_mutex_unlock(&ledStatusMutex);




    postColors = (*currentFunc)(preColors, i, &iMax);

    i++;
    if (i >= iMax) {
      i = 0;
    }
    printf("R: %d G: %d B: %d\n", postColors.r, postColors.g, postColors.b);
    colourWrite(postColors);
    nanosleep(&delay,NULL);
  }
}
