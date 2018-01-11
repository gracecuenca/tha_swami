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
  printf("\n");
  printf(RESET);
  return screen;
}
