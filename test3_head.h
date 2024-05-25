//
// Created by wxj on 2024/5/9.
//

#ifndef RUBICSCUBE_TEST3_HEAD_H
#define RUBICSCUBE_TEST3_HEAD_H
#define Grey 0
#define Yellow 1
#define Blue 2
#define Red 3
#define Green 4
#define Orange 5
#define White 6
#define Black 7

extern SDL_GLContext ctx;
extern SDL_Window *window1;
extern SDL_Window *window2;

void handle();

void change_num(int *ptr[], int flag);

void update_face();

void change_face(int ins);

void init();

static
void draw(int flag, double angle);

void draw_cube(int flag);

void exe_line(int *ptr);


void change_line(int *ptr);

void solve();
void exe_str(char * str);
int get_pos(int x, int y);

void create_button(int x, int y, int w, int h, char *str,void (*func1)(void));

void draw_window();
int get_button_num(int x,int y);
void empty();
void reset();
void random_cube();
int get_block_num(int x,int y);
void change_color(int faceAndPos,int color);
#endif //RUBICSCUBE_TEST3_HEAD_H
