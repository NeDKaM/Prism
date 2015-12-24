/* File name : list.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C Linked list implementation
 * Dependencies : -
 * Last modifications : 06/7/2015
 */

#ifndef H_PRISM_LIST_INCLUDED
#define H_PRISM_LIST_INCLUDED

#include <Prism/config.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct pr_list_t   Pr_List;
	typedef struct pr_node_t * Pr_ListIterator;

    extern PRISM_API Pr_List *          Pr_NewList(void);
    extern PRISM_API void               Pr_DeleteList(Pr_List * ap_list);
    extern PRISM_API void               Pr_ClearList(Pr_List * ap_list);
    extern PRISM_API int                Pr_PushFrontList(Pr_List * ap_list);
    extern PRISM_API void               Pr_PopFrontList(Pr_List * ap_list);
    extern PRISM_API unsigned int       Pr_ListSize(Pr_List * ap_list);
    extern PRISM_API int                Pr_PushBackList(Pr_List * ap_list);
    extern PRISM_API void               Pr_PopBackList(Pr_List * ap_list);
    extern PRISM_API void *             Pr_ListAt(Pr_List * ap_list, unsigned int a_n);
    extern PRISM_API void *             Pr_FrontList(Pr_List * ap_list);
    extern PRISM_API void *             Pr_BackList(Pr_List * ap_list);
    extern PRISM_API void               Pr_PopListAt(Pr_List * ap_list, unsigned int a_n);
    extern PRISM_API int                Pr_InsertList(Pr_List * ap_list, unsigned int a_n);
    extern PRISM_API int                Pr_SetListDataAt(Pr_List * ap_list, unsigned int a_n, void * ap_data);
    extern PRISM_API int                Pr_GetListDataPosition(Pr_List * ap_list, void * ap_data);
    extern PRISM_API int                Pr_PushBackListData(Pr_List * ap_list, void * ap_data);
    extern PRISM_API Pr_ListIterator    Pr_FrontListIterator(Pr_List * ap_list);
    extern PRISM_API Pr_ListIterator    Pr_NextListIterator(Pr_ListIterator ap_it);
    extern PRISM_API void *             Pr_ListIteratorData(Pr_ListIterator ap_it);
    extern PRISM_API void               Pr_SetListIteratorData(Pr_ListIterator ap_it, void * ap_data);
    extern PRISM_API void               Pr_SwapListElements(Pr_ListIterator ap_1, Pr_ListIterator ap_2);

	#define	PR_LIST_FOREACH(list,it) \
        for ((it)=Pr_FrontListIterator(list) ; (it)!=NULL ; (it)=Pr_NextListIterator(it))

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_LIST_INCLUDED */
