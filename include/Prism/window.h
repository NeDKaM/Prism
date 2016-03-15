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
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_window_t, Pr_Window);

    extern PRISM_API Pr_Window *    Pr_NewWindow(void);
    extern PRISM_API void           Pr_DeleteWindow(Pr_Window *);
    extern PRISM_API unsigned long  Pr_GetWindowId(Pr_WindowRef);
    extern PRISM_API pr_bool_t      Pr_GetWindowSize(Pr_WindowRef, pr_u32_t *, pr_u32_t *);

    /**< signals >*/               
    
    extern PRISM_API PR_SIGNAL(Pr_WindowClosed)(Pr_WindowRef);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMoved)(Pr_WindowRef);           /**< (int, int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowSizeChanged)(Pr_WindowRef);     /**< (unsigned int, unsigned int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMinimized)(Pr_WindowRef);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMaximized)(Pr_WindowRef);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowShown)(Pr_WindowRef);           /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowRestored)(Pr_WindowRef);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowHidden)(Pr_WindowRef);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFramed)(Pr_WindowRef);          /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowUpdated)(Pr_WindowRef);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowPainted)(Pr_WindowRef);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowLeaved)(Pr_WindowRef);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowEntered)(Pr_WindowRef);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowDeleted)(Pr_WindowRef);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFocusGained)(Pr_WindowRef);     /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFocusLost)(Pr_WindowRef);       /**< (void) >*/

    /**< slots >*/
     
    PR_SLOT_EXTERN_API(Pr_SetWindowTitle)(Pr_Window *, char const *); 
    PR_SLOT_EXTERN_API(Pr_CloseWindow)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_SetWindowPosition)(Pr_Window *, int, int);
    PR_SLOT_EXTERN_API(Pr_SetWindowSize)(Pr_Window *, unsigned int, unsigned int);
    PR_SLOT_EXTERN_API(Pr_SetWindowMinimized)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_SetWindowMaximized)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_SetWindowFramed)(Pr_Window *, int);
    PR_SLOT_EXTERN_API(Pr_HideWindow)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_ShowWindow)(Pr_Window *);
    PR_SLOT_EXTERN_API(Pr_RaiseWindow)(Pr_Window *);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_WINDOW_INCLUDED */
