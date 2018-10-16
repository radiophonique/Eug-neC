

/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <string.h>
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>



#include "hookVerif/hookVerif.h"
#include "loader/loader.h"
#include "postProcess/postProcess.h"
#include "status.h"
#include "led/led.h"
#include "led/tty.h"

#define PORT 44443
#define KEY "ssl/cert.key"
#define CERT "ssl/cert.pem"



static int answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{

  int ret;
  if(strcmp(method, "GET") == 0 && strcmp(url, "/webhook") == 0){
    printf("GET %s\n", url);
    ret = hookVerif(connection);
  }else
  if(strcmp(method, "POST") == 0 && strcmp(url, "/webhook") == 0){
    printf("POST %s\n", url);

    if (NULL == *con_cls) {
      return postFirstTime(connection, con_cls);
    }else {
      postNotFirstTime(connection, upload_data, upload_data_size, con_cls);
    }
    //ret = hookVerif(connection);
  }
  else{
    struct MHD_Response *response;

    const char *error = "404 Not Found";

    response =
      MHD_create_response_from_buffer (strlen(error), (void *) error,
               MHD_RESPMEM_PERSISTENT);
    printf("404: %s\n", url);
    ret = MHD_queue_response (connection, 404, response);
    MHD_destroy_response (response);

  }

  return ret;
}


void request_completed (void *cls, struct MHD_Connection *connection,
     		        void **con_cls,
                        enum MHD_RequestTerminationCode toe)
{
  struct connection_info_struct *con_info = *con_cls;

  if (NULL == con_info) return;

      if (con_info->data) free (con_info->data);

  free (con_info);
  *con_cls = NULL;
}

int running;
int main (int argc, char const *argv[]) {
  struct MHD_Daemon *daemon;

  char *key_pem;
  char *cert_pem;
  pthread_mutex_init(&ledStatusMutex, NULL);

  running =1;

  currentStatus = malloc(sizeof(struct status));
  currentStatus->r =0;
  currentStatus->g =0;
  currentStatus->b =0;
  currentStatus->mode =0;

  key_pem = load_file (KEY);
  cert_pem = load_file (CERT);

  if ((key_pem == NULL) || (cert_pem == NULL))
  {
    printf ("The key/certificate files could not be read.\n");
    return 1;
  }

  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY | MHD_USE_SSL, PORT, NULL, NULL,
                             &answer_to_connection, NULL,
                             MHD_OPTION_NOTIFY_COMPLETED, &request_completed, NULL,
                             MHD_OPTION_HTTPS_MEM_KEY, key_pem,
                             MHD_OPTION_HTTPS_MEM_CERT, cert_pem,
                            MHD_OPTION_END);
  if (NULL == daemon){
    printf("daemon is NULL\n");
    return 1;
  }

  int initResult = init(argv[1]);
  if (initResult == -1) {
    running = -1;
  }
  ledLoop();

  MHD_stop_daemon (daemon);
  pthread_mutex_destroy(&ledStatusMutex);
  end();
  free(key_pem);
  free(cert_pem);
  free(currentStatus);
  return 0;
}
