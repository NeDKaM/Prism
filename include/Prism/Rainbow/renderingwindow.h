#ifndef H_RAINBOW_RENDERINGWINDOW_INCLUDED
#define H_RAINBOW_RENDERINGWINDOW_INCLUDED

#include <Prism/window.h>
#include <Prism/Rainbow/renderingtarget.h>
#include <Prism/Rainbow/contextsettings.h>

PR_CPP_PROTECT_ON

    PR_OBJECT(pr_renderwindow_t, Pr_RenderWindow) {
    
        PR_INHERIT(Pr_SystemWindow);

        Pr_RenderingTarget  renderTarget;
        void *              sdlglContext;
        Pr_Color            clearColor;
    };

    extern PRISM_API Pr_Class Pr_RenderWindowClass;

    extern PRISM_API void   Pr_ClearRenderWindow(Pr_RenderWindowRef);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_RENDERINGWINDOW_INCLUDED */
