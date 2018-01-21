#include "headers.h"

void process(char *s);
//todo: separate subservers into three functionalities
//void color_subserver(int from_client);
//void scroll_subserver(int from_client);
//void matrix_subserver(int from_client);
void subserver(int from_client);

int main() {
  int listen_socket;
  int f;
  int a;

  //shared memory initizlizations
  int shmid = shmget(KEY, 7*sizeof(int), IPC_CREAT | 0666);
  if(shmid < 0){
    printf("error in shmget\n");
  }
  //THIS ARRAY HOLDS ALL THE pids (clients)
  //pids[0] = pids[0]
  //pids[1] = pids[1]
  //the rest are actual pids
  int* pids = shmat(shmid, 0, 0);
  if((int)pids < 0){
    printf("error in shmat\n");
  }
  for (a = 0; a < 7; a ++) {
    pids[a] = 0;
    printf("%d\n", pids[a]);
  }

  listen_socket = server_setup();

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

int getRandPID(int*array) {
  srand(time(NULL));
  int r = rand() % 7;
  while(!array[r] || r == 0 || r == 1){
    r = rand()%7;
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
  int shmid = shmget(KEY, 5*sizeof(int), IPC_CREAT | 0666);
  int * pids = shmat(shmid, 0, 0);

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);

    //adding pid to pids array
    if(!already_connected(getpid(),pids,7)){
      int index=2;
      while(pids[index] !=0 && index< 7){
        index++;
      }
      pids[index] = getpid();
      printf("pids[%d]: %d\n", index, getpid());
    }

    //initial setup to turn all clients red
    write(client_socket, RED, sizeof(char *));

    //game start fxn
    if (!pids[1] && getpid() == getRandPID(pids)) {
      write(client_socket, CYAN, sizeof(char *));
      pids[1] = 1;
    }

    //write(client_socket, CYAN, sizeof(char *));

    //if read = blue
    if (!strcmp(CYAN, buffer)) {
      //getrandpid
      pids[0] = 1;
    }
    if (pids[0] && getpid() == getRandPID(pids)) {
      //if getpid = getrandpid write blue
      write(client_socket, CYAN, sizeof(char *));
      pids[0] = 0;
    }

  }//end read loop
  close(client_socket);
  exit(0);
}
