#ifndef H_RAINBOWSDL_TEXTURE_INCLUDED
#define H_RAINBOWSDL_TEXTURE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

#include <Prism/ressource.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    extern PRISM_API SDL_Texture *  Pr_LoadSDLTexture(SDL_Renderer * rnd, char const * file);
    
    PR_OBJECT(pr_texture_t, Pr_Texture) {
        PR_INHERIT(Pr_Resource);

        SDL_Texture *   impl;
    };

    extern PRISM_API Pr_TextureRef  Pr_NewTexture(pr_cstring_t file);

    extern PRISM_API Pr_Class Pr_TextureClass;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_TEXTURE_INCLUDED */

