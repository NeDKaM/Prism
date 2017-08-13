#include <Prism/RainbowSDL/Components/particlecomponent.h>

static pr_bool_t s_Pr_ParticleEmitterInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_ParticleEmitterComponent * lp_comp = ap_data;

    lp_comp->emitter = Pr_NewParticleEmitter();
    if (!lp_comp->emitter) return PR_FALSE;

    return PR_TRUE;
}

static void s_Pr_ParticleEmitterDeleter(void * ap_data)
{
    Pr_ParticleEmitterComponent * lp_comp = ap_data;

    if (!lp_comp) return;

    Pr_DeleteParticleEmitter(lp_comp->emitter);
}

Pr_ComponentInfo Pr_ParticleEmitterComponentInfo = {
    PR_COMPONENT_PARTICLEEMITTER,
    s_Pr_ParticleEmitterInitializer,
    s_Pr_ParticleEmitterDeleter,
    sizeof(Pr_ParticleEmitterComponent)
};

