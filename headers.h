#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define RED     "\x1b[41m"
#define GREEN   "\x1b[42m"
#define YELLOW  "\x1b[43m"
#define BLUE    "\x1b[44m"
#define MAGENTA "\x1b[45m"
#define CYAN    "\x1b[46m"
#define GREY    "\x1b[47m"
#define RESET   "\x1b[0m"
#define CLEAR   "\x1b[2J"

int rows();
int cols();
int term_size();
int print_change();
void clearscreen();
