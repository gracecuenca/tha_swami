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
int client_socket;
int mem_matrix[10];

static void sighandler(int i) {
  if (i == SIGINT) {
    //printf("sighandler triggered\n");
    write(client_socket, "END", sizeof("END"));
    close(client_socket);
    exit(0);
  }
}

/*
  int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), IPC_CREAT | 0666);
  if(mem_matrix_shmid < 0){
    printf("error in shmget 2\n");
  }

  int* mem_matrix = shmat(mem_matrix_shmid, 0, 0);
  if((int)mem_matrix < 0){
    printf("error in shmat 2\n");
  }
*/


int main() {
  srand(time(NULL));
  int listen_socket;
  int f;
  int a;
  int i;
  int j;
  int h;
  int max_pos;
  char buffer[BUFFER_SIZE];
  pid_t pids_list[4];

  signal(SIGINT, sighandler);

  for (i = 0; i < 4; i++) {
    pipe(&server_to_ss[2 * i]);
    pipe(&ss_to_server[2 * i]);
  }

  listen_socket = server_setup();

  for (i = 0; i < 10; i++) {
    mem_matrix[i] = rand()%4;
    //printf("position %d: value %d\n", i, mem_matrix[i]);
  }
  while (subserver_color_num < 4) {
    client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0) {
      pids_list[subserver_color_num] = getpid();
      close(server_to_ss[(2 * subserver_color_num) + WRITE]);
      close(ss_to_server[(2 * subserver_color_num) + READ]);
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


  for (max_pos = 1; max_pos <= 10; max_pos++) {

    //printf("Sending subservers the signal to begin flashing...\n");

    sleep(3);

    for (i = 0; i < 4; i++) {
      write(server_to_ss[2 * i + WRITE], "FILLER", sizeof("FILLER0"));
      //printf("server made it past init stage %d\n", i);
    }

    //printf("Sending subservers the flashing signals...\n");

    for (i = 0; i < max_pos; i++) {
      sleep(1);
      write(server_to_ss[(2 * mem_matrix[i]) + WRITE], "FILLER", sizeof("FILLER1"));
      //printf("server used pipe index %d\n", (2 * mem_matrix[i]) + WRITE);
      //printf("server made it past write stage %d\n", i);
      read(ss_to_server[(2 * mem_matrix[i]) + READ], &buffer, sizeof(buffer));
      //printf("server made it past read stage %d\n", i);
    }

    //printf("Sending subservers the signal to begin receiving input...\n");

    for (i = 0; i < 4; i++) {
      write(server_to_ss[2 * i + WRITE], "FILLER0", sizeof("FILLER0"));
      //printf("server made it past init stage %d\n", i);
    }

    //printf("Checking client's response...\n");

    for (i = 0; i < max_pos; i++) {
      sleep(1);

      write(server_to_ss[(2 * 0) + WRITE], "FILLER1", sizeof("FILLER1"));
      write(server_to_ss[(2 * 1) + WRITE], "FILLER1", sizeof("FILLER1"));
      write(server_to_ss[(2 * 2) + WRITE], "FILLER1", sizeof("FILLER1"));
      write(server_to_ss[(2 * 3) + WRITE], "FILLER1", sizeof("FILLER1"));

      for (j = 0; j < 4; j++) {
	read(ss_to_server[(2 * j) + READ], &buffer, sizeof(buffer));
	if (j == mem_matrix[i]) {
	  if (strcmp(buffer, "y")) {
	    printf("GAME OVER\n");
	    for (h = 0; h < 4; h++) {
	      kill(pids_list[h], SIGINT);
	    }
	    exit(0);
	  }
	}
	else {
	  if (!strcmp(buffer, "y")) {
	    printf("GAME OVER\n");
	    for (h = 0; h < 4; h++) {
	      kill(pids_list[h], SIGINT);
	    }
	    exit(0);
	  }
	}
      }

      printf("Correct... Continue\n");
      //printf("server made it past read stage %d\n", i);
    }
  }

  printf("You Win!\n");

}





void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];
  int index_of_pattern;
  int i;
  int j;

  /*
  int mem_matrix_shmid = shmget(KEY2, 10*sizeof(int), 0666);
  int * mem_matrix = shmat(mem_matrix_shmid, 0, 0);
  */

  char * color = COLORS[subserver_color_num];

  read(client_socket, &buffer, sizeof(buffer));
  printf("[subserver %d] received: [%s]\n", getpid(), buffer);
  write(client_socket, color, sizeof(char *));

  //printf("subserver %d made it past sending client color\n", subserver_color_num);



  for (index_of_pattern = 1; index_of_pattern <= 10; index_of_pattern++) {


    read(server_to_ss[(2 * subserver_color_num) + READ], &buffer, sizeof(buffer));
    //printf("buffer: %s\n", buffer);


    for (i = 0; i < index_of_pattern; i++) {
      if (mem_matrix[i] == subserver_color_num) {
	//printf("subserver %d triggered the %d read part of the loop\n", subserver_color_num, i);
	read(server_to_ss[(2 * subserver_color_num) + READ], &buffer, sizeof(buffer));
	//printf("subserver %d read from main server during the %d read part of the loop\n", subserver_color_num, i);
	write(client_socket, GREY, sizeof(char *));
	//printf("subserver %d wrote to the client during the %d read part of the loop\n", subserver_color_num, i);
      }

      //printf("subserver %d made it past the %d read part of the loop\n", subserver_color_num, i);

      sleep(1);

      if (mem_matrix[i] == subserver_color_num) {
	//printf("subserver %d triggered the %d write part of the loop\n", subserver_color_num, i);
	write(client_socket, color, sizeof(char *));
	//printf("subserver %d wrote to the client during the %d write part of the loop\n", subserver_color_num, i);
	write(ss_to_server[(2 * subserver_color_num) + WRITE], "FILLER2", sizeof("FILLER2"));
	//printf("subserver %d wrote to main server during the %d write part of the loop\n", subserver_color_num, i);
      }

      //printf("subserver %d made it past the %d write part of the loop\n", subserver_color_num, i);

    }

    read(server_to_ss[(2 * subserver_color_num) + READ], &buffer, sizeof(buffer));
    //printf("subserver %d made it past the second init point\n", subserver_color_num);
    //printf("buffer val: %s\n", buffer);

    for (j = 0; j < index_of_pattern; j++) {

      read(server_to_ss[(2 * subserver_color_num) + READ], &buffer, sizeof(buffer));
      //printf("subserver %d read from the server during the %d loop\n", subserver_color_num, j);

      write(client_socket, "PUSH", sizeof("PUSH"));
      //printf("subserver %d wrote to the client during the %d loop\n", subserver_color_num, j);

      read(client_socket, &buffer, sizeof(buffer));
      //printf("subserver %d read from the client during the %d loop\n", subserver_color_num, j);
      //printf("buffer: %s\n", buffer);

      write(ss_to_server[(2 * subserver_color_num) + WRITE], buffer, sizeof(buffer));
      //printf("subserver %d wrote to main server during the %d loop\n", subserver_color_num, j);

    }
  }

  write(client_socket, "END", sizeof("END"));

  close(client_socket);
  printf("closed client socket\n");
  exit(0);
}
