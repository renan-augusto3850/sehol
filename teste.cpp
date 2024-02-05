#include <SDL2/SDL.h>
#include <cstdlib>  // Para a função exit

int WinMain(int argc, char** argv) {
    // Inicializar a SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // Se a inicialização falhar, imprima uma mensagem de erro e saia
        SDL_Log("Erro ao inicializar a SDL: %s", SDL_GetError());
        return 1;
    }

    // Criar uma janela
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 It's Work!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        // Se a criação da janela falhar, imprima uma mensagem de erro, libere recursos e saia
        SDL_Log("Erro ao criar a janela: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Criar um renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        // Se a criação do renderer falhar, imprima uma mensagem de erro, libere recursos e saia
        SDL_Log("Erro ao criar o renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Definir a cor de desenho do renderer
    SDL_SetRenderDrawColor(renderer, 9, 20, 33, 255);

    // Loop principal
    while (true) {
        SDL_Event event;
        // Processar eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // Se o usuário fechar a janela, sair do loop
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }

        // Limpar o renderer
        SDL_RenderClear(renderer);

        // Atualizar o renderer
        SDL_RenderPresent(renderer);
    }

    // Libere recursos e saia
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
