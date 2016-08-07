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
    extern PRISM_API Pr_List *          Pr_CopyList(Pr_ListRef);

    extern PRISM_API Pr_ListIterator    Pr_PushFrontList(Pr_List *);
    extern PRISM_API Pr_ListIterator    Pr_PushBackList(Pr_List *);
    extern PRISM_API Pr_ListIterator    Pr_InsertList(Pr_List *, pr_u32_t, void *);
    extern PRISM_API Pr_ListIterator    Pr_PushBackListData(Pr_List *, void *);

    extern PRISM_API void               Pr_PopFrontList(Pr_List *);
    extern PRISM_API void               Pr_PopBackList(Pr_List *);
    extern PRISM_API void               Pr_PopListAt(Pr_List *, pr_u32_t);

    extern PRISM_API pr_u32_t           Pr_ListSize(Pr_ListRef);
    
    extern PRISM_API void *             Pr_ListAt(Pr_ListRef, pr_u32_t);
    extern PRISM_API void *             Pr_FrontList(Pr_ListRef);
    extern PRISM_API void *             Pr_BackList(Pr_ListRef);
    
    extern PRISM_API Pr_ListIterator    Pr_FrontListIterator(Pr_ListRef);
    extern PRISM_API Pr_ListIterator    Pr_NextListIterator(Pr_ListIterator);
    extern PRISM_API void *             Pr_ListIteratorData(Pr_ListIterator);
    extern PRISM_API void               Pr_SetListIteratorData(Pr_ListIterator, void *);

    extern PRISM_API void               Pr_SwapListElements(Pr_ListIterator, Pr_ListIterator);
    

	#define	PR_LIST_FOREACH(list,it) \
        for ((it)=Pr_FrontListIterator(list) ; (it)!=NULL ; (it)=Pr_NextListIterator(it))

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_LIST_INCLUDED */
