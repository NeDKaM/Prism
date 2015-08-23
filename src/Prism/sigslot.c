/* File name : sigslots.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C signal/slot implementation
 * Dependencies :	<Prism/list.h>, 
					<Prism/sigslot.h>,
					<stdlib.h>
 * Last modifications : 27/7/2015
 */

/**< Notes :
     (1) : might not be standard ; casting <void*> to <void(*)(va_list)>
>*/

#include <Prism/list.h>
#include <Prism/sigslot.h>
#include <stdlib.h>

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
            Pr_Slot lp_cb = Pr_ListIteratorData(lp_it);
            lp_cb(lp_args);
        }

        va_end(lp_args);
    }
}

int Pr_Connect(Pr_Signal * ap_sig, Pr_Slot ap_slot)
{
    if (!ap_sig || !ap_slot) { return 0; }

    return Pr_PushBackListData(ap_sig->slots,ap_slot);
                                              /**< ^ : (1) >*/
}
