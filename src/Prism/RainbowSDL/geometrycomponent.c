#include <Prism/RainbowSDL/geometrycomponent.h>

#include <string.h>

static pr_bool_t s_Pr_ComponentInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_GeometryComponent * lp_component = ap_data;

    lp_component->rotation = 0.f;
    lp_component->rectangle.x = 0;
    lp_component->rectangle.y = 0;
    lp_component->rectangle.w = 0;
    lp_component->rectangle.h = 0;

    return PR_TRUE;
}

Pr_ComponentInfo Pr_GeometryComponentInfo = {
    PR_GEOMETRYCOMPONENT,
    s_Pr_ComponentInitializer,
    NULL,
    sizeof(Pr_GeometryComponent)
};
