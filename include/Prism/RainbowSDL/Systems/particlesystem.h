#ifndef H_RAINBOWSDL_PARTICLESYSTEM_INCLUDED
#define H_RAINBOWSDL_PARTICLESYSTEM_INCLUDED

#include <Prism/ECS/system.h>

PR_CPP_PROTECT_ON

    extern PRISM_API Pr_SystemInfo Pr_ParticleSystemInfo;

    PR_STRUCT(pr_particlesystem_t, Pr_ParticleSystem) {
        pr_u32_t    emitterCount;
    };

PR_CPP_PROTECT_OFF

#endif

/*

*/
