#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include<setjmp.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <time.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "test3_head.h"

#define OFFSET 0x100
#define INS_ARR_SIZE 500
typedef enum ins {
    Rc = 1,
    Ru = 2,
    R2c = 3,
    R2u = 4,
    Lc = 5,
    Lu = 6,
    L2c = 7,
    L2u = 8,
    Fc = 9,
    Fu = 10,
    F2c = 11,
    F2u = 12,
    Bc = 13,
    Bu = 14,
    B2c = 15,
    B2u = 16,
    Uc = 17,
    Uu = 18,
    U2c = 19,
    U2u = 20,
    Dc = 21,
    Du = 22,
    D2c = 23,
    D2u = 24,
    rc = 25,
    ru = 26,
    r2c = 27,
    r2u = 28,
    lc = 29,
    lu = 30,
    l2c = 31,
    l2u = 32,
    fc = 33,
    fu = 34,
    f2c = 35,
    f2u = 36,
    bc = 37,
    bu = 38,
    b2c = 39,
    b2u = 40,
    uc = 41,
    uu = 42,
    u2c = 43,
    u2u = 44,
    dc = 45,
    du = 46,
    d2c = 47,
    d2u = 48,
    Mc = 49,
    Mu = 50,
    M2c = 51,
    M2u = 52,
    Ec = 53,
    Eu = 54,
    E2c = 55,
    E2u = 56,
    Sc = 57,
    Su = 58,
    S2c = 59,
    S2u = 60,
    yc = 61,
    yu = 62,
    y2c = 63,
    y2u = 64,
    xc = 65,
    xu = 66,
    x2c = 67,
    x2u = 68,
    zc = 69,
    zu = 70,
    z2c = 71,
    z2u = 72,
} INS;
typedef struct button {
    int x, y, w, h;
    char name[12];

    void (*func1)(void);

} Button;
int button_num = 0, click_button, current_button, up_button, click_color_block, current_color_block, color_flag;
Button button_arr[10];
int stack_size = 500, stack_pos = 0;
int *stack;
char inputText[260] = "";
char outputText[32] = "";
int input_flag = 1;
SDL_GLContext ctx;
SDL_Window *window1;
SDL_Window *window2;
SDL_Renderer *renderer;
TTF_Font *font;
SDL_Surface *image;
SDL_Texture *texture1;
int *ins_arr2, ins_pos2 = 0, ins_size2 = 500;
double base_color[8][3] = {
        {0.5,   0.5,   0.5},//grey
        {1.0,   1.0,   0.0},//yellow
        {0.0,   0.0,   1.0},//blue
        {1.0,   0.0,   0.0},//red
        {0.0,   1.0,   0.0},//green
        {1.0,   0.565, 0.0},//orange
        {1.0,   1.0,   1.0},//white
        {0.862, 0.862, 0.862}//LightGrey
};
int button_color[8][4] = {
        {128, 128, 128, 255},
        {255, 255, 0,   255},
        {0,   0,   255, 255},
        {255, 0,   0,   255},
        {0,   255, 0,   255},
        {255, 144, 0,   255},
        {255, 255, 255, 255},
        {0,   0,   0,   255}
};
int block_color[7][4] = {

        {255, 255, 0,   255},
        {0,   0,   255, 255},
        {255, 0,   0,   255},
        {0,   255, 0,   255},
        {255, 144, 0,   255},
        {255, 255, 255, 255},
        {220, 220, 220, 255},
};
double base_cube[8][3] = {
        {0.5,  0.5,  -0.5},
        {0.5,  -0.5, -0.5},
        {-0.5, -0.5, -0.5},
        {-0.5, 0.5,  -0.5},
        {-.5,  0.5,  0.5},
        {0.5,  0.5,  0.5},
        {0.5,  -0.5, 0.5},
        {-0.5, -0.5, 0.5}
};
int vector[27][3] = {};
int color_vector[27][6] = {
        {1, 2, 3, 0, 0, 0},
        {1, 0, 3, 0, 0, 0},
        {1, 0, 3, 4, 0, 0},
        {0, 2, 3, 0, 0, 0},
        {0, 0, 3, 0, 0, 0},
        {0, 0, 3, 4, 0, 0},
        {0, 2, 3, 0, 0, 6},
        {0, 0, 3, 0, 0, 6},
        {0, 0, 3, 4, 0, 6},
        {1, 2, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 4, 0, 0},
        {0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 4, 0, 0},
        {0, 2, 0, 0, 0, 6},
        {0, 0, 0, 0, 0, 6},
        {0, 0, 0, 4, 0, 6},
        {1, 2, 0, 0, 5, 0},
        {1, 0, 0, 0, 5, 0},
        {1, 0, 0, 4, 5, 0},
        {0, 2, 0, 0, 5, 0},
        {0, 0, 0, 0, 5, 0},
        {0, 0, 0, 4, 5, 0},
        {0, 2, 0, 0, 5, 6},
        {0, 0, 0, 0, 5, 6},
        {0, 0, 0, 4, 5, 6},
};
int faces[6][9] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1,},
        {2, 2, 2, 2, 2, 2, 2, 2, 2,},
        {3, 3, 3, 3, 3, 3, 3, 3, 3,},
        {4, 4, 4, 4, 4, 4, 4, 4, 4,},
        {5, 5, 5, 5, 5, 5, 5, 5, 5,},
        {6, 6, 6, 6, 6, 6, 6, 6, 6},
};
int neighbor_vector[6][9][4] = {
        {{3, 2, 4, 0}, {3, 1, -1, -1}, {2, 2, 3, 0}, {4, 1, -1, -1}, {-1, -1, -1, -1}, {2, 1, -1, -1}, {1, 0, 4, 2}, {1, 1, -1, -1}, {1, 2, 2, 0},},
        {{0, 6, 4, 2}, {0, 7, -1, -1}, {0, 8, 2, 0}, {4, 5, -1, -1}, {-1, -1, -1, -1}, {2, 3, -1, -1}, {4, 8, 5, 0}, {5, 1, -1, -1}, {2, 6, 5, 2},},
        {{0, 8, 1, 2}, {0, 5, -1, -1}, {0, 2, 3, 0}, {1, 5, -1, -1}, {-1, -1, -1, -1}, {3, 3, -1, -1}, {1, 8, 5, 2}, {5, 5, -1, -1}, {3, 6, 5, 8},},
        {{0, 2, 2, 2}, {0, 1, -1, -1}, {0, 0, 4, 0}, {2, 5, -1, -1}, {-1, -1, -1, -1}, {4, 3, -1, -1}, {2, 8, 5, 8}, {5, 7, -1, -1}, {4, 6, 5, 6},},
        {{0, 0, 3, 2}, {0, 3, -1, -1}, {0, 6, 1, 0}, {3, 5, -1, -1}, {-1, -1, -1, -1}, {1, 3, -1, -1}, {3, 8, 5, 6}, {5, 3, -1, -1}, {1, 6, 5, 0},},
        {{1, 6, 4, 8}, {1, 7, -1, -1}, {1, 8, 2, 6}, {4, 7, -1, -1}, {-1, -1, -1, -1}, {2, 7, -1, -1}, {3, 8, 4, 6}, {3, 7, -1, -1}, {2, 8, 3, 6},},
};
int ins_vector[73][5] = {
        {0,          0,    0, 0, 0},
        {0x000001FF, -90,  1, 0, 0},
        {0x000001FF, 90,   1, 0, 0},
        {0x000001FF, -180, 1, 0, 0},
        {0x000001FF, 180,  1, 0, 0},
        {0x07FC0000, 90,   1, 0, 0},
        {0x07FC0000, -90,  1, 0, 0},
        {0x07FC0000, 180,  1, 0, 0},
        {0x07FC0000, -180, 1, 0, 0},
        {0x01249249, -90,  0, 0, 1},
        {0x01249249, 90,   0, 0, 1},
        {0x01249249, -180, 0, 0, 1},
        {0x01249249, 180,  0, 0, 1},
        {0x04924924, 90,   0, 0, 1},
        {0x04924924, -90,  0, 0, 1},
        {0x04924924, 180,  0, 0, 1},
        {0x04924924, -180, 0, 0, 1},
        {0x001C0E07, -90,  0, 1, 0},
        {0x001C0E07, 90,   0, 1, 0},
        {0x001C0E07, -180, 0, 1, 0},
        {0x001C0E07, 180,  0, 1, 0},
        {0x070381C0, 90,   0, 1, 0},
        {0x070381C0, -90,  0, 1, 0},
        {0x070381C0, 180,  0, 1, 0},
        {0x070381C0, -180, 0, 1, 0},


        {0x0003FFFF, -90,  1, 0, 0},
        {0x0003FFFF, 90,   1, 0, 0},
        {0x0003FFFF, -180, 1, 0, 0},
        {0x0003FFFF, 180,  1, 0, 0},
        {0x07FFFE00, 90,   1, 0, 0},
        {0x07FFFE00, -90,  1, 0, 0},
        {0x07FFFE00, 180,  1, 0, 0},
        {0x07FFFE00, -180, 1, 0, 0},
        {0x036DB6DB, -90,  0, 0, 1},
        {0x036DB6DB, 90,   0, 0, 1},
        {0x036DB6DB, -180, 0, 0, 1},
        {0x036DB6DB, 180,  0, 0, 1},
        {0x06DB6DB6, 90,   0, 0, 1},
        {0x06DB6DB6, -90,  0, 0, 1},
        {0x06DB6DB6, 180,  0, 0, 1},
        {0x06DB6DB6, -180, 0, 0, 1},
        {0x00FC7E3F, -90,  0, 1, 0},
        {0x00FC7E3F, 90,   0, 1, 0},
        {0x00FC7E3F, -180, 0, 1, 0},
        {0x00FC7E3F, 180,  0, 1, 0},
        {0x07E3F1F8, 90,   0, 1, 0},
        {0x07E3F1F8, -90,  0, 1, 0},
        {0x07E3F1F8, 180,  0, 1, 0},
        {0x07E3F1F8, -180, 0, 1, 0},


        {0x0003FE00, 90,   1, 0, 0},
        {0x0003FE00, -90,  1, 0, 0},
        {0x0003FE00, 180,  1, 0, 0},
        {0x0003FE00, -180, 1, 0, 0},
        {0x00E07038, 90,   0, 1, 0},
        {0x00E07038, -90,  0, 1, 0},
        {0x00E07038, 180,  0, 1, 0},
        {0x00E07038, -180, 0, 1, 0},
        {0x02492492, -90,  0, 0, 1},
        {0x02492492, 90,   0, 0, 1},
        {0x02492492, -180, 0, 0, 1},
        {0x02492492, 180,  0, 0, 1},
        {0x07FFFFFF, -90,  0, 1, 0},
        {0x07FFFFFF, 90,   0, 1, 0},
        {0x07FFFFFF, -180, 0, 1, 0},
        {0x07FFFFFF, 180,  0, 1, 0},
        {0x07FFFFFF, -90,  1, 0, 0},
        {0x07FFFFFF, 90,   1, 0, 0},
        {0x07FFFFFF, -180, 1, 0, 0},
        {0x07FFFFFF, 180,  1, 0, 0},
        {0x07FFFFFF, -90,  0, 0, 1},
        {0x07FFFFFF, 90,   0, 0, 1},
        {0x07FFFFFF, -180, 0, 0, 1},
        {0x07FFFFFF, 180,  0, 0, 1},

};
int ins_set1[6] = {0,
                   xc,
                   zu,
                   xu,
                   zc,
                   x2c};
