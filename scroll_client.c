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
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    clearscreen();
    char** screen = terminal_line();
    screen_move(screen);
  }
}
