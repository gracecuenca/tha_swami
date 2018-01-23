#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE]; //color read from server
  char buffer2[BUFFER_SIZE]; //previous color
  char buffer3[BUFFER_SIZE]; //stdin
  char * color;
  int counter = 0; //temp for now just to keep track
  fd_set read_fds;
  
  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  write(server_socket, "SETUP", sizeof("SETUP"));
  while (1) {
    fflush(stdout);
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);

    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer3, sizeof(buffer3), stdin);
      if (!strcmp(buffer3, "\n")) {
	write(server_socket, buffer, sizeof(buffer));
      }
      //write(server_socket, "NO", sizeof("NO"));
    }

    if (FD_ISSET(server_socket, &read_fds)) {
      
      
      if (read(server_socket, buffer, sizeof(buffer)) == -1) {
	printf("%s\n", strerror(errno));
	exit(0);
      }
      //strncpy(buffer2, buffer, BUFFER_SIZE);
      //printf("READ");
      //printf("%s", buffer);
      color = change_color(buffer);
      // printf("COLOR %s", color);
      //write(server_socket, "", sizeof(char *));
      //write(server_socket, buffer2, sizeof(buffer2));
      if (fflush(stdout) != 0) {
	printf("%s\n", strerror(errno));
	exit(0);
      }
    }
  }
}
