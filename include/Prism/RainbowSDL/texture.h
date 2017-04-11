#ifndef H_RAINBOWSDL_TEXTURE_INCLUDED
#define H_RAINBOWSDL_TEXTURE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    extern PRISM_API SDL_Texture *  Pr_LoadSDLTexture(SDL_Renderer * rnd, char const * file);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_TEXTURE_INCLUDED */

