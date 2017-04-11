/* File name : application.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism main application implementation
 * Dependencies :   <Prism/config.h>,
                    <Prism/sigslot.h>,
                    <Prism/library.h>
*/

#ifndef H_PRISM_APPLICATION_INCLUDED
#define H_PRISM_APPLICATION_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/sigslot.h>
#include <Prism/window.h>

PR_CPP_PROTECT_ON

    extern PRISM_API pr_bool_t  Pr_InitApp(void);
    extern PRISM_API int        Pr_ExecApp(void);
    extern PRISM_API pr_bool_t  Pr_RegisterWindow(Pr_SystemWindowRef);
    extern PRISM_API void       Pr_UnregisterWindow(Pr_SystemWindowRef);

    /**< signals >*/

    extern PRISM_API PR_SIGNAL(Pr_KeyDown)(void);         /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_KeyUp)(void);           /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonDown)(void); /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonUp)(void);   /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseMoved)(void);      /**< (int, int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonDown)(void); /**< (int, int x, int y) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonUp)(void);   /**< (int, int x, int y) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseMoved)(void);      /**< (int x, int y) >*/
    extern PRISM_API PR_SIGNAL(Pr_AppStarted)(void);      /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_AppQuitted(void));      /**< (void) >*/

    /**< slots >*/

    PR_SLOT_EXTERN_API(Pr_QuitApp)(void);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_APPLICATION_INCLUDED */
