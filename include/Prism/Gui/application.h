#ifndef H_PRISM_APPLICATION_INCLUDED
#define H_PRISM_APPLICATION_INCLUDED

#include <Prism/config.h>
#include <Prism/sigslot.h>

#ifdef __cplusplus
extern "C"
{
#endif

	extern PRISM_API int            Pr_InitApp(void);
	extern PRISM_API void           Pr_UpdateApp(void);
    extern PRISM_API void           Pr_QuitApp(void);

    /**< signals >*/

    extern PRISM_API Pr_Signal *    Pr_KeyDown(void);
    /**<                            (int) >*/
    extern PRISM_API Pr_Signal *    Pr_KeyUp(void);
    /**<                            (int) >*/
    extern PRISM_API Pr_Signal *    Pr_MouseButtonDown(void);
    /**<                            (int) >*/
    extern PRISM_API Pr_Signal *    Pr_MouseButtonUp(void);
    /**<                            (int) >*/
    extern PRISM_API Pr_Signal *    Pr_MouseMoved(void);
    /**<                            (int, int) >*/

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_APPLICATION_INCLUDED */
