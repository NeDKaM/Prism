#include <Prism/statemachine.h>
#include <Prism/list.h>

#include <stdlib.h>

struct pr_statemachine_t {
    Pr_List * states;
};

Pr_StateMachine *   Pr_NewStateMachine(Pr_State * ap_state)
{
    Pr_StateMachine * lp_out;

    if (!ap_state) return NULL;

    lp_out = malloc(sizeof(Pr_StateMachine));
    if (!lp_out) return NULL;

    lp_out->states = Pr_NewList();
    if (!lp_out->states) {
        free(lp_out);
        return NULL;
    }

    Pr_PushStateMachine(lp_out, ap_state);

    return lp_out;
}

void                Pr_DeleteStateMachine(Pr_StateMachine * ap_fsm)
{
    Pr_ListIterator lp_it;

    if (!ap_fsm) return;

    PR_LIST_FOREACH(ap_fsm->states, lp_it) {
        Pr_State * lp_state = Pr_ListIteratorData(lp_it);
        lp_state->leave(lp_state);
    }

    Pr_DeleteList(ap_fsm->states);
    free(ap_fsm);
}

void        PR_SLOT(Pr_UpdateStateMachine)(void * ap_obj, va_list ap_args)
{
    if (!ap_args) return;

    Pr_UpdateStateMachine(ap_obj, va_arg(ap_args, float));
}

void        Pr_UpdateStateMachine(Pr_StateMachine * ap_fsm, float a_time)
{
    Pr_ListIterator lp_it;

    if (!ap_fsm) return;

    PR_LIST_FOREACH(ap_fsm->states, lp_it) {
        Pr_State * lp_state = Pr_ListIteratorData(lp_it);
        if (!lp_state->update(lp_state, a_time)) {
            return;
        }
    }
}

void                Pr_ChangeStateMachine(Pr_StateMachine * ap_fsm, Pr_State * ap_state)
{
    if (ap_fsm && ap_state) {
        Pr_PopStateMachine(ap_fsm);
        Pr_PushStateMachine(ap_fsm, ap_state);
    }
}

Pr_State *          Pr_GetCurrentState(Pr_StateMachine * ap_fsm)
{
    return (ap_fsm) ? Pr_ListIteratorData(Pr_ListBack(ap_fsm->states)) : NULL;
}

Pr_State *          Pr_PopStateMachine(Pr_StateMachine * ap_fsm)
{
    Pr_State *      lp_state;
    Pr_ListIterator lp_it;

    if (!ap_fsm) return NULL;

    if (!Pr_ListSize(ap_fsm->states)) return NULL;

    lp_it = Pr_ListBack(ap_fsm->states);

    lp_state = Pr_ListIteratorData(lp_it);
    if (lp_state) {
        lp_state->leave(lp_state);
        Pr_EraseListElement(lp_it);
    }

    return lp_state;
}

void                Pr_PushStateMachine(Pr_StateMachine * ap_fsm, Pr_State * ap_state)
{
    if (ap_fsm && ap_state) {
        Pr_ListIterator lp_it;

        PR_LIST_FOREACH(ap_fsm->states, lp_it) {
            if (Pr_ListIteratorData(lp_it) == ap_state) return; 
        }

        if (Pr_PushBackList(ap_fsm->states, ap_state)) {
            ap_state->fsm = ap_fsm;
            ap_state->enter(ap_state);
        }
    }
}

void                Pr_SetStateMachine(Pr_StateMachine * ap_fsm, Pr_State * ap_state)
{
    if (ap_fsm && ap_state) {
        while (Pr_ListSize(ap_fsm->states)) {
            Pr_PopStateMachine(ap_fsm);
        }

        Pr_PushStateMachine(ap_fsm, ap_state);
    }
}

