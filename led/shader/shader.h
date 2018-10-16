#ifndef SHADERH
#define SHADERH
#include "../led.h"

struct colors modeStatic(struct colors col, int i, int *iMax);
struct colors modeFade(struct colors col, int i, int *iMax);
struct colors modeBifade(struct colors col, int i, int *iMax);
struct colors modeMonofade(struct colors col, int i, int *iMax);


#endif
