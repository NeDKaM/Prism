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
#include <Prism/string.h>
#include <stdio.h>
#include <Prism/logger.h>
#include <string.h>

enum {
    QUIT,
    START,
    MOUSEMOVED,
    MOUSEBUTTONDOWN,
    MOUSEBUTTONUP,
    KEYUP,
    KEYDOWN,
    TEXTINPUT,

    SIGNAL_COUNT
};

struct {
    pr_bool_t   initialized;
    Pr_List *   wndlist;
    Pr_Logger * log;
    Pr_Signal * signals[SIGNAL_COUNT];
    SDL_Event   input;
} static s_app = { 0, NULL, NULL, NULL};

static void s_Pr_DeleteSignalList(void)
{
    pr_u32_t l_i;

    for (l_i = 0 ; l_i < SIGNAL_COUNT ; l_i++) {
        Pr_DeleteSignal(s_app.signals[l_i]);
    }
}

static pr_bool_t s_Pr_CreateSignals(void)
{
    pr_u32_t l_i;

    /* make sure... */
    memset(s_app.signals, 0, SIGNAL_COUNT * sizeof(Pr_Signal *));

    for (l_i = 0 ; l_i < SIGNAL_COUNT ; l_i++) {
        s_app.signals[l_i] = Pr_NewSignal();
        if (!s_app.signals[l_i]) {
            s_Pr_DeleteSignalList();
            return PR_FALSE;
        }
    }

    return PR_TRUE;
}

static void s_Pr_HandleWindowEvent(Pr_SystemWindow * ap_wnd)
{
    if (!ap_wnd) return;

    if (s_app.input.type == SDL_WINDOWEVENT && s_app.input.window.windowID == ap_wnd->id) {
        switch (s_app.input.window.event) {
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
                s_app.input.window.data1, s_app.input.window.data2
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
                Pr_Emit(Pr_MouseButtonDown(), 
                    s_app.input.button.button, 
                    s_app.input.button.x,
                    s_app.input.button.y
                );
                break;
            case SDL_MOUSEBUTTONUP:
                Pr_Emit(Pr_MouseButtonUp(), 
                    s_app.input.button.button,
                    s_app.input.button.x,
                    s_app.input.button.y
                );
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
                /* Pr_Emit(Pr_TextInput(), s_app.input.text.text); */
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
        Pr_Delete(Pr_ListIteratorData(Pr_ListBegin(s_app.wndlist)));
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
    if (!s_app.log) return PR_FALSE;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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

Pr_Logger * Pr_GetAppLog(void)
{
    if (!s_app.initialized) return NULL;

    return s_app.log;
}

#define PR_SIG_IMPL(name, signalId) \
    Pr_Signal * name(void) \
    { \
        if (!s_app.initialized) return NULL; \
        return s_app.signals[signalId]; \
    }

PR_SIG_IMPL(Pr_KeyUp,           KEYUP)
PR_SIG_IMPL(Pr_KeyDown,         KEYDOWN)
PR_SIG_IMPL(Pr_MouseButtonUp,   MOUSEBUTTONUP)
PR_SIG_IMPL(Pr_MouseButtonDown, MOUSEBUTTONDOWN)
PR_SIG_IMPL(Pr_MouseMoved,      MOUSEMOVED)
PR_SIG_IMPL(Pr_AppStarted,      START)
PR_SIG_IMPL(Pr_AppQuitted,      QUIT)
PR_SIG_IMPL(Pr_TextInput,       TEXTINPUT)

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

    return Pr_PushBackList(s_app.wndlist, ap_wnd) ? PR_TRUE : PR_FALSE;
}

void Pr_UnregisterWindow(Pr_SystemWindowRef ap_wnd)
{
    Pr_ListIterator l_it;

    if (!ap_wnd) return;

    PR_LIST_FOREACH(s_app.wndlist, l_it) {
        if (Pr_ListIteratorData(l_it) == ap_wnd) {
            Pr_EraseListElement(l_it);
            return;
        }
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


