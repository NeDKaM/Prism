#include <Prism/RainbowSDL/particle.h>

#include <Prism/memory.h>
#include <Prism/list.h>

struct pr_particleemitter_t {
    pr_bool_t               active;
    Pr_List *               groups;
};

struct pr_particlegroup_t {
    Pr_Array *              particles;
    Pr_ParticleGenerator *  generator; 
    Pr_Array *              ctrlsHandlers; 
};

PR_STRUCT(pr_particlecontrollerhandler_t, Pr_ParticleControllerHandler) {
    Pr_ParticleControllerInfo * info;
    void *                      data;
    pr_bool_t                   active;
};

static void s_Pr_RenderableInitializer(Pr_Renderable * ap_rnd)
{
    *ap_rnd = Pr_MakeRenderable();

    ap_rnd->originOnly = PR_TRUE;
}

static pr_bool_t s_Pr_ParticleInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_Particle * lp_particle = ap_data;

    s_Pr_RenderableInitializer(&lp_particle->renderable);

    lp_particle->acceleration.x = 0.f;
    lp_particle->acceleration.y = 0.f;

    lp_particle->velocity.x = 0.f;
    lp_particle->velocity.y = 0.f;

    lp_particle->position.x = 0.f;
    lp_particle->position.y = 0.f;

    lp_particle->mass = 0.f;

    lp_particle->lifeSpan = 0.f;

    return PR_TRUE;
}

static void s_Pr_DeleteParticleGroup(Pr_ParticleGroup * ap_group)
{
    pr_u32_t l_i;
    Pr_ParticleControllerHandler * lp_hnds;

    if (!ap_group) return;

    Pr_DeleteArray(ap_group->particles);

    lp_hnds = Pr_GetArrayData(ap_group->ctrlsHandlers);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_group->ctrlsHandlers) ; l_i++) {
        Pr_ParticleControllerHandler * lp_hnd = &lp_hnds[l_i];
        if (lp_hnd->info->deleter) {
            lp_hnd->info->deleter(lp_hnd->data);
        }
    }
    Pr_DeleteArray(ap_group->ctrlsHandlers);

    free(ap_group);
}

static Pr_ParticleGroup * s_Pr_NewParticleGroup(Pr_ParticleGenerator * ap_gen)
{
    Pr_ParticleGroup * lp_out;

    lp_out = malloc(sizeof(Pr_ParticleGroup));
    if (!lp_out) return NULL;

    lp_out->particles = Pr_NewArray(sizeof(Pr_Particle), s_Pr_ParticleInitializer);
    lp_out->ctrlsHandlers = Pr_NewArray(sizeof(Pr_ParticleControllerHandler), NULL);

    if (lp_out->particles && lp_out->ctrlsHandlers) {
        lp_out->generator = ap_gen;
        return lp_out;
    }

    s_Pr_DeleteParticleGroup(lp_out);

    return NULL;
}

Pr_ParticleEmitter *   Pr_NewParticleEmitter(void)
{
    Pr_ParticleEmitter * lp_out;

    lp_out = malloc(sizeof(Pr_ParticleEmitter));
    if (!lp_out) return NULL;

    lp_out->groups = Pr_NewList();

    if (lp_out->groups) {
        lp_out->active = PR_TRUE;

        return lp_out;
    }

    Pr_DeleteParticleEmitter(lp_out);

    return NULL;
}

void                   Pr_DeleteParticleEmitter(Pr_ParticleEmitter * ap_emitter)
{
    Pr_ListIterator lp_it;

    if (!ap_emitter) return;

    PR_LIST_FOREACH(ap_emitter->groups, lp_it) {
        Pr_ParticleGroup * lp_group = Pr_ListIteratorData(lp_it);
        s_Pr_DeleteParticleGroup(lp_group);
    }

    Pr_DeleteList(ap_emitter->groups);

    free(ap_emitter);
}

static Pr_Particle * s_Pr_CreateParticle(Pr_ParticleGroup * ap_group)
{
    return Pr_SetArrayAt(ap_group->particles, Pr_ArraySize(ap_group->particles), NULL);
}

static void s_Pr_RemoveParticle(Pr_Particle * ap_particle, Pr_Array * ap_particles)
{
    Pr_Particle *   lp_particles = Pr_GetArrayData(ap_particles);
    pr_u32_t        l_lastIndex = Pr_ArraySize(ap_particles) - 1;

    if (l_lastIndex == 0) {
        s_Pr_ParticleInitializer(ap_particle, 0);
    } else {
        *ap_particle = lp_particles[l_lastIndex];
    }

    Pr_ResizeArray(ap_particles, l_lastIndex);
}