int ins_set2[6][9][10] = {
        {{},                                 {},                                      {Fu,  Uu, Fc,  Rc,},         {Su,  Du,  Sc, Mc,  Bc, Mu, Bu}, {}, {Sc,  Dc,  Su, Mc,  Bc, Mu, Bu}, {Rc, Uc,  Ru, Fu},
                                                                                                                                                                                                                        {Mc,  Dc, Mu,  Dc, Mc, Bc, Mu, Bu}, {Bc, Rc,  Bu,  Dc, Lu, Du, Lc},},
        {{Lu},                               {Sc,  Mu,  Su},                          {Fu,  Dc, Lu,  Du,  Fc},     {Eu,  Bu,  Mc, Bc,  Mu},         {}, {Ec,  Bc,  Mc, Bu,  Mu},         {Bc, Du,  Bu},
                                                                                                                                                                                                                        {D2c, Mc, Bc,  Mu, Bu},             {Lu, D2c, Lc},},
        {{Rc, Du, Bc,  Dc,  Ru},             {Rc,  Bc,  Mc, Bu,  Mu, Ru},             {Bc},                        {R2c, Bc,  Mc, Bu,  Mu, R2u},    {}, {Bc,  Mc,  Bu, Mu},
                                                                                                                                                                                         {Bc, D2c, Bu},                 {Ru,  Bc, Mc,  Bu, rc},             {Lu, Dc,  Lc},},
        {{Fu, Rc, B2c, Fc},                  {Mc,  Bc,  Mu, Bu,  Ec, Bu, Mc, Bc, Mu}, {Lc,  Bu, Lu,  B2c, Dc, Bu}, {Ec,  Bu,  Mc, Bc,  Mu},         {},
                                                                                                                                                        {Eu,  Bc,  Mc, Bu,  Mu},         {Dc, Lu,  Du, Lc},             {Mc,  Bc, Mu,  Bu},                 {Bc, Dc,  Bu},},
        {{Bu, Lc, Bc,  L2u, Du, Lc},         {Lu,  Bu,  Mc, Bc,  Mu, Lc},             {Rc,  Fu, L2c, Ru},          {Bu,  Mc,  Bc, Mu},              {},
                                                                                                                                                        {L2u, Bu,  Mc, Bc,  Mu, L2c},    {Lu, Du,  Lc},                 {Lc,  Bu, Mc,  Bc, lu},             {Du, Bc,  Dc,  Bu},},
        {{Du, Fc, L2c, Fu,  Du, Bc, Dc, Bu}, {D2c, B2c, Mc, B2u, Mu},                 {D2u, Fc, L2c, Fu,  Du, Bc, Dc, Bu},
                                                                                                                   {Du,  B2c, Mc, B2u, Mu},         {}, {Dc,  B2c, Mc, B2u, Mu},         {Fc, L2c, Fu, Du, Bc, Dc, Bu}, {B2c, Mc, B2u, Mu},
                                                                                                                                                                                                                                                            {Dc, Fc,  L2c, Fu, Du, Bc, Dc, Bu},},

};

int ins_set3[15][15] = {
        {Ru, Fc,  Rc,  Fu, Uu,  Fu,  Uc, Fc},
        {yc, Rc,  Uu,  Ru, Uu,  Fu,  Uc, Fc},
        {Fu, Uu,  Fc,  Uc, Fc,  Ru,  Fu, Rc},
        {Fc, Ru,  Fu,  Rc, Fu,  Uu,  Fc},
        {Rc, Uc,  Ru,  Uu, Fu,  Uu,  Fc},
        {yc, Rc,  U2c, Ru, U2u, Ru,  Fc, Rc,  Fu},
        {Ru, Fc,  Rc,  Fu, Rc,  Uu,  Ru, U2u, Ru, Fc, Rc, Fu},
        {Rc, Uc,  Ru,  Uc, Rc,  U2c, Ru},
        {Fu, Uc,  Fc,  Ru, Fc,  Rc,  Fu},
        {Rc, U2u, Ru,  Uu, Rc,  Uu,  Ru},

};
int ins_set4[10][20] = {
        {Mc,  Uc,  Mu,  Uu, Su, Uu, Mc, Uc, M2u, Uc, Mc, Uu, Sc, Uu, Mu, Uc, Mc},
        {M2c, B2c, M2c, B2c},
        {Mc,  B2c, Mu,  B2c},
        {z2c}
};
int mouse_ins[28][4] = {
        {Lu,  Bu,  Lc,  Bc},
        {Mu,  Bu,  Mc,  Bc},
        {Rc,  Bu,  Ru,  Bc},
        {Lu,  Sc,  Lc,  Su},
        {Mu,  Sc,  Mc,  Su},
        {Rc,  Sc,  Ru,  Su},
        {Lu,  Fc,  Lc,  Fu},
        {Mu,  Fc,  Mc,  Fu},
        {Rc,  Fc,  Ru,  Fu},
        {Lu,  Uu,  Lc,  Uc},
        {Mu,  Uu,  Mc,  Uc},
        {Rc,  Uu,  Ru,  Uc},
        {Lu,  Ec,  Lc,  Eu},
        {Mu,  Ec,  Mc,  Eu},
        {Rc,  Uu,  Ru,  Ec},
        {Lu,  Dc,  Lc,  Du},
        {Mu,  Dc,  Mc,  Du},
        {Rc,  Dc,  Ru,  Du},
        {Fu,  Uu,  Fc,  Uc},
        {Su,  Uu,  Sc,  Uc},
        {Bc,  Uu,  Bu,  Uc},
        {Fu,  Ec,  Fc,  Eu},
        {Su,  Ec,  Sc,  Eu},
        {Bc,  Ec,  Bu,  Eu},
        {Fu,  Dc,  Fc,  Du},
        {Su,  Dc,  Sc,  Du},
        {Bc,  Dc,  Bu,  Du},
        {x2c, y2u, x2u, y2c},
};
int ins_arr1[INS_ARR_SIZE];
int ins_num = 0;

