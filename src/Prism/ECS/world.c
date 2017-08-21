#include <Prism/ECS/world.h>

#include <stdlib.h>

struct pr_world_t  {
    Pr_List *   systems;
    Pr_Array *  entities;
    Pr_List *   freeEntities;
    Pr_List *   dirtyEntities;
    pr_bool_t   needUpdate;
};

PR_STRUCT(pr_componenthandler_t, Pr_ComponentHandler) {
    Pr_ComponentInfo *  info;
    void *              data;
};

static pr_bool_t s_Pr_EntityInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_Entity * lp_ent = ap_data;

    lp_ent->id = 0;
    lp_ent->componentHandlers = NULL;
    lp_ent->alive = PR_FALSE;
    lp_ent->dirty = PR_FALSE;
    lp_ent->world = NULL;

    return PR_TRUE;
}

Pr_World *      Pr_NewWorld(void)
{
    Pr_World * lp_out;

    lp_out = malloc(sizeof(Pr_World));
    if (!lp_out) return NULL;

    lp_out->entities = Pr_NewArray(sizeof(Pr_Entity), s_Pr_EntityInitializer);
    lp_out->freeEntities = Pr_NewList();
    lp_out->systems = Pr_NewList();
    lp_out->dirtyEntities = Pr_NewList();

    if (lp_out->entities 
        && lp_out->dirtyEntities
        && lp_out->freeEntities 
        && lp_out->systems
    ) {
        if (Pr_SetArrayAt(lp_out->entities, 0, NULL)) {
            lp_out->needUpdate = PR_TRUE;
            return lp_out;
        }
    } 

    Pr_DeleteWorld(lp_out);

    return NULL;
}

static void s_Pr_DeleteSystem(Pr_System * ap_sys)
{
    if (ap_sys->info->deleter) {
        ap_sys->info->deleter(ap_sys->data);
    }

    free(ap_sys->data);
    Pr_DeleteArray(ap_sys->entities);
    free(ap_sys);
}

void            Pr_DeleteWorld(Pr_World * ap_world)
{
    Pr_ListIterator lp_it;
    pr_u32_t l_i;

    if (!ap_world) return;

    for (l_i=0 ; l_i<Pr_ArraySize(ap_world->entities) ; l_i++) {
        Pr_Entity * lp_entities = Pr_GetArrayData(ap_world->entities);
        Pr_RemoveWorldEntity(&lp_entities[l_i]);
        Pr_DeleteArray(lp_entities[l_i].componentHandlers);
    } 

    Pr_DeleteArray(ap_world->entities);

    Pr_DeleteList(ap_world->freeEntities);
    Pr_DeleteList(ap_world->dirtyEntities);
    
    PR_LIST_FOREACH(ap_world->systems, lp_it) {
        s_Pr_DeleteSystem(Pr_ListIteratorData(lp_it));
    }

    Pr_DeleteList(ap_world->systems);

    free(ap_world);
}

static pr_bool_t s_Pr_CompareComponents(Pr_Entity * ap_ent, Pr_System * ap_sys)
{
    pr_u32_t * lp_required = ap_sys->info->requiredComponents;
    pr_u32_t * lp_excluded = ap_sys->info->excludedComponents;
    Pr_ComponentHandler * lp_components = Pr_GetArrayData(ap_ent->componentHandlers);
    pr_u32_t l_i;

    if (lp_required) {
        pr_u32_t l_size = lp_required[0];
        for (l_i=1 ; l_i<=l_size ; l_i++) {
            if (lp_required[l_i] >= Pr_ArraySize(ap_ent->componentHandlers)) {
                return PR_FALSE;   
            }

            if (!lp_components[lp_required[l_i]].info) {
                return PR_FALSE;
            }
        }
    }

    if (lp_excluded) {
        pr_u32_t l_size = lp_excluded[0];
        for (l_i=1 ; l_i<=l_size ; l_i++) {
            if (lp_excluded[l_i] >= Pr_ArraySize(ap_ent->componentHandlers)) {
                continue;
            }

            if (lp_components[lp_excluded[l_i]].info) {
                return PR_FALSE;
            }
        }
    }

    return PR_TRUE;
}

static void s_Pr_RemoveSystemEntity(Pr_Entity * ap_ent, Pr_Array * ap_entities)
{
    Pr_Entity * lp_entities = Pr_GetArrayData(ap_entities);
    pr_u32_t    l_lastIndex = Pr_ArraySize(ap_entities) - 1;

    if (l_lastIndex != 0) {
        *ap_ent = lp_entities[l_lastIndex];
    }

    Pr_ResizeArray(ap_entities, l_lastIndex);
}

