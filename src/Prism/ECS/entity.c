#include <Prism/ECS/entity.h>
#include <Prism/ECS/world.h>

#include <stdlib.h>

PR_STRUCT(pr_componenthandler_t, Pr_ComponentHandler) {
    Pr_ComponentInfo *  info;
    void *              data;
};

void *     Pr_GetEntityComponent(Pr_Entity * ap_entity, Pr_ComponentInfo * ap_info)
{
    Pr_ComponentHandler * lp_components;

    if (!ap_entity || !ap_info) return NULL;

    if (!ap_entity->alive) return NULL;

    if (ap_info->id >= Pr_ArraySize(ap_entity->componentHandlers)) return NULL;

    lp_components = Pr_GetArrayData(ap_entity->componentHandlers);

    return (lp_components[ap_info->id].info) ? lp_components[ap_info->id].data : NULL;
}

void *     Pr_AddEntityComponent(Pr_Entity * ap_entity, Pr_ComponentInfo * ap_info)
{
    Pr_ComponentHandler l_hnd;
    void * lp_data;

    if (!ap_entity || !ap_info) return NULL;

    if (!ap_entity->alive) return NULL;

    lp_data = Pr_GetEntityComponent(ap_entity, ap_info);
    if (lp_data) {
        return lp_data;
    }

    lp_data = malloc(ap_info->dataSize);
    if (!lp_data) return NULL;

    l_hnd.data = lp_data;
    l_hnd.info = ap_info;

    if (ap_info->initializer(l_hnd.data, ap_info->dataSize)) {
        if (Pr_SetArrayAt(ap_entity->componentHandlers, ap_info->id, &l_hnd)) {
            Pr_ComponentHandler * lp_components = Pr_GetArrayData(ap_entity->componentHandlers);
            Pr_InvalidateWorldEntity(ap_entity);
            return lp_components[ap_info->id].data;
        }
    }

    return NULL;
}
