#include <Prism/timing.h>

#include <stdlib.h>
#include <string.h>

#include <Prism/memory.h>
#include <Prism/list.h>

static float s_Pr_LinearTimeModifier(float a_dt)
{
    return a_dt;
}

struct pr_timeline_t {
    Pr_Array *  sequences;
    float       time;
};

struct pr_timesequence_t {
    Pr_List *       phrases;
    Pr_ListIterator currentPhrase;
    pr_bool_t       active;
};

typedef struct {
    Pr_Phrase * info;
    float       currentTime;
    void *      data;
} Pr_PhraseHandle;

static pr_bool_t s_Pr_SequenceInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_TimeSequence * lp_seq = ap_data;

    lp_seq->phrases         = NULL;
    lp_seq->active          = PR_FALSE;
    lp_seq->currentPhrase   = NULL;

    return PR_TRUE;
}

Pr_TimeLine *      Pr_NewTimeLine(void)
{
    Pr_TimeLine * lp_out;

    lp_out = malloc(sizeof(Pr_TimeLine));
    if (!lp_out) return NULL;

    lp_out->sequences = Pr_NewArray(sizeof(Pr_TimeSequence), s_Pr_SequenceInitializer);

    if (lp_out->sequences) {
        if (Pr_SetArrayAt(lp_out->sequences, 0, NULL)) {
            lp_out->time = 0.f;
            return lp_out;
        }
    }

    Pr_DeleteTimeLine(lp_out);

    return NULL;
}

void               Pr_DeleteTimeLine(Pr_TimeLine * ap_tl)
{
    if (!ap_tl) return;

    Pr_ClearTimeLine(ap_tl);

    Pr_DeleteArray(ap_tl->sequences);
    free(ap_tl);
}

static void s_Pr_FreeTimeSequence(Pr_TimeSequence * ap_seq)
{
    Pr_DeleteList(ap_seq->phrases);
}

static pr_bool_t s_Pr_CreateTimeSequence(Pr_TimeSequence * ap_seq)
{
    ap_seq->phrases = Pr_NewList();

    if (ap_seq->phrases) {
        ap_seq->currentPhrase = NULL;
        ap_seq->active = PR_FALSE;
        return PR_TRUE;
    }

    return PR_FALSE;
}

pr_sequenceid_t    Pr_CreateTimeLineSequence(Pr_TimeLine * ap_tl)
{
    Pr_TimeSequence l_seq;

    if (!ap_tl) return 0;

    if (!s_Pr_CreateTimeSequence(&l_seq)) return 0;

    if (Pr_SetArrayAt(ap_tl->sequences, Pr_ArraySize(ap_tl->sequences), &l_seq)) {
        return Pr_ArraySize(ap_tl->sequences) - 1;
    }

    s_Pr_FreeTimeSequence(&l_seq);

    return 0;
}

Pr_TimeSequence *  Pr_GetTimeLineSequence(Pr_TimeLine * ap_tl, pr_sequenceid_t a_id)
{
    Pr_TimeSequence * lp_sequences;

    if (!ap_tl || !a_id) return NULL;

    if (a_id >= Pr_ArraySize(ap_tl->sequences)) return NULL;

    lp_sequences = Pr_GetArrayData(ap_tl->sequences);

    return &lp_sequences[a_id];
}

long s_Pr_CompareFloats(float a_1, float a_2)
{
     float l_diff = a_1 - a_2;
     float l_epsi = 0.000001f;

     if (l_diff > l_epsi) return 1;
     if (l_diff < -l_epsi) return -1;
     
     return 0;  
}

void               Pr_StepTimeLine(Pr_TimeLine * ap_tl, float a_dt)
{
    pr_u32_t l_i;
    Pr_TimeSequence * lp_sequences;

    if (!ap_tl) return;

    lp_sequences = Pr_GetArrayData(ap_tl->sequences);

    for (l_i=1 ; l_i<Pr_ArraySize(ap_tl->sequences) ; l_i++) {
        Pr_TimeSequence * lp_seq = &lp_sequences[l_i];
        if (lp_seq->active) {
            float l_step    = a_dt;
            long l_dir      = s_Pr_CompareFloats(l_step, 0.f);

            while (s_Pr_CompareFloats(l_step, 0.f)) {
                Pr_Phrase * lp_p = Pr_ListIteratorData(lp_seq->currentPhrase);
                float l_advance = 0.f;

                if (l_dir > 0) {
                    float l_rest;

                    if (!lp_p) break;

                    if (s_Pr_CompareFloats(lp_p->currentTime, 0.f) == 0) {
                        if (lp_p->startCue.callback) {
                            lp_p->startCue.callback(lp_p->startCue.data);
                        }
                    }

                    l_rest = lp_p->duration - lp_p->currentTime;
                    
                    l_advance = (l_step > l_rest) ? l_rest : l_step;

                    lp_p->currentTime += l_advance;

                    if (lp_p->valueModifier) {
                        lp_p->valueModifier(lp_p->data, lp_p->timeModifier, lp_p->currentTime, lp_p->duration);
                    }

                    if (lp_p->currentTime >= lp_p->duration) {
                        if (lp_p->endCue.callback) {
                            lp_p->endCue.callback(lp_p->endCue.data);
                        }

                        lp_p->currentTime = lp_p->duration;
                        lp_seq->currentPhrase = Pr_NextListIterator(lp_seq->currentPhrase);
                    }
                } else {
                    float l_rest;
                    Pr_ListIterator lp_it;

                    if (!lp_p) {
                        lp_it = Pr_PreviousListIterator(lp_seq->currentPhrase);

                        /* very end of list */
                        if (!lp_it) {
                            lp_it = Pr_ListBack(lp_seq->phrases);
                        }

                        if (lp_it) {
                            lp_seq->currentPhrase = lp_it;
                            lp_p = Pr_ListIteratorData(lp_it);
                        } else {
                            break;
                        }
                    }

                    if (s_Pr_CompareFloats(lp_p->currentTime, lp_p->duration) == 0) {
                        if (lp_p->endCue.callback) {
                            lp_p->endCue.callback(lp_p->endCue.data);
                        }
                    }

                    l_rest = lp_p->currentTime;

                    l_advance = ((- l_step) > l_rest) ? l_rest : (-l_step);

                    lp_p->currentTime -= l_advance;

                    if (lp_p->valueModifier) {
                        lp_p->valueModifier(lp_p->data, lp_p->timeModifier, lp_p->currentTime, lp_p->duration);
                    }

                    if (lp_p->currentTime <= 0.f) {
                        if (lp_p->startCue.callback) {
                            lp_p->startCue.callback(lp_p->startCue.data);
                        }

                        lp_it = Pr_PreviousListIterator(lp_seq->currentPhrase);

                        lp_p->currentTime = 0.f;

                        if (lp_it) {
                            lp_seq->currentPhrase = lp_it;
                        } else {
                            break;
                        }
                    }
                }

                l_step -= l_advance;
            }
        }
    }

    ap_tl->time += a_dt;
    if (ap_tl->time < 0.f) {
        ap_tl->time = 0.f;
    }
}

