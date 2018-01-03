#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int rows();
int cols();
int print_change();

int rows() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  printf("number of lines:%d\n", w.ws_row);
  return w.ws_row;
}

int cols() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  printf("number of cols: %d\n", w.ws_col);
  return w.ws_col;
}

int print_change(char * base) {
  int row, col, i;
  row = rows();
  col = cols();
  printf("col: %d\n", col);
  printf("a");
  char change_color[1000];
  printf("a");
  char one_row[1000];

  for (i = 0; i < col; i ++) {
    strcat(one_row, "B");
    printf("b");
  }
  strcat(one_row, "\n");

  for (i = 0; i < row; i ++) {
    strcat(change_color, one_row);
  }
  printf(RED "%s" RESET, change_color);
  return 0;
}

int main() {
  print_change("RED");
  return 0;
}
