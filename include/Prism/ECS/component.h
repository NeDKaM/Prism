#ifndef H_ECS_COMPONENT_INCLUDED
#define H_ECS_COMPONENT_INCLUDED

#include <Prism/config.h>

#include <Prism/memory.h>

PR_CPP_PROTECT_ON

    typedef struct pr_componentinfo_t Pr_ComponentInfo;
    struct pr_componentinfo_t {
        pr_u32_t        id;
        Pr_Initializer  initializer;
        Pr_Deleter      deleter;
        pr_u32_t        dataSize;
    };

    PR_STRUCT(pr_componenthandler_t, Pr_ComponentHandler) {
        Pr_ComponentInfo *  info;
        void *              data;
    };

PR_CPP_PROTECT_OFF

#endif /* H_ECS_COMPONENET_INCLUDED */


