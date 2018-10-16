#include <stdio.h>
#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include "hookVerif.h"

int populateQueryValues(void *cls, enum MHD_ValueKind kind, const char *key, const char *value){
  struct queryValues *valuesStruct = (struct queryValues*)cls;
  if (strcmp(key, "hub.mode") == 0) {
    valuesStruct->mode = malloc(strlen(value));
    strcpy(valuesStruct->mode, value);
  }else if (strcmp(key, "hub.verify_token")== 0) {
    valuesStruct->verification_token = malloc(strlen(value));
    strcpy(valuesStruct->verification_token, value);
  }else if (strcmp(key, "hub.challenge")== 0) {
    valuesStruct->challenge = malloc(strlen(value));
    strcpy(valuesStruct->challenge, value);
  }

  return MHD_YES;
}

int sendOk(struct MHD_Connection *connection, char *challenge){
  int ret;
  struct MHD_Response *response;

  response =
    MHD_create_response_from_buffer (strlen(challenge), (void *) challenge,
             MHD_RESPMEM_MUST_COPY);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}

int sendForbidden(struct MHD_Connection *connection){
  int ret;
  struct MHD_Response *response;
  const char *forbidden = "Forbidden";

  response =
    MHD_create_response_from_buffer (strlen(forbidden), (void *) forbidden,
             MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, 403, response);
  MHD_destroy_response (response);
  return ret;
}

int checkVerif(struct queryValues *valuesStruct){
  if (valuesStruct->mode && valuesStruct->verification_token && valuesStruct->challenge) {
    //printf("mode: %s\n", valuesStruct->mode);
    //printf("verification_token: %s\n", valuesStruct->verification_token);
    //printf("challenge: %s\n", valuesStruct->challenge);
    //printf("Required token %s\n", ACCESS_TOKEN);

    if (strcmp(valuesStruct->mode, "subscribe") == 0 && strcmp(valuesStruct->verification_token, ACCESS_TOKEN) ==0) {
      return 0;
    } else return -1;

  }
  return -1;
}


int hookVerif(struct MHD_Connection *connection){
  int ret =0;
  struct queryValues valuesStruct;
  valuesStruct.mode= NULL;
  valuesStruct.verification_token= NULL;
  valuesStruct.challenge = NULL;
  MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, &populateQueryValues, (void *)&valuesStruct);

  if (checkVerif(&valuesStruct) >= 0) {
      printf("WEBHOOK CHECKED\n");
      ret = sendOk(connection, valuesStruct.challenge);
  }else {
      ret = sendForbidden(connection);
  }
  free(valuesStruct.mode);
  free(valuesStruct.verification_token);
  free(valuesStruct.challenge);

  return ret;
}
