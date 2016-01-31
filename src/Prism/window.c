#include <SDL2/SDL.h>
#include <Prism/application.h>
#include <Prism/window.h>
#include <Prism/array.h>
#include <string.h>

enum {
    PR_UPDATED = 0,
    PR_DELETED,

    PR_SHOWN,
    PR_HIDDEN,
    PR_EXPOSED,
    PR_SIZE_CHANGED,
    PR_RESIZED,
    PR_ENTER,
    PR_LEAVE,
    PR_FOCUS_GAINED,
    PR_FOCUS_LOST,
    PR_MINIMIZED,
    PR_MAXIMIZED,
    PR_CLOSED,
    PR_MOVED,
    PR_RESTORED,
    PR_FRAMED,

    PR_SIGNALS_COUNT
};

struct pr_window_t {
    SDL_Window * wnd;
    unsigned long id;
    PR_ARRAY(Pr_Signal *) signals;
    SDL_Renderer * rnd;
};

#define PR_SIG_IMPL(name,signalVal) \
    Pr_Signal * name(Pr_Window * ap_wnd) \
    { \
        if (!ap_wnd) return NULL; \
        return ap_wnd->signals.list[signalVal]; \
    } 

PR_SIG_IMPL(Pr_WindowClosed, PR_CLOSED)
PR_SIG_IMPL(Pr_WindowMoved, PR_MOVED)
PR_SIG_IMPL(Pr_WindowSizeChanged, PR_SIZE_CHANGED)
PR_SIG_IMPL(Pr_WindowFramed, PR_FRAMED)
PR_SIG_IMPL(Pr_WindowUpdated, PR_UPDATED)
PR_SIG_IMPL(Pr_WindowHidden, PR_HIDDEN)
PR_SIG_IMPL(Pr_WindowShown, PR_SHOWN)
PR_SIG_IMPL(Pr_WindowMinimized, PR_MINIMIZED)
PR_SIG_IMPL(Pr_WindowMaximized, PR_MAXIMIZED)
PR_SIG_IMPL(Pr_WindowFocusGained, PR_FOCUS_GAINED)
PR_SIG_IMPL(Pr_WindowFocusLost, PR_FOCUS_LOST)
PR_SIG_IMPL(Pr_WindowEntered, PR_ENTER)
PR_SIG_IMPL(Pr_WindowLeaved, PR_LEAVE)
PR_SIG_IMPL(Pr_WindowRestored, PR_RESTORED)
PR_SIG_IMPL(Pr_WindowDeleted, PR_DELETED)

void Pr_RenderClear(Pr_Window * ap_wnd)
{
    SDL_RenderClear(ap_wnd->rnd);
}

void Pr_RenderDisplay(Pr_Window * ap_wnd)
{
    SDL_RenderPresent(ap_wnd->rnd);
}

void Pr_SetRenderDrawColor(Pr_Window * ap_wnd, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(ap_wnd->rnd,r,g,b,a);
}

static int s_Pr_CreateWindowSignals(Pr_Window * ap_wnd)
{
    int l_i;

    Pr_MakeSizedArray(ap_wnd->signals,PR_SIGNALS_COUNT);
    if (!ap_wnd->signals.size) return 0;

    for (l_i = 0; l_i<PR_SIGNALS_COUNT; l_i++) {
        Pr_Signal ** lp_tmp = &ap_wnd->signals.list[l_i];
        *lp_tmp = Pr_NewSignal();
        if (!*lp_tmp) {
            for (l_i = l_i - 1; l_i >= 0; l_i--) {
                Pr_DeleteSignal(*lp_tmp);
                *lp_tmp = NULL;
            }

            return 0;
        }
    }

    return 1;
}

static int s_Pr_SetBaseRenderer(Pr_Window * ap_wnd)
{
    ap_wnd->rnd = SDL_CreateRenderer(ap_wnd->wnd,-1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE
    );

    return (ap_wnd->rnd) ? 1 : 0;
}