void push(int num) {
    if (!num)
        return;
    if (stack_size == stack_pos) {
        stack_size *= 2;
        stack = realloc(stack, stack_size);
    }
    if (num < OFFSET || stack_pos == 0 || stack[stack_pos - 1] != num)
        stack[stack_pos++] = num;
}

int pop() {
    if (stack_pos > 0)
        return stack[--stack_pos];
    return 0;
}

void add_ins(int ins) {
    if (ins_num == INS_ARR_SIZE)
        printf("Error: ins_arr1 is full\n");
    else if (ins) {
        change_face(ins);
        if (ins_num > 0 && (ins - 1) / 4 == (ins_arr1[ins_num - 1] - 1) / 4) {
            int last_ins = ins_arr1[ins_num - 1];
            int base = (ins - 1) / 4 * 4, res1 = (ins - 1) % 4, res2 = (last_ins - 1) % 4;
            if (res1 == 0 && res2 == 0)
                ins_arr1[ins_num - 1] = base + 3;
            else if (res1 == 1 && res2 == 1)
                ins_arr1[ins_num - 1] = base + 4;
            else if (res1 == 0 && res2 == 1 || res1 == 1 && res2 == 0 || res1 == 2 && res2 == 3 ||
                     res1 == 3 && res2 == 2 || res1 == 2 && res2 == 2 || res1 == 3 && res2 == 3)
                ins_arr1[--ins_num] = 0;
            else if (res1 == 0 && res2 == 2 || res1 == 2 && res2 == 0 || res1 == 0 && res2 == 3 ||
                     res1 == 3 && res2 == 0)
                ins_arr1[ins_num - 1] = base + 2;
            else if (res1 == 1 && res2 == 2 || res1 == 2 && res2 == 1 || res1 == 1 && res2 == 3 ||
                     res1 == 3 && res2 == 1)
                ins_arr1[ins_num - 1] = base + 1;
        } else
            ins_arr1[ins_num++] = ins;
    }
}

int color_block_w = 50, color_block_h = 50;
int color_block[7][4] = {
        {300, 60,},
        {350, 60,},
        {400, 60,},
        {300, 110,},
        {350, 110,},
        {400, 110,},
        {450, 110,},
};
int mouse_flag = 0, x_pos, y_pos, x_rel = 0, y_rel = 0, facePos;
int int_flag1 = 0, int_flag2 = 0, int_flag3 = 0;
jmp_buf buf1, buf2;

void change_num(int *ptr[], int flag) {
    switch (flag) {
        case 0: {
            int arr[3] = {*ptr[9], *ptr[10], *ptr[11]};
            for (int i = 11; i >= 3; i--)
                *ptr[i] = *ptr[i - 3];
            *ptr[0] = arr[0], *ptr[1] = arr[1], *ptr[2] = arr[2];
            return;
        }
        case 1: {
            int arr[3] = {*ptr[6], *ptr[7]};
            for (int i = 7; i >= 2; i--)
                *ptr[i] = *ptr[i - 2];
            *ptr[0] = arr[0], *ptr[1] = arr[1];
            return;
        }
    }

}

void update_face() {
    color_vector[20][0] = faces[0][0], color_vector[11][0] = faces[0][1], color_vector[2][0] = faces[0][2], color_vector[19][0] = faces[0][3], color_vector[10][0] = faces[0][4], color_vector[1][0] = faces[0][5], color_vector[18][0] = faces[0][6], color_vector[9][0] = faces[0][7], color_vector[0][0] = faces[0][8];
    color_vector[18][1] = faces[1][0], color_vector[9][1] = faces[1][1], color_vector[0][1] = faces[1][2], color_vector[21][1] = faces[1][3], color_vector[12][1] = faces[1][4], color_vector[3][1] = faces[1][5], color_vector[24][1] = faces[1][6], color_vector[15][1] = faces[1][7], color_vector[6][1] = faces[1][8];
    color_vector[0][2] = faces[2][0], color_vector[1][2] = faces[2][1], color_vector[2][2] = faces[2][2], color_vector[3][2] = faces[2][3], color_vector[4][2] = faces[2][4], color_vector[5][2] = faces[2][5], color_vector[6][2] = faces[2][6], color_vector[7][2] = faces[2][7], color_vector[8][2] = faces[2][8];
    color_vector[2][3] = faces[3][0], color_vector[11][3] = faces[3][1], color_vector[20][3] = faces[3][2], color_vector[5][3] = faces[3][3], color_vector[14][3] = faces[3][4], color_vector[23][3] = faces[3][5], color_vector[8][3] = faces[3][6], color_vector[17][3] = faces[3][7], color_vector[26][3] = faces[3][8];
    color_vector[20][4] = faces[4][0], color_vector[19][4] = faces[4][1], color_vector[18][4] = faces[4][2], color_vector[23][4] = faces[4][3], color_vector[22][4] = faces[4][4], color_vector[21][4] = faces[4][5], color_vector[26][4] = faces[4][6], color_vector[25][4] = faces[4][7], color_vector[24][4] = faces[4][8];
    color_vector[24][5] = faces[5][0], color_vector[15][5] = faces[5][1], color_vector[6][5] = faces[5][2], color_vector[25][5] = faces[5][3], color_vector[16][5] = faces[5][4], color_vector[7][5] = faces[5][5], color_vector[26][5] = faces[5][6], color_vector[17][5] = faces[5][7], color_vector[8][5] = faces[5][8];
}

