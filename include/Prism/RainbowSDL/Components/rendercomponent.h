#ifndef H_RAINBOWSDL_RENDERCOMPONENT_INCLUDED
#define H_RAINBOWSDL_RENDERCOMPONENT_INCLUDED

#include <Prism/config.h>

#include <Prism/ECS/component.h>
#include <Prism/RainbowSDL/renderable.h>

#include <Prism/memory.h>

PR_CPP_PROTECT_ON

    #define PR_COMPONENT_RENDER /**/ 1 /**/

    extern PRISM_API Pr_ComponentInfo Pr_RenderComponentInfo;

    PR_STRUCT(pr_rendercomponent_t, Pr_RenderComponent) {
        Pr_Array *      renderables;

        Pr_Renderable * renderableList;
        pr_u32_t        listSize;
    };

    extern PRISM_API Pr_Renderable *    Pr_AttachRenderable(Pr_RenderComponent *, Pr_Renderable *);
    extern PRISM_API Pr_Renderable *    Pr_AttachRenderableList(Pr_RenderComponent *, Pr_Renderable *, pr_u32_t);

    extern PRISM_API void               Pr_ClearComponentRenderables(Pr_RenderComponent *);

PR_CPP_PROTECT_OFF

#endif

/*


*/
