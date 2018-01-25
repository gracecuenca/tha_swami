#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  int shmdt;
  char * color;
  
  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  write(server_socket, "SETUP", sizeof("SETUP"));
  clearscreen();
  
  while (1) {
    read(server_socket, buffer, sizeof(buffer));
    color = change_color(buffer);
    if (getchar() == '\n') {
      write(server_socket, buffer, sizeof(buffer));
    }
    else {
      write(server_socket, "TEST", sizeof("TEST"));
    }

  }
}
