#include "headers.h"
void subserver(int from_client);

static char COLORS[10][10] = {
  "RED",
  "GREEN",
  "YELLOW",
  "BLUE",
  "MAGENTA",
  "CYAN",
  "GREY"
};

int main() {
  srand(time(NULL));
  int listen_socket;
  int f;
  int a;

  //even indices are pids
  //odd indices are the index of colors that are assigned to the pid right before it
  int shmid = shmget(KEY, 14*sizeof(int), IPC_CREAT | 0666);
  if(shmid < 0){
    printf("error in shmget\n");
  }
  int* pids = shmat(shmid, 0, 0);
  if((int)pids < 0){
    printf("error in shmat\n");
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

//random index to choose form static COLORS array
int rand_index() {
  int r = rand() % 7;
  return r;
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
  int shmid = shmget(KEY, 14*sizeof(int), IPC_CREAT | 0666);
  int * pids = shmat(shmid, 0, 0);
  char * randd;

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);

    //adding pid to pids array
    if(!already_connected(getpid(),pids,14)){
      int index=0;
      while(pids[index] !=0 && index< 14){
        index++;
      }
      pids[index] = getpid();
      pids[index+1] = rand_index();
      printf("pids[%d]: %d is the color %s\n", index, pids[index], COLORS[pids[index+1]]);
      randd= COLORS[pids[index+1]];
    }

    //initial setup to turn all clients clear
    write(client_socket, randd, sizeof(char *));

  }//end read loop
  close(client_socket);
  exit(0);
}
