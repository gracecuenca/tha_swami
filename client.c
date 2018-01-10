#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  char * color;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  while (1) {
    /***
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    ***/
    read(server_socket, buffer, sizeof(buffer));
    color = change_color(buffer);
    if (color) {
      write(server_socket, "CHANGED COLOR", sizeof(char *));
    }
    else {
      write(server_socket, buffer, sizeof(buffer));
    }
    //free(color);
    //printf("received: [%s]\n", buffer);
  }
  //free(color);
}
