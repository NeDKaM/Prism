#include <Prism/ECS/entity.h>

#include <stdlib.h>

void *     Pr_GetEntityComponent(Pr_Entity * ap_entity, Pr_ComponentInfo * ap_info)
{
    Pr_ComponentHandler * lp_components;

    if (!ap_entity || !ap_info) return NULL;

    if (!ap_entity->alive) return NULL;

    if (ap_info->id >= Pr_ArraySize(ap_entity->componentHandlers)) return NULL;

    lp_components = Pr_GetArrayData(ap_entity->componentHandlers);

    return (lp_components[ap_info->id].id) ? &lp_components[ap_info->id] : NULL;
}

void *     Pr_AddEntityComponent(Pr_Entity * ap_entity, Pr_ComponentInfo * ap_info)
{
    Pr_ComponentHandler l_hnd;
    void * lp_data;

    if (!ap_entity || !ap_info) return NULL;

    if (!ap_entity->alive) return NULL;

    lp_data = malloc(sizeof(ap_info->dataSize));
    if (!lp_data) return NULL;

    l_hnd.data = lp_data;
    l_hnd.id = ap_info->id;

    if (ap_info->initializer(l_hnd.data, ap_info->dataSize)) {
        if (Pr_SetArrayAt(ap_entity->componentHandlers, ap_info->id, &l_hnd)) {
            Pr_ComponentHandler * lp_components = Pr_GetArrayData(ap_entity->componentHandlers);
            Pr_InvalidateWorldEntity(ap_entity->world, ap_entity);
            return &lp_components[ap_info->id];
        }
    }

    return NULL;
}
