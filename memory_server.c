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
int ss_to_server[2 * 4];

int main() {
  srand(time(NULL));
  int listen_socket;
  int f;
  int a;
  char buffer[BUFFER_SIZE];

  for (int i = 0; i < 4; i++) {
    pipe(&server_to_ss[2 * i]);
    pipe(&ss_to_server[2 * i]);
  }

  int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), IPC_CREAT | 0666);
  if(mem_matrix_shmid < 0){
    printf("error in shmget 2\n");
  }

  int* mem_matrix = shmat(mem_matrix_shmid, 0, 0);
  if((int)mem_matrix < 0){
    printf("error in shmat 2\n");
  }

  int player_choice_shmid = shmget(KEY3, 10*sizeof(int), IPC_CREAT | 0666);
  if(player_choice_shmid < 0){
    printf("error in shmget 2\n");
  }
  int* player_choice = shmat(player_choice_shmid, 0, 0);
  if((int)player_choice < 0){
    printf("error in shmat 2\n");
  }
  

  listen_socket = server_setup();

  while (subserver_color_num < 4) {
    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) {
      close(server_to_ss[(2 * subserver_color_num) + WRITE]);
      close(ss_to_server[2 * subserver_color_num + READ]);
      subserver(client_socket);
    }
    else {
      close(server_to_ss[(2 * subserver_color_num) + READ]);
      close(ss_to_server[(2 * subserver_color_num) + WRITE]);
      close(client_socket);
    }
    subserver_color_num += 1;
  }

  sleep(1);

  for (int i = 0; i < 10; i++) {
    mem_matrix[i] = rand()%4;
    printf("position %d: value %d\n", i, mem_matrix[i]);
  }

  printf("Sending subservers the signal to begin flashing...\n");

  for (int i = 0; i < 4; i++) {
    write(server_to_ss[2 * i + WRITE], "FILLER", sizeof("FILLER0"));
    printf("server made it past init stage %d\n", i);
  }
  
  printf("Sending subservers the flashing signals...\n");

  for (int i = 0; i < 10; i++) {
    write(server_to_ss[(2 * mem_matrix[i]) + WRITE], "FILLER", sizeof("FILLER1"));
    printf("server made it past write stage %d\n", i);
    read(ss_to_server[(2 * mem_matrix[i]) + READ], &buffer, sizeof(buffer));
    printf("server made it past read stage %d\n", i);
  }

  printf("Main server finished\n");
  
}





void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), 0666);
  int * mem_matrix = shmat(mem_matrix_shmid, 0, 0);

  int player_choice_shmid = shmget(KEY3, 10*sizeof(int), 0666);
  int * player_choice = shmat(player_choice_shmid, 0, 0);
  
  char * color = COLORS[subserver_color_num];
  
  read(client_socket, buffer, sizeof(buffer));
  printf("[subserver %d] received: [%s]\n", getpid(), buffer);
  write(client_socket, color, sizeof(char *));

  printf("subserver %d made it past sending client color\n", subserver_color_num);

  read(server_to_ss[(2 * subserver_color_num) + READ], &buffer, sizeof(buffer));

  for (int index_of_pattern = 0; index_of_pattern < 10; index_of_pattern++) {
    
    if (mem_matrix[index_of_pattern] == subserver_color_num) {
      printf("subserver %d triggered the %d read part of the loop\n", subserver_color_num, index_of_pattern);
      read(server_to_ss[(2 * subserver_color_num) + READ], &buffer, sizeof(buffer));
      printf("subserver %d read from main server during the %d read part of the loop\n", subserver_color_num, index_of_pattern);
      write(client_socket, GREY, sizeof(char *));
      printf("subserver %d wrote to the client during the %d read part of the loop\n", subserver_color_num, index_of_pattern);
    }

    printf("subserver %d made it past the %d read part of the loop\n", subserver_color_num, index_of_pattern);
    
    if (mem_matrix[index_of_pattern] == subserver_color_num) {
      printf("subserver %d triggered the %d write part of the loop\n", subserver_color_num, index_of_pattern);
      write(client_socket, color, sizeof(char *));
      printf("subserver %d wrote to the client during the %d write part of the loop\n", subserver_color_num, index_of_pattern);
      write(ss_to_server[(2 * subserver_color_num) + WRITE], "FILLER2", sizeof("FILLER2"));
      printf("subserver %d wrote to main server during the %d write part of the loop\n", subserver_color_num, index_of_pattern);
    }
    
    printf("subserver %d made it past the %d write part of the loop\n", subserver_color_num, index_of_pattern);
    
  }

  printf("subserver %d made it past the flash stage\n", subserver_color_num);

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[subserver %d] received: [%s]\n", getpid(), buffer);


    //initial setup to turn all clients to their color
    write(client_socket, color, sizeof(char *));

  }//end read loop
  close(client_socket);
  exit(0);
}



