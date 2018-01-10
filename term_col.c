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
/***
int main() {
  char * color;
  //print_change(RED);
  //print_change(RESET);
  color = change_color(GREY);
  sleep(1);
  color = change_color(RED);
  sleep(1);
  color = change_color(GREEN);
  sleep(1);
  color = change_color(YELLOW);
  sleep(1);
  color = change_color(BLUE);
  sleep(1);
  color = change_color(CYAN);
  sleep(1);
  color = change_color(MAGENTA);
  free(color);
  return 0;
}
***/
