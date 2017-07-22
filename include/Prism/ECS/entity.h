#ifndef H_ECS_ENTITY_INCLUDED
#define H_ECS_ENTITY_INCLUDED

#include <Prism/config.h>
#include <Prism/memory.h>

#include <Prism/ECS/component.h>
#include <Prism/ECS/world.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_entity_t, Pr_Entity) {
        pr_u32_t    id; 
        Pr_World *  world;
        Pr_Array *  componentHandlers;
        pr_bool_t   alive;
    };

    extern PRISM_API void *     Pr_GetEntityComponent(Pr_Entity *, Pr_ComponentInfo *);
    extern PRISM_API void *     Pr_AddEntityComponent(Pr_Entity *, Pr_ComponentInfo *);


PR_CPP_PROTECT_OFF

#endif /* H_ECS_ENTITY_INCLUDED */

/*

Pr_AddEntityComponent(entity, RENDER_COMPONENT);

system() {
    for (entities, at) {
        Pr_Entity * it = entities[at];
        RenderComponent * rndcomp = Pr_GetEntityComponent(it, RENDER_COMPONENT);

        ...
    }
}

*/
