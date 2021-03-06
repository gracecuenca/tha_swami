#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  char msg[BUFFER_SIZE];
  int seconds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  write(server_socket, "hi", sizeof("hi"));
  read(server_socket, &msg, sizeof(msg));
  if (!strcmp(msg, "")) {
    write(server_socket, "", sizeof(""));
    read(server_socket, &msg, sizeof(msg));
  }
  clearscreen();
  char** screen = terminal_line(msg);

  while (1) {
    //scroll(screen,msg);
    if(screen_move(screen)){
      write(server_socket, "done", sizeof("done"));
      clearscreen();
    }

  }
}
