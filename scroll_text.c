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

void print_screen(char** screen){
  int i;
  for(i = 0; i < rows(); i++){
    printf("%s\n", screen[i]);
  }
}

char** terminal_line(){
  int num_rows = rows();
  int num_cols = cols();
  char** lines = malloc(num_rows * sizeof(char*));
  int index = 0;
  int i = 0;
  //where index 0 is top
  for(index = 0; index < num_rows; index++){
    lines[index] = malloc(num_cols * sizeof(char*));
    while(i < num_cols - 1){
      strcat(lines[index], "-");
      i++;
    }
    i=0;
    strcat(lines[index], "a");
  }
  print_screen(lines);
  //add freeing stuff
  return lines;
}

char** screen_shift(char** init_screen){
  int row;
  char * temp;
  for(row = 0; row < rows(); row++){
    init_screen[row]+=1;
    strcat(init_screen[row], "-");
  }
  clearscreen();
  print_screen(init_screen);
  //add freeing stuff
  return init_screen;
}

char** screen_move(char** screen){
  int col;
  char** term = screen;
  for(col = 0; col < cols(); col++){
    term = screen_shift(term);
    sleep(1);
  }
  return term;
}
