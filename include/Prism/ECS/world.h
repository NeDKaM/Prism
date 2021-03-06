#ifndef H_ECS_WORLD_INCLUDED
#define H_ECS_WORLD_INCLUDED

#include <Prism/config.h>
#include <Prism/memory.h>
#include <Prism/list.h>

#include <Prism/ECS/entity.h>
#include <Prism/ECS/component.h>
#include <Prism/ECS/system.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_world_t, Pr_World);

    extern PRISM_API Pr_World *     Pr_NewWorld(void);
    extern PRISM_API void           Pr_DeleteWorld(Pr_World *);

    extern PRISM_API void           Pr_UpdateWorld(Pr_World *, float);

    extern PRISM_API pr_entityid_t  Pr_CreateWorldEntity(Pr_World *); 
    extern PRISM_API void           Pr_RemoveWorldEntity(Pr_Entity *);
    extern PRISM_API Pr_Entity *    Pr_GetWorldEntity(Pr_World *, pr_entityid_t);

    extern PRISM_API void           Pr_InvalidateWorldEntity(Pr_Entity *);

    extern PRISM_API pr_bool_t      Pr_RegisterWorldSystem(Pr_World *, Pr_SystemInfo *);

    extern PRISM_API void *         Pr_GetWorldSystem(Pr_World *, Pr_SystemInfo *);
    extern PRISM_API void           Pr_SetWorldSystemActivated(Pr_World *, Pr_SystemInfo *, pr_bool_t);

PR_CPP_PROTECT_OFF

#endif /* H_ECS_WORLD_INCLUDED */

