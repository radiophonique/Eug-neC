#include "headerType.h"
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>


int iterate(void *cls, enum MHD_ValueKind kind, const char *key, const char *value){
  if (strcmp("Content-Type", key) == 0 && strcmp("application/json", value) ==0) {
    *(int *)cls = 1;
    return MHD_NO;
  }
  return MHD_YES;
}

int isHeaderJSON(struct MHD_Connection *connection){
  int ret = 0;
  MHD_get_connection_values (connection, MHD_HEADER_KIND, &iterate, (void *) &ret);

  if (ret == 1) {
    printf("JSON\n");
  }
  return ret;
}
