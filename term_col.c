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

char * change_color(char * color){
  clearscreen();
  int i;
  int size = term_size();
  char * screen;
  screen = malloc(size);

  for(i = 0; i < size; i++){
    strcat(screen, " ");
  }
  printf("%s", color);
  printf("%s", screen);
  printf(RESET);
  return screen;
}

int main() {
  char * color;
  //print_change(RED);
  //print_change(RESET);
  color = change_color(GREY);
  sleep(3);
  color = change_color(RED);
  sleep(3);
  color = change_color(GREEN);
  sleep(3);
  color = change_color(YELLOW);
  sleep(3);
  color = change_color(BLUE);
  sleep(3);
  color = change_color(CYAN);
  sleep(3);
  color = change_color(MAGENTA);
  free(color);
  return 0;
}
