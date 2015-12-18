#include <SDL2/SDL.h>
#include <Prism/Gui/application.h>
#include <Prism/Gui/window.h>
#include <Prism/list.h>
#include <Prism/Gui/Internal/request.h>
#include <Prism/array.h>
#include <Prism/string.h>
#include <stdio.h>
#include <Prism/Gui/class.h>

static int              s_initialized   = 0;
static Pr_List *        s_wndlist       = NULL;
PR_ARRAY(Pr_Signal *)   s_signals;

enum {
    PR_QUIT = 0,

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

    Pr_MakeSizedArray(s_signals, PR_SIGNALS_COUNT);

    if (!s_signals.size) return 0;

    for (l_i = 0; l_i<PR_SIGNALS_COUNT; l_i++) {
        Pr_Signal ** lp_tmp = & Pr_ArrayAt(s_signals, l_i);
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

static void s_Pr_UpdateApp(void)
{
    SDL_Event l_ev;

    while (SDL_PollEvent(&l_ev)) {
        Pr_ListIterator l_it;

        PR_LIST_FOREACH(s_wndlist, l_it) {
            Pr_Window * lp_wnd = Pr_ListIteratorData(l_it);
            Pr_HandleWindowEvent(lp_wnd, &l_ev);
            Pr_Emit(Pr_WindowUpdated(lp_wnd));
        }

        switch (l_ev.type) {
            case SDL_MOUSEMOTION:
                Pr_Emit(Pr_MouseMoved(), l_ev.motion.x, l_ev.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                Pr_Emit(Pr_MouseButtonDown(), l_ev.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                Pr_Emit(Pr_MouseButtonUp(), l_ev.button.button);
                break;
            case SDL_MOUSEWHEEL:
                break;
            case SDL_KEYDOWN:
                Pr_Emit(Pr_KeyDown(), l_ev.key.keysym.sym);
                break;
            case SDL_KEYUP:
                Pr_Emit(Pr_KeyUp(), l_ev.key.keysym.sym);
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
    unsigned int l_i;

    while (Pr_ListSize(s_wndlist)) {
        Pr_DeleteWindow(Pr_FrontList(s_wndlist));
    }

    Pr_DeleteList(s_wndlist);

    for (l_i = 0; l_i < Pr_ArraySize(s_signals); l_i++) {
        Pr_DeleteSignal(Pr_ArrayAt(s_signals,l_i));
    }

    Pr_ClearArray(s_signals);

    SDL_Quit();

    s_initialized = 0;
    s_wndlist = NULL;
}

int Pr_ExecApp(void)
{
    if (!s_initialized) return -1;

    while (s_initialized) {
        s_Pr_UpdateApp();
    }

    s_Pr_QuitApp();

    return 0;
}

int Pr_InitApp(void)
{
	if (s_initialized) return 1;

	if (SDL_Init(SDL_INIT_VIDEO)) return 0;

    s_wndlist = Pr_NewList();

    if (s_wndlist && s_Pr_CreateSignals()) {
	    s_initialized = 1;
        return 1;
    }

    Pr_DeleteList(s_wndlist);
    Pr_ClearArray(s_signals);

    SDL_Quit();

	return 0;
}

int Pr_LoadApp(char const * ap_code)
{
    Pr_String * lp_code;

    if (!ap_code) return 0;
    if (!ap_code[0]) return 0;

    lp_code = Pr_NewStringStr(ap_code);
    if (!lp_code) return 0;

    Pr_StringReplace(lp_code,  " ", "");
    Pr_StringReplace(lp_code, "\n", "");
    Pr_StringReplace(lp_code, "\t", "");
    puts(Pr_StringCStr(lp_code));

    /**< should load a file like this one >*/
    /**<
            [hydrogen]

            PrApplication : app {
                PrWindow : wnd {
                    background  = 0xffffff;
                    width       = 640;
                    height      = 480;
                    title       = "Main application";

                    PrButton : b_info {
                        onclick -> window2 . show;
                        text = "Show";
                    }

                    PrButton : b_quit {
                        onclick -> app . quit;
                        text = "Quit";
                    }
                }

                PrWindow : window2 {
                    x           = 50;
                    y           = 50;
                    width       = wnd . width;
                    title       = "New window";
                    background  = 0x0;
                }

                PrButton : b_close {
                    width   = 150;
                    height  = 25;
                    owner   = window2;
                    onclick -> window2 . close;
                    text = "Close";
                }
            }
    >*/

    Pr_DeleteString(lp_code);

    return 0;
}

#define PR_SIG_IMPL(name,value) \
    Pr_Signal * name(void) \
    { \
        if (!s_initialized) return NULL; \
        return Pr_ArrayAt(s_signals,value); \
    }

PR_SIG_IMPL(Pr_KeyUp, PR_KEY_UP)
PR_SIG_IMPL(Pr_KeyDown, PR_KEY_DOWN)
PR_SIG_IMPL(Pr_MouseButtonUp, PR_MOUSEBUTTON_UP)
PR_SIG_IMPL(Pr_MouseButtonDown, PR_MOUSEBUTTON_DOWN)
PR_SIG_IMPL(Pr_MouseMoved, PR_MOUSE_MOVED)

int Pr_Request_NewWindow(Pr_Window * ap_wnd)
{
    Pr_ListIterator l_it;

    if (!ap_wnd) return 0;
    if (!s_wndlist) return 0;

    PR_LIST_FOREACH(s_wndlist, l_it) {
        Pr_Window * lp_tmp = Pr_ListIteratorData(l_it);
        if (lp_tmp == ap_wnd) {
            return 0;
        }
    }

    return Pr_PushBackListData(s_wndlist,ap_wnd);
}

int Pr_Request_DeleteWindow(Pr_Window * ap_wnd)
{
    Pr_ListIterator l_it;
    int l_i=0;

    if (!ap_wnd) return 0;
    if (!s_wndlist) return 0;

    PR_LIST_FOREACH(s_wndlist, l_it) {
        if (Pr_ListIteratorData(l_it) == ap_wnd) {
            Pr_PopListAt(s_wndlist,l_i);
            return 1;
        }

        l_i++;
    }

    return 0;
}

#define PR_SLOT_IMPL(name) void name(void * ap_obj, va_list ap_args)

PR_SLOT_IMPL(Pr_Slot_QuitApp)
{
    s_initialized = 0;
}




