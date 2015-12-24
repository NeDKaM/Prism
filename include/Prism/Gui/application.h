#ifndef H_PRISM_APPLICATION_INCLUDED
#define H_PRISM_APPLICATION_INCLUDED

#include <Prism/config.h>
#include <Prism/sigslot.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern PRISM_API int    Pr_InitApp(void);
    extern PRISM_API int    Pr_ExecApp(void);
    extern PRISM_API int    Pr_LoadApp(char const *);

    /**< signals >*/

#define PR_SIGNAL(name) Pr_Signal * name(void)
    extern PRISM_API PR_SIGNAL(Pr_KeyDown);         /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_KeyUp);           /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonDown); /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseButtonUp);   /**< (int) >*/
    extern PRISM_API PR_SIGNAL(Pr_MouseMoved);      /**< (int,int) >*/
    extern PRISM_API PR_SIGNAL(Pr_AppStarted);      /**< (void) >*/
#undef PR_SIGNAL

    /**< slots >*/

#define PR_SLOT(name) void name(void *, va_list)
    extern PRISM_API PR_SLOT(Pr_Slot_QuitApp);
    extern PRISM_API PR_SLOT(Pr_Slot_LoadApp);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_APPLICATION_INCLUDED */
