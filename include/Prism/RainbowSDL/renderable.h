#ifndef H_RAINBOWSDL_RENDERABLE_INCLUDED
#define H_RAINBOWSDL_RENDERABLE_INCLUDED

#include <Prism/config.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_renderable_t, Pr_Renderable) {
        SDL_Texture *               texture;
        SDL_Rect                    textureCoords;
        SDL_Point                   origin;
        struct { pr_u32_t w, h; }   dstSize;
        SDL_Color                   colorMod;
        SDL_BlendMode               blendMode;
        pr_bool_t                   originOnly;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_RENDERABLE_INCLUDED */

/*


*/
