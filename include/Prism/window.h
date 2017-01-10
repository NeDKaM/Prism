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
#include <Prism/objects.h>
#include <Prism/sigslot.h>
#include <Prism/application.h>
#include <Prism/types.h>
#include <Prism/list.h>

PR_CPP_PROTECT_ON

    PR_OBJECT(pr_window_t, Pr_SystemWindow) {

        PR_INHERIT(Pr_Object);

        void *          sdlWindow;
        pr_u32_t        id;
        Pr_List *       signals;

        void (*getSize)(Pr_SystemWindowRef, pr_u32_t *, pr_u32_t *);

        Pr_SlotFunction(setTitle)(Pr_SystemWindowRef, pr_cstring_t);
        Pr_SlotFunction(setPosition)(Pr_SystemWindowRef, long, long);
        Pr_SlotFunction(setSize)(Pr_SystemWindowRef, pr_u32_t, pr_u32_t);
        Pr_SlotFunction(setMaximized)(Pr_SystemWindowRef);
        Pr_SlotFunction(setMinimized)(Pr_SystemWindowRef);
        Pr_SlotFunction(setBordered)(Pr_SystemWindowRef, pr_bool_t);
        Pr_SlotFunction(hide)(Pr_SystemWindowRef);
        Pr_SlotFunction(show)(Pr_SystemWindowRef);
        Pr_SlotFunction(raise)(Pr_SystemWindowRef);
        Pr_SlotFunction(display)(Pr_SystemWindowRef);
    };

    extern PRISM_API Pr_Class Pr_SystemWindowClass;
           
    extern PRISM_API PR_SIGNAL(Pr_WindowClosed)(Pr_SystemWindowRef);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMoved)(Pr_SystemWindowRef);           /**< (int, int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowSizeChanged)(Pr_SystemWindowRef);     /**< (unsigned int, unsigned int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMinimized)(Pr_SystemWindowRef);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowMaximized)(Pr_SystemWindowRef);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowShown)(Pr_SystemWindowRef);           /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowRestored)(Pr_SystemWindowRef);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowHidden)(Pr_SystemWindowRef);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFramed)(Pr_SystemWindowRef);          /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowLeaved)(Pr_SystemWindowRef);          /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowEntered)(Pr_SystemWindowRef);         /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowOnDelete)(Pr_SystemWindowRef);        /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFocusGained)(Pr_SystemWindowRef);     /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowFocusLost)(Pr_SystemWindowRef);       /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_WindowOnDisplay)(Pr_SystemWindowRef);       /**< (void) >*/

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_WINDOW_INCLUDED */


