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

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct pr_list_t   Pr_List;
	typedef struct pr_node_t * Pr_ListIterator;

	Pr_List *       Pr_NewList      (void);
	void            Pr_DeleteList   (Pr_List * ap_list);
	void            Pr_ClearList    (Pr_List * ap_list);
	int             Pr_PushFrontList(Pr_List * ap_list);
	void            Pr_PopFrontList (Pr_List * ap_list);
	unsigned int    Pr_ListSize     (Pr_List * ap_list);
	int             Pr_PushBackList (Pr_List * ap_list);
	void            Pr_PopBackList  (Pr_List * ap_list);
	void *          Pr_ListAt       (Pr_List * ap_list, unsigned int a_n);
	void *          Pr_FrontList    (Pr_List * ap_list);
	void *          Pr_BackList     (Pr_List * ap_list);
	void            Pr_PopListAt    (Pr_List * ap_list, unsigned int a_n);
	int             Pr_InsertList   (Pr_List * ap_list, unsigned int a_n);
	int             Pr_SetListDataAt        (Pr_List * ap_list, unsigned int a_n, void * ap_data);
	int             Pr_GetListDataPosition  (Pr_List * ap_list, void * ap_data);
	int             Pr_PushBackListData     (Pr_List * ap_list, void * ap_data);
	Pr_ListIterator Pr_FrontListIterator    (Pr_List * ap_list);
	Pr_ListIterator Pr_NextListIterator     (Pr_ListIterator ap_it);
	void *          Pr_ListIteratorData     (Pr_ListIterator ap_it);
	void			Pr_SetListIteratorData	(Pr_ListIterator ap_it, void * ap_data);

	#define         PR_LIST_FOREACH(list,it) \
						for ((it)=Pr_FrontListIterator(list) ; \
							(it)!=NULL ; \
							(it)=Pr_NextListIterator(it))

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_LIST_INCLUDED */
