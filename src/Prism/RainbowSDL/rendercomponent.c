#include <Prism/RainbowSDL/rendercomponent.h>

static pr_bool_t s_Pr_RenderableInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_Renderable * lp_renderable = ap_data;
    SDL_Rect l_zero = { 0, 0, 0, 0 };

    lp_renderable->texture          = NULL;
    lp_renderable->textureCoords    = l_zero;

    lp_renderable->origin.x = 0;
    lp_renderable->origin.y = 0;

    return PR_TRUE;
}

static pr_bool_t s_Pr_ComponentInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_RenderComponent * lp_render = ap_data;

    lp_render->renderables = Pr_NewArray(sizeof(Pr_Renderable), s_Pr_RenderableInitializer);
    if (!lp_render->renderables) return PR_FALSE;

    return PR_TRUE;
}

static void s_Pr_ComponentDeleter(void * ap_data)
{
    Pr_RenderComponent * lp_component = ap_data;

    if (!lp_component) return;

    Pr_DeleteArray(lp_component->renderables);
}

Pr_ComponentInfo Pr_RenderComponentInfo = {
    PR_RENDERCOMPONENT,
    s_Pr_ComponentInitializer,
    s_Pr_ComponentDeleter,
    sizeof(Pr_RenderComponent)
};

Pr_Renderable * Pr_AttachRenderable(Pr_RenderComponent * ap_comp, Pr_Renderable * ap_rnd)
{
    if (!ap_comp || !ap_rnd) return PR_FALSE;

    return Pr_SetArrayAt(ap_comp->renderables, Pr_ArraySize(ap_comp->renderables), ap_rnd);
}
