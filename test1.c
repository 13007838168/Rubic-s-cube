#include <stdio.h>
#include <SDL2/SDL.h>
int main(int args,char * argv[]){
    SDL_Init(SDL_INIT_VIDEO) ;
    SDL_Window *window= SDL_CreateWindow("Hello, World!",100,100,800,600,SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer= SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_Delay(1500);
    SDL_SetRenderDrawColor(renderer,0x60,0xc0,0xa0,0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(1500);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}