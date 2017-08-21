/* File name : sigslots.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C signal/slot implementation
 * Dependencies :	<Prism/list.h>, 
					<Prism/sigslot.h>,
					<stdlib.h>
 */

#include <Prism/list.h>
#include <Prism/sigslot.h>
#include <stdlib.h>
#include <Prism/memory.h>

typedef struct pr_connection_t Pr_Connection;
struct pr_connection_t {
    Pr_Slot slot;
    void *  obj;
};

struct pr_signal_t {
    Pr_List * slots;
    Pr_List * relays;
    Pr_List * emitters;
};

Pr_Signal * Pr_NewSignal(void)
{
    Pr_Signal * lp_out = malloc(sizeof(Pr_Signal));

    if (lp_out) {
        lp_out->slots = Pr_NewList();
        lp_out->relays = Pr_NewList();
        lp_out->emitters = Pr_NewList();
        if (lp_out->slots 
            && lp_out->relays
            && lp_out->emitters
        ) {
            return lp_out;
        }

        Pr_DeleteList(lp_out->relays);
        Pr_DeleteList(lp_out->slots);
        Pr_DeleteList(lp_out->emitters);

        free(lp_out);
    }

    return NULL;
}

void Pr_DeleteSignal(Pr_Signal * ap_sig)
{
    Pr_ListIterator lp_it;

    if (!ap_sig) return;

    PR_LIST_FOREACH(ap_sig->slots, lp_it) {
        Pr_Connection * lp_tmp = Pr_ListIteratorData(lp_it);
        if (lp_tmp) {
            free(lp_tmp);
        }
    }

    PR_LIST_FOREACH(ap_sig->emitters, lp_it) {
        Pr_ListIterator lp_itrelay;
        Pr_Signal * lp_emitter = Pr_ListIteratorData(lp_it);

        for (lp_itrelay=Pr_ListBegin(lp_emitter->relays) ; lp_itrelay!=NULL ; ) {
            Pr_Signal * lp_relay = Pr_ListIteratorData(lp_itrelay);
            if (lp_relay == ap_sig) {
                lp_itrelay = Pr_EraseListElement(lp_itrelay);
                continue;
            }

            lp_itrelay = Pr_NextListIterator(lp_itrelay);
        }
    }

    PR_LIST_FOREACH(ap_sig->relays, lp_it) {
        Pr_Signal * lp_relay = Pr_ListIteratorData(lp_it);
        Pr_ListIterator lp_itemitter;

        for (lp_itemitter=Pr_ListBegin(lp_relay->emitters) ; lp_itemitter!=NULL ; ) {
            Pr_Signal * lp_emitter = Pr_ListIteratorData(lp_itemitter);
            if (lp_emitter == ap_sig) {
                lp_itemitter = Pr_EraseListElement(lp_itemitter);
                continue;
            }

            lp_itemitter = Pr_NextListIterator(lp_itemitter);
        }
    }

    Pr_DeleteList(ap_sig->relays);
    Pr_DeleteList(ap_sig->slots);
    Pr_DeleteList(ap_sig->emitters);
    free(ap_sig);
}

static void s_Pr_EmitArgs(Pr_Signal * ap_sig, va_list ap_args)
{
    Pr_ListIterator lp_it;

    PR_LIST_FOREACH(ap_sig->relays, lp_it) {
        va_list lp_tmp;

        va_copy(lp_tmp, ap_args);

        s_Pr_EmitArgs(Pr_ListIteratorData(lp_it), lp_tmp);

        va_end(lp_tmp);
    }  

    PR_LIST_FOREACH(ap_sig->slots, lp_it) {
        Pr_Connection * lp_c = Pr_ListIteratorData(lp_it);
        va_list lp_tmp;

        va_copy(lp_tmp, ap_args);

        lp_c->slot(lp_c->obj, lp_tmp);

        va_end(lp_tmp);
    }
}

void Pr_Emit(Pr_Signal * ap_sig, ...)
{
    va_list lp_args;

    if (!ap_sig) return;

    va_start(lp_args, ap_sig);

    s_Pr_EmitArgs(ap_sig, lp_args);

    va_end(lp_args);
}

pr_bool_t Pr_Connect(Pr_Signal * ap_sig, void * ap_to, Pr_Slot ap_slot)
{
    Pr_Connection * lp_con;

    if (!ap_sig || !ap_slot) return PR_FALSE; 

    lp_con = malloc(sizeof(Pr_Connection));
    if (!lp_con) return PR_FALSE;

    lp_con->obj = ap_to;
    lp_con->slot = ap_slot;

    if (Pr_PushBackList(ap_sig->slots, lp_con)) {
        return PR_TRUE;
    }

    free(lp_con);
    return PR_FALSE;
}

pr_bool_t Pr_ConnectRelay(Pr_Signal * ap_sig, Pr_Signal * ap_relay)
{
    Pr_ListIterator lp_it;

    if (!ap_sig || ! ap_relay) return PR_FALSE;

    PR_LIST_FOREACH(ap_sig->relays, lp_it) {
        Pr_Signal * lp_tmp = Pr_ListIteratorData(lp_it);
        if (lp_tmp == ap_relay) {
            return PR_FALSE;
        }
    }

    if (Pr_PushBackList(ap_sig->relays, ap_relay)) {
        if (Pr_PushBackList(ap_relay->emitters, ap_sig)) {
            return PR_TRUE;
        }

        Pr_EraseListElement(Pr_ListBack(ap_sig->relays));
    }

    return PR_FALSE;
}

void Pr_SetSignalMuted(Pr_Signal * ap_sig, pr_bool_t a_bool)
{
    if (!ap_sig) return;
}

long Pr_SignalState(Pr_Signal * ap_sig)
{
    if (!ap_sig) return 0;

    return 0;
}