static void s_Pr_UpdateParticleGroup(Pr_ParticleGroup * ap_group, float a_time)
{
    Pr_Particle *                   lp_particles;
    Pr_ParticleControllerHandler *  lp_hnds;
    pr_u32_t                        l_i;

    if (!ap_group) return;

    lp_particles = Pr_GetArrayData(ap_group->particles);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_group->particles) ; ) {
        Pr_Particle * lp_particle = &lp_particles[l_i];

        lp_particle->lifeSpan -= a_time;

        if (lp_particle->lifeSpan < 0.f) {
            s_Pr_RemoveParticle(lp_particle, ap_group->particles);
            continue;
        }

        lp_particle->renderable.origin.x = (long)lp_particle->position.x;
        lp_particle->renderable.origin.y = (long)lp_particle->position.y;

        l_i++;
    }

    lp_hnds = Pr_GetArrayData(ap_group->ctrlsHandlers);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_group->ctrlsHandlers) ; l_i++) {
        Pr_ParticleControllerHandler * lp_hnd = &lp_hnds[l_i];

        lp_hnd->info->stepFunc(lp_hnd->data, lp_particles, Pr_ArraySize(ap_group->particles), a_time);
    }
}


void    Pr_UpdateParticleEmitter(Pr_ParticleEmitter * ap_emitter, float a_time)
{
    Pr_ListIterator lp_it;

    if (!ap_emitter) return;

    for (lp_it=Pr_ListBegin(ap_emitter->groups) ; lp_it!=NULL ; ) {
        Pr_ParticleGroup * lp_group = Pr_ListIteratorData(lp_it);

        if (ap_emitter->active) {
            pr_u32_t l_particleCount = Pr_ArraySize(lp_group->particles);

            if (l_particleCount < lp_group->generator->maxParticleCount) {
                pr_u32_t l_particleGenerated = 0;

                if (lp_group->generator->persistent) {
                    lp_group->generator->timeAccumulator += a_time;
                }

                while (lp_group->generator->timeAccumulator > 1.f / lp_group->generator->emissionRate) {
                    if (l_particleGenerated + l_particleCount < lp_group->generator->maxParticleCount) {
                        Pr_Particle * lp_particle = s_Pr_CreateParticle(lp_group);
                        if (lp_particle) {
                            lp_group->generator->generate(lp_particle, lp_group->generator->data);
                        }

                        lp_group->generator->timeAccumulator -= 1.f / lp_group->generator->emissionRate;

                        l_particleGenerated++;
                    } else {
                        break;
                    }
                }
            }
        }

        s_Pr_UpdateParticleGroup(lp_group, a_time);

        if (!lp_group->generator->persistent && !Pr_ArraySize(lp_group->particles)) {
            s_Pr_DeleteParticleGroup(lp_group);
            Pr_EraseListElement(ap_emitter->groups, lp_it);
            continue;
        }

        lp_it = Pr_NextListIterator(lp_it);
    }
}

void    Pr_SetParticleEmitterActivated(Pr_ParticleEmitter * ap_emitter, pr_bool_t a_active)
{
    if (!ap_emitter) return;

    ap_emitter->active = a_active;
}

Pr_ParticleGroup * Pr_AddParticleGenerator(Pr_ParticleEmitter * ap_emitter, Pr_ParticleGenerator * ap_gen)
{
    Pr_ParticleGroup * lp_group;

    if (!ap_emitter || !ap_gen) return NULL;

    lp_group = s_Pr_NewParticleGroup(ap_gen);
    if (lp_group) {
        if (Pr_PushBackList(ap_emitter->groups, lp_group)) {
            return lp_group;
        }
    }

    s_Pr_DeleteParticleGroup(lp_group);

    return NULL;
}

void * Pr_AddParticleController(Pr_ParticleGroup * ap_group, Pr_ParticleControllerInfo * ap_ctrlinfo)
{
    Pr_ParticleControllerHandler l_hnd;

    if (!ap_group || !ap_ctrlinfo) return NULL;

    l_hnd.active = PR_TRUE;
    l_hnd.info = ap_ctrlinfo;
    l_hnd.data = malloc(ap_ctrlinfo->dataSize);

    if (l_hnd.data) {
        if (l_hnd.info->initializer(l_hnd.data, l_hnd.info->dataSize)) {   
            l_hnd.active = PR_TRUE;
            l_hnd.info = ap_ctrlinfo;

            if (Pr_SetArrayAt(ap_group->ctrlsHandlers, Pr_ArraySize(ap_group->ctrlsHandlers), &l_hnd)) {
                return l_hnd.data;
            }

            if (l_hnd.info->deleter) {
                l_hnd.info->deleter(l_hnd.data);
            }
        }
    }

    free(l_hnd.data);

    return NULL;
}

Pr_Array *  Pr_GetGroupParticles(Pr_ParticleGroup * ap_group)
{
    if (!ap_group) return NULL;

    return ap_group->particles;
}

void   Pr_SetParticleControllerActivated(Pr_ParticleGroup * ap_group, Pr_ParticleControllerInfo * ap_ctrlinfo, pr_bool_t a_bool)
{
    pr_u32_t l_i;
    Pr_ParticleControllerHandler * lp_hnds;

    if (!ap_group || ap_ctrlinfo)  return;

    lp_hnds = Pr_GetArrayData(ap_group->ctrlsHandlers);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_group->ctrlsHandlers) ; l_i++) {
        Pr_ParticleControllerHandler * lp_hnd = &lp_hnds[l_i];
        if (lp_hnd->info == ap_ctrlinfo) {
            lp_hnd->active = a_bool;
            return;
        }
    }
}


