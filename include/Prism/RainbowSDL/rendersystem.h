#ifndef H_RAINBOWSDL_RENDERSYSTEM_INCLUDED
#define H_RAINBOWSDL_RENDERSYSTEM_INCLUDED

#include <Prism/config.h>

#include <Prism/ECS/system.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    extern PRISM_API Pr_SystemInfo Pr_RenderSystemInfo;

    PR_STRUCT(pr_rendersystem, Pr_RenderSystem) {
        SDL_Renderer *  renderer;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_RENDERSYSTEM_INCLUDED */


