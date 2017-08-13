#include <Prism/RainbowSDL/Systems/particlesystem.h>

#include <Prism/RainbowSDL/Components/geometrycomponent.h>
#include <Prism/RainbowSDL/Components/particlecomponent.h>

#include <Prism/ECS/entity.h>

static void s_Pr_ParticleSystemCallback(Pr_System * ap_sys, float a_dt)
{
    Pr_ListIterator lp_it;
    Pr_ParticleSystem * lp_system = ap_sys->data;

    lp_system->emitterCount = 0;

    PR_LIST_FOREACH(ap_sys->entities, lp_it) {
        Pr_Entity *     lp_entity = Pr_ListIteratorData(lp_it);
        Pr_ParticleEmitterComponent * lp_emitterComp = Pr_GetEntityComponent(lp_entity, &Pr_ParticleEmitterComponentInfo);

        Pr_UpdateParticleEmitter(lp_emitterComp->emitter, a_dt);

        lp_system->emitterCount++;
    }
}

static pr_u32_t s_requiredComponents[] = {
    1,
    PR_COMPONENT_PARTICLEEMITTER
};

static pr_bool_t s_Pr_ParticleSystemInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_ParticleSystem * lp_system = ap_data;

    lp_system->emitterCount = 0;

    return PR_TRUE;
}

Pr_SystemInfo Pr_ParticleSystemInfo = {
    s_Pr_ParticleSystemCallback,
    sizeof(Pr_ParticleSystem),
    s_Pr_ParticleSystemInitializer,
    NULL,
    s_requiredComponents,
    NULL
};
