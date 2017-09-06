#include <Prism/RainbowSDL/Systems/timingsystem.h>

#include <Prism/RainbowSDL/Components/timingcomponent.h>

#include <Prism/ECS/entity.h>

#include <stdlib.h>

static void s_Pr_TimingSystemCallback(Pr_System * ap_sys, float a_dt)
{
    Pr_TimingSystem *   lp_sys = ap_sys->data;
    Pr_Entity *         lp_entities = Pr_GetArrayData(ap_sys->entities);
    pr_u32_t            l_i;

    lp_sys->timeLineCount = 0;

    for (l_i=0 ; l_i<Pr_ArraySize(ap_sys->entities) ; l_i++) {
        Pr_TimingComponent * lp_timing = Pr_GetEntityComponent(&lp_entities[l_i], &Pr_TimingComponentInfo);
        
        Pr_StepTimeLine(lp_timing->timeLine, a_dt);

        lp_sys->timeLineCount++;
    }
}

static pr_bool_t s_Pr_SystemInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_TimingSystem * lp_sys = ap_data;

    lp_sys->timeLineCount = 0;

    return PR_TRUE;
}

static pr_u32_t s_requiredComponents[] = {
    1,
    PR_COMPONENT_TIMING
};

Pr_SystemInfo Pr_TimingSystemInfo = {
    s_Pr_TimingSystemCallback,
    sizeof(Pr_TimingSystem),
    s_Pr_SystemInitializer,
    NULL,
    s_requiredComponents,
    NULL,
};
