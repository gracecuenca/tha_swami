#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/mman.h>

#define KEY 1

#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 256
#define PORT "9002"
#define TEST_IP "127.0.0.1"

#define RED     "\x1b[41m"
#define GREEN   "\x1b[42m"
#define YELLOW  "\x1b[43m"
#define BLUE    "\x1b[44m"
#define MAGENTA "\x1b[45m"
#define CYAN    "\x1b[46m"
#define GREY    "\x1b[47m"
#define RESET   "\x1b[0m"
#define CLEAR   "\x1b[2J"

void error_check(int i, char *s);
int server_setup();
int server_connect(int sd);
int client_setup(char * server);

#endif

int rows();
int cols();
int term_size();
char * change_color(char *);
void clearscreen();
void print_screen(char** screen);
char** terminal_line();
char** screen_shift(char** init_screen);
char** screen_move(char** screen);
