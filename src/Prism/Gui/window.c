#include <SDL2/SDL.h>
#include <Prism/Gui/application.h>
#include <Prism/Gui/window.h>
#include <Prism/Gui/Internal/request.h>
#include <Prism/Gui/Internal/sdl2renderer.h>
#include <Prism/array.h>
#include <string.h>

enum {
    PR_UPDATED = 0,
    PR_PAINTED,

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

    PR_DELETED,

    PR_SIGNALS_COUNT
};

struct pr_window_t {

    SDL_Window * wnd;
    Pr_Renderer rndImpl;
    Pr_Renderer rndBase;

    unsigned long id;
    int available;

    PR_ARRAY(Pr_Signal *,signals);
};

#define PR_SIG_IMPL(name,signalVal) \
    Pr_Signal * name(Pr_Window * ap_wnd) \
    { \
        if (!ap_wnd) return NULL; \
        if (!ap_wnd->available) return NULL; \
        return ap_wnd->signals.list[signalVal]; \
    } 

PR_SIG_IMPL(Pr_WindowClosed, PR_CLOSED)
PR_SIG_IMPL(Pr_WindowMoved, PR_MOVED)
PR_SIG_IMPL(Pr_WindowSizeChanged, PR_SIZE_CHANGED)
PR_SIG_IMPL(Pr_WindowFramed, PR_FRAMED)
PR_SIG_IMPL(Pr_WindowUpdated, PR_UPDATED)
PR_SIG_IMPL(Pr_WindowPainted, PR_PAINTED)
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
    ap_wnd->rndBase.impl = SDL_CreateRenderer(ap_wnd->wnd,-1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE
    );

    if (!ap_wnd->rndBase.impl) return 0;

    Pr_SetSDL2Renderer(&ap_wnd->rndBase);

    return 1;
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
        lp_out->rndBase.impl = NULL;
        lp_out->rndImpl.impl = NULL;
        if (s_Pr_CreateWindowSignals(lp_out)) {
            if (s_Pr_SetBaseRenderer(lp_out)) {
                lp_out->available = Pr_Request_NewWindow(lp_out);
                if (lp_out->available) {
                    lp_out->id = SDL_GetWindowID(lp_out->wnd);
                    Pr_AttachRenderer(lp_out,NULL);
                    return lp_out;
                }
            }
        }

        SDL_DestroyRenderer(lp_out->rndBase.impl);
        SDL_DestroyWindow(lp_out->wnd);
        Pr_ClearArray(lp_out->signals);
    } 

    free(lp_out);

    return NULL;
}

Pr_Renderer * Pr_GetWindowRenderer(Pr_Window * ap_wnd)
{
    return ap_wnd ? &ap_wnd->rndImpl : NULL;
}

int Pr_AttachRenderer(Pr_Window * ap_wnd, Pr_Renderer * ap_rnd)
{
    if (!ap_wnd) return 0;

    if (ap_rnd) {
        memcpy(&ap_wnd->rndImpl, ap_rnd, sizeof(Pr_Renderer));
    } else {
        memcpy(&ap_wnd->rndImpl, &ap_wnd->rndBase, sizeof(Pr_Renderer));
    }

    return 1;
}

void Pr_DeleteWindow(Pr_Window * ap_wnd)
{
    if (ap_wnd) {
        int l_i;

        Pr_Request_DeleteWindow(ap_wnd);

        Pr_Emit(Pr_WindowDeleted(ap_wnd));

        for (l_i = 0; l_i < PR_SIGNALS_COUNT; l_i++) {
            Pr_DeleteSignal(ap_wnd->signals.list[l_i]);
        }

        Pr_ClearArray(ap_wnd->signals);
        SDL_DestroyRenderer(ap_wnd->rndBase.impl);
        SDL_DestroyWindow(ap_wnd->wnd);
        free(ap_wnd);
    }
}

