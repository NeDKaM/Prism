#ifndef H_RAINBOWSDL_PARTICLE_INCLUDED
#define H_RAINBOWSDL_PARTICLE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

#include <Prism/memory.h>

#include <Prism/RainbowSDL/renderable.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_particle_t, Pr_Particle) {
        Pr_Renderable           renderable;
        float                   lifeSpan;
        float                   mass;
        struct { float x, y; }  position;
        struct { float x, y; }  acceleration;
        struct { float x, y; }  velocity;
    };

    PR_STRUCT(pr_particlegenerator_t, Pr_ParticleGenerator) {
        float       emissionRate;
        float       timeAccumulator;
        pr_u32_t    maxParticleCount;
        void *      data;
        pr_bool_t   persistent;

        void        (*generate)(Pr_Particle *, void *);
    };

    PR_STRUCT(pr_particlecontroller_t, Pr_ParticleControllerInfo) {
        pr_u32_t        dataSize;
        Pr_Initializer  initializer;
        Pr_Deleter      deleter;

        void            (*stepFunc)(void * controller, Pr_Particle * particles, pr_u32_t, float);
    };
    
    PR_STRUCT(pr_particlegroup_t, Pr_ParticleGroup);

    extern PRISM_API void *     Pr_AddParticleController(Pr_ParticleGroup *, Pr_ParticleControllerInfo *);
    extern PRISM_API void       Pr_SetParticleControllerActivated(Pr_ParticleGroup *, Pr_ParticleControllerInfo *, pr_bool_t);
    extern PRISM_API Pr_Array * Pr_GetGroupParticles(Pr_ParticleGroup *);

    PR_STRUCT(pr_particleemitter_t, Pr_ParticleEmitter);

    extern PRISM_API Pr_ParticleEmitter *   Pr_NewParticleEmitter(void);
    extern PRISM_API void                   Pr_DeleteParticleEmitter(Pr_ParticleEmitter *);

    extern PRISM_API Pr_ParticleGroup *     Pr_AddParticleGenerator(Pr_ParticleEmitter *, Pr_ParticleGenerator *);

    extern PRISM_API void                   Pr_UpdateParticleEmitter(Pr_ParticleEmitter *, float);
    extern PRISM_API void                   Pr_SetParticleEmitterActivated(Pr_ParticleEmitter *, pr_bool_t);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_PARTICLE_INCLUDED */

/*

    

*/
