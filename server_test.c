#include "headers.h"
#define READ 0
#define WRITE 1

void subserver(int, int);
void send_colors(int, char *, char *);

//for pipes, server to subserver and vice versa, up to 5 subservers, 2 pipes per subserver
int server_to_ss[2 * 5];
int ss_to_server[2 * 5];

int main() {
  int listen_socket, client_socket, a, i, f, pipes;
  int subserver_count = 0;
  int started = 0;
  int can_change = 0;
  fd_set read_fds;
  char buffer[BUFFER_SIZE];

  //initalize pipes
  for (i = 0; i < 5; i ++) {
    pipe(&server_to_ss[2 * i]);
    pipe(&ss_to_server[2 * i]);
  }

  listen_socket = server_setup();

  while (1) {
    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
      client_socket = server_connect(listen_socket);
      
      f = fork();
      if (f == 0)
	subserver(client_socket, subserver_count);
      else { //if main server
	subserver_count++;

	/***
	    The problem is here: none of the messages in this block are printing. I think the problem
	    has to do with which fd is selected, it might be because we want to listen to the pipe from 
	    the subservers to the main server, not the socket. 
	 ***/
	if (started) { //if started
	  printf("Game started\n");
	  for (pipes = 0; pipes < subserver_count; pipes ++) { //loop and read what subservers are saying
	    close(ss_to_server[2 * pipes + WRITE]);
	    read(ss_to_server[2 * pipes + READ], &buffer, sizeof(buffer));
	    printf("Started: Main server recieved %s ", buffer); printf(RESET);
	    printf("from subserver %d\n", pipes);
	    if (!strcmp(CYAN, buffer) && !can_change) { //if recieved CYAN, colors can change
	      can_change = 1;
	      break;
	    }
	  }
	  if (can_change) { //if colors can change
	    send_colors(subserver_count, RED, CYAN);
	  }
	}
	close(client_socket);
      }
    }//end listen_socket select
    
    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      if (!strcmp(buffer, "\n") && !started) {
	send_colors(subserver_count, RED, CYAN);
	started = 1;
      }
    }//end stdin select
  }
}

/***
    Sends strings from main server to subservers via pipes
    Takes:
      int subserver_count: number of connected subservers
      char * to_send_all: color to send to all subservers
      char * to_send_rand: color to send to random subserver
 ***/
void send_colors(int subserver_count, char * to_send_all, char * to_send_rand) {
  int pipes = 0;
  int rand_ss = rand() % subserver_count;

  for (pipes; pipes < subserver_count; pipes ++) { //send to_send_all to all subservers
    close(server_to_ss[2 * pipes + READ]);
    write(server_to_ss[2 * pipes + WRITE], to_send_all, sizeof(char *));
    printf("Main server sending %s ", to_send_all);
    printf(RESET);
    printf("to all subservers\n");
  }
  sleep(1); //sometimes it sends too quick so need to sleep

  //send to_send_rand to subserver rand_ss
  close(server_to_ss[2 * rand_ss + READ]);
  write(server_to_ss[2 * rand_ss + WRITE], to_send_rand, sizeof(char *));
  printf("Main server sending %s ", to_send_rand);
  printf(RESET);
  printf("to subserver %d\n", rand_ss);
}

void subserver(int client_socket, int subserver_num) {
  char from_server[BUFFER_SIZE];
  char from_client[BUFFER_SIZE];

  while(read(client_socket, from_client, sizeof(from_client))) {

    /***
	The problem could also be here, it might have to do with the order of how the subserver is handling
	sending to/from client and to/from main server.
     ***/

    //recieve from client
    printf("subserver %d recieved %s ", subserver_num, from_client);
    printf(RESET);
    printf("from client\n");

    //send what was recieved from client to main server
    printf("subserver %d writing %s ", subserver_num, from_client);
    printf(RESET);
    printf("to main server\n");
    close(ss_to_server[2 * subserver_num + READ]);
    write(ss_to_server[2 * subserver_num + WRITE], &from_client, sizeof(from_client));

    //read response from main server
    close(server_to_ss[2 * subserver_num + WRITE]);
    read(server_to_ss[2 * subserver_num + READ], &from_server, sizeof(from_server));
    printf("subserver %d received %s ", subserver_num, from_server);
    printf(RESET);
    printf("from main server\n");

    //send response from server to client
    write(client_socket, from_server, sizeof(from_server));
    printf("subserver %d wrote %s ", subserver_num, from_server); 
    printf(RESET);
    printf("to client\n");

  }
  close(client_socket);
  exit(0);
}
