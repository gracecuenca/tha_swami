#include "headers.h"

void process(char *s);
void subserver(int from_client);

static int pids[5];
int i = 0;
int can_change = 0;
int started = 0;

int main() {
  int listen_socket;
  int f;
  listen_socket = server_setup();
  for (int a = 0; a < 5; a ++) {
    pids[a] = 0;
  }
  while (1) {

    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) {
      subserver(client_socket);
    }
    else {
      close(client_socket);
    }
  }
}

int getRandPID() {
  srand(time(NULL));
  int r = rand() % 5;
  if (!pids[r]) {
    getRandPID();
  }
  printf("PID: %d\n", pids[r]);
  return pids[r];
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];
  pids[i] = getpid();
  printf("pids[%d]: %d\n", i, getpid());
  i ++;

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    //initial setup to turn all clients red
    write(client_socket, RED, sizeof(char *));


    //game start fxn
    if (!started && getpid() == getRandPID()) {
      write(client_socket, CYAN, sizeof(char *));
      started = 1;
    }

    //write(client_socket, CYAN, sizeof(char *));

    //if read = blue
    if (!strcmp(CYAN, buffer)) {
      //getrandpid
      can_change = 1;
    }
    if (can_change && getpid() == getRandPID()) {
      //if getpid = getrandpid write blue
      write(client_socket, CYAN, sizeof(char *));
      can_change = 0;
    }

  }//end read loop
  close(client_socket);
  exit(0);
}