void Pr_HandleWindowEvent(Pr_Window * ap_wnd, void * ap_ev)
{
    SDL_Event * lp_ev = ap_ev;

    if (!ap_wnd) return;
    if (!ap_ev) return;

    if (lp_ev->type == SDL_WINDOWEVENT && lp_ev->window.windowID == ap_wnd->id) {
        switch (lp_ev->window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                Pr_Emit(Pr_WindowShown(ap_wnd));
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                Pr_Emit(Pr_WindowHidden(ap_wnd));
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                Pr_Emit(Pr_WindowSizeChanged(ap_wnd),
                    lp_ev->window.data1,lp_ev->window.data2
                );
                break;
            case SDL_WINDOWEVENT_RESIZED:
                break;
            case SDL_WINDOWEVENT_ENTER:
                Pr_Emit(Pr_WindowEntered(ap_wnd));
                break;
            case SDL_WINDOWEVENT_LEAVE:
                Pr_Emit(Pr_WindowLeaved(ap_wnd));
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                Pr_Emit(Pr_WindowFocusGained(ap_wnd));
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                Pr_Emit(Pr_WindowFocusLost(ap_wnd));
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                Pr_Emit(Pr_WindowMinimized(ap_wnd));
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                Pr_Emit(Pr_WindowMaximized(ap_wnd));
                break;
            case SDL_WINDOWEVENT_RESTORED:
                Pr_Emit(Pr_WindowRestored(ap_wnd));
                break;
            case SDL_WINDOWEVENT_CLOSE:
                Pr_Emit(Pr_WindowClosed(ap_wnd));
                break;
            case SDL_WINDOWEVENT_MOVED: 
                Pr_Emit(Pr_WindowMoved(ap_wnd),
                    lp_ev->window.data1,lp_ev->window.data2
                );
                break;
            default:
                break;
        }
    }
}

unsigned long Pr_GetWindowId(Pr_Window * ap_wnd)
{
    return ap_wnd ? ap_wnd->id : 0;
}

void Pr_SetWindowTitle(Pr_Window * ap_wnd, char const * ap_text)
{
    if (!ap_wnd || !ap_text) return;

    SDL_SetWindowTitle(ap_wnd->wnd, ap_text);
}

void Pr_SetWindowPosition(Pr_Window * ap_wnd, int a_x, int a_y)
{
    if (!ap_wnd) return;

    SDL_SetWindowPosition(ap_wnd->wnd, a_x, a_y);

    Pr_Emit(Pr_WindowMoved(ap_wnd), a_x, a_y);
}

void Pr_SetWindowSize(Pr_Window * ap_wnd, unsigned int a_w, unsigned int a_h)
{
    if (!ap_wnd) return;

    SDL_SetWindowSize(ap_wnd->wnd, a_w, a_h);

    Pr_Emit(Pr_WindowSizeChanged(ap_wnd), a_w, a_h);
}

void Pr_SetWindowFramed(Pr_Window * ap_wnd, int a_bool)
{
    if (!ap_wnd) return;

    SDL_SetWindowBordered(ap_wnd->wnd,a_bool);

    Pr_Emit(Pr_WindowFramed(ap_wnd), a_bool);
}

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

#define PR_SLOT_IMPL(name) void name(void * ap_obj, va_list ap_args)

PR_SLOT_IMPL(Pr_Slot_SetWindowTitle)
{
    Pr_Window * lp_wnd;
    char const * lp_text;

    if (!ap_args || !ap_obj) return;

    lp_wnd = ap_obj;
    lp_text = va_arg(ap_args, char const *);
  
    if (!lp_wnd || !lp_text) return;

    SDL_SetWindowTitle(lp_wnd->wnd,lp_text);
}


PR_SLOT_IMPL(Pr_Slot_SetWindowPosition)
{
    Pr_Window * lp_wnd;
    int l_x;
    int l_y;

    if (!ap_args || !ap_obj) return;

    lp_wnd = ap_obj;
    l_x = va_arg(ap_args, int);
    l_y = va_arg(ap_args, int);

    SDL_SetWindowPosition(lp_wnd->wnd,l_x,l_y);

    Pr_Emit(Pr_WindowMoved(lp_wnd),l_x,l_y);
}

PR_SLOT_IMPL(Pr_Slot_SetWindowSize)
{
    Pr_Window * lp_wnd;
    unsigned int l_w;
    unsigned int l_h;

    if (!ap_args || !ap_obj) return;

    lp_wnd = ap_obj;
    l_w = va_arg(ap_args, unsigned int);
    l_h = va_arg(ap_args, unsigned int);

    SDL_SetWindowSize(lp_wnd->wnd,l_w,l_h);

    Pr_Emit(Pr_WindowSizeChanged(lp_wnd),l_w,l_h);
}

PR_SLOT_IMPL(Pr_Slot_SetWindowBordered)
{
    Pr_Window * lp_wnd;
    int l_bool;

    if (!ap_args || !ap_obj) return;

    lp_wnd = ap_obj;
    l_bool = va_arg(ap_args,  int);

    SDL_SetWindowBordered(lp_wnd->wnd,l_bool);

    Pr_Emit(Pr_WindowFramed(lp_wnd), l_bool);
}


