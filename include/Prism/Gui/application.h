#ifndef H_PRISM_APPLICATION_INCLUDED
#define H_PRISM_APPLICATION_INCLUDED

#include <Prism/config.h>
#include <Prism/sigslot.h>

#ifdef __cplusplus
extern "C"
{
#endif

	extern PRISM_API int    Pr_InitApp(void);
	extern PRISM_API void   Pr_QuitApp(void);
	extern PRISM_API void   Pr_UpdateApp(void);

	/**< slots >*/
	extern PRISM_API void   Pr_Slot_QuitApp(va_list ap_args);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_APPLICATION_INCLUDED */
