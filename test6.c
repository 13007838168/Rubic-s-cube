#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// 全局变量
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
SDL_Color inputColor = { 255, 255, 255, 255 };
SDL_Color outputColor = { 255, 255, 255, 255 };
char* inputText = NULL;
char* outputText = "Output Box";
bool running = true;

// 函数声明
void renderWindow();
void handleEvent(SDL_Event* event);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // 创建窗口和渲染器
    window = SDL_CreateWindow("Text Box Demo",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // 加载字体
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);

    // 初始化输入文本框
    inputText = (char*)malloc(sizeof(char));
    inputText[0] = '\0';

    // 主循环
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        renderWindow();

        // 延时一段时间，例如100毫秒
        //SDL_Delay(100);
    }

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void renderWindow() {
    // 清空窗口
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 渲染输入文本框
    if(inputText[0])
    {
        SDL_Surface *inputSurface = TTF_RenderText_Solid(font, inputText, inputColor);
        SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
        SDL_Rect inputRect = {100, 100, inputSurface->w, inputSurface->h};
        SDL_RenderCopy(renderer, inputTexture, NULL, &inputRect);
        SDL_DestroyTexture(inputTexture);
        SDL_FreeSurface(inputSurface);
    }
    // 渲染输出文本框
    if(outputText[0]) {
        SDL_Surface *outputSurface = TTF_RenderText_Solid(font, outputText, outputColor);
        SDL_Texture *outputTexture = SDL_CreateTextureFromSurface(renderer, outputSurface);
        SDL_Rect outputRect = {100, 200, outputSurface->w, outputSurface->h};
        SDL_RenderCopy(renderer, outputTexture, NULL, &outputRect);
        SDL_DestroyTexture(outputTexture);
        SDL_FreeSurface(outputSurface);
    }

    // 将文本框渲染到窗口上



    // 更新窗口
    SDL_RenderPresent(renderer);

    // 释放资源


}

void handleEvent(SDL_Event* event) {
    if (event->type == SDL_QUIT) {
        // 点击关闭按钮退出程序
        running = false;
    }
    else if (event->type == SDL_TEXTINPUT) {
        // 处理输入
        strcat(inputText, event->text.text);
    }
    else if (event->type == SDL_KEYDOWN) {
        // 处理退格键
        if (event->key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
            inputText[strlen(inputText) - 1] = '\0';
        }
    }
}
