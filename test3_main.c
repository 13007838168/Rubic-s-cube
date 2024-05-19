#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <time.h>
#include "test3_head.h"
/* Undefine this if you want a flat cube instead of a rainbow cube */
#undef SHADED_CUBE


/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc) {
    if (ctx) {
        /* SDL_GL_MakeCurrent(0, NULL); *//* doesn't do anything */
        SDL_GL_DeleteContext(ctx);
    }
    SDL_DestroyWindow(window1);
    SDL_Quit();
    exit(rc);
}


int
main(int argc, char *argv[]) {
    int value;
    int i, done;
    SDL_DisplayMode mode;
    SDL_Event event;
    Uint32 then, now, frames;
    int status;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    //Create window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window1 = SDL_CreateWindow
            (
                    "SDL AR Example",
                    600, SDL_WINDOWPOS_UNDEFINED,
                    800, 800,
                    SDL_WINDOW_OPENGL
            );
    if (window1 == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    init();

    //creating new context
    ctx = SDL_GL_CreateContext(window1);
    SDL_GL_SetSwapInterval(1);



    /* Set rendering settings */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    int t = 0;
    update_face();
    int randomNum;
    // 设置随机种子




    while (!done) {
        /* Check for events */
        ++frames;
        handle();
        int w, h;
        SDL_GL_MakeCurrent(window1, ctx);
        SDL_GetWindowSize(window1, &w, &h);
        glViewport(0, 0, w, h);
        draw_cube(0);
//int arr[18]={Ru,  Du, Rc, Du, Lc, Bu,  Lu,  Bc, D2c, Fu, Rc, Fc,  Ru,  x2c, yc};
        //    exe_line(arr);
        /*  int i = Rc;
          exe_line(ins_set4[0]);
          while (1)
              draw_cube(yc);*/
        /*if (t++ % 200 == 0) {
            srand(time(NULL));
            //change_face(zu);
            for (int i = 0; i < 100; i++)

                // 生成随机数
            {

                randomNum = rand();
                //change_line(ins_set4[randomNum%4]);
                //  if(t%100==0)
                //change_face(randomNum % 72 + 1);
            }
            solve();
          //  change_face(zc);
            for (int i = 1; i < 5; i++) {
                if (faces[i][0] != faces[i][2] || faces[i][6] != faces[i][8]) {
                    printf("error");
                    while (1)
                        draw_cube(yc);
                }
            }
        }*/





        // draw_cube(0);
        /*for(int i=61;i<=72;i++) {
            for(int j=0;j<4;j++)
                draw_cube(i);
        }
          return 0;*/


        // SDL_Delay(1000);

    }

    /* Print out some timing information */
    /*now = SDL_GetTicks();
    if (now > then) {
        printf("%2.2f frames per second\n",
               ((double) frames * 1000) / (now - then));
    }
    quit(0);*/
    return 0;
}