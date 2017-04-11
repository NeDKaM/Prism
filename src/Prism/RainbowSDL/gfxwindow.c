#include <Prism/RainbowSDL/gfxwindow.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

static SDL_Renderer * s_currentRenderer = NULL;

SDL_Renderer * const Pr_GetCurrentRenderer(void)
{
    return s_currentRenderer;
}

static void s_Pr_RenderPresent_Slot(void * ap_obj, va_list ap)
{
    Pr_GfxWindowRef lp_wnd;

    lp_wnd = ap_obj;

    s_currentRenderer = lp_wnd->renderer;

    SDL_RenderPresent(s_currentRenderer);
    SDL_RenderClear(s_currentRenderer);
}

static pr_bool_t s_Pr_ConstructWindow(Pr_ObjectRef ap_wnd)
{
    Pr_GfxWindowRef lp_wnd;
    Pr_SystemWindowRef lp_self;

    lp_wnd = (Pr_GfxWindowRef)ap_wnd;
    lp_self = &lp_wnd->self;

    lp_wnd->renderer = SDL_CreateRenderer(lp_self->sdlWindow, -1, SDL_RENDERER_ACCELERATED);

    if (lp_wnd->renderer
        && Pr_Connect(Pr_WindowOnDisplay(lp_self), lp_self, s_Pr_RenderPresent_Slot)
    ) {
        s_currentRenderer = lp_wnd->renderer;
        return PR_TRUE;
    } 

    return PR_FALSE;
}

static void s_Pr_DestructWindow(Pr_ObjectRef ap_wnd)
{
    Pr_GfxWindowRef lp_wnd;

    lp_wnd = (Pr_GfxWindowRef)ap_wnd;

    SDL_DestroyRenderer(lp_wnd->renderer);
}

Pr_Class Pr_GfxWindowClass = {
    "Pr_GfxWindow",
    sizeof(Pr_GfxWindow),
    0,
    &Pr_SystemWindowClass,
    NULL,
    8,
    PR_FALSE,
    s_Pr_ConstructWindow,
    s_Pr_DestructWindow
};
