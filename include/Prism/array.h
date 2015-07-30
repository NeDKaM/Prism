/* File name : array.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Array implementation
 * Dependencies :	<stdlib.h>
 * Last modifications : 24/7/2015
 */

#ifndef H_PRISM_ARRAY_INCLUDED
#define H_PRISM_ARRAY_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define PR_ARRAY(a_Type,a_Name) \
	struct Pr_##a_Name { \
		a_Type * list; \
		unsigned long size; \
		unsigned long capacity; \
		int reducible; \
	} a_Name

#define Pr_SetArrayDynamic(a_array,a_bool) \
	do { \
		a_array.reducible = a_bool; \
	} while (0)

#define Pr_MakeArray(a_array) \
	do { \
		a_array.size = 0; \
		a_array.capacity = 0; \
		a_array.list = NULL; \
		a_array.reducible = 0; \
	} while (0)

#define Pr_MakeSizedArray(a_array,a_size) \
	do { \
		unsigned long l_cap = 1; \
		\
		if (a_size < 1) { \
			Pr_MakeArray(a_array); \
			break; \
		} \
		\
		while (l_cap < a_size) { \
			l_cap *= 2; \
		} \
		\
		a_array.list = malloc(l_cap*sizeof(*a_array.list)); \
		if (a_array.list) { \
			a_array.size = a_size; \
			a_array.capacity = l_cap; \
			a_array.reducible = 0; \
		} else { \
			Pr_MakeArray(a_array); \
		} \
	} while (0) 

#define Pr_ClearArray(a_array) \
	do { \
		a_array.size = 0; \
		a_array.capacity = 0; \
		free(a_array.list); \
		a_array.list = NULL; \
	} while (0)

#define Pr_PushBackArray(a_array) \
	do { \
		if (!a_array.size) { \
			a_array.list = malloc(sizeof(*a_array.list)); \
			if (a_array.list) { \
				a_array.size++; \
				a_array.capacity = 1; \
			} \
		} else { \
			if (a_array.size == a_array.capacity) { \
				void * lp_tmp = realloc(a_array.list,sizeof(*a_array.list) * a_array.capacity * 2); \
				if (lp_tmp) { \
					a_array.list = lp_tmp; \
					a_array.size++; \
					a_array.capacity *= 2; \
				} \
			} else { \
				a_array.size++; \
			} \
		} \
	} while (0)

#define Pr_PopBackArray(a_array) \
	do { \
		if (a_array.size) { \
			a_array.size--; \
			if (a_array.reducible) { \
				unsigned long l_cap = a_array.capacity; \
				while (a_array.size <= (l_cap >> 1)) { \
					l_cap >>= 1; \
				} \
				\
				if (a_array.capacity) { \
					void * lp_tmp = realloc(a_array.list,l_cap); \
					if (lp_tmp) { \
						a_array.list = lp_tmp; \
						a_array.capacity = l_cap; \
					} \
				} else { \
					free(a_array.list); \
					a_array.list = NULL; \
				} \
			} \
		} \
	} while (0)

#define Pr_ExtendArray(a_array,a_size) \
	do { \
		if (!a_array.size) { \
			Pr_MakeSizedArray(a_array,a_size); \
		} else { \
			if (a_size > 0) { \
				unsigned long l_size = a_size + a_array.size; \
				unsigned long l_cap = a_array.capacity; \
				void * lp_tmp; \
				\
				while (l_cap < l_size) { \
					l_cap *= 2; \
				} \
				\
				lp_tmp = realloc(a_array.list, l_cap*sizeof(*a_array.list)); \
				if (lp_tmp) { \
					a_array.list = lp_tmp; \
					a_array.size = l_size; \
					a_array.capacity = l_cap; \
				} \
			} \
		} \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif