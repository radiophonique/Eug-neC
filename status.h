#ifndef STATUSH
#define STATUSH
#include <pthread.h>

struct status{
  long mode;
  unsigned char r;
  unsigned char g;
  unsigned char b;
};




struct status *currentStatus;
pthread_mutex_t ledStatusMutex;
extern int running;
int statusUpdate;
#endif
