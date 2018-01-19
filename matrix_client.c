#include "headers.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  int shmdt;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  while (1) {
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    clearscreen();
    //insert matrix client fxns

    //locating memory segment
    shmdt = shmget(KEY, sizeof(int), 0666);

    //attaching memory segment to data space
    int * shm = shmat(shmdt, 0, 0);
  }
}
