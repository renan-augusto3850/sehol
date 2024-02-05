#include <iostream>
#include <SDL2/SDL.H>
#include <string>
using namespace std;

void drawWindow(std::string title, int width, int heigth){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_GetError();
    }
    const char* Title = title.c_str();
    SDL_Window* window = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, heigth, SDL_WINDOW_SHOWN);
    if(window == NULL){
        std::cout << "Nao foi possivel criar a janela" << std::endl;
    }
    bool fechar = false;
    SDL_Event evento;
    while(!fechar){
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_QUIT){
                fechar = true;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}