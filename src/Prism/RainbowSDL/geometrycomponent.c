#include <Prism/RainbowSDL/geometrycomponent.h>

#include <string.h>

static pr_bool_t s_Pr_ComponentInitializer(void * ap_data, pr_u32_t a_size)
{
    Pr_GeometryComponent * lp_geometry = ap_data;

    lp_geometry->rotation = 0.f;
    lp_geometry->rectangle.x = 0;
    lp_geometry->rectangle.y = 0;
    lp_geometry->rectangle.w = 0;
    lp_geometry->rectangle.h = 0;

    return PR_TRUE;
}

Pr_ComponentInfo Pr_GeometryComponentInfo = {
    PR_GEOMETRYCOMPONENT,
    s_Pr_ComponentInitializer,
    NULL,
    sizeof(Pr_GeometryComponent)
};
