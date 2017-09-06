#ifndef H_PRISM_TIMING_INCLUDED
#define H_PRISM_TIMING_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_timeline_t, Pr_TimeLine);

    PR_STRUCT(pr_timesequence_t, Pr_TimeSequence);

    typedef float (*Pr_TimeModifier)(float);

    PR_STRUCT(pr_phrasecue_t, Pr_PhraseCue) {
        void *  data;
        void    (*callback)(void *);
    };

    typedef void (*Pr_ValueModifier)(
        void * data,
        Pr_TimeModifier, 
        float time, 
        float duration
    );

    PR_STRUCT(pr_phrase_t, Pr_Phrase) {
        float               currentTime;
        float               duration;
        void *              data;
        Pr_PhraseCue        startCue;
        Pr_PhraseCue        endCue;
        Pr_TimeModifier     timeModifier;
        Pr_ValueModifier    valueModifier;
    };
    
    typedef pr_u32_t pr_sequenceid_t;

    extern PRISM_API Pr_TimeLine *      Pr_NewTimeLine(void);
    extern PRISM_API void               Pr_DeleteTimeLine(Pr_TimeLine *);
    extern PRISM_API pr_sequenceid_t    Pr_CreateTimeLineSequence(Pr_TimeLine *);
    extern PRISM_API void               Pr_StepTimeLine(Pr_TimeLine *, float);
    extern PRISM_API Pr_TimeSequence *  Pr_GetTimeLineSequence(Pr_TimeLine *, pr_sequenceid_t);
    extern PRISM_API void               Pr_ClearTimeLine(Pr_TimeLine *);
    extern PRISM_API void               Pr_SetTimeLineTime(Pr_TimeLine *, float);

    extern PRISM_API void           Pr_SetTimeSequenceEnabled(Pr_TimeSequence *, pr_bool_t);
    extern PRISM_API pr_bool_t      Pr_AddTimeSequencePhrase(Pr_TimeSequence *, Pr_Phrase *);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_TIMING_INCLUDED */

/*

*/
