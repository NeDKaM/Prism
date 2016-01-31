/* File name : window.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism widnow implementation
 * Dependencies :   <Prism/config.h>,
                    <Prism/sigslot.h>,
                    <Prism/application.h>
*/

#ifndef H_PRISM_WINDOW_INCLUDED
#define H_PRISM_WINDOW_INCLUDED

#include <Prism/config.h>
#include <Prism/sigslot.h>
#include <Prism/application.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct pr_window_t      Pr_Window;

    extern PRISM_API Pr_Window *    Pr_NewWindow(void);
    extern PRISM_API void           Pr_DeleteWindow(Pr_Window *);
    extern PRISM_API unsigned long  Pr_GetWindowId(Pr_Window *);

    extern PRISM_API void           Pr_RenderClear(Pr_Window *);
    extern PRISM_API void           Pr_RenderDisplay(Pr_Window *);
    extern PRISM_API void           Pr_SetRenderDrawColor(Pr_Window *, int, int, int, int);

    extern PRISM_API int            Pr_RegisterWindow(Pr_Window *);
    extern PRISM_API void           Pr_UnregisterWindow(Pr_Window *);

    /**< signals >*/               
    
    extern PRISM_API PR_SIGNAL(Pr_WindowClosed)(Pr_Window *);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMoved)(Pr_Window *);           /**< (int, int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowSizeChanged)(Pr_Window *);     /**< (unsigned int, unsigned int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMinimized)(Pr_Window *);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMaximized)(Pr_Window *);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowShown)(Pr_Window *);           /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowRestored)(Pr_Window *);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowHidden)(Pr_Window *);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFramed)(Pr_Window *);          /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowUpdated)(Pr_Window *);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowPainted)(Pr_Window *);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowLeaved)(Pr_Window *);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowEntered)(Pr_Window *);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowDeleted)(Pr_Window *);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFocusGained)(Pr_Window *);     /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFocusLost)(Pr_Window *);       /**< (void) >*/

    /**< slots >*/
     
    PR_SLOT_EXTERN_API(Pr_SetWindowTitle)(Pr_Window *, char const *); 
    PR_SLOT_EXTERN_API(Pr_CloseWindow)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_SetWindowPosition)(Pr_Window *, int, int);
    PR_SLOT_EXTERN_API(Pr_SetWindowSize)(Pr_Window *, unsigned int, unsigned int);
    PR_SLOT_EXTERN_API(Pr_SetWindowMinimized)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_SetWindowMaximized)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_SetWindowFramed)(Pr_Window *, int);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_WINDOW_INCLUDED */
