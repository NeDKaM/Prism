#ifndef H_RAINBOWSDL_TIMINGSYSTEM_INCLUDED
#define H_RAINBOWSDL_TIMINGSYSTEM_INCLUDED

#include <Prism/ECS/system.h>

PR_CPP_PROTECT_ON

    extern PRISM_API Pr_SystemInfo Pr_TimingSystemInfo;

    PR_STRUCT(pr_timingsystem_t, Pr_TimingSystem) {
        pr_u32_t    timeLineCount;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_TIMINGSYSTEM_INCLUDED */

/*

*/
