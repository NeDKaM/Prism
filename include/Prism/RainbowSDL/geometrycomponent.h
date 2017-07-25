#ifndef H_RAINBOWSDL_GEOMETRYCOMPONENT_INCLUDED
#define H_RAINBOWSDL_GEOMETRYCOMPONENT_INCLUDED

#include <Prism/config.h>

#include <Prism/ECS/component.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    #define PR_GEOMETRYCOMPONENT /**/ 0 /**/

    extern PRISM_API Pr_ComponentInfo Pr_GeometryComponentInfo;

    PR_STRUCT(pr_geometrycomponent_t, Pr_GeometryComponent) {
        SDL_Rect    rectangle;
        float       rotation;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_GEOMETRYCOMPONENT_INCLUDED */

