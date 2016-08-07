#include <SDL2/SDL.h>
#include <Prism/application.h>
#include <Prism/window.h>
#include <Prism/list.h>
#include <string.h>

enum {
    PR_UPDATED = 0,
    PR_ONDELETE,
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

    PR_SIGNALS_COUNT
};

struct pr_window_t {
    SDL_Window *    wnd;
    pr_u32_t        id;
    Pr_List *       signals;
};

#define PR_SIG_IMPL(name, signalId) \
    Pr_Signal * name(Pr_WindowRef ap_wnd) \
    { \
        if (!ap_wnd) return NULL; \
        return Pr_ListAt(ap_wnd->signals, signalId); \
    } 

PR_SIG_IMPL(Pr_WindowClosed,        PR_CLOSED)
PR_SIG_IMPL(Pr_WindowMoved,         PR_MOVED)
PR_SIG_IMPL(Pr_WindowSizeChanged,   PR_SIZE_CHANGED)
PR_SIG_IMPL(Pr_WindowFramed,        PR_FRAMED)
PR_SIG_IMPL(Pr_WindowUpdated,       PR_UPDATED)
PR_SIG_IMPL(Pr_WindowHidden,        PR_HIDDEN)
PR_SIG_IMPL(Pr_WindowShown,         PR_SHOWN)
PR_SIG_IMPL(Pr_WindowMinimized,     PR_MINIMIZED)
PR_SIG_IMPL(Pr_WindowMaximized,     PR_MAXIMIZED)
PR_SIG_IMPL(Pr_WindowFocusGained,   PR_FOCUS_GAINED)
PR_SIG_IMPL(Pr_WindowFocusLost,     PR_FOCUS_LOST)
PR_SIG_IMPL(Pr_WindowEntered,       PR_ENTER)
PR_SIG_IMPL(Pr_WindowLeaved,        PR_LEAVE)
PR_SIG_IMPL(Pr_WindowRestored,      PR_RESTORED)
PR_SIG_IMPL(Pr_WindowOnDelete,      PR_ONDELETE)
PR_SIG_IMPL(Pr_WindowPainted,       PR_PAINTED)

static void s_Pr_DeleteWindowSignalList(Pr_Window * ap_wnd)
{
    Pr_ListIterator lp_it;

    PR_LIST_FOREACH(ap_wnd->signals, lp_it) {
        Pr_Signal * lp_tmp = Pr_ListIteratorData(lp_it);
        Pr_DeleteSignal(lp_tmp);
    }

    Pr_DeleteList(ap_wnd->signals);
    ap_wnd->signals = NULL;
}

static pr_bool_t s_Pr_CreateWindowSignals(Pr_Window * ap_wnd)
{
    int l_i = 0;
    pr_bool_t l_fail = PR_FALSE;

    ap_wnd->signals = Pr_NewList();
    if (!ap_wnd->signals) return PR_FALSE;

    while (l_i < PR_SIGNALS_COUNT) {
        Pr_Signal * lp_tmp = Pr_NewSignal();
        if (lp_tmp) {
            if (!Pr_PushBackListData(ap_wnd->signals, lp_tmp)) {
                l_fail = PR_TRUE;
                Pr_DeleteSignal(lp_tmp);
                break;
            }
        } else {
            l_fail = PR_TRUE;
            break;
        }

        l_i++;
    }

    if (l_fail) {
        s_Pr_DeleteWindowSignalList(ap_wnd);
        return PR_FALSE;
    }

    return PR_TRUE;
}

Pr_Window * Pr_NewWindow(void)
{
    Pr_Window * lp_out;

    lp_out = calloc(1,sizeof(Pr_Window));
    if (!lp_out) return NULL;

    lp_out->wnd = SDL_CreateWindow("Prism Application" ,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_HIDDEN
    );

    if (lp_out->wnd) {
        if (s_Pr_CreateWindowSignals(lp_out)) {
            if (Pr_RegisterWindow(lp_out)) {
                lp_out->id = SDL_GetWindowID(lp_out->wnd);
                return lp_out;
            }

            s_Pr_DeleteWindowSignalList(lp_out);
        }

        SDL_DestroyWindow(lp_out->wnd);
    } 

    free(lp_out);

    return NULL;
}

void Pr_DeleteWindow(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return;

    Pr_Emit(Pr_WindowOnDelete(ap_wnd));

    Pr_UnregisterWindow(ap_wnd);

    s_Pr_DeleteWindowSignalList(ap_wnd);

    SDL_DestroyWindow(ap_wnd->wnd);
    free(ap_wnd);
}

pr_u32_t Pr_GetWindowId(Pr_WindowRef ap_wnd)
{
    return ap_wnd ? ap_wnd->id : 0;
}

pr_bool_t Pr_GetWindowSize(Pr_WindowRef ap_wnd, pr_u32_t * ap_w, pr_u32_t * ap_h)
{
    if (!ap_wnd) return PR_FALSE;
    if (!ap_w && !ap_h) return PR_FALSE;

    SDL_GetWindowSize(ap_wnd->wnd, ap_w, ap_h);

    return PR_TRUE;
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

void Pr_HideWindow(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return;

    SDL_HideWindow(ap_wnd->wnd);

    Pr_Emit(Pr_WindowHidden(ap_wnd));
}

PR_SLOT_IMPL(Pr_HideWindow_Slot)
{
    Pr_HideWindow(ap_obj);
}

void Pr_ShowWindow(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return;

    SDL_ShowWindow(ap_wnd->wnd);

    Pr_Emit(Pr_WindowShown(ap_wnd));
}

PR_SLOT_IMPL(Pr_ShowWindow_Slot)
{
    Pr_ShowWindow(ap_obj);
}
