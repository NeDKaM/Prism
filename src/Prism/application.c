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
#include <Prism/var.h>
#include <stdio.h>
#include <Prism/logger.h>

struct {
    int initialized;
    Pr_List * wndlist;
    Pr_Library * library;
    Pr_Array(Pr_Signal *) signals;
    SDL_Event input;
    Pr_Logger * log;
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

static int s_Pr_CreateSignals(void)
{
    int l_i;

    Pr_MakeSizedArray(s_app.signals, PR_SIGNALS_COUNT);

    if (!s_app.signals.size) return 0;

    for (l_i = 0; l_i<PR_SIGNALS_COUNT; l_i++) {
        Pr_Signal ** lp_tmp = & Pr_ArrayAt(s_app.signals, l_i);
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

void s_Pr_HandleWindowEvent(Pr_Window * ap_wnd)
{
    if (!ap_wnd) return;

    if (s_app.input.type == SDL_WINDOWEVENT && s_app.input.window.windowID == Pr_GetWindowId(ap_wnd)) {
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
    while (SDL_PollEvent(&s_app.input)) {
        Pr_ListIterator l_it;

        PR_LIST_FOREACH(s_app.wndlist, l_it) {
            Pr_Window * lp_wnd = Pr_ListIteratorData(l_it);
            s_Pr_HandleWindowEvent(lp_wnd);
            Pr_Emit(Pr_WindowUpdated(lp_wnd));
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
}

static void s_Pr_QuitApp(void)
{
    unsigned int l_i = 0;

    Pr_DeleteLogger(s_app.log);

    while (Pr_ListSize(s_app.wndlist)) {
        Pr_DeleteWindow(Pr_FrontList(s_app.wndlist));
        Pr_PopFrontList(s_app.wndlist);
    }

    Pr_DeleteList(s_app.wndlist);

    for (l_i = 0 ; l_i < Pr_ArraySize(s_app.signals) ; l_i++) {
        Pr_DeleteSignal(Pr_ArrayAt(s_app.signals, l_i));
    }

    Pr_ClearArray(s_app.signals);

    SDL_Quit();

    s_app.initialized = 0;
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

static void * s_Pr_IntegerCons(void)
{
    return calloc(1,sizeof(long));
}

static void s_Pr_IntergerDel(void * ap_that)
{
    if (!ap_that) return;
    free(ap_that);
}

/*

static void s_Pr_StringDel(void * ap_that)
{
    if (!ap_that) return;
    Pr_DeleteString(ap_that);
}

static void s_Pr_SignalDel(void * ap_that)
{
    if (!ap_that) return;
    Pr_DeleteSignal(ap_that);
}

*/

static int s_Pr_LoadStdLibrary(void)
{
    int         l_err = 0;
    Pr_Class *  lp_integerCls;
    Pr_Class *  lp_appCls;
    Pr_Class *  lp_stringCls;
    Pr_Class *  lp_signalCls;
    Pr_Class *  lp_windowCls;
    
    s_app.library = Pr_NewLibrary("PrAppLib");

    if (!s_app.library) return 0;

    lp_integerCls = Pr_NewClass("PrInteger",s_Pr_IntegerCons,s_Pr_IntergerDel,NULL);
    if (!Pr_RegisterClass(s_app.library,lp_integerCls)) l_err++;

    lp_stringCls = Pr_NewClass("PrString",Pr_NewString,Pr_DeleteString,NULL);
    if (!Pr_RegisterClass(s_app.library,lp_stringCls)) l_err++;

    lp_signalCls = Pr_NewClass("PrSignal",Pr_NewSignal,Pr_DeleteSignal,NULL);
    if (!Pr_RegisterClass(s_app.library,lp_signalCls)) l_err++;

#define PR_PARAMETER_CHECK(maincls,cls,s_var,i) if (!Pr_SetClassParameter((maincls), (cls), (s_var))) (i)++

    lp_appCls = Pr_NewClass("PrApplication",NULL,NULL,NULL);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "started", l_err);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "quitted", l_err);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "keyDown", l_err);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "keyUp", l_err);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "mouseButtonDown", l_err);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "mouseButtonUp", l_err);
    PR_PARAMETER_CHECK(lp_appCls, lp_signalCls, "mouseMoved", l_err);
    if (!Pr_RegisterClass(s_app.library,lp_appCls)) l_err++;

    lp_windowCls = Pr_NewClass("PrWindow",Pr_NewWindow,Pr_DeleteWindow,NULL);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "closed", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "moved", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "updated", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "painted", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "entered", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "leaved", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "sizeChanged", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "framed", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "minimized", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "maximized", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "hidden", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "restored", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "shown", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_signalCls, "deleted", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_stringCls, "title", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_integerCls, "width", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_integerCls, "height", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_integerCls, "x", l_err);
    PR_PARAMETER_CHECK(lp_windowCls, lp_integerCls, "y", l_err);
    if (!Pr_RegisterClass(s_app.library,lp_windowCls)) l_err++;

#undef PR_PARAMETER_CHECK

