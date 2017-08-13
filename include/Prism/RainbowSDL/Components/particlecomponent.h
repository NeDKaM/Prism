#ifndef H_RAINBOWSDL_PARTICLECOMPONENT_INCLUDED
#define H_RAINBOWSDL_PARTICLECOMPONENT_INCLUDED

#include <Prism/ECS/component.h>

#include <Prism/RainbowSDL/particle.h>

PR_CPP_PROTECT_ON

    #define PR_COMPONENT_PARTICLEEMITTER /**/ 2 /**/ 

    extern PRISM_API Pr_ComponentInfo   Pr_ParticleEmitterComponentInfo;

    PR_STRUCT(pr_particleemittercomponent_t, Pr_ParticleEmitterComponent) {
        Pr_ParticleEmitter *    emitter;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_PARTICLECOMPONENT_INCLUDED */

/*


*/

