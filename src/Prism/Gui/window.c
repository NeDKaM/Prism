#include <SDL2/SDL.h>
#include <Prism/Gui/application.h>
#include <Prism/Gui/window.h>
#include <Prism/Gui/Internal/request.h>
#include <Prism/array.h>

enum {
    PR_SHOWN = 0,
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

    PR_SIGNALS_COUNT
};

struct pr_window_t {

    SDL_Window * wnd;
    SDL_Renderer * rnd;
#ifdef PRISM_OPENGL
    SDL_GLContext ctx;
#endif

    unsigned long id;
    int available;

    PR_ARRAY(Pr_Signal *,signals);
};

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

Pr_Window * Pr_NewWindow(void)
{
    Pr_Window * lp_out;

    lp_out = calloc(1,sizeof(Pr_Window));
    if (!lp_out) return NULL;

    lp_out->wnd = SDL_CreateWindow("Prism Application" 
#ifdef PRISM_OPENGL 
    " - OpenGL" 
#endif
        ,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_SHOWN
#ifdef PRISM_OPENGL
        |SDL_WINDOW_OPENGL
#endif
        );

    if (lp_out->wnd) {
        lp_out->rnd = SDL_CreateRenderer(lp_out->wnd,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
        if (lp_out->rnd && s_Pr_CreateWindowSignals(lp_out)) {
            lp_out->id = SDL_GetWindowID(lp_out->wnd);
#ifdef PRISM_OPENGL
            lp_out->ctx = SDL_GL_CreateContext(lp_out->wnd);
#endif
            lp_out->available = Pr_Request_NewWindow(lp_out);
            return lp_out;
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

        Pr_Request_DeleteWindow(ap_wnd);

        for (l_i = 0; l_i < PR_SIGNALS_COUNT; l_i++) {
            Pr_DeleteSignal(ap_wnd->signals.list[l_i]);
        }

        SDL_DestroyRenderer(ap_wnd->rnd);
#ifdef PRISM_OPENGL
        SDL_GL_DeleteContext(ap_wnd->ctx);
#endif
        SDL_DestroyWindow(ap_wnd->wnd);
        free(ap_wnd);
    }
}

Pr_Signal * Pr_WindowClosed(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return NULL;
    if (!ap_wnd->available) return NULL;

    return ap_wnd->signals.list[PR_CLOSED];
}

Pr_Signal * Pr_WindowMoved(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return NULL;
    if (!ap_wnd->available) return NULL;

    return ap_wnd->signals.list[PR_MOVED];
}

Pr_Signal * Pr_WindowSizeChanged(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return NULL;
    if (!ap_wnd->available) return NULL;

    return ap_wnd->signals.list[PR_SIZE_CHANGED];
}

void Pr_HandleWindowEvent(Pr_Window * ap_wnd, void * ap_ev)
{
    SDL_Event * lp_ev = ap_ev;

    if (!ap_wnd) return;
    if (!ap_ev) return;

    if (lp_ev->type == SDL_WINDOWEVENT && lp_ev->window.windowID == ap_wnd->id) {
        switch (lp_ev->window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                break;
            case SDL_WINDOWEVENT_RESIZED:
                break;
            case SDL_WINDOWEVENT_ENTER:
                break;
            case SDL_WINDOWEVENT_LEAVE:
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                break;
            case SDL_WINDOWEVENT_RESTORED:
                break;
            case SDL_WINDOWEVENT_CLOSE:
                Pr_Emit(Pr_WindowClosed(ap_wnd));
                break;
            case SDL_WINDOWEVENT_MOVED: {
                int l_x, l_y;
                SDL_GetWindowPosition(ap_wnd->wnd,&l_x,&l_y);
                Pr_Emit(Pr_WindowMoved(ap_wnd),l_x,l_y);
                break;
            } 
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

void Pr_Slot_SetWindowTitle(void * ap_obj, va_list ap_args)
        {
            Pr_Window * lp_wnd;
            char const * lp_text;

            if (!ap_args || !ap_obj) return;

            lp_wnd = ap_obj;
            lp_text = va_arg(ap_args, char const *);
  
            if (!lp_wnd || !lp_text) return;

            SDL_SetWindowTitle(lp_wnd->wnd,lp_text);
        }

void Pr_SetWindowPosition(Pr_Window * ap_wnd, int a_x, int a_y)
{
    if (!ap_wnd) return;

    SDL_SetWindowPosition(ap_wnd->wnd, a_x, a_y);

    Pr_Emit(Pr_WindowMoved(ap_wnd), a_x, a_y);
}

void Pr_Slot_SetWindowPosition(void * ap_obj, va_list ap_args)
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

void Pr_SetWindowSize(Pr_Window * ap_wnd, unsigned int a_w, unsigned int a_h)
{
    if (!ap_wnd) return;

    SDL_SetWindowSize(ap_wnd->wnd, a_w, a_h);

    Pr_Emit(Pr_WindowSizeChanged(ap_wnd), a_w, a_h);
}

void Pr_Slot_SetWindowSize(void * ap_obj, va_list ap_args)
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

