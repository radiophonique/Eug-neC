#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <limits.h>
#include "../status.h"

#define NBRMODES 4
static char *modes[] = {"static", "fade", "bifade", "monofade"};
static char *modesAbbr[] = {"s", "f", "b", "m"};
static long modeLongs[] = {0l,1l,2l,3l};

long getMode(char *text){
  char *modeText = strtok(text, " ");
  if (modeText == NULL) return -1;

  bool found = false;
  int i =0;
  char *endptr;
  while (!found && i < NBRMODES) {
    if(strcasecmp(text, modes[i] ) == 0 ||
      strcasecmp(text,modesAbbr[i] ) == 0 ||
      (strtol(text ,&endptr,10) == modeLongs[i] && text != endptr))
    {
      found = true;
      return modeLongs[i];
    }
    i++;
  }
  return -1;

}

void parseColors(struct status *newStatus){
  long colors[3];
  char *colptr;
  for (int i = 0; i < 3; i++) {
    colptr = strtok(NULL, ":");
    if (colptr == NULL) {
      colors[i] = 0;
      continue;
    }
    colors[i] = strtol(colptr, NULL, 10);
    if (colors[i] > UCHAR_MAX) {
      colors[i] = UCHAR_MAX;
    }else if (colors[i] < 0) {
      colors[i] = 0;
    }
  }
  newStatus->r = colors[0];
  newStatus->g = colors[1];
  newStatus->b = colors[2];
}

int processText(char *text){
  printf("Text: %s\n", text);
  char *tempText = strdup(text);

  struct status *newStatus = malloc(sizeof(struct status));

  int mode = getMode(tempText);
  printf("TEXTOK %d\n", mode);
  if (mode == -1) {
    mode = 0;
    //TODO echo un message derreur
  }

  parseColors(newStatus);


  newStatus->mode = mode;




  pthread_mutex_lock(&ledStatusMutex);
  free(currentStatus);
  currentStatus = newStatus;
  statusUpdate =1;
  pthread_mutex_unlock(&ledStatusMutex);

  free(tempText);
  return 0;
}
