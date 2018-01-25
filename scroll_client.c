#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  int seconds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  write(server_socket, getpid(), sizeof(getpid()));

  while (1) {
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    clearscreen();
    char** screen = terminal_line();
    seconds = screen_move(screen);
    if(seconds == cols()){
      //do stuff w/ named pipes
    }
  }
}
