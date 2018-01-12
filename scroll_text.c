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

char** terminal_line(){
  int num_rows = rows();
  char** lines = calloc(num_rows, 256);
  int index;
  //where index 0 is top
  for(index = 0; index < num_rows; index++){
    lines[index] = "a";//temp, find out how we will get text in first place
  }
  index = 0;
  while(num_rows){
    printf("%s\n", lines[index]);
    index++;
    num_rows--;
  }
  return lines;
}

int main(){
  clearscreen();
  terminal_line();
}
