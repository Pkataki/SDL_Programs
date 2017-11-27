#include <SDL2/SDL.h>
#include <iostream>

const int  W_WINDOW = 640;
const int  L_WINDOW = 480;
const int  W_RECT = 100;
const int  L_RECT = 100;

int main()
{
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("TRABALHO 1 DE COMPUTACAO GRAFICA",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WINDOW, L_WINDOW, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
   
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, W_WINDOW, L_WINDOW);
    
    Uint32 * pixels = new Uint32[W_WINDOW * L_WINDOW];
    
    memset(pixels, 255, W_WINDOW * L_WINDOW * sizeof(Uint32));
    int posx= 0 ;
    int posy = 0;
    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, W_WINDOW * sizeof(Uint32));
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_KEYDOWN:
            switch( event.key.keysym.sym )
            {
                case SDLK_a:
                    if(posx > 1)
                        posx -= 1;
                    break;
                case SDLK_d:
                    if(posx < W_WINDOW - 1)
                    posx += 1;
                    break;
                case SDLK_w:
                    if(posy > 1)
                        posy -= 1;
                    break;
                case SDLK_x:
                    if(posy < L_WINDOW - 1)
                        posy += 1;
                    break;
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_s:
                    memset(pixels, 255, W_WINDOW * L_WINDOW * sizeof(Uint32));
                    break;
            }
            break;
            case SDL_QUIT:
              quit = true;
              break;
        }
        
        for(int i = 0 ; i < L_RECT; i++)
        {
            pixels[(posy + i) * W_WINDOW + posx] = 0;
            pixels[(posy + i) * W_WINDOW + posx + W_RECT] = 0;
        }
        for(int i = 0 ; i < W_RECT; i++)
        {
            pixels[posy * W_WINDOW + posx + i] = 0;
            pixels[(posy + L_RECT) * W_WINDOW + posx + i] = 0;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
