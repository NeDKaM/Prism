#ifndef H_PRISM_STATEMACHINE_INCLUDED
#define H_PRISM_STATEMACHONE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_statemachine_t, Pr_StateMachine);

    PR_STRUCT(pr_state_t, Pr_State) {
        void *              data;
        Pr_StateMachine *   fsm;

        void        (*enter)(Pr_State *);
        void        (*leave)(Pr_State *);
        pr_bool_t   (*update)(Pr_State *, float);
    };

    extern PRISM_API Pr_StateMachine *   Pr_NewStateMachine(Pr_State *);
    extern PRISM_API void                Pr_DeleteStateMachine(Pr_StateMachine *);

    extern PRISM_API pr_bool_t           Pr_UpdateStateMachine(Pr_StateMachine *, float);

    extern PRISM_API void                Pr_ChangeStateMachine(Pr_StateMachine *, Pr_State *);
    extern PRISM_API Pr_State *          Pr_GetCurrentState(Pr_StateMachine *);
    extern PRISM_API Pr_State *          Pr_PopStateMachine(Pr_StateMachine *);
    extern PRISM_API void                Pr_PushStateMachine(Pr_StateMachine *, Pr_State *);
    extern PRISM_API void                Pr_SetStateMachine(Pr_StateMachine *, Pr_State *);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_STATEMACHINE_INCLUDED */

