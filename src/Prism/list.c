/* File name : list.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C Linked list implementation
 * Dependencies :	<stdlib.h>, 
					<Prism/list.h>
 */

#include <stdlib.h>
#include <Prism/list.h>

typedef struct pr_node_t pr_node_t;
struct pr_node_t {
    pr_node_t * next;
    pr_node_t * prev;
    void *      data;
    Pr_List *   owner;
};

struct pr_list_t {
    pr_node_t * first;
    pr_node_t * last;
    pr_u32_t    size;
};

static pr_node_t * s_Pr_NewNode(Pr_List * ap_list, void * ap_data)
{
    pr_node_t * lp_out = malloc(sizeof(pr_node_t));

    if (lp_out) {
        lp_out->data = ap_data;
        lp_out->next = NULL;
        lp_out->prev = NULL;
        lp_out->owner = ap_list;
    }

    return lp_out;
}

//static pr_node_t * Pr_NodeAt(Pr_List * ap_list, pr_u32_t a_n)
//{
//    pr_node_t * lp_out = NULL;
//
//    if (ap_list) {
//        if (a_n < ap_list->size) {
//            pr_u32_t l_i=0;
//            lp_out = ap_list->first;
//
//            while (lp_out && l_i != a_n) {
//                lp_out = lp_out->next;
//                l_i++;
//            }
//        }
//    }
//
//    return lp_out;
//}

Pr_List *       Pr_NewList(void)
{
    Pr_List * lp_out = malloc(sizeof(Pr_List));

    if (lp_out) {
        lp_out->first = NULL;
        lp_out->size = 0;
        lp_out->last = NULL;
    }

    return lp_out;
}

void            Pr_DeleteList(Pr_List * ap_list)
{
    if (ap_list) {
        Pr_ClearList(ap_list);
        free(ap_list);
    }
}

void            Pr_ClearList(Pr_List * ap_list)
{
    if (ap_list) {
        while (ap_list->first) {
            Pr_EraseListElement(Pr_ListBegin(ap_list));
        }
    }
}

Pr_ListIterator Pr_PushBackList(Pr_List * ap_list, void * ap_data)
{
    Pr_ListIterator lp_out;

    if (!ap_list || !ap_data) return NULL;

    lp_out = s_Pr_NewNode(ap_list, ap_data);
    if (!lp_out) return NULL;

    ap_list->size++;

    if (!ap_list->first) {
        ap_list->first = lp_out;
        ap_list->last = lp_out;
        return lp_out;
    }

    lp_out->prev = ap_list->last;
    ap_list->last->next = lp_out;
    ap_list->last = lp_out;

    return lp_out;
}

Pr_ListIterator Pr_PushFrontList(Pr_List * ap_list, void * ap_data)
{
    Pr_ListIterator lp_out;

    if (!ap_list || !ap_data) return NULL;

    lp_out = s_Pr_NewNode(ap_list, ap_data);
    if (!lp_out) return NULL;

    ap_list->size++;

    if (!ap_list->first) {
        ap_list->first = lp_out;
        ap_list->last = lp_out;
        return lp_out;
    }

    ap_list->first->prev = lp_out;
    lp_out->next = ap_list->first;
    ap_list->first = lp_out;

    return lp_out;
}

Pr_ListIterator Pr_EraseListElement(Pr_ListIterator ap_it)
{
    Pr_ListIterator lp_out = ap_it->next;

    if (ap_it == ap_it->owner->first) {
        ap_it->owner->first = lp_out;
        if (lp_out) {
            lp_out->prev = NULL;
        } else {
            ap_it->owner->last = NULL;
        }
    } else {
        ap_it->prev->next = lp_out;
        if (lp_out) {
            lp_out->prev = ap_it->prev;
        }
    }

    ap_it->owner->size--;

    free(ap_it);

    return lp_out;
}

pr_u32_t        Pr_ListSize(Pr_List * ap_list)
{
    return (ap_list) ? ap_list->size : 0;
}

