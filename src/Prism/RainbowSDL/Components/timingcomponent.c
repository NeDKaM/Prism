#include <Prism/RainbowSDL/Components/timingcomponent.h>

static pr_bool_t s_Pr_ComponentInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_TimingComponent * lp_timing = ap_data;

    lp_timing->timeLine = Pr_NewTimeLine();

    if (lp_timing->timeLine) {
        return PR_TRUE;
    }

    return PR_FALSE;
}

static void s_Pr_ComponentDeleter(void * ap_data)
{
    Pr_TimingComponent * lp_timing = ap_data;

    Pr_DeleteTimeLine(lp_timing->timeLine);
}

Pr_ComponentInfo Pr_TimingComponentInfo = {
    PR_COMPONENT_TIMING,
    s_Pr_ComponentInitializer,
    s_Pr_ComponentDeleter,
    sizeof(Pr_TimingComponent)
};
