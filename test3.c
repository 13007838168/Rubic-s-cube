#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include<setjmp.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <time.h>
#include "test3_head.h"

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
SDL_GLContext ctx;
SDL_Window *window1;
double base_color[8][3] = {
        {0.5, 0.5,   0.5},//grey
        {1.0, 1.0,   0.0},//yellow
        {0.0, 0.0,   1.0},//blue
        {1.0, 0.0,   0.0},//red
        {0.0, 1.0,   0.0},//green
        {1.0, 0.565, 0.0},//orange
        {1.0, 1.0,   1.0},//white
        {0.0, 0.0,   1.0}
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
int ins_set1[6] = {0, xc, zu, xu, zc, x2c};
int ins_set2[6][9][10] = {
        {{},                                 {},                                      {Fu,  Uu, Fc,  Rc,},                 {Su,  Du,  Sc, Mc,  Bc, Mu, Bu}, {}, {Sc,  Dc,  Su, Mc,  Bc, Mu, Bu}, {Rc, Uc,  Ru, Fu},             {Mc,  Dc, Mu,  Dc, Mc, Bc, Mu, Bu}, {Bc, Rc,  Bu,  Dc, Lu, Du, Lc},},
        {{Lu},                               {Sc,  Mu,  Su},                          {Fu,  Dc, Lu,  Du,  Fc},             {Eu,  Bu,  Mc, Bc,  Mu},         {}, {Ec,  Bc,  Mc, Bu,  Mu},         {Bc, Du,  Bu},                 {D2c, Mc, Bc,  Mu, Bu},             {Lu, D2c, Lc},},
        {{Rc, Du, Bc,  Dc,  Ru},             {Rc,  Bc,  Mc, Bu,  Mu, Ru},             {Bc},                                {R2c, Bc,  Mc, Bu,  Mu, R2u},    {}, {Bc,  Mc,  Bu, Mu},              {Bc, D2c, Bu},                 {Ru,  Bc, Mc,  Bu, rc},             {Lu, Dc,  Lc},},
        {{Fu, Rc, B2c, Fc},                  {Mc,  Bc,  Mu, Bu,  Ec, Bu, Mc, Bc, Mu}, {Lc,  Bu, Lu,  B2c, Dc, Bu},         {Ec,  Bu,  Mc, Bc,  Mu},         {}, {Eu,  Bc,  Mc, Bu,  Mu},         {Dc, Lu,  Du, Lc},             {Mc,  Bc, Mu,  Bu},                 {Bc, Dc,  Bu},},
        {{Bu, Lc, Bc,  L2u, Du, Lc},         {Lu,  Bu,  Mc, Bc,  Mu, Lc},             {Rc,  Fu, L2c, Ru},                  {Bu,  Mc,  Bc, Mu},              {}, {L2u, Bu,  Mc, Bc,  Mu, L2c},    {Lu, Du,  Lc},                 {Lc,  Bu, Mc,  Bc, lu},             {Du, Bc,  Dc,  Bu},},
        {{Du, Fc, L2c, Fu,  Du, Bc, Dc, Bu}, {D2c, B2c, Mc, B2u, Mu},                 {D2u, Fc, L2c, Fu,  Du, Bc, Dc, Bu}, {Du,  B2c, Mc, B2u, Mu},         {}, {Dc,  B2c, Mc, B2u, Mu},         {Fc, L2c, Fu, Du, Bc, Dc, Bu}, {B2c, Mc, B2u, Mu},                 {Dc, Fc,  L2c, Fu, Du, Bc, Dc, Bu},},

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
int mouse_flag = 0, x_pos, y_pos, x_rel = 0, y_rel = 0, faceAndPos;
int ins_flag = 1;

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
    if(flag)
    ins_flag = 0;
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
    if(flag)
    ins_flag = 1;
}

void exe_line(int *ptr) {
    while (*ptr != 0) {
        draw_cube(*ptr);
        ptr++;
    }
}

void change_line(int *ptr) {
    while (*ptr != 0) {
        change_face(*ptr);
        ptr++;
    }
}

void solve() {
    for (int i = 0; i < 6; i++)
        if (faces[i][4] == White) {
            draw_cube(ins_set1[i]);
            break;
        }
    int arr[4][2];
    //if (faces[0][0] != White || faces[0][2] != White || faces[0][6] != White || faces[0][8] != White)
    for (int count = 0; count < 4; count++) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 9; j++) {
                if (faces[i][j] == White && neighbor_vector[i][j][2] != -1) {
                    int color1 = faces[neighbor_vector[i][j][0]][neighbor_vector[i][j][1]], color2 = faces[neighbor_vector[i][j][2]][neighbor_vector[i][j][3]];
                    if (color1 == faces[3][4] && color2 == faces[4][4] ||
                        color2 == faces[3][4] && color1 == faces[4][4]) {
                        exe_line(ins_set2[i][j]);
                        arr[count][0] = i, arr[count][1] = j;
                        if (faces[3][4] != faces[3][2] || faces[4][4] != faces[4][0])
                            printf("%d %d\n", i, j);
                    }
                }
            }
        }
        draw_cube(yc);
    }
    if (faces[0][0] != faces[0][2] || faces[0][2] != faces[0][6] || faces[0][6] != faces[0][8] ||
        faces[0][8] != faces[0][0]) {
        printf("error1");
        for (int i = 0; i < 4; i++)
            printf("%d %d ", arr[i][0], arr[i][1]);
        while (1)
            draw_cube(yc);
    }
    draw_cube(x2u);

    int flag = 0;
    // if (faces[0][0] != faces[0][2] || faces[0][2] != faces[0][6] || faces[0][6] != faces[0][8])
    while (flag == 0) {
        for (int i = 0; i < 4; i++) {
            if (faces[0][0] == faces[0][8] && faces[0][8] == faces[1][0] && faces[1][0] == faces[2][2]) {
                if (faces[1][2] == faces[4][0]) {
                    exe_line(ins_set3[0]);
                    break;
                } else if (faces[0][2] != faces[1][2]) {
                    draw_cube(y2c);
                    exe_line(ins_set3[7]);
                    exe_line(ins_set3[7]);
                    exe_line(ins_set3[7]);
                } else {

                    exe_line(ins_set3[1]);

                    break;
                }
            } else if (faces[0][0] == faces[0][2] && faces[0][2] == faces[0][6] && faces[0][6] == faces[0][8]) {
                if (faces[1][0] != faces[1][2] && faces[3][0] == faces[3][2] ||
                    faces[1][0] != faces[1][2] && faces[2][0] != faces[2][2] && faces[3][0] != faces[3][2] &&
                    faces[4][0] != faces[4][2]) {
                    exe_line(ins_set3[2]);
                    break;
                } else if (faces[1][0] == faces[1][2] && faces[2][0] == faces[2][2]) {
                    flag = 1;
                    break;
                }
            } else if (faces[0][8] == faces[2][2] && faces[2][2] == faces[3][2] && faces[3][2] == faces[4][2]) {
                exe_line(ins_set3[3]);
                break;
            } else if (faces[0][6] == faces[0][8] && faces[0][8] == faces[2][2] && faces[2][2] == faces[4][0]) {
                exe_line(ins_set3[4]);
                break;
            } else if (faces[0][2] == faces[1][0] && faces[1][0] == faces[2][0] && faces[2][0] == faces[4][0]) {
                if (faces[0][8] == faces[3][2]) {
                    if (faces[1][2] != faces[2][2]) {
                        exe_line(ins_set3[8]);
                        break;
                    } else {
                        exe_line(ins_set3[9]);
                        break;
                    }
                } else {
                    exe_line(ins_set3[5]);
                    break;
                }
            } else if (faces[2][0] == faces[2][2] && faces[2][2] == faces[4][0] && faces[4][0] == faces[4][2] &&
                       faces[0][6] == faces[0][8]) {
                exe_line(ins_set3[6]);
                break;
            } else if (faces[0][0] == faces[0][2] && faces[0][2] == faces[1][0] && faces[1][0] == faces[1][2]) {
                exe_line(ins_set3[7]);
                break;
            } else if (faces[1][2] == faces[4][2] && faces[4][2] == faces[4][0] && faces[4][0] == faces[3][0]) {
                exe_line(ins_set3[7]);
                break;
            } else if (faces[1][0] == faces[1][2] && faces[1][2] == faces[3][0] && faces[3][0] == faces[3][2]) {
                exe_line(ins_set3[7]);
                break;
            }
            draw_cube(yc);
        }
    }

    if (faces[0][0] != faces[0][2] || faces[0][2] != faces[0][6] || faces[0][6] != faces[0][8] ||
        faces[0][8] != faces[0][0])
        printf("error5\n");
    while (1) {
        for (int count = 0; count < 4; count++) {
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 9; j++) {
                    if (faces[i][j] == faces[0][4] && neighbor_vector[i][j][2] == -1 &&
                        faces[neighbor_vector[i][j][0]][neighbor_vector[i][j][1]] == faces[3][0]) {
                        exe_line(ins_set2[i][j]);
                        arr[count][0] = i, arr[count][1] = j;
                        if (faces[3][0] != faces[3][1])
                            printf("error2 %d %d\n", i, j);
                        if (faces[0][0] != faces[0][2] || faces[0][2] != faces[0][6] ||
                            faces[0][6] != faces[0][8] ||
                            faces[0][8] != faces[0][0])
                            printf("error3 %d %d\n", i, j);
                    }
                }
            }
            if (count == 0 || count == 2)
                draw_cube(y2c);
            else if (count == 1) {
                if (faces[4][1] == faces[0][4])
                    draw_cube(yc);
                else draw_cube(yu);
            }

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
        draw_cube(ins_arr[pos]);
        draw_cube(x2u);
    }

    if (!(faces[0][1] == faces[0][3] && faces[0][3] == faces[0][5] && faces[0][5] == faces[0][7] &&
          faces[1][0] == faces[1][1] && faces[2][0] == faces[2][1] && faces[3][0] == faces[3][1] &&
          faces[5][1] == faces[5][3] && faces[5][3] == faces[5][5] && faces[5][5] == faces[5][7] &&
          faces[1][6] == faces[1][7] && faces[2][6] == faces[2][7] && faces[3][6] == faces[3][7]))
        printf("error6\n");
    draw_cube(zu);
    while (faces[0][0] != 2)
        draw_cube(Lc);
    while (faces[0][4] != 2)
        draw_cube(Mc);
    while (faces[0][2] != 2)
        draw_cube(Ru);
    for (int i = 0; i < 4; i++) {
        int dif1 = abs(faces[0][1] - faces[0][4]);
        if (dif1 != 2 && dif1 != 0) {
            int dif2 = abs(faces[1][1] - faces[1][4]), dif3 = abs(faces[3][4] - faces[3][7]);
            if (dif2 != 2 && dif2 != 0)
                exe_line(ins_set4[0]);
            else if (dif3 == 2 || dif3 == 0) {
                draw_cube(F2c);
                exe_line(ins_set4[0]);
                draw_cube(F2c);
                break;
            }
        }
        draw_cube(xu);
    }
    for (int i = 0; i < 4; i++) {
        if (faces[0][1] != faces[0][4] && faces[0][4] != faces[0][7]) {
            exe_line(ins_set4[1]);
        } else if (faces[0][1] != faces[0][4] && faces[5][1] == faces[0][4] && faces[1][7] == faces[3][4]) {
            exe_line(ins_set4[2]);
            break;
        } else if (faces[0][7] != faces[0][4] && faces[5][7] == faces[0][4] && faces[3][7] == faces[1][4]) {
            draw_cube(y2c);
            exe_line(ins_set4[2]);
            break;
        }
        draw_cube(xu);
    }
    for (int i = 0; i < 4; i++) {
        if (faces[0][1] != faces[0][4] || faces[0][3] != faces[0][4] || faces[0][5] != faces[0][4] ||
            faces[0][7] != faces[0][4]) {
            printf("error9\n");
            while (1)
                draw_cube(yc);
            break;
        }
        draw_cube(yc);
    }
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
                SDL_DestroyWindow(window1);
                SDL_Quit();
                exit(0);
            }
            switch (ev.type) {
                case SDL_MOUSEBUTTONDOWN: {
                    if (ev.button.button == SDL_BUTTON_LEFT) {
                        mouse_flag = 1;
                        x_rel = y_rel = 0;
                        x_pos = ev.button.x;
                        y_pos = ev.button.y;
                        faceAndPos = get_pos(x_pos, y_pos);
                        printf("%d %d\n", x_pos, y_pos);
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    if (ev.button.button == SDL_BUTTON_LEFT)
                        mouse_flag = 0;
                    break;
                }
                case SDL_MOUSEMOTION: {
                    if (ev.button.button == SDL_BUTTON_LEFT && mouse_flag == 1) {
                        x_rel += ev.motion.xrel;
                        y_rel += ev.motion.yrel;
                        if (abs(x_rel) > 50 || abs(y_rel) > 50) {
                            mouse_flag = 0;
                            if (ins_flag) {
                                printf("%d %d\n", x_rel, y_rel);

                                exe_vector(x_rel, y_rel, faceAndPos);
                            }
                        }
                    }
                    break;
                }

            }
        }
    }
};


void init() {
    int x = 1, y = 1, z = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++) {
                vector[9 * i + 3 * j + k][0] = x - i;
                vector[9 * i + 3 * j + k][1] = y - j;
                vector[9 * i + 3 * j + k][2] = z - k;
            }
}

