#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  char * color;
  int counter = 0; //temp for now just to keep track

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
    //printf("CLIENT START\n");
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    color = change_color(buffer);
    if (color) {
      write(server_socket, "CHANGED COLOR", 256);
    }
    else {
      write(server_socket, "NOT CHANGED COLOR", 256);
    }

    printf("%d\n", counter);
    //awaiting user to hit enter, incidicating that it's reached the computer to wack
    getchar();
    counter++;

    //free(color);
    //printf("received: [%s]\n", buffer);
  }
  //free(color);
}