/*
    
static Pr_Phrase * s_Pr_NewPhraseHandle(Pr_Phrase * ap_p)
{
    Pr_PhraseHandle * lp_out = malloc(sizeof(Pr_PhraseHandle));
    if (!) return NULL;

    if (!ap_p->timeModifier) {
        ap_p->timeModifier = s_Pr_LinearTimeModifier;
    }

    lp_out->info = ap_p;

    lp_out->currentTime = 0.f;

    lp_out->data = malloc(ap_p->dataSize);
    if (lp_out->data) {
        return lp_out;
    }

    free(lp_out);

    return NULL;
}

*/

static Pr_Phrase * s_Pr_NewPhrase(Pr_Phrase * ap_p)
{
    Pr_Phrase * lp_out = malloc(sizeof(Pr_Phrase));
    if (!lp_out) return NULL;

    memcpy(lp_out, ap_p, sizeof(Pr_Phrase));

    if (!lp_out->timeModifier) {
        lp_out->timeModifier = s_Pr_LinearTimeModifier;
    }

    return lp_out;
}

/*

static void s_Pr_DeletePhraseHandle(Pr_Phrase * ap_p)
{
    free(ap_p->data);
    free(ap_p);
}

*/

static void s_Pr_DeletePhrase(Pr_Phrase * ap_p)
{
    free(ap_p);
}

pr_bool_t Pr_AddTimeSequencePhrase(Pr_TimeSequence * ap_seq, Pr_Phrase * ap_p)
{
    Pr_Phrase * lp_phrase;
    /* Pr_PhraseHandle * lp_phrase; */
    Pr_ListIterator lp_it;

    if (!ap_seq || !ap_p) return PR_FALSE;

    lp_phrase = s_Pr_NewPhrase(ap_p);
    /* lp_phrase = s_Pr_NewPhraseHandle(ap_p); */

    if (!lp_phrase) return PR_FALSE;

    lp_it = Pr_PushBackList(ap_seq->phrases, lp_phrase);
    if (lp_it) {
        if (!ap_seq->currentPhrase) {
            ap_seq->currentPhrase = lp_it;
        }

        return PR_TRUE;
    }

    s_Pr_DeletePhrase(lp_phrase);
    /* s_Pr_DeletePhraseHandle(lp_phrase); */

    return PR_FALSE;
}

void Pr_SetTimeSequenceEnabled(Pr_TimeSequence * ap_seq, pr_bool_t a_active)
{
    if (!ap_seq) return;

    ap_seq->active = a_active;
}

Pr_Phrase * Pr_GetCurrentSequencePhrase(Pr_TimeSequence * ap_seq)
{
    if (!ap_seq) return NULL;

    return Pr_ListIteratorData(ap_seq->currentPhrase);
}

void               Pr_ClearTimeLine(Pr_TimeLine * ap_tl)
{
    Pr_TimeSequence * lp_sequences;
    pr_u32_t l_i;

    if (!ap_tl) return;

    lp_sequences = Pr_GetArrayData(ap_tl->sequences);

    for (l_i=1 ; l_i<Pr_ArraySize(ap_tl->sequences) ; l_i++) {
        Pr_TimeSequence * lp_s = &lp_sequences[l_i];
        Pr_ListIterator lp_it;

        PR_LIST_FOREACH(lp_s->phrases, lp_it) {
            s_Pr_DeletePhrase(Pr_ListIteratorData(lp_it));
        }

        s_Pr_FreeTimeSequence(lp_s);
    }

    ap_tl->time = 0.f;

    Pr_ResizeArray(ap_tl->sequences, 1);
}

void               Pr_SetTimeLineTime(Pr_TimeLine * ap_tl, float a_time)
{
    float   l_diff;

    if (!ap_tl) return;

    l_diff = a_time - ap_tl->time;

    Pr_StepTimeLine(ap_tl, l_diff);
}
