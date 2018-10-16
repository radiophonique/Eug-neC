#ifndef POSTPROCESS
#define POSTPROCESS
#define POSTBUFFERSIZE 1024

int postFirstTime(struct MHD_Connection *connection,void **con_cls);

int postNotFirstTime(struct MHD_Connection *connection, const char *upload_data,
                      size_t *upload_data_size, void **con_cls);

struct connection_info_struct
{
  char *data;
  int size;
  struct MHD_PostProcessor *postprocessor;
};


#endif