static void     s_Pr_UpdateSystem(Pr_System * ap_sys, Pr_World * ap_world)
{
    Pr_ListIterator lp_it;
    pr_u32_t l_i;

    for (l_i=0 ; l_i<Pr_ArraySize(ap_sys->entities) ; ) {
        Pr_Entity * lp_entities = Pr_GetArrayData(ap_sys->entities);
        Pr_Entity * lp_ent = &lp_entities[l_i];

        if (!lp_ent->alive) {
            s_Pr_RemoveSystemEntity(lp_ent, ap_sys->entities);
            continue;
        }

        if (lp_ent->dirty) {
            s_Pr_RemoveSystemEntity(lp_ent, ap_sys->entities);
            continue;
            /* TODO : shouldnt be done that way, entity should stay in system but 
                not be added once more from dirty entities */
        }

        if (!s_Pr_CompareComponents(lp_ent, ap_sys)) {
            s_Pr_RemoveSystemEntity(lp_ent, ap_sys->entities);
            continue;
        }

        l_i++;
    }

    PR_LIST_FOREACH(ap_world->dirtyEntities, lp_it) {
        Pr_Entity * lp_ent = Pr_ListIteratorData(lp_it);

        if (s_Pr_CompareComponents(lp_ent, ap_sys)) {
            Pr_SetArrayAt(ap_sys->entities, Pr_ArraySize(ap_sys->entities), lp_ent);
        }
    }
}

void            Pr_UpdateWorld(Pr_World * ap_world, float a_delta)
{
    Pr_ListIterator lp_it;
    pr_bool_t       l_clean = PR_TRUE;

    if (!ap_world) return;

    PR_LIST_FOREACH(ap_world->systems, lp_it) {
        Pr_System * lp_sys = Pr_ListIteratorData(lp_it);

        if (!lp_sys->active) {
            l_clean = PR_FALSE;
            continue;
        }

        if (ap_world->needUpdate) {
            s_Pr_UpdateSystem(lp_sys, ap_world);
        }

        lp_sys->info->callback(lp_sys, a_delta);
    }

    if (l_clean) {
        PR_LIST_FOREACH(ap_world->dirtyEntities, lp_it) {
            Pr_Entity * lp_ent = Pr_ListIteratorData(lp_it);
            lp_ent->dirty = PR_FALSE;
        }

        Pr_ClearList(ap_world->dirtyEntities);
    }

    ap_world->needUpdate = PR_FALSE;
}

static pr_u32_t s_Pr_GetFreeEntityId(Pr_World * ap_world)
{
    if (Pr_ListSize(ap_world->freeEntities)) {
        Pr_Entity * lp_ent = Pr_ListIteratorData(Pr_ListBegin(ap_world->freeEntities));
            return lp_ent->id;
    }

    return 0;
}

static pr_bool_t s_Pr_ComponentInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_ComponentHandler * lp_hnd = ap_data;

    lp_hnd->info = NULL;
    lp_hnd->data = NULL;

    return PR_TRUE;
}

Pr_Entity *     Pr_CreateWorldEntity(Pr_World * ap_world)
{
    Pr_Entity   l_ent;
    Pr_Entity * lp_entities;

    if (!ap_world) return NULL;

    lp_entities = Pr_GetArrayData(ap_world->entities);

    l_ent.id = s_Pr_GetFreeEntityId(ap_world);
    if (!l_ent.id) {
        l_ent.id = Pr_ArraySize(ap_world->entities);
        l_ent.componentHandlers = Pr_NewArray(sizeof(Pr_ComponentHandler), s_Pr_ComponentInitializer);
        if (!l_ent.componentHandlers) {
            return NULL;
        }
    } else {
        Pr_Entity * lp_entity = &lp_entities[l_ent.id];
        lp_entity->alive = PR_TRUE;
        lp_entity->dirty = PR_FALSE;
        Pr_InvalidateWorldEntity(lp_entity);
        Pr_EraseListElement(Pr_ListBegin(ap_world->freeEntities));

        return lp_entity;
    }

    l_ent.world = ap_world;
    l_ent.alive = PR_TRUE;
    l_ent.dirty = PR_FALSE;

    if (Pr_SetArrayAt(ap_world->entities, l_ent.id, &l_ent)) {
        lp_entities = Pr_GetArrayData(ap_world->entities);
        Pr_InvalidateWorldEntity(&lp_entities[l_ent.id]);
        return &lp_entities[l_ent.id];
    }

    Pr_DeleteArray(l_ent.componentHandlers);

    return NULL;
}

