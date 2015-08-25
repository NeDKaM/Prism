#include <SDL2/SDL.h>
#include <Prism/Gui/application.h>
#include <Prism/Gui/window.h>
#include <Prism/list.h>
#include <Prism/Gui/Internal/request.h>
#include <Prism/array.h>

static int              s_initialized   = 0;
static Pr_List *        s_wndlist       = NULL;
PR_ARRAY(Pr_Signal *,   s_signals);

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
        Pr_Signal ** lp_tmp = &s_signals.list[l_i];
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

Pr_Signal * Pr_KeyUp(void)
{
    if (!s_initialized) return NULL;

    return s_signals.list[PR_KEY_UP];
}

Pr_Signal * Pr_KeyDown(void)
{
    if (!s_initialized) return NULL;

    return s_signals.list[PR_KEY_DOWN];
}

Pr_Signal * Pr_MouseButtonUp(void)
{
    if (!s_initialized) return NULL;

    return s_signals.list[PR_MOUSEBUTTON_UP];
}

Pr_Signal * Pr_MouseButtonDown(void)
{
    if (!s_initialized) return NULL;

    return s_signals.list[PR_KEY_DOWN];
}

Pr_Signal * Pr_MouseMoved(void)
{
    if (!s_initialized) return NULL;

    return s_signals.list[PR_MOUSE_MOVED];
}

void Pr_UpdateApp(void)
{
    SDL_Event l_ev;

    if (!s_initialized) return;

    while (SDL_PollEvent(&l_ev)) {
        Pr_ListIterator l_it;

        PR_LIST_FOREACH(s_wndlist, l_it) {
            Pr_Window * lp_wnd = Pr_ListIteratorData(l_it);
            Pr_HandleWindowEvent(lp_wnd,&l_ev);
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

void Pr_Slot_QuitApp(va_list ap_args)
{
    int l_i;

    (void)ap_args;

    if (!s_initialized) return;

    while (Pr_ListSize(s_wndlist)) {
        Pr_DeleteWindow(Pr_FrontList(s_wndlist));
    }

    Pr_DeleteList(s_wndlist);

    for (l_i = 0; l_i < PR_SIGNALS_COUNT; l_i++) {
        Pr_DeleteSignal(s_signals.list[l_i]);
    }

    Pr_ClearArray(s_signals);

    SDL_Quit();

    s_initialized = 0;
    s_wndlist = NULL;
}

void Pr_QuitApp(void)
{
    Pr_Slot_QuitApp(NULL);
}

