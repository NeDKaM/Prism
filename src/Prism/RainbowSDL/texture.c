#include <Prism/RainbowSDL/texture.h>
#include <SDL2/SDL_image.h>

SDL_Texture * Pr_LoadSDLTexture(SDL_Renderer * ap_rnd, char const * ap_file)
{
    SDL_Texture * lp_out;
    SDL_Surface * lp_surface;

    if (!ap_rnd) {
        return NULL;
    }

    lp_surface = IMG_Load(ap_file);
    if (!lp_surface) {
        return NULL;
    }

    lp_out = SDL_CreateTextureFromSurface(ap_rnd, lp_surface);

    SDL_FreeSurface(lp_surface);

    if (lp_out) {
        return lp_out;
    }

    return NULL;
}

