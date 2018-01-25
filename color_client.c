#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE]; //color read from server
  char std_in[BUFFER_SIZE]; //stdin
  char * color;
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
      fgets(std_in, sizeof(std_in), stdin);
      if (!strcmp(std_in, "\n")) {
        write(server_socket, buffer, sizeof(buffer));
        //color = change_color(buffer);
      }
    }

    if (FD_ISSET(server_socket, &read_fds)) {
      if (read(server_socket, buffer, sizeof(buffer)) == -1) {
        printf("%s\n", strerror(errno));
        exit(0);
      }
      color = change_color(buffer);
      sleep(1);
      write(server_socket, "", sizeof(""));
      if (fflush(stdout) != 0) {
        printf("%s\n", strerror(errno));
        exit(0);
      }
    }
  }
}
