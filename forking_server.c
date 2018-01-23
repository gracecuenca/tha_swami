#include "headers.h"

int pid_pos = 2;

void process(char *s);
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
  int subserver_count = 0;
  //shared memory initizlizations
  int shmid = shmget(KEY, 7*sizeof(int), IPC_CREAT | 0666);
  if(shmid < 0){
    printf("error in shmget\n");
  }
  //THIS ARRAY HOLDS ALL THE pids (clients)
  //pids[0] = can_change
  //pids[1] = started
  //the rest are actual pids
  int* pids = shmat(shmid, 0, 0);
  if((int)pids < 0){
    printf("%s\n", strerror(errno));
    printf("error in shmat\n");
  }
  for (a = 0; a < 7; a ++) {
    pids[a] = 0;
    printf("pids[%d] : %d\n", a, pids[a]);
  }
  listen_socket = server_setup();

  while (1) {

    //if listen_socket triggered select
      client_socket = server_connect(listen_socket);

      f = fork();
      if (f == 0)
	subserver(client_socket);
      else {
	close(client_socket);
      }
    }//end listen_socket select
    f = fork();
    if (f == 0) {
      subserver(client_socket);
    }
    else {
      close(client_socket);
    }
    pid_pos++;
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
    printf("[subserver %d] received: [%s]", getpid(), buffer);
    printf(RESET);
    printf("\n");

    if (!pids[1]) {
      write(client_socket, RED, BUFFER_SIZE);
      //pids[1] = 0;
    }
    printf("4444 \%s \n", buffer);

    fflush(stdout);
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(client_socket, & read_fds);
    select(client_socket + 1, &read_fds, NULL, NULL, NULL);

    printf("3333 \%s \n", buffer);

    if (FD_ISSET(client_socket, &read_fds)) {
    }
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      if (!strcmp(buffer, "\n")) {
	printf("CHANGE1 pids[1]: %d\n", pids[1]);
	pids[1] = 1;
	printf("CHANGE2 pids[1]: %d\n", pids[1]);
      }
    }


    printf("0000 \%s \n", buffer);
    if(!already_connected(getpid(), pids, 7)){
      int index = 2;
      while(pids[index] != 0 && index < 7){
	index++;
      }
      pids[index] = getpid();
      printf("pids[%d]: %d\n", index, getpid());
    }

    printf("1111 \%s \n", buffer);
    if (!strcmp(CYAN, buffer) && pids[1]) {
      printf("GOT CYAN\n");
      //pids[0] = 1;
      write(client_socket, RED, sizeof(char *));
    }
    printf("2222 \%s \n", buffer);

    if (pids[1] && getpid() == getRandPID(pids)) {
      //write(client_socket, "", BUFFER_SIZE);
      if (write(client_socket, CYAN, BUFFER_SIZE) == -1) {
	printf("%s\n", strerror(errno));
	exit(0);
      }
      printf("WRITE CYAN\n");
      //pids[0] = 0;
    }
    /***
    if (pids[0]) { //&& getpid() == getRandPID(pids)) {
      //if getpid = getrandpid write blue
      write(client_socket, CYAN, sizeof(char *));
      pids[0] = 0;
    }
    ***/

  }//end read loop
  close(client_socket);
  exit(0);
}
