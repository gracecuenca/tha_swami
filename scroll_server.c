#include "headers.h"
#define READ 0
#define WRITE 1
#define MY_KEY 1001

void subserver(int, int);

//for pipes, server to subserver and vice versa, up to 5 subservers, 2 pipes per subserver
int server_to_ss[2 * 5];
int ss_to_server[2 * 5];

int main() {

  int listen_socket, client_socket;
  int f, i;
  int started = 0;
  int can_change = 0;
  int pipes = 0;
  int cyan = 0;
  int subserver_count = 0;
  fd_set read_fds;
  char buffer[BUFFER_SIZE]; //enter on stdin server
  char input[BUFFER_SIZE];

  int self_pipe[2];

  pipe(self_pipe);

  //initalize pipes
  for (i = 0; i < 5; i ++) {
    pipe(&server_to_ss[2 * i]);
    pipe(&ss_to_server[2 * i]);
  }

  listen_socket = server_setup();

  printf("\n\n=====Scrolling Text=====\n");
  printf("Connect all the clients you want to send message to first, then hit ENTER\n");

  while (1) {


    if (started) {
      //loop through pipes and look for cyan
      for (pipes = 0; pipes < subserver_count; pipes ++) {
        close(ss_to_server[2 * pipes + WRITE]);
        read(ss_to_server[2 * pipes + READ], &buffer, sizeof(buffer));
	printf("Main server recieved %s from subserver %d\n", buffer, pipes);
        //if cyan, increment score, allow colors to be changed, exit loop
        if (pipes == cyan && !strcmp(buffer, "done") && !can_change) {
	  can_change = 1;
	  break;
	}
      }
      if (can_change) {
	cyan ++;
	close(server_to_ss[2 * cyan + READ]);
	write(server_to_ss[2 * cyan + WRITE], &input, sizeof(input));
	printf("Main server writing %s to subserver %d\n", input, cyan);
	can_change = 0;
      }
      else {
	for (pipes = 0; pipes < subserver_count; pipes ++) {
	  if (pipes != cyan) {
	    close(server_to_ss[2 * pipes + READ]);
	    write(server_to_ss[2 * pipes + WRITE], "", sizeof(""));
	    printf("Main server writing \"\" to subserver %d\n", pipes);
	  }
	  else {
	    close(server_to_ss[2 * pipes + READ]);
	    write(server_to_ss[2 * pipes + WRITE], "", sizeof(""));
	  }
	}
      }
      if (cyan == subserver_count) {
	printf("Scrolling text complete.\n");
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
      fgets(input, sizeof(input), stdin);

      //start game
      if (!strcmp(input, "\n") && !started) {
        printf("Enter input: ");
	fgets(input, sizeof(input), stdin);
	input[strlen(input) - 1] = 0;
        close(server_to_ss[2 * cyan + READ]);
	write(server_to_ss[2 * cyan + WRITE], &input, sizeof(input));
        started = 1;
      }
    }
  }
}

void subserver(int client_socket, int subserver_num) {
  char from_server[BUFFER_SIZE];
  char from_client[BUFFER_SIZE];

  while(read(client_socket, from_client, sizeof(from_client))) {
    printf("subserver %d received %s from client\n", subserver_num, from_client);
    printf("subserver %d writing %s to main server\n", subserver_num, from_client);
    //send what was recieved from client to main server
    close(ss_to_server[2 * subserver_num + READ]);
    write(ss_to_server[2 * subserver_num + WRITE], &from_client, sizeof(from_client));

    //read response from main server
    close(server_to_ss[2 * subserver_num + WRITE]);
    read(server_to_ss[2 * subserver_num + READ], &from_server, sizeof(from_server));
    printf("subserver %d received %s from main server\n", subserver_num, from_server);
    printf("subserver %d writing %s to client\n", subserver_num, from_server);
    //send response from server to client
    write(client_socket, from_server, sizeof(from_server));

  }
  close(client_socket);
  exit(0);
}
