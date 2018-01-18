#include "headers.h"

void process(char *s);
void subserver(int from_client);

int pids [5] = malloc(5, sizeof(int * ));
int i = 0;

int main() {

  int listen_socket;
  int f;
  listen_socket = server_setup();

  while (1) {

    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) {
      pids[i] = getpid();
      i ++;
      subserver(client_socket);
    }
    else {
      close(client_socket);
    }
  }
}

int getRandPID() {

}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {
    //printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    //initial setup to turn all clients red
    write(client_socket, RED, sizeof(char *));

    //game start fxn

    //if read = blue
    //getrandpid
    //if getpid = getrandpid write blue
    
  }//end read loop
  close(client_socket);
  exit(0);
}
