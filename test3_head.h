//
// Created by wxj on 2024/5/9.
//

#ifndef RUBICSCUBE_TEST3_HEAD_H
#define RUBICSCUBE_TEST3_HEAD_H
#define White 6

extern  SDL_GLContext ctx;
extern SDL_Window *window1;
void handle();

void change_num(int *ptr[], int flag) ;

void update_face() ;

void change_face(int ins) ;
void init();

static
void draw(int flag, double angle) ;

void draw_cube(int flag);

void exe_line(int *ptr) ;


void change_line(int *ptr) ;

void solve() ;

int get_pos(int x, int y) ;




#endif //RUBICSCUBE_TEST3_HEAD_H
