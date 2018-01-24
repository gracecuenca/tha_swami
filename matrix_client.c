#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  int shmdt;
  char * color;
  
  int pid_list_shmid = shmget(KEY, 10*sizeof(int), 0666);
  int * pids_list = shmat(pid_list_shmid, 0, 0);
  
  int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), 0666);
  int * mem_matrix = shmat(mem_matrix_shmid, 0, 0);

  int player_choice_shmid = shmget(KEY3, 10*sizeof(int), 0666);
  int * player_choice = shmat(player_choice_shmid, 0, 0);

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  write(server_socket, "SETUP", sizeof("SETUP"));
  clearscreen();
  
  while (1) {
    read(server_socket, buffer, sizeof(buffer));
    color = change_color(buffer);
    if (getchar() == '\n') {
      write(server_socket, buffer, sizeof(buffer));
    }
    else {
      write(server_socket, "TEST", sizeof("TEST"));
    }

  }
}
