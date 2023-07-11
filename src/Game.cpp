// #include <SDL2/SDL.h>
// #include"headers/Game.h"
// #include<iostream>

// int main (int argc,char* args[]) {
//     bool quit = false;
//     SDL_Event event;
//     SDL_Init (SDL_INIT_EVERYTHING);
//     SDL_Window *window = SDL_CreateWindow ("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 680, 680, 0);
//     SDL_Renderer *render = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//     int startPos = 0;
//     for (int y = 0; y < 8; y++) {
//         for (int x = startPos; x < 8; x+=2) {
//             SDL_Rect rect;
//             rect.x = x * 85;
//             rect.y = y * 85;
//             rect.w = 85;
//             rect.h = 85;
//             SDL_SetRenderDrawColor (render, 238, 238, 210, 200);
//             SDL_RenderFillRect (render, &rect);
//         }
//         startPos = 1 - startPos;

//     }
//     SDL_RenderPresent (render);
//     while (!quit) {
//         SDL_WaitEvent (&event);
//         switch (event.type) {
//             case SDL_QUIT:
//                 quit = true;
//                 break;
//         }
//     }
//     SDL_Quit ();
//     return 0;
// }