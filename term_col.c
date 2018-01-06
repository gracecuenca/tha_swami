#include "headers.h"

int rows() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  //printf("number of lines:%d\n", w.ws_row);
  return w.ws_row;
}

int cols() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  //printf("number of cols: %d\n", w.ws_col);
  return w.ws_col;
}

int term_size(){
  return rows() * cols();
}

void clearscreen(){
  printf(CLEAR);
}

int print_change(char * base) {
  int row, col, i;
  row = rows();
  col = cols();
  //clearscreen(row);
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

void change_color(char * color){
  int i;
  clearscreen();
  int size = term_size();
  char * screen;
  screen = (char*)malloc(size);
  for(i = 0; i < size; i++){
    strcat(screen, " ");
  }
  printf("%s", color);
  printf("%s", screen);
  printf(RESET);
  free(screen);
}

int main() {
  //print_change(RED);
  //print_change(RESET);
  change_color(GREY);
  change_color(RED);
  change_color(GREEN);
  change_color(YELLOW);
  change_color(BLUE);
  change_color(CYAN);
  change_color(MAGENTA);
  return 0;
}
