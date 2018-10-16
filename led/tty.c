#include "tty.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include "led.h"

static struct termios config;
static int fd;

int configure(){
  //prends la config et la met dans ;a struct config
  if(tcgetattr(fd, &config) < 0) {printf("Config erreur\n");return -1; }

  config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                     INLCR | PARMRK | INPCK | ISTRIP | IXON);
   config.c_oflag = 0;

   config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

 //
 // Turn off character processing
 //
 // clear current char size mask, no parity checking,
 // no output processing, force 8 bit input
 //
 config.c_cflag &= ~(CSIZE );
 config.c_cflag |= CS8;
 config.c_cflag &= ~PARENB;     /* no parity bit */
 config.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
 config.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

 //
 // One input byte is enough to return from read()
 // Inter-character timer off
 //
 config.c_cc[VMIN]  = 0;
 config.c_cc[VTIME] = 0;

 if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
    printf("Erreur baud rate\n"); return -1;
 }
if(tcsetattr(fd, TCSAFLUSH, &config) < 0) { printf("err set conf\n"); return -1; }

return 0;
}


int colourWrite(struct colors col){
  unsigned char str[13];
  int bWritten = sprintf(str, "%d %d %d;", col.r, col.g,col.b );
  int wr = write(fd, str, bWritten);
}

int init(const char *ttyAddr) {
  fd = open(ttyAddr, O_RDWR| O_NOCTTY| O_NDELAY);
  if (fd == -1) {
    printf("Erreur %s", ttyAddr);
  }
  if (!isatty(fd)) {
    printf("Pas un TTY\n");
    return -1;
  }
  configure();

  sleep(2);
  return 0;
}

void end(){
  close(fd);
}
