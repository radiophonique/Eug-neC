#ifndef hookVerifH
#define hookVerifH
#define ACCESS_TOKEN "poutineAuBacon"

#include <microhttpd.h>

//valider le hook sur un GET sent par fb
int hookVerif(struct MHD_Connection *connection);


//Les 3 infos sent par fb
struct queryValues{
  char *mode;
  char *verification_token;
  char *challenge;
};


#endif
