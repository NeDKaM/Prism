#ifndef H_RAINBOWSDL_GFXWINDOW_INCLUDED
#define H_RAINBOWSDL_GFXWINDOW_INCLUDED

#include <Prism/window.h>
#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    PR_OBJECT(pr_gfxwindow_t, Pr_GfxWindow) {
        PR_INHERIT(Pr_SystemWindow);

        SDL_Renderer * renderer;
    };

    extern PRISM_API SDL_Renderer * const Pr_GetCurrentRenderer(void);

    extern PRISM_API Pr_Class Pr_GfxWindowClass;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_GFXWINDOW_INCLUDED */

