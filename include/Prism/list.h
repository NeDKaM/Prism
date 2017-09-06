/* File name : list.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C Linked list implementation
 * Dependencies :   <Prism/config.h>
 */

#ifndef H_PRISM_LIST_INCLUDED
#define H_PRISM_LIST_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

	PR_STRUCT(pr_list_t, Pr_List);

	typedef struct pr_node_t * Pr_ListIterator;

    extern PRISM_API Pr_List *          Pr_NewList(void);
    extern PRISM_API void               Pr_DeleteList(Pr_List *);
    extern PRISM_API void               Pr_ClearList(Pr_List *);
    extern PRISM_API Pr_List *          Pr_CopyList(Pr_List *);

    extern PRISM_API Pr_ListIterator    Pr_PushBackList(Pr_List *, void *);
    extern PRISM_API Pr_ListIterator    Pr_PushFrontList(Pr_List *, void *);

    extern PRISM_API Pr_ListIterator    Pr_EraseListElement(Pr_List *, Pr_ListIterator);

    extern PRISM_API pr_u32_t           Pr_ListSize(Pr_List *);
    
    extern PRISM_API Pr_ListIterator    Pr_ListBegin(Pr_List *);
    extern PRISM_API Pr_ListIterator    Pr_ListBack(Pr_List *);
    extern PRISM_API Pr_ListIterator    Pr_NextListIterator(Pr_ListIterator);
    extern PRISM_API Pr_ListIterator    Pr_PreviousListIterator(Pr_ListIterator);
    extern PRISM_API void *             Pr_ListIteratorData(Pr_ListIterator);
    extern PRISM_API void               Pr_SetListIteratorData(Pr_ListIterator, void *);

    extern PRISM_API void               Pr_SwapListElements(Pr_ListIterator, Pr_ListIterator);

    #define	PR_LIST_FOREACH(list, it) \
        for ((it)=Pr_ListBegin(list) ; (it)!=NULL ; (it)=Pr_NextListIterator(it))

    typedef int (*Pr_ListComparator)(void const *, void const *);

    extern PRISM_API void Pr_SortList(Pr_ListIterator ap_from, Pr_ListIterator ap_to, Pr_ListComparator af_comp);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_LIST_INCLUDED */


