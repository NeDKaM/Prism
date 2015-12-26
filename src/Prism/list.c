/* File name : list.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C Linked list implementation
 * Dependencies :	<stdlib.h>, 
					<Prism/list.h>
 * Last modifications : 18/7/2015
 */

#include <stdlib.h>
#include <Prism/list.h>

typedef struct pr_node_t pr_node_t;
struct pr_node_t {
    struct pr_node_t * next;
    void * data;
};

struct pr_list_t {
    pr_node_t * first;
    unsigned int size;
};

static pr_node_t * Pr_NewNode(void)
{
    pr_node_t * lp_out = malloc(sizeof(pr_node_t));

    if (lp_out) {
        lp_out->data = NULL;
        lp_out->next = NULL;
    }

    return lp_out;
}

static pr_node_t * Pr_NodeAt(Pr_List * ap_list, unsigned int a_n)
{
    pr_node_t * lp_out = NULL;

    if (ap_list) {
        if (a_n < ap_list->size) {
            unsigned int l_i=0;
            lp_out = ap_list->first;

            while (lp_out && l_i != a_n) {
                lp_out = lp_out->next;
                l_i++;
            }
        }
    }

    return lp_out;
}

Pr_List * Pr_NewList(void)
{
    Pr_List * lp_out = malloc(sizeof(Pr_List));

    if (lp_out) {
        lp_out->first = NULL;
        lp_out->size = 0;
    }

    return lp_out;
}

void Pr_DeleteList(Pr_List * ap_list)
{
    if (ap_list) {
        Pr_ClearList(ap_list);
        free(ap_list);
    }
}

void Pr_ClearList(Pr_List * ap_list)
{
    if (ap_list) {
        while (ap_list->first) {
            Pr_PopFrontList(ap_list);
        }
    }
}

int Pr_PushFrontList(Pr_List * ap_list)
{
    int l_code = 0;

    if (ap_list) {
        pr_node_t * lp_nf = Pr_NewNode();
        if (lp_nf) {
            if (ap_list->first) {
                lp_nf->next = ap_list->first;
            }

            ap_list->first = lp_nf;
            ap_list->size++;
            l_code = 1;
        }
    }

    return l_code;
}

void Pr_PopFrontList(Pr_List * ap_list)
{
    if (ap_list) {
        if (ap_list->first) {
            pr_node_t * lp_nf = ap_list->first;
            ap_list->first = lp_nf->next;
            free(lp_nf);
            lp_nf = NULL;
            ap_list->size--;
        }
    }
}

void * Pr_ListAt(Pr_List * ap_list, unsigned int a_n)
{
    void * lp_out = NULL;
    pr_node_t * lp_nd = Pr_NodeAt(ap_list,a_n);

    if (lp_nd) {
        lp_out = lp_nd->data;
    }

    return lp_out;
}

unsigned int Pr_ListSize(Pr_List * ap_list)
{
    return (ap_list) ? ap_list->size : 0;
}

int Pr_PushBackList(Pr_List * ap_list)
{
    int l_code = 0;

    if (ap_list) {
        unsigned int l_size = ap_list->size;

        if (l_size) {
            pr_node_t * l_n = Pr_NodeAt(ap_list,l_size-1);

            if (l_n) {
                l_n->next = Pr_NewNode();
                l_code = 1;
                ap_list->size++;
            }
        } else {
            l_code = Pr_PushFrontList(ap_list);
        }
    }

    return l_code;
}

void Pr_PopBackList(Pr_List * ap_list)
{
    pr_node_t * lp_tmp = Pr_FrontList(ap_list);

    while (lp_tmp->next) {
        lp_tmp = lp_tmp->next;
    }

    if (lp_tmp) {
        free(lp_tmp);
        lp_tmp = NULL;
        ap_list->size--;
    }
}

void * Pr_FrontList(Pr_List * ap_list)
{
    return Pr_ListAt(ap_list,0);
}

void * Pr_BackList(Pr_List * ap_list)
{
    void * lp_out = NULL;
    unsigned int l_size = ap_list->size;

    if (l_size) {
        lp_out = Pr_ListAt(ap_list,l_size-1);
    }

    return lp_out;
}

int Pr_SetListDataAt(Pr_List * ap_list, unsigned int a_n, void * ap_data)
{
    pr_node_t * lp_tmp = Pr_NodeAt(ap_list,a_n);
    int code = 0;

    if (lp_tmp) {
        lp_tmp->data = ap_data;
        code = 1;
    }

    return code;
}

int Pr_GetListDataPosition(Pr_List * ap_list, void * ap_data)
{
    int l_out = -1;

    if (ap_list && ap_data) {
        Pr_ListIterator lp_it;
        l_out = 0;

        PR_LIST_FOREACH(ap_list,lp_it) {
            if (Pr_ListIteratorData(lp_it)==ap_data) {
                break;
            } else {
                l_out++;
            }
        }
    }

    return l_out;
}

void Pr_PopListAt(Pr_List * ap_list, unsigned int a_n)
{
    unsigned int l_n = ap_list->size;

    if (ap_list && a_n<l_n) {
        if (a_n == 0) {
            Pr_PopFrontList(ap_list);
        } else if (a_n == l_n-1) {
            Pr_PopBackList(ap_list);
        } else {
            pr_node_t * lp_ptr = Pr_NodeAt(ap_list,a_n-1);
            if (lp_ptr) {
                pr_node_t * lp_tmp = lp_ptr->next;
                lp_ptr->next = lp_tmp->next;
                free(lp_tmp);
                ap_list->size--;
            }
        }
    }
}

int Pr_InsertList(Pr_List * ap_list, unsigned int a_n)
{
    int l_code = 0;

    if (ap_list) {
        unsigned int l_size = ap_list->size;
        if (a_n == 0 || l_size == 0) {
            l_code = Pr_PushFrontList(ap_list);
        } else if (a_n >= l_size) {
            l_code = Pr_PushBackList(ap_list);
        } else {
            pr_node_t * lp_tmp = Pr_NodeAt(ap_list,a_n-1);
            pr_node_t * lp_new = Pr_NewNode();

            if (lp_new) {
                lp_new->next = lp_tmp->next->next;
                lp_tmp->next = lp_new;
                ap_list->size++;
                l_code = 1;
            }
        }
    }

    return l_code;
}

int Pr_PushBackListData(Pr_List * ap_list, void * ap_data)
{
    int l_code = 0;

    if (ap_list) {
        if (Pr_PushBackList(ap_list)) {
            l_code = Pr_SetListDataAt(ap_list,ap_list->size-1,ap_data);
        }
    }

    return l_code;
}

Pr_ListIterator Pr_FrontListIterator(Pr_List * ap_list)
{
    return ap_list ? ap_list->first : NULL;
}

Pr_ListIterator Pr_NextListIterator(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->next : NULL;
}

void * Pr_ListIteratorData(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->data : NULL;
}

void Pr_SetListIteratorData(Pr_ListIterator ap_it, void * ap_data)
{
	if (ap_it) {
		ap_it->data = ap_data;
	}
}

void Pr_SwapListElements(Pr_ListIterator ap_1, Pr_ListIterator ap_2)
{
    void * lp_tmp;

    if (!ap_1 && !ap_2) return;

    lp_tmp = ap_1->data;
    ap_1->data = ap_2->data;
    ap_2->data = lp_tmp;
}
