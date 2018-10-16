#ifndef LEDH
#define LEDH


struct colors{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

typedef struct colors (*funcPtr)(struct colors, int, int*);

void ledLoop();




#endif
