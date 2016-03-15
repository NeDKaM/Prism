/* File name : class.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism class implementation
 * Dependencies :   <Prism/config.h>
*/

#ifndef H_PRISM_CLASS_INCLUDED
#define H_PRISM_CLASS_INCLUDED

#include <Prism/config.h>

PR_CPP_PROTECT_ON

    typedef unsigned long Pr_ClassId;

    typedef void *  (*Pr_Constructor)(void);
    typedef void    (*Pr_Deleter)(void *);

    PR_STRUCT(pr_class_t, Pr_Class);

    extern PRISM_API Pr_Class *     Pr_NewClass(char const *, Pr_Constructor, Pr_Deleter, Pr_ClassRef);
    extern PRISM_API int            Pr_SetClassParameter(Pr_Class *, Pr_ClassRef, char const *);
    extern PRISM_API void           Pr_DeleteClass(Pr_Class *);
    extern PRISM_API Pr_ClassId     Pr_GetClassId(Pr_ClassRef);
    extern PRISM_API char const *   Pr_GetClassName(Pr_ClassRef);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_CLASS_INCLUDED */
