/* File name : application.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism main application implementation
 * Dependencies :   --
*/

#include <SDL2/SDL.h>
#include <Prism/application.h>
#include <Prism/window.h>
#include <Prism/list.h>
#include <Prism/array.h>
#include <Prism/string.h>
#include <stdio.h>
#include <Prism/logger.h>
#include <string.h>

struct {
    pr_bool_t   initialized;
    Pr_List *   wndlist;
    Pr_Logger * log;
    Pr_List *   signals;
    SDL_Event   input;
} static s_app = { 0, NULL, NULL, NULL };

enum {
    PR_QUIT = 0,
    PR_START,

    PR_MOUSE_MOVED,
    PR_MOUSEBUTTON_UP,
    PR_MOUSEBUTTON_DOWN,

    PR_KEY_UP,
    PR_KEY_DOWN,

    PR_SIGNALS_COUNT
};

static void s_Pr_DeleteSignalList(void)
{
    Pr_ListIterator lp_it;

    PR_LIST_FOREACH(s_app.signals, lp_it) {
        Pr_Signal * lp_tmp = Pr_ListIteratorData(lp_it);
        if (lp_tmp) {
            Pr_DeleteSignal(lp_tmp);
        }
    }

    Pr_DeleteList(s_app.signals);
    s_app.signals = NULL;
}