void           Pr_RemoveWorldEntity(Pr_Entity * ap_entity)
{
    Pr_ComponentHandler * lp_components;
    pr_u32_t l_i;

    if (!ap_entity) return;
    
    if (!ap_entity->alive) return;

    ap_entity->alive = PR_FALSE;
    ap_entity->dirty = PR_TRUE;

    if (!Pr_PushBackList(ap_entity->world->freeEntities, ap_entity)) {

        /* CRITICAL */

        return;
    }

    lp_components = Pr_GetArrayData(ap_entity->componentHandlers);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_entity->componentHandlers) ; l_i++) {
        Pr_ComponentHandler * lp_tmp = &lp_components[l_i];
        if (lp_tmp->info) {
            if (lp_tmp->info->deleter) {
                lp_tmp->info->deleter(lp_tmp->data);
            }

            free(lp_tmp->data);
            lp_tmp->data = NULL;
            lp_tmp->info = NULL;
        }
    }

    ap_entity->world->needUpdate = PR_TRUE;
}

static Pr_System * s_Pr_NewSystem(Pr_SystemInfo * ap_info)
{
    Pr_System * lp_out;

    lp_out = malloc(sizeof(Pr_System));
    if (!lp_out) return NULL;

    lp_out->entities = Pr_NewArray(sizeof(Pr_Entity), s_Pr_EntityInitializer);
    lp_out->data = malloc(ap_info->dataSize);
    
    if (lp_out->data
        && lp_out->entities
    ) {
        if (ap_info->initializer(lp_out->data, ap_info->dataSize)) {
            lp_out->info = ap_info;
            return lp_out;
        }
    } 

    s_Pr_DeleteSystem(lp_out);

    return NULL;
}

pr_bool_t      Pr_RegisterWorldSystem(Pr_World * ap_world, Pr_SystemInfo * ap_info)
{
    Pr_System * lp_system;

    if (!ap_world || !ap_info) return PR_FALSE;

    lp_system = s_Pr_NewSystem(ap_info);
    if (!lp_system) return PR_FALSE;

    lp_system->world = ap_world;

    if (Pr_PushBackList(ap_world->systems, lp_system)) {
        lp_system->active = PR_TRUE;
        return PR_TRUE;
    }

    s_Pr_DeleteSystem(lp_system);
    
    return PR_FALSE;
}

void           Pr_InvalidateWorldEntity(Pr_Entity * ap_ent)
{
    if (!ap_ent) return;

    if (!ap_ent->alive) return;

    if (!ap_ent->dirty) {
        Pr_PushBackList(ap_ent->world->dirtyEntities, ap_ent);
        ap_ent->dirty = PR_TRUE;
    }

    ap_ent->world->needUpdate = PR_TRUE;
}

void *         Pr_GetWorldSystem(Pr_World * ap_world, Pr_SystemInfo * ap_info)
{
    Pr_ListIterator lp_it;

    if (!ap_world || !ap_info) return NULL;

    PR_LIST_FOREACH(ap_world->systems, lp_it) {
        Pr_System * lp_tmp = Pr_ListIteratorData(lp_it);
        if (lp_tmp->info == ap_info) {
            return lp_tmp->data;
        }
    }

    return NULL;
}

void           Pr_SetWorldSystemActivated(Pr_World * ap_world, Pr_SystemInfo * ap_info, pr_bool_t a_bool)
{
    Pr_ListIterator lp_it;

    if (!ap_world || !ap_info) return;

    PR_LIST_FOREACH(ap_world->systems, lp_it) {
        Pr_System * lp_tmp = Pr_ListIteratorData(lp_it);
        if (lp_tmp->info == ap_info) {
            lp_tmp->active = a_bool;
            if (a_bool) {
                ap_world->needUpdate = PR_TRUE;
            }

            return;
        }
    }
}

Pr_Entity *    Pr_GetWorldEntity(Pr_World * ap_world, pr_u32_t ap_id)
{
    Pr_Entity * lp_entities;

    if (!ap_world || ap_id == 0) return NULL;

    if (ap_id >= Pr_ArraySize(ap_world->entities)) return NULL;

    lp_entities = Pr_GetArrayData(ap_world->entities);

    return &lp_entities[ap_id];
}