Pr_ListIterator Pr_ListBegin(Pr_List * ap_list)
{
    return ap_list ? ap_list->first : NULL;
}

Pr_ListIterator Pr_ListBack(Pr_List * ap_list)
{
    return ap_list ? ap_list->last : NULL;
}

Pr_ListIterator Pr_NextListIterator(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->next : NULL;
}

Pr_ListIterator Pr_PreviousListIterator(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->prev : NULL;
}

void *          Pr_ListIteratorData(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->data : NULL;
}

void            Pr_SetListIteratorData(Pr_ListIterator ap_it, void * ap_data)
{
	if (ap_it) {
		ap_it->data = ap_data;
	}
}

void            Pr_SwapListElements(Pr_ListIterator ap_1, Pr_ListIterator ap_2)
{
    void * lp_tmp;

    if (!ap_1 && !ap_2) return;

    lp_tmp = ap_1->data;
    ap_1->data = ap_2->data;
    ap_2->data = lp_tmp;
}

Pr_List *       Pr_CopyList(Pr_List * ap_list)
{
    Pr_List * lp_out;
    Pr_ListIterator lp_it;

    if (!ap_list) return NULL;

    lp_out = Pr_NewList();
    if (!lp_out) return NULL;

    lp_out->size = ap_list->size;

    PR_LIST_FOREACH(ap_list, lp_it) {
        if (!Pr_PushBackList(lp_out, Pr_ListIteratorData(lp_it))) {
            Pr_DeleteList(lp_out);
            return NULL;
        }
    }

    return lp_out;
}

static void s_Pr_QSort(Pr_ListIterator ap_from, Pr_ListIterator ap_to, Pr_ListComparator af_comp)
{
    Pr_ListIterator lp_right = ap_to;
    Pr_ListIterator lp_pivot = ap_from;
    Pr_ListIterator lp_left = Pr_NextListIterator(ap_from);

    if (ap_from == ap_to) return;

    while (lp_left != lp_right) {
        if (af_comp(Pr_ListIteratorData(lp_left), Pr_ListIteratorData(lp_pivot))) {
            lp_left = Pr_NextListIterator(lp_left);
        } else {
            while((lp_left != lp_right) 
                && af_comp(Pr_ListIteratorData(lp_pivot), Pr_ListIteratorData(lp_right))
            ) {
                lp_right = Pr_PreviousListIterator(lp_right);
            }

            Pr_SwapListElements(lp_left, lp_right);
        }
    }

    lp_left = Pr_PreviousListIterator(lp_left);

    Pr_SwapListElements(lp_pivot, lp_left);

    s_Pr_QSort(ap_from, lp_left, af_comp);
    s_Pr_QSort(lp_right, ap_to, af_comp);
}

void Pr_SortList(Pr_ListIterator ap_from, Pr_ListIterator ap_to, Pr_ListComparator af_comp)
{
    if (!ap_from || !ap_to || !af_comp) return;

    if (ap_from->owner != ap_to->owner) return;

    s_Pr_QSort(ap_from, ap_to, af_comp);
}

//Pr_ListIterator Pr_PushFrontList(Pr_List * ap_list)
//{
//    Pr_ListIterator lp_it = NULL;
//
//    if (ap_list) {
//        pr_node_t * lp_nf = s_Pr_NewNode();
//        if (lp_nf) {
//            if (ap_list->first) {
//                lp_nf->next = ap_list->first;
//            }
//
//            ap_list->first = lp_nf;
//            ap_list->size++;
//            lp_it = lp_nf;
//        }
//    }
//
//    return lp_it;
//}

//void Pr_PopFrontList(Pr_List * ap_list)
//{
//    if (ap_list) {
//        if (ap_list->first) {
//            pr_node_t * lp_nf = ap_list->first;
//            ap_list->first = lp_nf->next;
//            free(lp_nf);
//            lp_nf = NULL;
//            ap_list->size--;
//        }
//    }
//}

