#include <Prism/Gui/Internal/sdl2renderer.h>
#include <SDL2/SDL.h>

static void s_Pr_RenderClear(Pr_Renderer * ap_rnd)
{
    SDL_RenderClear(ap_rnd->impl);
}

static void s_Pr_RenderDisplay(Pr_Renderer * ap_rnd)
{
    SDL_RenderPresent(ap_rnd->impl);
}

static void s_Pr_SetDrawColor(Pr_Renderer * ap_rnd, int a_r, int a_g, int a_b, int a_a)
{
    SDL_SetRenderDrawColor(ap_rnd->impl,a_r,a_g,a_b,a_a);
}

static void s_Pr_DrawPoint(Pr_Renderer * ap_rnd, int a_x, int a_y)
{
    SDL_RenderDrawPoint(ap_rnd->impl,a_x,a_y);
}

static void s_Pr_DrawLine(Pr_Renderer * ap_rnd, int a_x1, int a_y1, int a_x2, int a_y2)
{
    SDL_RenderDrawLine(ap_rnd->impl,a_x1,a_y1,a_x2,a_y2);
}

static void s_Pr_DrawRect(Pr_Renderer * ap_rnd, int a_x, int a_y, unsigned int a_w, unsigned int a_h)
{
    SDL_Rect l_rect = {a_x,a_y,a_w,a_h};

    SDL_RenderDrawRect(ap_rnd->impl,&l_rect);
}

static void s_Pr_DrawFilledRect(Pr_Renderer * ap_rnd, int a_x, int a_y, unsigned int a_w, unsigned int a_h)
{
    SDL_Rect l_rect = {a_x,a_y,a_w,a_h};

    SDL_RenderFillRect(ap_rnd->impl,&l_rect);
}

void Pr_SetSDL2Renderer(Pr_Renderer * ap_rnd)
{
    ap_rnd->renderClear     = s_Pr_RenderClear;
    ap_rnd->renderDisplay   = s_Pr_RenderDisplay;

    ap_rnd->setDrawColor    = s_Pr_SetDrawColor;
    ap_rnd->drawPoint       = s_Pr_DrawPoint;
    ap_rnd->drawLine        = s_Pr_DrawLine;
    ap_rnd->drawRect        = s_Pr_DrawRect;
    ap_rnd->drawFilledRect  = s_Pr_DrawFilledRect;
}