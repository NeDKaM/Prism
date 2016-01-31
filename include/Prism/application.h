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
#include <Prism/sigslot.h>
#include <Prism/library.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern PRISM_API int    Pr_InitApp(void);
    extern PRISM_API int    Pr_ExecApp(void);
    extern PRISM_API int    Pr_ImportLibApp(Pr_Library *);

    /**< signals >*/

    extern PRISM_API PR_SIGNAL(Pr_KeyDown)(void);         /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_KeyUp)(void);           /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonDown)(void); /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonUp)(void);   /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseMoved)(void);      /**< (int,int) >*/
    extern PRISM_API PR_SIGNAL(Pr_AppStarted)(void);      /**< (void) >*/
    extern PRISM_API PR_SIGNAL(Pr_AppQuitted(void));      /**< (void) >*/

    /**< slots >*/

    PR_SLOT_EXTERN_API(Pr_QuitApp)(void);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_APPLICATION_INCLUDED */
