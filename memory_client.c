#include "headers.h"

static void sighandler(int i) {
  if (i == SIGINT) {
    //close(server_socket);
    exit(0);
  }
}

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  char std_in[BUFFER_SIZE]; //stdin
  int shmdt;
  char * color;
  fd_set read_fds;

  signal(SIGINT, sighandler);

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  write(server_socket, "SETUP", sizeof("SETUP"));
  clearscreen();
  
  while (1) {
    read(server_socket, &buffer, sizeof(buffer));
    printf("buffer val: %s\n", buffer);
    
    if (!strcmp(buffer, "PUSH")) {
      printf("enter y or n: ");
      scanf("%s", buffer);
      printf("buffer val: %s", buffer);
      write(server_socket, buffer, sizeof(buffer));
    }

    else {
      //change color
      color = change_color(buffer);
    }

  }
}
