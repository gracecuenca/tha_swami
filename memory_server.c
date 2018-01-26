#include "headers.h"
void subserver(int from_client);

static char COLORS[10][10] = {
  RED,
  GREEN,
  YELLOW,
  BLUE,
  //MAGENTA,
  //CYAN,
  //GREY
};
int subserver_color_num = 0; //designates color
int server_to_ss[2 * 4];
//int ss_to_server[2 * 4];

int main() {
  srand(time(NULL));
  int listen_socket;
  int f;
  int a;

  for (int i = 0; i < 4; i++) {
    pipe(&server_to_ss[2 * i]);
  }

  int pid_list_shmid = shmget(KEY, 10*sizeof(int), IPC_CREAT | 0666);
  if(pid_list_shmid < 0){
    printf("error in shmget 1\n");
  }
  int* pids_list = shmat(pid_list_shmid, 0, 0);
  if((int)pids_list < 0){
    printf("error in shmat 1\n");
  }
  
 int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), IPC_CREAT | 0666);
  if(pid_list_shmid < 0){
    printf("error in shmget 2\n");
  }
  int* mem_matrix = shmat(mem_matrix_shmid, 0, 0);
  if((int)pids_list < 0){
    printf("error in shmat 2\n");
  }
  
 int player_choice_shmid = shmget(KEY3, 10*sizeof(int), IPC_CREAT | 0666);
  if(pid_list_shmid < 0){
    printf("error in shmget 2\n");
  }
  int* player_choice = shmat(player_choice_shmid, 0, 0);
  if((int)pids_list < 0){
    printf("error in shmat 2\n");
  }
  

  listen_socket = server_setup();

  while (subserver_color_num < 4) {
    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) {
      close(server_to_ss[2 * subserver_color_num + WRITE]);
      subserver(client_socket);
    }
    else {
      close(server_to_ss[2 * subserver_color_num + READ]);
      close(client_socket);
    }
    subserver_color_num += 1;
  }

  sleep(1);
 
  for (int i = 0; i < 4; i++) {
    printf("pids_list[%d]: %d\n", i, pids_list[i]);
  }
  
  for (int i = 0; i < 10; i++) {
    mem_matrix[i] = pids_list[rand()%4];
    printf("position %d: pid %d\n", i, mem_matrix[i]);
  }

  printf("Sending subservers the signal to begin flashing...\n");

  for (int i = 0; i < 4; i++) {
    write(server_to_ss[2 * i + WRITE], "FILLER", sizeof("FILLER"));
  }

  printf("Main server finished\n");
  
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

  int pid_list_shmid = shmget(KEY, 10*sizeof(int), 0666);
  int * pids_list = shmat(pid_list_shmid, 0, 0);
  
  int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), 0666);
  int * mem_matrix = shmat(mem_matrix_shmid, 0, 0);

  int player_choice_shmid = shmget(KEY3, 10*sizeof(int), 0666);
  int * player_choice = shmat(player_choice_shmid, 0, 0);
  
  char * color = COLORS[subserver_color_num];
  
  pids_list[subserver_color_num] = getpid();
  printf("pids_list[%d]: %d is the color %s\n", subserver_color_num, pids_list[subserver_color_num], COLORS[subserver_color_num]);

  read(client_socket, buffer, sizeof(buffer));
  printf("[subserver %d] received: [%s]\n", getpid(), buffer);
  write(client_socket, color, sizeof(char *));

  printf("made it here\n");

  read(server_to_ss[2 * subserver_color_num + READ], &buffer, sizeof(buffer));

  printf("made it here\n");

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);


    //initial setup to turn all clients to their color
    write(client_socket, color, sizeof(char *));

  }//end read loop
  close(client_socket);
  exit(0);
}

/*
  for (int index_of_pattern = 0; index_of_pattern < 10; index_of_pattern++) {
    if (mem_matrix[index_of_pattern] == getpid()) {
      write(client_socket, WHITE, sizeof(char *));
    }
    sleep(1);
    if (mem_matrix[index_of_pattern] == getpid()) {
      write(client_socket, color, sizeof(char *));
    }
    sleep(1);
  }
*/
