#ifndef TTYH
#define TTYH
#include "led.h"
int colourWrite(struct colors col);
int init(const char *ttyAddr);
void end();
#endif
