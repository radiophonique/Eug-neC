#include "processJSON.h"
#include <string.h>
#include <stdio.h>
#include "../cJSON/cJSON.h"
#include "processText.h"
#include <stdlib.h>






int isPage(cJSON *json){
  cJSON *object = cJSON_GetObjectItemCaseSensitive(json, "object");
  if (cJSON_IsString(object) && strcmp(object->valuestring, "page") == 0) {
    return 1;
  }
  return 0;
}

int iterateEntries(cJSON *json){
  cJSON *array = cJSON_GetObjectItemCaseSensitive(json, "entry");
  if (!cJSON_IsArray(array)) return -1;
  cJSON *entry;
  cJSON_ArrayForEach(entry , array){
    cJSON *messaging = cJSON_GetObjectItemCaseSensitive(entry, "messaging");
    if (!cJSON_IsArray(messaging)) continue;
    cJSON *messagingObject = cJSON_GetArrayItem(messaging , 0);
    if (!cJSON_IsObject(messagingObject)) continue;
    cJSON *message = cJSON_GetObjectItemCaseSensitive(messagingObject, "message");
    if (!cJSON_IsObject(message)) continue;
    cJSON *text = cJSON_GetObjectItemCaseSensitive(message,"text");
    if (!cJSON_IsString(text)) continue;
    processText(text->valuestring);
  }
  entry = NULL;



  return 0;
}

int processJSON(char *data){
  cJSON *json = cJSON_Parse(data);
  if (!cJSON_IsObject(json)) return-1;
  if (!isPage(json)) return -1;
  iterateEntries(json);


  cJSON_Delete(json);
  return 0;
}
