#include <Prism/RainbowSDL/Systems/rendersystem.h>

#include <Prism/RainbowSDL/Components/geometrycomponent.h>
#include <Prism/RainbowSDL/Components/rendercomponent.h>

#include <Prism/RainbowSDL/renderable.h>

#include <Prism/RainbowSDL/gfxwindow.h>

#include <Prism/ECS/entity.h>

#include <stdlib.h>

#ifdef PRISM_DEBUG
    static void s_Pr_RenderEntityOrigin(SDL_Renderer * ap_rnd, long a_x, long a_y)
    {
        SDL_SetRenderDrawColor(ap_rnd, 0, 255, 0, 127);
        SDL_RenderDrawLine(ap_rnd, a_x, a_y, a_x, a_y + 20);
        SDL_SetRenderDrawColor(ap_rnd, 0, 0, 255, 127);
        SDL_RenderDrawLine(ap_rnd, a_x, a_y, a_x + 20, a_y);
    }
#endif

static void s_Pr_RenderSystemCallback(Pr_System * ap_sys, float a_time)
{
    Pr_ListIterator     lp_it;
    Pr_RenderSystem *   lp_system = ap_sys->data;

    if (!lp_system->renderer) return;

    PR_LIST_FOREACH(ap_sys->entities, lp_it) {
        Pr_Entity * lp_entity = Pr_ListIteratorData(lp_it);
        Pr_GeometryComponent * lp_geometry = Pr_GetEntityComponent(lp_entity, &Pr_GeometryComponentInfo);
        Pr_RenderComponent * lp_render = Pr_GetEntityComponent(lp_entity, &Pr_RenderComponentInfo);
        Pr_Renderable * lp_renderables = Pr_GetArrayData(lp_render->renderables);
        pr_u32_t l_i;

        for (l_i=0 ; l_i<Pr_ArraySize(lp_render->renderables) ; l_i++) {
            Pr_Renderable * lp_renderable = &lp_renderables[l_i];

            SDL_Rect l_dstRect;
            if (!lp_renderable->originOnly) {
                l_dstRect.x = lp_geometry->position.x + lp_renderable->origin.x;
                l_dstRect.y = lp_geometry->position.y + lp_renderable->origin.y;
            } else {
                l_dstRect.x = lp_renderable->origin.x;
                l_dstRect.y = lp_renderable->origin.y;
            }
                l_dstRect.w = lp_renderable->dstSize.w;
                l_dstRect.h = lp_renderable->dstSize.h;

            SDL_SetRenderDrawColor(lp_system->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_SetTextureAlphaMod(lp_renderable->texture, lp_renderable->colorMod.a);
            SDL_SetTextureBlendMode(lp_renderable->texture, lp_renderable->blendMode);
            SDL_SetTextureColorMod(lp_renderable->texture, 
                lp_renderable->colorMod.r,
                lp_renderable->colorMod.g,
                lp_renderable->colorMod.b
            );

            SDL_RenderCopyEx(lp_system->renderer,
                lp_renderable->texture,
                &lp_renderable->textureCoords,
                &l_dstRect,
                lp_geometry->rotation,
                &lp_renderable->origin,
                SDL_FLIP_NONE
            );
            
            #ifdef PRISM_DEBUG
                SDL_SetRenderDrawColor(lp_system->renderer, 127, 0, 0, 127);
                SDL_RenderDrawRect(lp_system->renderer, &l_dstRect);
            #endif
        }

        #ifdef PRISM_DEBUG
            s_Pr_RenderEntityOrigin(lp_system->renderer, lp_geometry->position.x, lp_geometry->position.y);
        #endif
    }
}

static pr_bool_t s_Pr_SystemInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_RenderSystem * lp_system = ap_data;

    lp_system->renderer = (SDL_Renderer *)Pr_GetCurrentRenderer();

    return lp_system->renderer ? PR_TRUE : PR_FALSE;
}

static pr_u32_t s_requiredComponents[] = {
    2,
    PR_COMPONENT_GEOMETRY,
    PR_COMPONENT_RENDER
};

Pr_SystemInfo Pr_RenderSystemInfo = {
    s_Pr_RenderSystemCallback,
    sizeof(Pr_RenderSystem),
    s_Pr_SystemInitializer,
    NULL,
    s_requiredComponents,
    NULL
};