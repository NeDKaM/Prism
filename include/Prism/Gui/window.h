#ifndef H_PRISM_WINDOW_INCLUDED
#define H_PRISM_WINDOW_INCLUDED

#include <Prism/config.h>
#include <Prism/sigslot.h>
#include <Prism/Gui/renderer.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct pr_window_t Pr_Window;

    extern PRISM_API Pr_Window *    Pr_NewWindow(void);
    extern PRISM_API void           Pr_DeleteWindow(Pr_Window *);
    extern PRISM_API int            Pr_AttachRenderer(Pr_Window *, Pr_Renderer *);
    extern PRISM_API unsigned long  Pr_GetWindowId(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowTitle(Pr_Window *, char const *);
    extern PRISM_API void           Pr_SetWindowPosition(Pr_Window *, int, int);
    extern PRISM_API void           Pr_SetWindowSize(Pr_Window *, unsigned int, unsigned int);
    extern PRISM_API void           Pr_CloseWindow(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowMinimized(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowMaximized(Pr_Window *);

    /* PRIVATE */ extern PRISM_API void Pr_HandleWindowEvent(Pr_Window *, void *);

    /**< signals >*/

    extern PRISM_API Pr_Signal *    Pr_WindowClosed(Pr_Window *);
    /**<                            (void) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowMoved(Pr_Window *);
    /**<                            (int, int) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowSizeChanged(Pr_Window *);
    /**<                            (unsigned int, unsigned int) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowMinimized(Pr_Window *);
    /**<                            (void) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowMaximized(Pr_Window *);
    /**<                            (void) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowShown(Pr_Window *);
    /**<                            (void) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowRestored(Pr_Window *);
    /**</                           (void) >*/
    extern PRISM_API Pr_Signal *    Pr_WindowHidden(Pr_Window *);
    /**<                            (void) >*/       

    /**< slots >*/

#define PR_SLOT(name) void name(void *, va_list)
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowTitle);
    extern PRISM_API PR_SLOT(Pr_Slot_CloseWindow);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowPosition);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowSize);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowMinimized);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowMaximized);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_WINDOW_INCLUDED */