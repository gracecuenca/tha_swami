#include "headers.h"
//todo: separate subservers into three functionalities
//void color_subserver(int from_client);
//void scroll_subserver(int from_client);
//void matrix_subserver(int from_client);
void subserver(int from_client);

int main() {
  int listen_socket, client_socket;
  int f;
  int a;
  char * buffer[BUFFER_SIZE];
  fd_set read_fds;

  //shared memory initizlizations
  int shmid = shmget(KEY, 7*sizeof(int), IPC_CREAT | 0666);
  if(shmid < 0){
    printf("error in shmget\n");
  }
  int * pids = shmat(shmid, 0, 0);
  if((int)pids < 0){
    printf("error in shmat\n");
  }

  //THIS ARRAY HOLDS ALL THE pids (clients)
  //pids[0] = can_change, 0 means no (is blue), 1 means yes (is red)
  //pids[1] = started, 0 means no, 1 means yes
  //the rest are actual pids
  for (a = 0; a < 7; a ++) {
    pids[a] = 0;
    printf("pids[%d] : %d\n", a, pids[a]);
  }

  listen_socket = server_setup();

  while (1) {
    //if listen_socket triggered select
    client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0){
      subserver(client_socket);
    }
    else {
      close(client_socket);
    }
  }
}

int getRandPID(int*array) {
  srand(time(NULL));
  int r = rand() % 7;
  while(!array[r] || r == 0 || r == 1){
    r = rand() % 7;
  }
  //printf("PID: %d\n", pids[r]);
  return array[r];
}

int already_connected(int client_pid, int*array, int max){
  int i;
  for(i = 0; i < max; i++){
    if(array[i] == client_pid){
      //return true
      return 1;
    }
  }
  //return false
  return 0;
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];
  int shmid = shmget(KEY, 7*sizeof(int), IPC_CREAT | 0666);
  int * pids = shmat(shmid, 0, 0);
  fd_set read_fds;

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);

    printf("0000 \%s \n", buffer);

    if(!already_connected(getpid(), pids, 7)){
      int index = 2;
      while(pids[index] != 0 && index < 7){
        index++;
      }
      pids[index] = getpid();
      printf("pids[%d]: %d\n", index, pids[index]);
    }

    printf("4444 \%s \n", buffer);

    //select initializations
    fflush(stdout);
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(client_socket, & read_fds);
    select(client_socket + 1, &read_fds, NULL, NULL, NULL);

    printf("3333 \%s \n", buffer);

    //if reading from client socket on server side
    if (FD_ISSET(client_socket, &read_fds)) {
    }

    //if reading from stdin on server side
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      //game start function
      //sends red to all the client
      //WORKS
      if (!strcmp(buffer, "\n") && !pids[1]) {
        write(client_socket, RED, BUFFER_SIZE);
        printf("DEFAULT RED SENT\n");
        sleep(1);
        //change game started status to started
        pids[1] = 1;
        //clients are now red, so they are prone to being changed
        pids[0] =1;
      }
    }

    printf("1111 \%s \n", buffer);

    //if game started
    if (pids[1]){ //&& getpid() == getRandPID(pids)) {
      //if need to send CYAN to a client
      if(pids[0]){
        write(client_socket, CYAN, BUFFER_SIZE);
        //cyan set
        pids[0] = 0;
        printf("WRITE CYAN\n");
      }
      //if the server receives CYAN color code and the game has started already
      if (!strcmp(CYAN, buffer)) {
        printf("GOT CYAN\n");
        //turn it back to red upon receiving the CYAN
        write(client_socket, RED, sizeof(char *));
        sleep(1);
        pids[0] =1;
      }
    }

    printf("2222 \%s \n", buffer);

    printf("pids[0]: %d\n",pids[0]);
    printf("pids[1]: %d\n",pids[1]);

    printf("=======================================\n\n");

  }
  close(client_socket);
  exit(0);
}