Pr_Window * Pr_NewWindow(void)
{
    Pr_Window * lp_out;

    lp_out = calloc(1,sizeof(Pr_Window));
    if (!lp_out) return NULL;

    lp_out->wnd = SDL_CreateWindow("Prism Application" ,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_SHOWN
    );

    if (lp_out->wnd) {
        if (s_Pr_CreateWindowSignals(lp_out)) {
            if (s_Pr_SetBaseRenderer(lp_out)) {
                if (Pr_RegisterWindow(lp_out)) {
                    lp_out->id = SDL_GetWindowID(lp_out->wnd);
                    return lp_out;
                }
            }
        }

        SDL_DestroyRenderer(lp_out->rnd);
        SDL_DestroyWindow(lp_out->wnd);
        Pr_ClearArray(lp_out->signals);
    } 

    free(lp_out);

    return NULL;
}

void Pr_DeleteWindow(Pr_Window * ap_wnd)
{
    if (ap_wnd) {
        int l_i;

        Pr_UnregisterWindow(ap_wnd);

        Pr_Emit(Pr_WindowDeleted(ap_wnd));

        for (l_i = 0; l_i < PR_SIGNALS_COUNT; l_i++) {
            Pr_DeleteSignal(Pr_ArrayAt(ap_wnd->signals,l_i));
        }

        Pr_ClearArray(ap_wnd->signals);
        SDL_DestroyRenderer(ap_wnd->rnd);
        SDL_DestroyWindow(ap_wnd->wnd);
        free(ap_wnd);
    }
}

unsigned long Pr_GetWindowId(Pr_Window * ap_wnd)
{
    return ap_wnd ? ap_wnd->id : 0;
}

#define PR_SLOT_IMPL(name) void name(void * ap_obj, va_list ap_args)

void Pr_SetWindowTitle(Pr_Window * ap_wnd, char const * ap_text)
{
    if (!ap_wnd || !ap_text) return;

    SDL_SetWindowTitle(ap_wnd->wnd, ap_text);
}

PR_SLOT_IMPL(Pr_SetWindowTitle_Slot)
{
    if (!ap_args) return;

    Pr_SetWindowTitle(ap_obj, va_arg(ap_args, char *));
}

void Pr_SetWindowPosition(Pr_Window * ap_wnd, int a_x, int a_y)
{
    if (!ap_wnd) return;

    SDL_SetWindowPosition(ap_wnd->wnd, a_x, a_y);

    Pr_Emit(Pr_WindowMoved(ap_wnd), a_x, a_y);
}

PR_SLOT_IMPL(Pr_SetWindowPosition_Slot)
{
    if (!ap_args) return;

    Pr_SetWindowPosition(ap_obj, va_arg(ap_args, int), va_arg(ap_args, int));
}

void Pr_SetWindowSize(Pr_Window * ap_wnd, unsigned int a_w, unsigned int a_h)
{
    if (!ap_wnd) return;

    SDL_SetWindowSize(ap_wnd->wnd, a_w, a_h);

    Pr_Emit(Pr_WindowSizeChanged(ap_wnd), a_w, a_h);
}

PR_SLOT_IMPL(Pr_SetWindowSize_Slot)
{
    if (!ap_args) return;

    Pr_SetWindowSize(ap_obj, va_arg(ap_args, unsigned int), va_arg(ap_args, unsigned int));
}

void Pr_SetWindowFramed(Pr_Window * ap_wnd, int a_bool)
{
    if (!ap_wnd) return;

    SDL_SetWindowBordered(ap_wnd->wnd,a_bool);

    Pr_Emit(Pr_WindowFramed(ap_wnd), a_bool);
}

PR_SLOT_IMPL(Pr_SetWindowFramed_Slot)
{
    if (!ap_args) return;

    Pr_SetWindowFramed(ap_obj, va_arg(ap_args, int));
}

/*

void Pr_HideWindow(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return;

    SDL_HideWindow(ap_wnd->wnd);

    Pr_Emit(Pr_WindowHidden(ap_wnd));
}

void Pr_ShowWindow(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return;

    SDL_ShowWindow(ap_wnd->wnd);

    Pr_Emit(Pr_WindowShown(ap_wnd));
}

*/

