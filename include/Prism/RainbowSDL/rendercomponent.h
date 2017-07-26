#ifndef H_RAINBOWSDL_RENDERCOMPONENT_INCLUDED
#define H_RAINBOWSDL_RENDERCOMPONENT_INCLUDED

#include <Prism/config.h>

#include <Prism/ECS/component.h>
#include <Prism/RainbowSDL/renderable.h>

#include <Prism/memory.h>

PR_CPP_PROTECT_ON

    #define PR_RENDERCOMPONENT /**/ 1 /**/

    extern PRISM_API Pr_ComponentInfo Pr_RenderComponentInfo;

    PR_STRUCT(pr_rendercomponent_t, Pr_RenderComponent) {
        Pr_Array *  renderables;
    };

    extern PRISM_API pr_bool_t  Pr_AttachRenderable(Pr_RenderComponent *, Pr_Renderable *);

PR_CPP_PROTECT_OFF

#endif

/*


*/
