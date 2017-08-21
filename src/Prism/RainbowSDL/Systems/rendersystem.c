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

static void s_Pr_DrawRenderable(Pr_Renderable * ap_it, Pr_RenderSystem * ap_sys, Pr_GeometryComponent * ap_g)
{
    SDL_Rect l_dstRect;

    if (ap_it->shape.type == PR_SHAPE_NONE) return;

    SDL_SetRenderDrawColor(
        ap_sys->renderer, 
        ap_it->colorMod.r, ap_it->colorMod.g, ap_it->colorMod.b, 
        ap_it->colorMod.a
    );
    SDL_SetRenderDrawBlendMode(ap_sys->renderer, ap_it->blendMode);

    if (ap_it->shape.type == PR_SHAPE_LINE) {
        SDL_Point p1 = ap_it->shape.data.line.start;
        SDL_Point p2 = ap_it->shape.data.line.end;
        SDL_RenderDrawLine(ap_sys->renderer, p1.x, p1.y, p2.x, p2.y);
        return;
    }

    if (!ap_it->originOnly) {
        l_dstRect.x = ap_g->position.x + ap_it->origin.x;
        l_dstRect.y = ap_g->position.y + ap_it->origin.y;
    } else {
        l_dstRect.x = ap_it->origin.x;
        l_dstRect.y = ap_it->origin.y;
    }
        l_dstRect.w = ap_it->shape.data.size.w;
        l_dstRect.h = ap_it->shape.data.size.h;

    if (ap_it->shape.type == PR_SHAPE_RECT) {
        SDL_RenderDrawRect(ap_sys->renderer, &l_dstRect);
        return;
    }

    if (ap_it->shape.type == PR_SHAPE_FILLEDRECT) {
        if (!ap_it->texture) {
            SDL_RenderFillRect(ap_sys->renderer, &l_dstRect);
            return;
        }

        SDL_SetTextureAlphaMod(ap_it->texture, ap_it->colorMod.a);
        SDL_SetTextureBlendMode(ap_it->texture, ap_it->blendMode);
        SDL_SetTextureColorMod(ap_it->texture, 
            ap_it->colorMod.r,
            ap_it->colorMod.g,
            ap_it->colorMod.b
        );

        SDL_RenderCopyEx(ap_sys->renderer,
            ap_it->texture,
            &ap_it->textureCoords,
            &l_dstRect,
            ap_g->rotation,
            &ap_it->origin,
            SDL_FLIP_NONE
        );

        #ifdef PRISM_DEBUG
            SDL_SetRenderDrawColor(ap_sys->renderer, 127, 0, 0, 127);
            SDL_RenderDrawRect(ap_sys->renderer, &l_dstRect);
        #endif
    }
}

static void s_Pr_RenderEntity(Pr_RenderSystem * ap_sys, Pr_Entity * ap_ent)
{
    Pr_Entity * lp_entity = ap_ent;
    Pr_GeometryComponent * lp_geometry = Pr_GetEntityComponent(lp_entity, &Pr_GeometryComponentInfo);
    Pr_RenderComponent * lp_render = Pr_GetEntityComponent(lp_entity, &Pr_RenderComponentInfo);
    Pr_Renderable * lp_renderables = Pr_GetArrayData(lp_render->renderables);
    pr_u32_t l_i;

    for (l_i=0 ; l_i<Pr_ArraySize(lp_render->renderables) ; l_i++) {
        s_Pr_DrawRenderable(&lp_renderables[l_i], ap_sys, lp_geometry);
    }

    if (lp_render->renderableList) {
        for (l_i=0 ; l_i<lp_render->listSize ; l_i++) {
            s_Pr_DrawRenderable(&lp_render->renderableList[l_i], ap_sys, lp_geometry);
        }
    }

    #ifdef PRISM_DEBUG
        s_Pr_RenderEntityOrigin(ap_sys->renderer, lp_geometry->position.x, lp_geometry->position.y);
    #endif
}

static void s_Pr_RenderSystemCallback(Pr_System * ap_sys, float a_time)
{
    Pr_RenderSystem *   lp_system = ap_sys->data;
    pr_u32_t            l_i;
    Pr_Entity *         lp_entities;

    if (!lp_system->renderer) return;

    lp_entities = Pr_GetArrayData(ap_sys->entities);

    for (l_i=0 ; l_i<Pr_ArraySize(ap_sys->entities) ; l_i++) {
        s_Pr_RenderEntity(lp_system, &lp_entities[l_i]);
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
