#ifndef H_ECS_SYSTEM_INCLUDED
#define H_ECS_SYSTEM_INCLUDED

#include <Prism/config.h>
#include <Prism/memory.h>
#include <Prism/list.h>

#include <stdarg.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_world_t, Pr_World);

    PR_STRUCT(pr_systeminfo_t, Pr_SystemInfo);

    PR_STRUCT(pr_system_t, Pr_System) {
        Pr_SystemInfo *     info;
        Pr_List *           entities;
        Pr_World *          world;
        pr_bool_t           active;
        void *              data;
    };

    typedef void (*Pr_SystemCallback)(Pr_System *, float);

    struct pr_systeminfo_t {
        Pr_SystemCallback   callback;
        pr_u32_t            dataSize;
        Pr_Initializer      initializer;
        Pr_Deleter          deleter;
        pr_u32_t *          requiredComponents;
        pr_u32_t *          excludedComponents;
    };

PR_CPP_PROTECT_OFF

#endif /* H_ECS_SYSTEM_INCLUDED */

/* 



*/