void change_face(int ins) {
    switch (ins) {
        case Rc: {
            int *arr1[12] = {faces[3] + 0, faces[3] + 3, faces[3] + 6, faces[5] + 8, faces[5] + 5, faces[5] + 2,
                             faces[1] + 8, faces[1] + 5, faces[1] + 2, faces[0] + 8, faces[0] + 5, faces[0] + 2};
            int *arr2[8] = {faces[2] + 0, faces[2] + 1, faces[2] + 2, faces[2] + 5, faces[2] + 8, faces[2] + 7,
                            faces[2] + 6, faces[2] + 3,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case Ru: {
            int *arr1[12] = {faces[0] + 2, faces[0] + 5, faces[0] + 8, faces[1] + 2, faces[1] + 5, faces[1] + 8,
                             faces[5] + 2, faces[5] + 5, faces[5] + 8, faces[3] + 6, faces[3] + 3, faces[3] + 0};
            int *arr2[8] = {faces[2] + 0, faces[2] + 3, faces[2] + 6, faces[2] + 7, faces[2] + 8, faces[2] + 5,
                            faces[2] + 2, faces[2] + 1,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case R2c: {
            change_face(Rc);
            change_face(Rc);
            break;
        }
        case R2u: {
            change_face(Ru);
            change_face(Ru);
            break;
        }
        case Lc: {
            int *arr1[12] = {faces[0] + 0, faces[0] + 3, faces[0] + 6, faces[1] + 0, faces[1] + 3, faces[1] + 6,
                             faces[5] + 0, faces[5] + 3, faces[5] + 6, faces[3] + 8, faces[3] + 5, faces[3] + 2};
            int *arr2[8] = {faces[4] + 0, faces[4] + 1, faces[4] + 2, faces[4] + 5, faces[4] + 8, faces[4] + 7,
                            faces[4] + 6, faces[4] + 3,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case Lu: {
            int *arr1[12] = {faces[0] + 6, faces[0] + 3, faces[0] + 0, faces[3] + 2, faces[3] + 5, faces[3] + 8,
                             faces[5] + 6, faces[5] + 3, faces[5] + 0, faces[1] + 6, faces[1] + 3, faces[1] + 0};
            int *arr2[8] = {faces[4] + 0, faces[4] + 3, faces[4] + 6, faces[4] + 7, faces[4] + 8, faces[4] + 5,
                            faces[4] + 2, faces[4] + 1,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case L2c: {
            change_face(Lc);
            change_face(Lc);
            break;
        }
        case L2u: {
            change_face(Lu);
            change_face(Lu);
            break;
        }
        case Fc: {
            int *arr1[12] = {faces[0] + 6, faces[0] + 7, faces[0] + 8, faces[2] + 0, faces[2] + 3, faces[2] + 6,
                             faces[5] + 2, faces[5] + 1, faces[5] + 0, faces[4] + 8, faces[4] + 5, faces[4] + 2};
            int *arr2[8] = {faces[1] + 0, faces[1] + 1, faces[1] + 2, faces[1] + 5, faces[1] + 8, faces[1] + 7,
                            faces[1] + 6, faces[1] + 3,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case Fu: {
            int *arr1[12] = {faces[4] + 2, faces[4] + 5, faces[4] + 8, faces[5] + 0, faces[5] + 1, faces[5] + 2,
                             faces[2] + 6, faces[2] + 3, faces[2] + 0, faces[0] + 8, faces[0] + 7, faces[0] + 6};
            int *arr2[8] = {faces[1] + 0, faces[1] + 3, faces[1] + 6, faces[1] + 7, faces[1] + 8, faces[1] + 5,
                            faces[1] + 2, faces[1] + 1,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case F2c: {
            change_face(Fc);
            change_face(Fc);
            break;
        }
        case F2u: {
            change_face(Fu);
            change_face(Fu);
            break;
        }
        case Bc: {
            int *arr1[12] = {faces[0] + 2, faces[0] + 1, faces[0] + 0, faces[4] + 0, faces[4] + 3, faces[4] + 6,
                             faces[5] + 6, faces[5] + 7, faces[5] + 8, faces[2] + 8, faces[2] + 5, faces[2] + 2};
            int *arr2[8] = {faces[3] + 0, faces[3] + 1, faces[3] + 2, faces[3] + 5, faces[3] + 8, faces[3] + 7,
                            faces[3] + 6, faces[3] + 3,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case Bu: {
            int *arr1[12] = {faces[0] + 0, faces[0] + 1, faces[0] + 2, faces[2] + 2, faces[2] + 5, faces[2] + 8,
                             faces[5] + 8, faces[5] + 7, faces[5] + 6, faces[4] + 6, faces[4] + 3, faces[4] + 0};
            int *arr2[8] = {faces[3] + 0, faces[3] + 3, faces[3] + 6, faces[3] + 7, faces[3] + 8, faces[3] + 5,
                            faces[3] + 2, faces[3] + 1,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case B2c: {
            change_face(Bc);
            change_face(Bc);
            break;
        }
        case B2u: {
            change_face(Bu);
            change_face(Bu);
            break;
        }
        case Uc: {
            int *arr1[12] = {faces[1] + 2, faces[1] + 1, faces[1] + 0, faces[4] + 2, faces[4] + 1, faces[4] + 0,
                             faces[3] + 2, faces[3] + 1, faces[3] + 0, faces[2] + 2, faces[2] + 1, faces[2] + 0};
            int *arr2[8] = {faces[0] + 0, faces[0] + 1, faces[0] + 2, faces[0] + 5, faces[0] + 8, faces[0] + 7,
                            faces[0] + 6, faces[0] + 3,};
            change_num(arr1, 0);
            change_num(arr2, 1);

            break;
        }
        case Uu: {
            int *arr1[12] = {faces[1] + 0, faces[1] + 1, faces[1] + 2, faces[2] + 0, faces[2] + 1, faces[2] + 2,
                             faces[3] + 0, faces[3] + 1, faces[3] + 2, faces[4] + 0, faces[4] + 1, faces[4] + 2};
            int *arr2[8] = {faces[0] + 0, faces[0] + 3, faces[0] + 6, faces[0] + 7, faces[0] + 8, faces[0] + 5,
                            faces[0] + 2, faces[0] + 1,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case U2c: {
            change_face(Uc);
            change_face(Uc);
            break;
        }
        case U2u: {
            change_face(Uu);
            change_face(Uu);
            break;
        }
        case Dc: {
            int *arr1[12] = {faces[1] + 6, faces[1] + 7, faces[1] + 8, faces[2] + 6, faces[2] + 7, faces[2] + 8,
                             faces[3] + 6, faces[3] + 7, faces[3] + 8, faces[4] + 6, faces[4] + 7, faces[4] + 8};
            int *arr2[8] = {faces[5] + 0, faces[5] + 1, faces[5] + 2, faces[5] + 5, faces[5] + 8, faces[5] + 7,
                            faces[5] + 6, faces[5] + 3,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case Du: {
            int *arr1[12] = {faces[1] + 8, faces[1] + 7, faces[1] + 6, faces[4] + 8, faces[4] + 7, faces[4] + 6,
                             faces[3] + 8, faces[3] + 7, faces[3] + 6, faces[2] + 8, faces[2] + 7, faces[2] + 6};
            int *arr2[8] = {faces[5] + 0, faces[5] + 3, faces[5] + 6, faces[5] + 7, faces[5] + 8, faces[5] + 5,
                            faces[5] + 2, faces[5] + 1,};
            change_num(arr1, 0);
            change_num(arr2, 1);
            break;
        }
        case D2c: {
            change_face(Dc);
            change_face(Dc);
            break;
        }
        case D2u: {
            change_face(Du);
            change_face(Du);
            break;
        }
        case Mc: {
            int *arr1[12] = {faces[0] + 1, faces[0] + 4, faces[0] + 7, faces[1] + 1, faces[1] + 4, faces[1] + 7,
                             faces[5] + 1, faces[5] + 4, faces[5] + 7, faces[3] + 7, faces[3] + 4, faces[3] + 1};
            change_num(arr1, 0);
            break;
        }

        case rc: {
            change_face(Rc);
            change_face(Mu);
            break;
        }
        case ru: {
            change_face(Ru);
            change_face(Mc);
            break;
        }
        case r2c: {
            change_face(rc);
            change_face(rc);
            break;
        }
        case r2u: {
            change_face(ru);
            change_face(ru);
            break;
        }
        case lc: {
            change_face(Lc);
            change_face(Mc);
            break;

        }
        case lu: {
            change_face(Lu);
            change_face(Mu);
            break;
        }
        case l2c: {
            change_face(lc);
            change_face(lc);
            break;
        }
        case l2u: {
            change_face(lu);
            change_face(lu);
            break;
        }
        case fc: {
            change_face(Fc);
            change_face(Sc);
            break;

        }
        case fu: {
            change_face(Fu);
            change_face(Su);
            break;
        }
        case f2c: {
            change_face(fc);
            change_face(fc);
            break;
        }
        case f2u: {
            change_face(fu);
            change_face(fu);
            break;
        }
        case bc: {
            change_face(Bc);
            change_face(Su);
            break;

        }
        case bu: {
            change_face(Bu);
            change_face(Sc);
            break;
        }
        case b2c: {
            change_face(bc);
            change_face(bc);
            break;
        }
        case b2u: {
            change_face(bu);
            change_face(bu);
            break;
        }
        case uc: {
            change_face(Uc);
            change_face(Eu);
            break;

        }
        case uu: {
            change_face(Uu);
            change_face(Ec);
            break;
        }
        case u2c: {
            change_face(uc);
            change_face(uc);
            break;
        }
        case u2u: {
            change_face(uu);
            change_face(uu);
            break;
        }
        case dc: {
            change_face(Dc);
            change_face(Ec);
            break;

        }
        case du: {
            change_face(Du);
            change_face(Eu);
            break;
        }
        case d2c: {
            change_face(dc);
            change_face(dc);
            break;
        }
        case d2u: {
            change_face(du);
            change_face(du);
            break;
        }
        case Mu: {
            int *arr1[12] = {faces[3] + 1, faces[3] + 4, faces[3] + 7, faces[5] + 7, faces[5] + 4, faces[5] + 1,
                             faces[1] + 7, faces[1] + 4, faces[1] + 1, faces[0] + 7, faces[0] + 4, faces[0] + 1};
            change_num(arr1, 0);

            break;
        }
        case M2c: {
            change_face(Mc);
            change_face(Mc);
            break;
        }
        case M2u: {
            change_face(Mu);
            change_face(Mu);
            break;
        }
        case Ec: {
            int *arr1[12] = {faces[1] + 3, faces[1] + 4, faces[1] + 5, faces[2] + 3, faces[2] + 4, faces[2] + 5,
                             faces[3] + 3, faces[3] + 4, faces[3] + 5, faces[4] + 3, faces[4] + 4, faces[4] + 5};
            change_num(arr1, 0);
            break;
        }
        case Eu: {
            int *arr1[12] = {faces[1] + 5, faces[1] + 4, faces[1] + 3, faces[4] + 5, faces[4] + 4, faces[4] + 3,
                             faces[3] + 5, faces[3] + 4, faces[3] + 3, faces[2] + 5, faces[2] + 4, faces[2] + 3};
            change_num(arr1, 0);
            break;
        }
        case E2c: {
            change_face(Ec);
            change_face(Ec);
            break;
        }
        case E2u: {
            change_face(Eu);
            change_face(Eu);
            break;
        }
        case Sc: {
            int *arr1[12] = {faces[0] + 3, faces[0] + 4, faces[0] + 5, faces[2] + 1, faces[2] + 4, faces[2] + 7,
                             faces[5] + 5, faces[5] + 4, faces[5] + 3, faces[4] + 7, faces[4] + 4, faces[4] + 1};
            change_num(arr1, 0);
            break;
        }
        case Su: {
            int *arr1[12] = {faces[4] + 1, faces[4] + 4, faces[4] + 7, faces[5] + 3, faces[5] + 4, faces[5] + 5,
                             faces[2] + 7, faces[2] + 4, faces[2] + 1, faces[0] + 5, faces[0] + 4, faces[0] + 3};
            change_num(arr1, 0);
            break;
        }
        case S2c: {
            change_face(Sc);
            change_face(Sc);
            break;
        }
        case S2u: {
            change_face(Su);
            change_face(Su);
            break;
        }
        case yc: {
            change_face(Uc);
            change_face(Eu);
            change_face(Du);
            break;

        }
        case yu: {
            change_face(Uu);
            change_face(Ec);
            change_face(Dc);
            break;
        }
        case y2c: {
            change_face(yc);
            change_face(yc);
            break;
        }
        case y2u: {
            change_face(yu);
            change_face(yu);
            break;
        }
        case xc: {
            change_face(Rc);
            change_face(Mu);
            change_face(Lu);
            break;

        }
        case xu: {
            change_face(Ru);
            change_face(Mc);
            change_face(Lc);
            break;
        }
        case x2c: {
            change_face(xc);
            change_face(xc);
            break;
        }
        case x2u: {
            change_face(xu);
            change_face(xu);
            break;
        }
        case zc: {
            change_face(Bu);
            change_face(Sc);
            change_face(Fc);
            break;

        }
        case zu: {
            change_face(Bc);
            change_face(Su);
            change_face(Fu);
            break;
        }
        case z2c: {
            change_face(zc);
            change_face(zc);
            break;
        }
        case z2u: {
            change_face(zu);
            change_face(zu);
            break;
        }
    }
    update_face();
}


static
void draw(int flag, double angle) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int mask = ins_vector[flag][0];
    double color[8][3] = {};
    double cube[8][3] = {};
    for (int block = 0; block < 27; block++) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 3; j++)
                cube[i][j] = base_cube[i][j] /*+1.005* vector[block][j]*/;
        }
        glLoadIdentity();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScaled(0.78, 0.78, 0.78);
        glRotated(-45.0, 0.0, 1.0, 0.0); // 绕Y轴旋转45度
        glRotated(45.0, 1.0, 0.0, 0.0);
        if ((1 << block) & mask) {
            glPushMatrix();
            //if(0.5*tt<90.0f)
            glRotated(/*5.0*/angle, ins_vector[flag][2], ins_vector[flag][3], ins_vector[flag][4]);
            //else
            //  glRotated(/*5.0*/90.0f, 1, 0, 0);

        }

        glTranslated(1.005 * vector[block][0], 1.005 * vector[block][1], 1.005 * vector[block][2]);

        //

        glBegin(GL_QUADS);

        glColor3d(base_color[color_vector[block][0]][0], base_color[color_vector[block][0]][1],
                  base_color[color_vector[block][0]][2]); // 黄色
        glVertex3dv(cube[5]);
        glVertex3dv(cube[0]);
        glVertex3dv(cube[3]);
        glVertex3dv(cube[4]);


        glColor3d(base_color[color_vector[block][1]][0], base_color[color_vector[block][1]][1],
                  base_color[color_vector[block][1]][2]);  // 蓝色
        glVertex3dv(cube[5]);
        glVertex3dv(cube[4]);
        glVertex3dv(cube[7]);
        glVertex3dv(cube[6]);


        glColor3d(base_color[color_vector[block][2]][0], base_color[color_vector[block][2]][1],
                  base_color[color_vector[block][2]][2]);  // 红色
        glVertex3dv(cube[0]);
        glVertex3dv(cube[5]);
        glVertex3dv(cube[6]);
        glVertex3dv(cube[1]);

        glColor3d(base_color[color_vector[block][3]][0], base_color[color_vector[block][3]][1],
                  base_color[color_vector[block][3]][2]);  // 绿色
        glVertex3dv(cube[0]);
        glVertex3dv(cube[1]);
        glVertex3dv(cube[2]);
        glVertex3dv(cube[3]);

        glColor3d(base_color[color_vector[block][4]][0], base_color[color_vector[block][4]][1],
                  base_color[color_vector[block][4]][2]);  // 橙色
        glVertex3dv(cube[3]);
        glVertex3dv(cube[4]);
        glVertex3dv(cube[7]);
        glVertex3dv(cube[2]);


        glColor3d(base_color[color_vector[block][5]][0], base_color[color_vector[block][5]][1],
                  base_color[color_vector[block][5]][2]);  // 白色
        glVertex3dv(cube[6]);
        glVertex3dv(cube[1]);
        glVertex3dv(cube[2]);
        glVertex3dv(cube[7]);

        glEnd();

        glFlush();
        if ((1 << block) & mask)
            glPopMatrix();
        //  glMatrixMode(GL_MODELVIEW);
    }
    SDL_GL_SwapWindow(window1);
    handle();
}

void draw_cube(int flag) {
    if (flag)
        int_flag1 = 1;
    int angle = ins_vector[flag][1];
    int sig = angle >= 0 ? 1 : -1;
    for (double f = 0; f < abs(angle); f = f + 1.0)
        draw(flag, sig * f);
    change_face(flag);
    /*for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++)
            printf("%d ", faces[i][j]);
        printf("\n");
    }*/
    draw(0, 0);
    push(flag);
    if (flag)
        int_flag1 = 0;
    if (int_flag3) {
        int_flag2 = 2;
        int_flag3 = 0;
        if (!setjmp(buf1))
            while (1)
                handle();
    }
}

void exe_line(int *ptr) {
    while (*ptr != 0) {
        draw_cube(*ptr);
        ptr++;
    }
}

void add_line(int *ptr) {
    while (*ptr != 0) {
        add_ins(*ptr);
        ptr++;
    }
}

void change_line(int *ptr) {
    while (*ptr != 0) {
        change_face(*ptr);
        ptr++;
    }
}

void copy_face(int face1[6][9], const int face2[6][9]) {
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 9; j++)
            face1[i][j] = face2[i][j];
}

void solve() {
    if (int_flag1 || int_flag2)
        return;
    int_flag2 = 1;
    if (setjmp(buf2)) {
        strcpy(button_arr[4].name, "Break");
        goto end2;
    }
    int temp_face[6][9], solvable = 0;
    copy_face(temp_face, faces);
    for (int i = 0; i < 6; i++)
        if (faces[i][4] == White) {
            add_ins(ins_set1[i]);
            solvable = 1;
            break;
        }
    if (!(solvable && faces[5][4] == Yellow)) {
        solvable = 0;
        goto end1;
    }

    solvable = 0;
    for (int i = 0; i < 4; i++) {
        if (faces[i % 4 + 1][4] == Orange && faces[(i + 1) % 4 + 1][4] == Green && faces[(i + 2) % 4 + 1][4] == Red &&
            faces[(i + 3) % 4 + 1][4] == Blue) {
            solvable = 1;
            break;
        }
    }
    if (!solvable)
        goto end1;
    solvable = 0;
    int arr[4][2];
    //if (faces[0][0] != White || faces[0][2] != White || faces[0][6] != White || faces[0][8] != White)
    for (int count = 0; count < 4; count++) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 9; j++) {
                if (faces[i][j] == White && neighbor_vector[i][j][2] != -1) {
                    int color1 = faces[neighbor_vector[i][j][0]][neighbor_vector[i][j][1]], color2 = faces[neighbor_vector[i][j][2]][neighbor_vector[i][j][3]];
                    if (color1 == faces[3][4] && color2 == faces[4][4] ||
                        color2 == faces[3][4] && color1 == faces[4][4]) {
                        add_line(ins_set2[i][j]);
                        arr[count][0] = i, arr[count][1] = j;
                        solvable++;
                    }
                }
            }
        }
        add_ins(yc);
    }
    if (solvable != 4) {
        solvable = 0;
        goto end1;
    }
    solvable = (faces[1][0] == faces[1][2] && faces[2][0] == faces[2][2] && faces[3][0] == faces[3][2] &&
                faces[4][0] == faces[4][2]);
    if (!solvable)
        goto end1;
    add_ins(x2u);
    int flag = 0;
    while (flag == 0) {
        if (ins_num > 400) {
            solvable = 0;
            goto end1;
        }
        solvable = 4;
        for (int i = 0; i < 4; i++) {
            if (faces[0][0] == faces[0][8] && faces[0][8] == faces[1][0] && faces[1][0] == faces[2][2]) {
                if (faces[1][2] == faces[4][0]) {
                    add_line(ins_set3[0]);
                    break;
                } else if (faces[0][2] != faces[1][2]) {
                    add_ins(y2c);
                    add_line(ins_set3[7]);
                    add_line(ins_set3[7]);
                    add_line(ins_set3[7]);
                } else {
                    add_line(ins_set3[1]);
                    break;
                }
            } else if (faces[0][0] == faces[0][2] && faces[0][2] == faces[0][6] && faces[0][6] == faces[0][8]) {
                if (faces[1][0] != faces[1][2] && faces[3][0] == faces[3][2] ||
                    faces[1][0] != faces[1][2] && faces[2][0] != faces[2][2] && faces[3][0] != faces[3][2] &&
                    faces[4][0] != faces[4][2]) {
                    add_line(ins_set3[2]);
                    break;
                } else if (faces[1][0] == faces[1][2] && faces[2][0] == faces[2][2]) {
                    flag = 1;
                    break;
                }
            } else if (faces[0][8] == faces[2][2] && faces[2][2] == faces[3][2] && faces[3][2] == faces[4][2]) {
                add_line(ins_set3[3]);
                break;
            } else if (faces[0][6] == faces[0][8] && faces[0][8] == faces[2][2] && faces[2][2] == faces[4][0]) {
                add_line(ins_set3[4]);
                break;
            } else if (faces[0][2] == faces[1][0] && faces[1][0] == faces[2][0] && faces[2][0] == faces[4][0]) {
                if (faces[0][8] == faces[3][2]) {
                    if (faces[1][2] != faces[2][2]) {
                        add_line(ins_set3[8]);
                        break;
                    } else {
                        add_line(ins_set3[9]);
                        break;
                    }
                } else {
                    add_line(ins_set3[5]);
                    break;
                }
            } else if (faces[2][0] == faces[2][2] && faces[2][2] == faces[4][0] && faces[4][0] == faces[4][2] &&
                       faces[0][6] == faces[0][8]) {
                add_line(ins_set3[6]);
                break;
            } else if (faces[0][0] == faces[0][2] && faces[0][2] == faces[1][0] && faces[1][0] == faces[1][2]) {
                add_line(ins_set3[7]);
                break;
            } else if (faces[1][2] == faces[4][2] && faces[4][2] == faces[4][0] && faces[4][0] == faces[3][0]) {
                add_line(ins_set3[7]);
                break;
            } else if (faces[1][0] == faces[1][2] && faces[1][2] == faces[3][0] && faces[3][0] == faces[3][2]) {
                add_line(ins_set3[7]);
                break;
            }
            solvable--;
            if (solvable == 0) {
                flag = 1;
                break;
            }
            add_ins(yc);
        }
    }
    if (!solvable)
        goto end1;
    while (1) {
        solvable = 0;
        for (int count = 0; count < 4; count++) {
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 9; j++) {
                    if (faces[i][j] == faces[0][4] && neighbor_vector[i][j][2] == -1 &&
                        faces[neighbor_vector[i][j][0]][neighbor_vector[i][j][1]] == faces[3][0]) {
                        add_line(ins_set2[i][j]);
                        arr[count][0] = i, arr[count][1] = j;
                        solvable++;
                    }
                }
            }
            if (count == 0 || count == 2)
                add_ins(y2c);
            else if (count == 1) {
                if (faces[4][1] == faces[0][4])
                    add_ins(yc);
                else add_ins(yu);
            }

        }
        if (solvable != 4) {
            solvable = 0;
            goto end1;
        }
        if (faces[5][1] == faces[5][3] && faces[5][3] == faces[5][5] && faces[5][5] == faces[5][7] &&
            faces[1][6] == faces[1][7] && faces[2][6] == faces[2][7] && faces[3][6] == faces[3][7])
            break;
        int pos = 0;
        int pos_arr[5] = {0, 7, 3, 1, 5}, ins_arr[5] = {0, 0, yc, y2c, yu};
        for (int i = 1; i <= 4; i++) {
            if (faces[i][6] == faces[5][pos_arr[i]] && faces[5][4] == faces[(i + 1) % 4 + 1][7]) {
                pos = i;
            }
        }
        //printf("%d\n", pos);
        add_ins(ins_arr[pos]);
        add_ins(x2u);
    }

    add_ins(zu);
    while (faces[0][0] != 2)
        add_ins(Lc);
    while (faces[0][4] != 2)
        add_ins(Mc);
    while (faces[0][2] != 2)
        add_ins(Ru);
    for (int i = 0; i < 4; i++) {
        int dif1 = abs(faces[0][1] - faces[0][4]);
        if (dif1 != 2 && dif1 != 0) {
            int dif2 = abs(faces[1][1] - faces[1][4]), dif3 = abs(faces[3][4] - faces[3][7]);
            if (dif2 != 2 && dif2 != 0)
                add_line(ins_set4[0]);
            else if (dif3 == 2 || dif3 == 0) {
                add_ins(F2c);
                add_line(ins_set4[0]);
                add_ins(F2c);
                break;
            }
        }
        add_ins(xu);
    }
    for (int i = 0; i < 4; i++) {
        if (faces[0][1] != faces[0][4] && faces[0][4] != faces[0][7]) {
            add_line(ins_set4[1]);
        } else if (faces[0][1] != faces[0][4] && faces[5][1] == faces[0][4] && faces[1][7] == faces[3][4]) {
            add_line(ins_set4[2]);
            break;
        } else if (faces[0][7] != faces[0][4] && faces[5][7] == faces[0][4] && faces[3][7] == faces[1][4]) {
            add_ins(y2c);
            add_line(ins_set4[2]);
            break;
        }
        add_ins(xu);
    }
    if (faces[0][1] != faces[0][4] || faces[0][7] != faces[0][4] || faces[1][1] != faces[1][4] ||
        faces[1][7] != faces[1][4] || faces[3][1] != faces[3][4] || faces[3][7] != faces[3][4] ||
        faces[5][1] != faces[5][4] || faces[5][7] != faces[5][4]) {
        solvable = 0;
        goto end1;
    }

    end1:
    copy_face(faces, temp_face);
    update_face();
    if (solvable) {
        strcpy(outputText, "");
        exe_line(ins_arr1);
    } else strcpy(outputText, "Error: Illegal cube!");
    end2:
    memset(ins_arr1, 0, sizeof(int) * INS_ARR_SIZE);
    ins_num = 0;
    int_flag2 = 0;
}

int get_pos(int x, int y) {
    int ori_x_pos = 0, ori_y_pos = 400, len1 = 110, len2 = 78;
    double dd = len1 * 1.0 / len2;
    if (y >= ori_y_pos - 3 * len1 && y <= ori_y_pos && dd * (x_pos - ori_x_pos) + y >= ori_y_pos &&
        dd * (x - 3 * len1 - ori_x_pos) + y <= ori_y_pos) {
        double del_x = x - (ori_x_pos + 3 * len2);
        double del_y = y - (ori_y_pos - 3 * len1);
        int y1 = del_y / len1;
        int x1 = (del_x + del_y * len2 / len1) / len1;
        return x1 + 3 * y1;
    } else if (y >= ori_y_pos && y <= ori_y_pos + 3 * len1 && y - dd * (x - ori_x_pos) <= ori_y_pos &&
               y - dd * (x - 3 * len1 - ori_x_pos) >= ori_y_pos) {
        double del_x = x - (ori_x_pos);
        double del_y = y - (ori_y_pos);
        int y1 = del_y / len1;
        int x1 = (del_x - del_y * len2 / len1) / len1;
        return x1 + 3 * y1 + 9;
    } else if (y + dd * (x - ori_x_pos - 3 * len1) >= ori_y_pos && y - dd * (x - ori_x_pos - 3 * len1) <= ori_y_pos &&
               y + dd * (x - ori_x_pos - 3 * len1 - 6 * len2) <= ori_y_pos &&
               y - dd * (x - ori_x_pos - 3 * len1 - 6 * len2) >= ori_y_pos) {
        double del_x = x - (ori_x_pos + 3 * len1);
        double del_y = y - (ori_y_pos);
        int y1 = (del_x / 78 + del_y / 110) / 2;
        int x1 = (del_x / 78 - del_y / 110) / 2;
        return x1 + 3 * y1 + 18;
    }
    return 27;
}

void exe_vector(int x, int y, int flag) {
    if (int_flag1 || int_flag2)
        return;
    int face = flag / 9;
    int ori_x_pos = 0, ori_y_pos = 400, len1 = 110, len2 = 78;
    int arr[4][4] = {
            {3 * len1, 3 * (len1 + len2), 3 * len1, 3 * (len2 - len1)},
            {3 * len1, 3 * (len1 - len2), 3 * len1, -3 * (len1 + len2)},
            {1,        0,                 0,        -1},
            {1,        1,                 1,        -1},
    };
    int dir1 = arr[face][0] * x + arr[face][1] * y, dir2 = arr[face][2] * x + arr[face][3] * y;
    if (dir1 <= 0 && dir2 >= 0)
        draw_cube(mouse_ins[flag][0]);
    else if (dir1 >= 0 && dir2 >= 0)
        draw_cube(mouse_ins[flag][1]);
    else if (dir1 >= 0 && dir2 <= 0)
        draw_cube(mouse_ins[flag][2]);
    else if (dir1 <= 0 && dir2 <= 0)
        draw_cube(mouse_ins[flag][3]);
}

void handle() {
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if ((SDL_QUIT == ev.type) ||
                (SDL_KEYDOWN == ev.type && SDLK_ESCAPE == ev.key.keysym.sym)) {
                TTF_CloseFont(font);
                SDL_FreeSurface(image);
                SDL_DestroyTexture(texture1);
                IMG_Quit();
                TTF_Quit();
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window1);
                SDL_DestroyWindow(window2);
                SDL_Quit();
                free(stack);
                free(ins_arr2);
                exit(0);
            }
            if (ev.window.windowID == SDL_GetWindowID(window1)) {
                switch (ev.type) {
                    case SDL_MOUSEBUTTONDOWN: {
                        if (ev.button.button == SDL_BUTTON_LEFT) {
                            mouse_flag = 1;
                            x_rel = y_rel = 0;
                            SDL_GetMouseState(&x_pos, &y_pos);
                            facePos = get_pos(x_pos, y_pos);
                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONUP: {
                        if (ev.button.button == SDL_BUTTON_LEFT) {
                            SDL_GetMouseState(&x_pos, &y_pos);
                            if (facePos == get_pos(x_pos, y_pos))
                                change_color(facePos, color_flag);
                            mouse_flag = 0;
                        }
                        break;
                    }
                    case SDL_MOUSEMOTION: {
                        if (ev.button.button == SDL_BUTTON_LEFT && mouse_flag == 1) {
                            x_rel += ev.motion.xrel;
                            y_rel += ev.motion.yrel;
                            if (abs(x_rel) > 50 || abs(y_rel) > 50) {
                                mouse_flag = 0;
                                exe_vector(x_rel, y_rel, facePos);

                            }
                        }
                        break;
                    }

                }
            } else if (ev.window.windowID == SDL_GetWindowID(window2)) {
                int x, y;
                switch (ev.type) {
                    case SDL_MOUSEBUTTONDOWN: {
                        SDL_GetMouseState(&x, &y);
                        click_button = get_button_num(x, y);
                        click_color_block = get_block_num(x, y);
                        break;
                    }
                    case SDL_MOUSEBUTTONUP: {
                        SDL_GetMouseState(&x, &y);
                        if (click_button != -1 && click_button == get_button_num(x, y)) {
                            up_button = click_button;
                            click_button = -1;
                            draw_window();
                            if (button_arr[up_button].func1)
                                button_arr[up_button].func1();
                        } else {
                            if (click_color_block != -1 && click_color_block == get_block_num(x, y))
                                color_flag = click_color_block + 1;
                            else color_flag = -1;
                        }
                        click_button = -1;
                        click_color_block = -1;
                        break;
                    }
                    case SDL_MOUSEMOTION: {
                        SDL_GetMouseState(&x, &y);
                        current_button = get_button_num(x, y);
                        current_color_block = get_block_num(x, y);
                        if (current_button == -1)
                            up_button = -1;
                        break;
                    }
                    case SDL_TEXTINPUT: {
                        if (input_flag) {
                            if (strlen(inputText) > 256) {
                                strcpy(outputText, "Error: The path is too long!");
                            } else {
                                strcat(inputText, ev.text.text);
                                if (strcmp(outputText, "Error: The path is too long!") == 0)
                                    strcpy(outputText, "");
                            }
                        }

                        break;
                    }
                    case SDL_KEYDOWN: {
                        if (ev.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0 && input_flag) {
                            {
                                inputText[strlen(inputText) - 1] = '\0';
                                if (strcmp(outputText, "Error: The path is too long!") == 0)
                                    strcpy(outputText, "");
                            }
                        } else if (ev.key.keysym.sym == SDLK_TAB)
                            input_flag = 1 - input_flag;
                        else if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
                            FILE *file = fopen(inputText, "r"); // 打开文件，以只读模式

                            if (file)
                                strcpy(outputText, "");
                            else {
                                strcpy(outputText, "Error: No such file!");
                                return;
                            }
                            // 计算文件大小
                            fseek(file, 0L, SEEK_END);
                            long size = ftell(file);
                            rewind(file); // 将文件指针重新定位到文件开头

                            char *buffer = (char *) malloc(size + 1); // 为文件内容分配内存

                            fread(buffer, size, 1, file); // 读取文件内容到缓冲区
                            fclose(file);

                            buffer[size] = '\0'; // 将缓冲区最后一个字符设置为 '\0'，以便将其作为 C 字符串使用
                            exe_str(buffer);
                            // printf("File contents:\n%s\n", buffer);
                            free(buffer); // 释放内存

                        }
                    }
                }
                draw_window();
            }
        }
    }
};

void empty() {
    if (int_flag1 || int_flag2)
        return;
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 9; j++)
            faces[i][j] = 7;
    stack_pos = 0;
}

void reset() {
    if (int_flag1 || int_flag2)
        return;
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 9; j++)
            faces[i][j] = i + 1;
    stack_pos = 0;
}

void random() {
    if (int_flag1 || int_flag2)
        return;
    int randomNum;
    srand(time(NULL));
    //change_face(zu);
    for (int i = 0; i < 100; i++) {

        randomNum = rand();
        change_face(randomNum % 72 + 1);
    }
    stack_pos = 0;
}

void change_color(int faceAndPos, int color) {
    if (int_flag1)
        return;
    if (color != -1 && faceAndPos < 27) {
        push(OFFSET + (facePos << 3) + faces[facePos / 9][facePos % 9]);
        faces[faceAndPos / 9][faceAndPos % 9] = color;
    }
}

void back() {
    if (int_flag1 || int_flag2)
        return;
    int ins = pop();
    if (ins >= OFFSET) {
        ins -= OFFSET;
        int face_pos = ins / 8, color = ins % 8;
        faces[face_pos / 9][face_pos % 9] = color;
    } else if (ins) {
        if (ins % 2 == 0)
            draw_cube(ins - 1);
        else
            draw_cube(ins + 1);
        stack_pos--;
    }
}

void suspend_or_break() {
    if (int_flag2 == 1) {
        strcpy(button_arr[4].name, "Stop");
        int_flag3 = 1;
    }
    else if (int_flag2 == 2)
        longjmp(buf2, 1);
}

void continue_ins() {
    if (int_flag2 == 2) {
        int_flag2 = 1;
        strcpy(button_arr[4].name, "Break");
        longjmp(buf1, 1);
    }
}

void init() {
    renderer = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    int x = 1, y = 1, z = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++) {
                vector[9 * i + 3 * j + k][0] = x - i;
                vector[9 * i + 3 * j + k][1] = y - j;
                vector[9 * i + 3 * j + k][2] = z - k;
            }
    create_button(60, 70, 120, 60, "Empty", empty);
    create_button(60, 180, 120, 60, "Reset", reset);
    create_button(60, 290, 120, 60, "Random", random);
    create_button(60, 400, 120, 60, "Resolve", solve);
    create_button(60, 510, 120, 60, "Break", suspend_or_break);
    create_button(60, 620, 120, 60, "Continue", continue_ins);
    create_button(300, 180, 120, 60, "Back", back);
    click_button = current_button = up_button = click_color_block = current_button = color_flag = -1;
    TTF_Init();
    font = TTF_OpenFont("constan.ttf", 24);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
    image = IMG_Load("a.jpg");
    texture1 = SDL_CreateTextureFromSurface(renderer, image);
    stack = malloc(stack_size * sizeof(int));
    ins_arr2 = malloc(ins_size2 * sizeof(int));
}

void create_button(int x, int y, int w, int h, char *str, void (*func1)(void)) {
    button_arr[button_num].x = x;
    button_arr[button_num].y = y;
    button_arr[button_num].w = w;
    button_arr[button_num].h = h;
    strcpy(button_arr[button_num].name, str);
    button_arr[button_num].func1 = func1;
    button_num++;
}

int get_button_num(int x, int y) {
    for (int i = 0; i < button_num; i++)
        if (button_arr[i].x <= x && x <= button_arr[i].x + button_arr[i].w && button_arr[i].y <= y &&
            y <= button_arr[i].y + button_arr[i].h)
            return i;
    return -1;
}

int get_block_num(int x, int y) {
    for (int i = 0; i < 7; i++)
        if (color_block[i][0] <= x && x <= color_block[i][0] + color_block_w && color_block[i][1] <= y &&
            y <= color_block[i][1] + color_block_h)
            return i;
    return -1;
}

void put_ins(int ins) {
    if (!ins)
        return;
    if (ins_pos2 == ins_size2) {
        ins_size2 *= 2;
        ins_arr2 = realloc(ins_arr2, ins_size2);
    }
    ins_arr2[ins_pos2++] = ins;
}

void exe_str(char *str) {
    if (int_flag1 || int_flag2)
        return;
    int_flag2 = 1;
    if (setjmp(buf2)) {
        strcpy(button_arr[4].name, "Break");
        int_flag2 = 0;
        return;
    }
    int flag = 0;
    int ins = 0;
    int match = 1;
    while (*str) {
        if (!match)
            break;
        switch (*str) {
            case 'R': {
                put_ins(ins);
                ins = Rc;
                flag = 1;
                break;
            }
            case 'L': {
                put_ins(ins);
                ins = Lc;
                flag = 1;
                break;
            }
            case 'F': {
                put_ins(ins);
                ins = Fc;
                flag = 1;
                break;
            }
            case 'B': {
                put_ins(ins);
                ins = Bc;
                flag = 1;
                break;
            }
            case 'U': {
                put_ins(ins);
                ins = Uc;
                flag = 1;
                break;
            }
            case 'D': {
                put_ins(ins);
                ins = Dc;
                flag = 1;
                break;
            }
            case 'r': {
                put_ins(ins);
                ins = rc;
                flag = 1;
                break;
            }
            case 'l': {
                put_ins(ins);
                ins = lc;
                flag = 1;
                break;
            }
            case 'f': {
                put_ins(ins);
                ins = fc;
                flag = 1;
                break;
            }
            case 'b': {
                put_ins(ins);
                ins = bc;
                flag = 1;
                break;
            }
            case 'u': {
                put_ins(ins);
                ins = uc;
                flag = 1;
                break;
            }
            case 'd': {
                put_ins(ins);
                ins = dc;
                flag = 1;
                break;
            }
            case 'M': {
                put_ins(ins);
                ins = Mc;
                flag = 1;
                break;
            }
            case 'E': {
                put_ins(ins);
                ins = Ec;
                flag = 1;
                break;
            }
            case 'S': {
                put_ins(ins);
                ins = Sc;
                flag = 1;
                break;
            }
            case 'y': {
                put_ins(ins);
                ins = yc;
                flag = 1;
                break;
            }
            case 'x': {
                put_ins(ins);
                ins = xc;
                flag = 1;
                break;
            }
            case 'z': {
                put_ins(ins);
                ins = zc;
                flag = 1;
                break;
            }
            case '\'': {
                if (flag == 1) {
                    ins++;
                    flag = 2;
                } else
                    match = 0;
                break;
            }
            case '2': {
                if (flag == 2) {
                    ins += 2;
                    flag = 0;
                } else
                    match = 0;
                break;
            }
            case ' ': {
            }
            case '\t': {
            }
            case '\n':
                break;
            default:
                match = 0;
        }
        str++;
    }
    put_ins(ins);
    if (match) {
        exe_line(ins_arr2);
        ins_pos2 = 0;
        strcpy(outputText, "");
    } else
        strcpy(outputText, "Error: No such instruction!");
    int_flag2 = 0;
}

void draw_window() {
    SDL_RenderClear(renderer);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 600, 800);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture1, NULL, NULL);


    for (int i = 0; i < button_num; i++) {
        if (i == current_button) {
            if (i == click_button)
                SDL_SetRenderDrawColor(renderer, button_color[Red][0], button_color[Red][1], button_color[Red][2],
                                       button_color[Red][3]);
            else if (i != up_button)
                SDL_SetRenderDrawColor(renderer, button_color[Blue][0], button_color[Blue][1], button_color[Blue][2],
                                       button_color[Blue][3]);
            else
                SDL_SetRenderDrawColor(renderer, button_color[Orange][0], button_color[Orange][1],
                                       button_color[Orange][2],
                                       button_color[Orange][3]);
        } else
            SDL_SetRenderDrawColor(renderer, button_color[Orange][0], button_color[Orange][1], button_color[Orange][2],
                                   button_color[Orange][3]);

        SDL_Rect rect = {button_arr[i].x, button_arr[i].y, button_arr[i].w, button_arr[i].h};
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, button_color[Black][0], button_color[Black][1], button_color[Black][2],
                               button_color[Black][3]);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface *surfaceMessage = TTF_RenderUTF8_Solid(font, button_arr[i].name, textColor);
        SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        int x = rect.x + (rect.w - surfaceMessage->w) / 2;
        int y = rect.y + (rect.h - surfaceMessage->h) / 2;
        SDL_Rect messageRect = {x, y, surfaceMessage->w, surfaceMessage->h};
        SDL_RenderCopy(renderer, message, NULL, &messageRect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);
    }
    for (int i = 0; i < 7; i++) {
        if (i == click_color_block && i == current_color_block)
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);//purple
        else
            SDL_SetRenderDrawColor(renderer, block_color[i][0], block_color[i][1], block_color[i][2],
                                   block_color[i][3]);
        SDL_Rect rect = {color_block[i][0], color_block[i][1], color_block_w, color_block_h};
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, button_color[Black][0], button_color[Black][1], button_color[Black][2],
                               button_color[Black][3]);
        SDL_RenderDrawRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Color inputColor = {255, 255, 255, 255};
    SDL_Color outputColor = {255, 255, 255, 255};
    // 渲染输入文本框
    if (inputText[0]) {
        int len = strlen(inputText) - 28;
        len = len < 0 ? 0 : len;
        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText + len, inputColor);
        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
        SDL_Rect inputRect = {250, 300, inputSurface->w, inputSurface->h};
        SDL_RenderCopy(renderer, inputTexture, NULL, &inputRect);
        SDL_DestroyTexture(inputTexture);
        SDL_FreeSurface(inputSurface);
    }
    // 渲染输出文本框
    if (outputText[0]) {
        SDL_Surface *outputSurface = TTF_RenderText_Solid(font, outputText, outputColor);
        SDL_Texture *outputTexture = SDL_CreateTextureFromSurface(renderer, outputSurface);
        SDL_Rect outputRect = {250, 400, outputSurface->w, outputSurface->h};
        SDL_RenderCopy(renderer, outputTexture, NULL, &outputRect);
        SDL_DestroyTexture(outputTexture);
        SDL_FreeSurface(outputSurface);
    }

    // 将文本框渲染到窗口上





    SDL_SetRenderTarget(renderer, NULL);

    // 将纹理渲染到屏幕上
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // 更新屏幕

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}
