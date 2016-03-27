#ifndef H_RAINBOW_RENDERINGWINDOW_INCLUDED
#define H_RAINBOW_RENDERINGWINDOW_INCLUDED

#include <Prism/config.h>
#include <Prism/window.h>
#include <Prism/Rainbow/renderingtarget.h>
#include <Prism/Rainbow/contextsettings.h>
#include <Prism/sigslot.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_renderingwindow_t, Pr_RenderingWindow);

    extern PRISM_API Pr_RenderingWindow *   Pr_NewRndWindow(Pr_ContextSettingsRef);
    extern PRISM_API void                   Pr_DeleteRndWindow(Pr_RenderingWindow *);
    extern PRISM_API Pr_WindowRef           Pr_GetRndWindowRef(Pr_RenderingWindowRef);
    extern PRISM_API Pr_RenderingTargetRef  Pr_GetRndWindowTargetRef(Pr_RenderingWindowRef);
    
    PR_SLOT_EXTERN_API(Pr_ClearRndWindow)(Pr_RenderingWindow *, Pr_ColorRef);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_RENDERINGWINDOW_INCLUDED */
