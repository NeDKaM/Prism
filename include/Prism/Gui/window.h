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
    extern PRISM_API Pr_Renderer *  Pr_GetWindowRenderer(Pr_Window *);
    extern PRISM_API unsigned long  Pr_GetWindowId(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowTitle(Pr_Window *, char const *);
    extern PRISM_API void           Pr_SetWindowPosition(Pr_Window *, int, int);
    extern PRISM_API void           Pr_SetWindowSize(Pr_Window *, unsigned int, unsigned int);
    extern PRISM_API void           Pr_CloseWindow(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowMinimized(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowMaximized(Pr_Window *);
    extern PRISM_API void           Pr_SetWindowFramed(Pr_Window *, int);
    extern PRISM_API void           Pr_HideWindow(Pr_Window *);
    extern PRISM_API void           Pr_ShowWindow(Pr_Window *);

    /* PRIVATE */ extern PRISM_API void Pr_HandleWindowEvent(Pr_Window *, void *);

    /**< signals >*/               

#define PR_SIGNAL(name) Pr_Signal * name(Pr_Window *)
    extern PRISM_API PR_SIGNAL(Pr_WindowClosed);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMoved);         /**< (int, int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowSizeChanged);   /**< (unsigned int, unsigned int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMinimized);     /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMaximized);     /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowShown);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowRestored);      /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowHidden);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFramed);        /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowUpdated);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowPainted);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowLeaved);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowEntered);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowDeleted);       /**< (void) >*/

    /**< slots >*/

#define PR_SLOT(name) void name(void *, va_list)
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowTitle);
    extern PRISM_API PR_SLOT(Pr_Slot_CloseWindow);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowPosition);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowSize);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowMinimized);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowMaximized);
    extern PRISM_API PR_SLOT(Pr_Slot_SetWindowFramed);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_WINDOW_INCLUDED */
