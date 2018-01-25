#include "headers.h"
#define READ 0
#define WRITE 1

void subserver(int, int);
int send_colors(int, char *, char *);

//for pipes, server to subserver and vice versa, up to 5 subservers, 2 pipes per subserver
int server_to_ss[2 * 5];
int ss_to_server[2 * 5];

int main() {
  int listen_socket, client_socket, a, i, f, pipes, cyan;
  int subserver_count = 0;
  int started = 0;
  int can_change = 0;
  int score = 0;
  int old_score = 0;
  fd_set read_fds;
  char buffer[BUFFER_SIZE];

  int self_pipe[2];

  pipe(self_pipe);

  //initalize pipes
  for (i = 0; i < 5; i ++) {
    pipe(&server_to_ss[2 * i]);
    pipe(&ss_to_server[2 * i]);
  }

  listen_socket = server_setup();
  printf("\n\n=====Whack-A-Mole=====\n");
  printf("Connect clients and press ENTER to start game\n\n");
  while (1) {

    /***
	If started, display score if it changed. Loop through pipes and
	see if CYAN was sent. If it was, send new colors. If not, keep
	same state.
     ***/
    if (started) {
      //score keeping
      if (old_score != score) {
        printf("=====SCORE: %d=====\n", score);
        old_score = score;
      }

      //loop through pipes and look for cyan
      for (pipes = 0; pipes < subserver_count; pipes ++) {
        close(ss_to_server[2 * pipes + WRITE]);
        read(ss_to_server[2 * pipes + READ], &buffer, sizeof(buffer));

        //if cyan, increment score, allow colors to be changed, exit loop
        if (!strcmp(CYAN, buffer) && !can_change && pipes == cyan) {
          can_change = 1;
          score ++;
          break;
        }
      }

      //assign new color values, stop colors from changing
      if (can_change) {
        cyan = send_colors(subserver_count, RED, CYAN);
        can_change = 0;
      }

      //keep same color state
      else {
        for (pipes = 0; pipes < subserver_count; pipes ++) {
          if (pipes == cyan) {
            close(server_to_ss[2 * cyan + READ]);
            write(server_to_ss[2 * cyan + WRITE], CYAN, sizeof(char *));
          }
          else {
            close(server_to_ss[2 * pipes + READ]);
            write(server_to_ss[2 * pipes + WRITE], RED, sizeof(char *));
          }
        }
      }
    }

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set
    FD_SET(self_pipe[READ], &read_fds); //add self pipe to stop blocking

    //select will block until either fd is ready
    //if started, run self pipe to stop blocking on select()
    if (started) {
      write(self_pipe[WRITE], "", sizeof(""));
      select(self_pipe[READ] + 1, &read_fds, NULL, NULL, NULL);
    }
    else {
      select(listen_socket + 1, &read_fds, NULL, NULL, NULL);
    }

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
      client_socket = server_connect(listen_socket);

      f = fork();
      if (f == 0) {
        printf("Client %d connected\n", subserver_count);
        subserver(client_socket, subserver_count);
      }
      else {
        subserver_count++;
        close(client_socket);
      }
    }

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);

      //start game
      if (!strcmp(buffer, "\n") && !started) {
        printf("Game started: Press ENTER on blue clients\n");
        sleep(1);
        cyan = send_colors(subserver_count, RED, CYAN);
        started = 1;
      }

      /***
      //end game
      if (!strcmp(buffer, "\n") && started) {
      send_colors(subserver_count, RED, RED);
      sleep(1);
      send_colors(subserver_count, CYAN, CYAN);
      sleep(1);
      send_colors(subserver_count, RED, RED);
      sleep(1);
      send_colors(subserver_count, CYAN, CYAN);
      sleep(1);
      send_colors(subserver_count, RESET, RESET);
      printf("=====Thanks for playing!=====\n\n");
      printf("=====  Final score: %d  =====\n\n", score);
      printf("==Press ENTER to play again==\n\n");
      score = 0;
      started = 0;
    }
      ***/
    }

  }
}

/***
    Sends strings from main server to subservers via pipes
    Takes:
      int subserver_count: number of connected subservers
      char * to_send_all: color to send to all subservers
      char * to_send_rand: color to send to random subserver
 ***/
int send_colors(int subserver_count, char * to_send_all, char * to_send_rand) {
  int pipes = 0;
  int rand_ss = rand() % subserver_count;

  for (pipes; pipes < subserver_count; pipes ++) {
    if (pipes == rand_ss) {
      //send to_send_rand to subserver rand_ss
      close(server_to_ss[2 * rand_ss + READ]);
      write(server_to_ss[2 * rand_ss + WRITE], to_send_rand, sizeof(char *));
    }
    else {
      //send to_send_all to all pipes
      close(server_to_ss[2 * pipes + READ]);
      write(server_to_ss[2 * pipes + WRITE], to_send_all, sizeof(char *));
    }
  }

  return rand_ss;
}

void subserver(int client_socket, int subserver_num) {
  char from_server[BUFFER_SIZE];
  char from_client[BUFFER_SIZE];

  while(read(client_socket, from_client, sizeof(from_client))) {

    //send what was recieved from client to main server
    close(ss_to_server[2 * subserver_num + READ]);
    write(ss_to_server[2 * subserver_num + WRITE], &from_client, sizeof(from_client));

    //read response from main server
    close(server_to_ss[2 * subserver_num + WRITE]);
    read(server_to_ss[2 * subserver_num + READ], &from_server, sizeof(from_server));

    //send response from server to client
    write(client_socket, from_server, sizeof(from_server));

  }
  close(client_socket);
  exit(0);
}