    if (!l_err) return 1;

#define PR_REGISTER_CHECK(cls) if (!Pr_GetClassId(cls)) Pr_DeleteClass(cls)
    PR_REGISTER_CHECK(lp_integerCls);
    PR_REGISTER_CHECK(lp_stringCls);
    PR_REGISTER_CHECK(lp_signalCls);
    PR_REGISTER_CHECK(lp_appCls);
    PR_REGISTER_CHECK(lp_windowCls);
#undef PR_REGISTER_CHECK
    
    return 0;
}

int Pr_InitApp(void)
{
    int l_retSig;
    int l_retLib;

	if (s_app.initialized) return 1;

    s_app.log = Pr_NewLogger();

	if (SDL_Init(SDL_INIT_VIDEO)) return 0;

    s_app.wndlist = Pr_NewList();
    l_retSig = s_Pr_CreateSignals();
    l_retLib = s_Pr_LoadStdLibrary();

    if (s_app.wndlist && l_retSig && l_retLib) {
	    s_app.initialized = 1;
        return 1;
    }

    Pr_DeleteLogger(s_app.log);
    Pr_DeleteList(s_app.wndlist);
    s_app.wndlist = NULL;
    Pr_ClearArray(s_app.signals);
    Pr_DeleteLibrary(s_app.library);
    s_app.library = NULL;

    SDL_Quit();

	return 0;
}

#define PR_SIG_IMPL(name,value) \
    Pr_Signal * name(void) \
    { \
        if (!s_app.initialized) return NULL; \
        return Pr_ArrayAt(s_app.signals,value); \
    }

PR_SIG_IMPL(Pr_KeyUp, PR_KEY_UP)
PR_SIG_IMPL(Pr_KeyDown, PR_KEY_DOWN)
PR_SIG_IMPL(Pr_MouseButtonUp, PR_MOUSEBUTTON_UP)
PR_SIG_IMPL(Pr_MouseButtonDown, PR_MOUSEBUTTON_DOWN)
PR_SIG_IMPL(Pr_MouseMoved, PR_MOUSE_MOVED)
PR_SIG_IMPL(Pr_AppStarted, PR_START)
PR_SIG_IMPL(Pr_AppQuitted, PR_QUIT)

int Pr_RegisterWindow(Pr_Window * ap_wnd)
{
    Pr_ListIterator l_it;

    if (!ap_wnd) return 0;
    if (!s_app.wndlist) return 0;

    PR_LIST_FOREACH(s_app.wndlist, l_it) {
        Pr_Window * lp_tmp = Pr_ListIteratorData(l_it);
        if (lp_tmp == ap_wnd) {
            return 0;
        }
    }

    return Pr_PushBackListData(s_app.wndlist,ap_wnd);
}

void Pr_UnregisterWindow(Pr_Window * ap_wnd)
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
        Pr_QuitApp_Slot(NULL,NULL);
    }
}

PR_SLOT_IMPL(Pr_QuitApp_Slot)
{
    s_app.initialized = 0;

    Pr_Emit(Pr_AppQuitted());
}

int Pr_ImportLibApp(Pr_Library * ap_lib)
{
    if (!ap_lib) return 0;

    return Pr_ImportLibrary(ap_lib,s_app.library);
}

