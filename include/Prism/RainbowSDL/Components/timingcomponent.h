#ifndef H_RAINBOWSDL_TIMINGCOMPONENT_INCLUDED
#define H_RAINBOWSDL_TIMINGCOMPONENT_INCLUDED

#include <Prism/timing.h>

#include <Prism/ECS/component.h>

PR_CPP_PROTECT_ON

    #define PR_COMPONENT_TIMING /**/ 3 /**/

    extern PRISM_API Pr_ComponentInfo Pr_TimingComponentInfo;

    PR_STRUCT(pr_timingcomponent_t, Pr_TimingComponent) {
        Pr_TimeLine * timeLine;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_TIMINGCOMPONENT_INCLUDED */

/*


*/
