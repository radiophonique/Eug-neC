#include <string.h>
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include "postProcess.h"
#include "processJSON.h"
#include "headerType.h"




int sendPostOk(struct MHD_Connection *connection){
  int ret;
  struct MHD_Response *response;
  const char *ok = "OK";
  response =
    MHD_create_response_from_buffer (strlen(ok), (void *) ok,
             MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}

int sendBadRequest(struct MHD_Connection *connection){
  int ret;
  struct MHD_Response *response;
  const char *forbidden = "Error";

  response =
    MHD_create_response_from_buffer (strlen(forbidden), (void *) forbidden,
             MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, 400, response);
  MHD_destroy_response (response);
  return ret;
}


int postFirstTime(struct MHD_Connection *connection,void **con_cls){
  struct connection_info_struct *con_info;

  con_info = malloc (sizeof (struct connection_info_struct));
  if (NULL == con_info) return MHD_NO;
  con_info->data = NULL;
  con_info->size = 0;


        *con_cls = (void*) con_info;
        return MHD_YES;

}

int postNotFirstTime(struct MHD_Connection *connection, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
      struct connection_info_struct *con_info = *con_cls;

      if (*upload_data_size != 0)
        {


          char *totalData = calloc(con_info->size + *upload_data_size, sizeof(char));
          memcpy(totalData, con_info->data, con_info->size);
          strncat(totalData, upload_data, *upload_data_size);
          free(con_info->data);
          con_info->data = totalData;

          *upload_data_size = 0;
          return MHD_YES;
        }
      else {
          if (isHeaderJSON(connection)) {
            int jsonStatus = processJSON(con_info->data);
            if (jsonStatus < 0) {
              sendBadRequest(connection);
              return MHD_YES;
            }
            sendPostOk(connection);
          }else{
            sendBadRequest(connection);
          }
          return MHD_YES;
      }


    }
