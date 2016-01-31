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

typedef struct pr_connection_t Pr_Connection;
struct pr_connection_t {
    Pr_Slot slot;
    void * obj;
};

struct pr_signal_t {
    Pr_List * slots;
};

Pr_Signal * Pr_NewSignal(void)
{
    Pr_Signal * lp_out = malloc(sizeof(Pr_Signal));

    if (lp_out) {
        lp_out->slots = Pr_NewList();
        if (lp_out->slots) {
            return lp_out;
        }

        free(lp_out);
    }

    return NULL;
}

void Pr_DeleteSignal(Pr_Signal * ap_sig)
{
    if (ap_sig) {
        Pr_DeleteList(ap_sig->slots);
        free(ap_sig);
    }
}

void Pr_Emit(Pr_Signal * ap_sig, ...)
{
    va_list lp_args;

    if (ap_sig) {
        Pr_ListIterator lp_it;

        va_start(lp_args,ap_sig);

        PR_LIST_FOREACH(ap_sig->slots,lp_it) {
            Pr_Connection * lp_c = Pr_ListIteratorData(lp_it);
            lp_c->slot(lp_c->obj,lp_args);
        }

        va_end(lp_args);
    }
}

int Pr_Connect(Pr_Signal * ap_sig, void * ap_to, Pr_Slot ap_slot)
{
    Pr_Connection * lp_con;

    if (!ap_sig || !ap_slot) return 0; 

    lp_con = malloc(sizeof(lp_con));
    if (!lp_con) return 0;

    lp_con->obj = ap_to;
    lp_con->slot = ap_slot;

    if (Pr_PushBackListData(ap_sig->slots, lp_con)) {
        return 1;
    }

    free(lp_con);
    return 0;
}