//void * Pr_ListAt(Pr_List * ap_list, pr_u32_t a_n)
//{
//    void * lp_out = NULL;
//    pr_node_t * lp_nd = Pr_NodeAt(ap_list,a_n);
//
//    if (lp_nd) {
//        lp_out = lp_nd->data;
//    }
//
//    return lp_out;
//}

//Pr_ListIterator Pr_PushBackList(Pr_List * ap_list)
//{
//    Pr_ListIterator lp_it = NULL;
//
//    if (ap_list) {
//        pr_u32_t l_size = ap_list->size;
//
//        if (l_size) {
//            pr_node_t * l_n = Pr_NodeAt(ap_list,l_size-1);
//
//            if (l_n) {
//                l_n->next = s_Pr_NewNode();
//                lp_it = l_n->next;
//                ap_list->size++;
//            }
//        } else {
//            lp_it = Pr_PushFrontList(ap_list);
//        }
//    }
//
//    return lp_it;
//}

//void Pr_PopBackList(Pr_List * ap_list)
//{
//    pr_node_t * lp_tmp = Pr_FrontList(ap_list);
//
//    while (lp_tmp->next) {
//        lp_tmp = lp_tmp->next;
//    }
//
//    if (lp_tmp) {
//        free(lp_tmp);
//        lp_tmp = NULL;
//        ap_list->size--;
//    }
//}

//void * Pr_FrontList(Pr_List * ap_list)
//{
//    return Pr_ListAt(ap_list,0);
//}

//void * Pr_BackList(Pr_List * ap_list)
//{
//    void * lp_out = NULL;
//    pr_u32_t l_size = ap_list->size;
//
//    if (l_size) {
//        lp_out = Pr_ListAt(ap_list,l_size-1);
//    }
//
//    return lp_out;
//}

//void Pr_PopListAt(Pr_List * ap_list, pr_u32_t a_n)
//{
//    pr_u32_t l_n = ap_list->size;
//
//    if (ap_list && a_n<l_n) {
//        if (a_n == 0) {
//            Pr_PopFrontList(ap_list);
//        } else if (a_n == l_n-1) {
//            Pr_PopBackList(ap_list);
//        } else {
//            pr_node_t * lp_ptr = Pr_NodeAt(ap_list,a_n-1);
//            if (lp_ptr) {
//                pr_node_t * lp_tmp = lp_ptr->next;
//                lp_ptr->next = lp_tmp->next;
//                free(lp_tmp);
//                ap_list->size--;
//            }
//        }
//    }
//}

//Pr_ListIterator Pr_InsertList(Pr_List * ap_list, pr_u32_t a_n, void * ap_data)
//{
//    Pr_ListIterator lp_it = NULL;
//
//    if (ap_list) {
//        pr_u32_t l_size = ap_list->size;
//        if (a_n == 0 || l_size == 0) {
//            lp_it = Pr_PushFrontList(ap_list);
//        } else if (a_n >= l_size) {
//            lp_it = Pr_PushBackList(ap_list);
//        } else {
//            pr_node_t * lp_tmp = Pr_NodeAt(ap_list,a_n-1);
//            pr_node_t * lp_new = s_Pr_NewNode();
//
//            if (lp_new) {
//                lp_new->next = lp_tmp->next->next;
//                lp_tmp->next = lp_new;
//                ap_list->size++;
//                lp_it = lp_new;
//            }
//        }
//
//        Pr_SetListIteratorData(lp_it, ap_data);
//    }
//
//    return lp_it;
//}

//Pr_ListIterator Pr_PushBackListData(Pr_List * ap_list, void * ap_data)
//{
//    Pr_ListIterator lp_it = NULL;
//
//    if (ap_list) {
//        lp_it = Pr_PushBackList(ap_list);
//        Pr_SetListIteratorData(lp_it, ap_data);
//    }
//
//    return lp_it;
//}

//Pr_ListIterator Pr_FrontListIterator(Pr_List * ap_list)
//{
//    return ap_list ? ap_list->first : NULL;
//}


