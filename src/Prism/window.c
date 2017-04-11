#include <SDL2/SDL.h>
#include <Prism/application.h>
#include <Prism/window.h>
#include <Prism/list.h>
#include <string.h>

enum {
    PR_ONDISPLAY,
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

#define PR_WINDOW_SLOT(name) void name##_Slot(void * ap_obj, va_list ap_args)

void Pr_SetWindowTitle(Pr_SystemWindowRef ap_wnd, pr_cstring_t ap_title)
{
    SDL_SetWindowTitle(ap_wnd->sdlWindow, ap_title);
}

PR_WINDOW_SLOT(Pr_SetWindowTitle)
{
    Pr_SystemWindowRef lp_this = ap_obj;

    if (!ap_args) return;

    Pr_SetWindowTitle(lp_this, va_arg(ap_args, char *));
}

void Pr_SetWindowPosition(Pr_SystemWindowRef ap_wnd, long a_x, long a_y)
{
    SDL_SetWindowPosition(ap_wnd->sdlWindow, a_x, a_y);

    Pr_Emit(Pr_WindowMoved(ap_wnd), a_x, a_y);
}

PR_WINDOW_SLOT(Pr_SetWindowPosition)
{
    Pr_SystemWindowRef lp_this = ap_obj;
    long l_x;
    long l_y;

    if (!ap_args) return;

    l_x = va_arg(ap_args, long);
    l_y = va_arg(ap_args, long);

    Pr_SetWindowPosition(lp_this, l_x, l_y);
}

void Pr_SetWindowSize(Pr_SystemWindowRef ap_wnd, pr_u32_t a_w, pr_u32_t a_h)
{
    SDL_SetWindowSize(ap_wnd->sdlWindow, a_w, a_h);

    Pr_Emit(Pr_WindowSizeChanged(ap_wnd), a_w, a_h);
}

PR_WINDOW_SLOT(Pr_SetWindowSize)
{
    Pr_SystemWindowRef lp_this = ap_obj;
    pr_u32_t l_w;
    pr_u32_t l_h;

    if (!ap_args) return;

    l_w = va_arg(ap_args, pr_u32_t);
    l_h = va_arg(ap_args, pr_u32_t);

    Pr_SetWindowSize(lp_this, l_w, l_h);
}

void Pr_SetWindowBordered(Pr_SystemWindowRef ap_wnd, pr_bool_t a_bool)
{
    SDL_SetWindowBordered(ap_wnd->sdlWindow, a_bool);

    Pr_Emit(Pr_WindowFramed(ap_wnd), a_bool);
}

PR_WINDOW_SLOT(Pr_SetWindowBordered)
{
    Pr_SystemWindowRef lp_this = ap_obj;
    pr_bool_t l_bool;

    if (!ap_args) return;

    l_bool = va_arg(ap_args, pr_bool_t);

    Pr_SetWindowBordered(lp_this, l_bool);
}

void Pr_HideWindow(Pr_SystemWindowRef ap_wnd)
{
    SDL_HideWindow(ap_wnd->sdlWindow);

    Pr_Emit(Pr_WindowHidden(ap_wnd));
}

PR_WINDOW_SLOT(Pr_HideWindow)
{
    Pr_SystemWindowRef lp_this = ap_obj;

    Pr_HideWindow(lp_this);
}

void Pr_ShowWindow(Pr_SystemWindowRef ap_wnd)
{
    SDL_ShowWindow(ap_wnd->sdlWindow);

    Pr_Emit(Pr_WindowShown(ap_wnd));
}

PR_WINDOW_SLOT(Pr_ShowWindow)
{
    Pr_SystemWindowRef lp_this = ap_obj;

    Pr_ShowWindow(lp_this);
}

static void s_Pr_DeleteWindowSignalList(Pr_SystemWindow * ap_wnd)
{
    Pr_ListIterator lp_it;

    PR_LIST_FOREACH(ap_wnd->signals, lp_it) {
        Pr_Signal * lp_tmp = Pr_ListIteratorData(lp_it);
        Pr_DeleteSignal(lp_tmp);
    }

    Pr_DeleteList(ap_wnd->signals);
    ap_wnd->signals = NULL;
}

static pr_bool_t s_Pr_CreateWindowSignals(Pr_SystemWindow * ap_wnd)
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

static void s_Pr_GetWindowSize(Pr_SystemWindowRef ap_wnd, pr_u32_t * ap_w, pr_u32_t * ap_h)
{
    pr_u32_t l_w;
    pr_u32_t l_h;

    if (!ap_wnd) return;

    SDL_GetWindowSize(ap_wnd->sdlWindow, (int *)&l_w, (int *)&l_h);

    if (ap_w) *ap_w = l_w;
    if (ap_h) *ap_h = l_h;
}

static pr_bool_t s_Pr_ConstructWindow(Pr_ObjectRef ap_obj)
{
    Pr_SystemWindowRef  lp_this = (Pr_SystemWindowRef)ap_obj;
    SDL_Window *        lp_wnd = NULL;

    lp_wnd = SDL_CreateWindow("Prism Application" ,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_HIDDEN
    );

    if (!lp_wnd) return PR_FALSE;

    if (Pr_RegisterWindow(lp_this)) {
        if (s_Pr_CreateWindowSignals(lp_this)) {
            lp_this->sdlWindow = lp_wnd;
            lp_this->id = SDL_GetWindowID(lp_this->sdlWindow);

            lp_this->getSize        = s_Pr_GetWindowSize;

            lp_this->setTitle       = Pr_SetWindowTitle;
            lp_this->setPosition    = Pr_SetWindowPosition;
            lp_this->setSize        = Pr_SetWindowSize;
            lp_this->setMaximized   = NULL;
            lp_this->setMinimized   = NULL;
            lp_this->setBordered    = Pr_SetWindowBordered;
            lp_this->hide           = Pr_HideWindow;
            lp_this->show           = Pr_ShowWindow;
            lp_this->raise          = NULL;

            lp_this->slot_setTitle      = Pr_SetWindowTitle_Slot;
            lp_this->slot_setPosition   = Pr_SetWindowPosition_Slot;
            lp_this->slot_setSize       = Pr_SetWindowSize_Slot;
            lp_this->slot_setMaximized  = NULL;
            lp_this->slot_setMinimized  = NULL;
            lp_this->slot_setBordered   = Pr_SetWindowBordered_Slot;
            lp_this->slot_hide          = Pr_HideWindow_Slot;
            lp_this->slot_show          = Pr_ShowWindow_Slot;
            lp_this->slot_raise         = NULL;

            return PR_TRUE;
        }
    }

    SDL_DestroyWindow(lp_wnd);

    lp_this->signals    = NULL;
    lp_this->sdlWindow  = NULL;
    lp_this->id         = 0;

    return PR_FALSE;
} 

static void s_Pr_DestructWindow(Pr_ObjectRef ap_obj)
{
    Pr_SystemWindowRef lp_this = (Pr_SystemWindowRef)ap_obj;

    Pr_Emit(ap_obj->onDelete);

    Pr_UnregisterWindow(lp_this);

    s_Pr_DeleteWindowSignalList(lp_this);

    SDL_DestroyWindow(lp_this->sdlWindow);
}

Pr_Class Pr_SystemWindowClass = {
    "Pr_SystemWindow",
    sizeof(Pr_SystemWindow),
    0,
    &Pr_ObjectClass,
    NULL,
    8,
    PR_FALSE,

    s_Pr_ConstructWindow,
    s_Pr_DestructWindow
};

#define PR_SIG_IMPL(name, signalId) \
    Pr_Signal * name(Pr_SystemWindowRef ap_wnd) \
    { \
        if (!ap_wnd) return NULL; \
        return Pr_ListAt(ap_wnd->signals, signalId); \
    } 

PR_SIG_IMPL(Pr_WindowClosed,        PR_CLOSED)
PR_SIG_IMPL(Pr_WindowMoved,         PR_MOVED)
PR_SIG_IMPL(Pr_WindowSizeChanged,   PR_SIZE_CHANGED)
PR_SIG_IMPL(Pr_WindowFramed,        PR_FRAMED)
PR_SIG_IMPL(Pr_WindowHidden,        PR_HIDDEN)
PR_SIG_IMPL(Pr_WindowShown,         PR_SHOWN)
PR_SIG_IMPL(Pr_WindowMinimized,     PR_MINIMIZED)
PR_SIG_IMPL(Pr_WindowMaximized,     PR_MAXIMIZED)
PR_SIG_IMPL(Pr_WindowFocusGained,   PR_FOCUS_GAINED)
PR_SIG_IMPL(Pr_WindowFocusLost,     PR_FOCUS_LOST)
PR_SIG_IMPL(Pr_WindowEntered,       PR_ENTER)
PR_SIG_IMPL(Pr_WindowLeaved,        PR_LEAVE)
PR_SIG_IMPL(Pr_WindowRestored,      PR_RESTORED)
PR_SIG_IMPL(Pr_WindowOnDisplay,     PR_ONDISPLAY)


