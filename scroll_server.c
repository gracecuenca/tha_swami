#include "headers.h"
#define READ 0
#define WRITE 1
#define MY_KEY 1001

void subserver(int from_client);
int send_text(int, char *, char *);
//counting index in pids[]
int index_p = 0;

int main() {

  int listen_socket, client_socket;
  int f;

  listen_socket = server_setup();
  fd_set read_fds;
  char buffer[BUFFER_SIZE]; //enter on stdin server
  char message[BUFFER_SIZE]; //message; get ascii art

  int shmid = shmget(MY_KEY, 5*sizeof(int), IPC_CREAT | 0666);
  int* pids = shmat(shmid, 0, 0);

  printf("\n\n=====Scrolling Text=====\n");
  printf("Connect all the clients you want to send message to first, then hit ENTER\n");

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      //start sending text
      if (!strcmp(buffer, "\n")) {
        printf("Enter message:\n");
        fgets(message, sizeof(message), stdin);
      }
    }

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds) && index_p < 5) {
      client_socket = server_connect(listen_socket);
      f = fork();
      if (f == 0) {
        subserver(client_socket);
      }
      else {
        close(client_socket);
      }
      index_p++;
    }

  }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    int shmid = shmget(MY_KEY, 5*sizeof(int), IPC_CREAT | 0666);
    int* pids = shmat(shmid, 0, 0);

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    pids[index_p] = getpid();
    printf("subserver %d is in %d position in array\n", pids[index_p], index_p);
    write(client_socket, buffer, sizeof(buffer));
    
  }

  close(client_socket);
  exit(0);
}