static pr_bool_t s_Pr_CreateSignals(void)
{
    int l_i = 0;
    pr_bool_t l_fail = PR_FALSE;

    s_app.signals = Pr_NewList();
    if (!s_app.signals) return PR_FALSE;
    
    while (l_i < PR_SIGNALS_COUNT) {
        Pr_Signal * lp_tmp = Pr_NewSignal();
        if (lp_tmp) {
            if (!Pr_PushBackListData(s_app.signals, lp_tmp)) {
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
        s_Pr_DeleteSignalList();
        return PR_FALSE;
    }

    return PR_TRUE;
}

static void s_Pr_HandleWindowEvent(Pr_SystemWindow * ap_wnd)
{
    if (!ap_wnd) return;

    if (s_app.input.type == SDL_WINDOWEVENT && s_app.input.window.windowID == ap_wnd->id) {
        switch (s_app.input.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            Pr_Emit(Pr_SystemWindowShown(ap_wnd));
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            Pr_Emit(Pr_SystemWindowHidden(ap_wnd));
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            Pr_Emit(Pr_SystemWindowSizeChanged(ap_wnd),
                s_app.input.window.data1, s_app.input.window.data2
            );
            break;
        case SDL_WINDOWEVENT_RESIZED:
            break;
        case SDL_WINDOWEVENT_ENTER:
            Pr_Emit(Pr_SystemWindowEntered(ap_wnd));
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Pr_Emit(Pr_SystemWindowLeaved(ap_wnd));
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            Pr_Emit(Pr_SystemWindowFocusGained(ap_wnd));
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            Pr_Emit(Pr_SystemWindowFocusLost(ap_wnd));
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            Pr_Emit(Pr_SystemWindowMinimized(ap_wnd));
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            Pr_Emit(Pr_SystemWindowMaximized(ap_wnd));
            break;
        case SDL_WINDOWEVENT_RESTORED:
            Pr_Emit(Pr_SystemWindowRestored(ap_wnd));
            break;
        case SDL_WINDOWEVENT_CLOSE:
            Pr_Emit(Pr_SystemWindowClosed(ap_wnd));
            break;
        case SDL_WINDOWEVENT_MOVED:
            Pr_Emit(Pr_SystemWindowMoved(ap_wnd),
                s_app.input.window.data1, s_app.input.window.data2
            );
            break;
        default:
            break;
        }
    }
}

static void s_Pr_UpdateApp(void)
{
    Pr_ListIterator l_it;

    while (SDL_PollEvent(&s_app.input)) {
        PR_LIST_FOREACH(s_app.wndlist, l_it) {
            s_Pr_HandleWindowEvent(Pr_ListIteratorData(l_it));
        }

        switch (s_app.input.type) {
            case SDL_MOUSEMOTION:
                Pr_Emit(Pr_MouseMoved(), s_app.input.motion.x, s_app.input.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                Pr_Emit(Pr_MouseButtonDown(), s_app.input.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                Pr_Emit(Pr_MouseButtonUp(), s_app.input.button.button);
                break;
            case SDL_MOUSEWHEEL:
                break;
            case SDL_KEYDOWN:
                Pr_Emit(Pr_KeyDown(), s_app.input.key.keysym.sym);
                break;
            case SDL_KEYUP:
                Pr_Emit(Pr_KeyUp(), s_app.input.key.keysym.sym);
                break;
            case SDL_TEXTEDITING:
                break;
            case SDL_TEXTINPUT:
                break;

            default:
                break;
        }
    }

    PR_LIST_FOREACH(s_app.wndlist, l_it) {
        Pr_Emit(Pr_WindowOnDisplay(Pr_ListIteratorData(l_it)));
    }
}

static void s_Pr_QuitApp(void)
{
    unsigned int l_i = 0;

    while (Pr_ListSize(s_app.wndlist)) {
        Pr_Delete(Pr_FrontList(s_app.wndlist));
    }

    Pr_DeleteList(s_app.wndlist);

    s_Pr_DeleteSignalList();

    SDL_Quit();

    Pr_DeleteLogger(s_app.log);

    s_app.initialized = PR_FALSE;
    s_app.wndlist = NULL;
}

int Pr_ExecApp(void)
{
    if (!s_app.initialized) return -1;

    Pr_Emit(Pr_AppStarted());

    while (s_app.initialized && Pr_ListSize(s_app.wndlist)) {
        s_Pr_UpdateApp();
    }

    s_Pr_QuitApp();

    return 0;
}

pr_bool_t Pr_InitApp(void)
{
    pr_bool_t l_retSig;

	if (s_app.initialized) return PR_TRUE;

    s_app.log = Pr_NewLogger();

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        Pr_DeleteLogger(s_app.log);
        s_app.log = NULL;
        return PR_FALSE;
    }

    s_app.wndlist = Pr_NewList();
    l_retSig = s_Pr_CreateSignals();

    if (s_app.wndlist && l_retSig) {
	    s_app.initialized = PR_TRUE;
        return PR_TRUE;
    }
    
    Pr_DeleteLogger(s_app.log);
    s_app.log = NULL;
    Pr_DeleteList(s_app.wndlist);
    s_app.wndlist = NULL;
    s_Pr_DeleteSignalList();
    
    SDL_Quit();
    
	return PR_FALSE;
}

#define PR_SIG_IMPL(name, signalId) \
    Pr_Signal * name(void) \
    { \
        if (!s_app.initialized) return NULL; \
        return Pr_ListAt(s_app.signals, signalId); \
    }

PR_SIG_IMPL(Pr_KeyUp,           PR_KEY_UP)
PR_SIG_IMPL(Pr_KeyDown,         PR_KEY_DOWN)
PR_SIG_IMPL(Pr_MouseButtonUp,   PR_MOUSEBUTTON_UP)
PR_SIG_IMPL(Pr_MouseButtonDown, PR_MOUSEBUTTON_DOWN)
PR_SIG_IMPL(Pr_MouseMoved,      PR_MOUSE_MOVED)
PR_SIG_IMPL(Pr_AppStarted,      PR_START)
PR_SIG_IMPL(Pr_AppQuitted,      PR_QUIT)

pr_bool_t Pr_RegisterWindow(Pr_SystemWindowRef ap_wnd)
{
    Pr_ListIterator l_it;

    if (!ap_wnd) return PR_FALSE;
    if (!s_app.wndlist) return PR_FALSE;

    PR_LIST_FOREACH(s_app.wndlist, l_it) {
        Pr_SystemWindowRef lp_tmp = Pr_ListIteratorData(l_it);
        if (lp_tmp == ap_wnd) {
            return PR_FALSE;
        }
    }

    return Pr_PushBackListData(s_app.wndlist, ap_wnd) ? PR_TRUE : PR_FALSE;
}

void Pr_UnregisterWindow(Pr_SystemWindowRef ap_wnd)
{
    Pr_ListIterator l_it;
    int l_i=0;

    if (!ap_wnd) return;

    PR_LIST_FOREACH(s_app.wndlist, l_it) {
        if (Pr_ListIteratorData(l_it) == ap_wnd) {
            Pr_PopListAt(s_app.wndlist,l_i);
            return;
        }

        l_i++;
    }
}

#define PR_SLOT_IMPL(name) void name(void * ap_obj, va_list ap_args)

void Pr_QuitApp(void)
{
    if (s_app.initialized) {
        s_app.initialized = PR_FALSE;
        Pr_Emit(Pr_AppQuitted());
    }
}

PR_SLOT_IMPL(Pr_QuitApp_Slot)
{
    Pr_QuitApp();
}


