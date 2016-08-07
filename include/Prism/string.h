/* File name : string.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C string implementation
 * Dependencies :   <Prism/config.h>
 */

#ifndef H_PRISM_STRING_INCLUDED
#define H_PRISM_STRING_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

	typedef enum pr_strcmpvalue_t Pr_Strcmpvalue;
	enum pr_strcmpvalue_t {
		PR_STRING_SMALLER = -1,
		PR_STRING_EQUAL,
		PR_STRING_GREATER,

        PR_STRING_CMPCOUNT
	};

	typedef enum pr_strresult_t Pr_Strresult;
	enum pr_strresult_t {
		PR_STRING_BADALLOC = -1,
		PR_STRING_NOSTRING,
		PR_STRING_NOALLOC,
		PR_STRING_ALLOCOK,
		PR_STRING_UNMODIFIED,

        PR_STRING_RESULTCOUNT
	};

	PR_STRUCT(pr_string_t, Pr_String);

    extern PRISM_API Pr_String *    Pr_NewString(void);
    extern PRISM_API Pr_String *    Pr_NewStringStr(Pr_CStringRef);
    extern PRISM_API void           Pr_DeleteString(Pr_String *);
    extern PRISM_API Pr_Strresult   Pr_SetStringStr(Pr_String *, Pr_CStringRef);
    extern PRISM_API Pr_Strresult   Pr_SetStringChar(Pr_String *, char);
    extern PRISM_API Pr_Strresult   Pr_SetString(Pr_String *, Pr_String *);
    extern PRISM_API Pr_Strcmpvalue Pr_StringSizeCmp(Pr_StringRef, Pr_StringRef);
    extern PRISM_API Pr_Strresult   Pr_StringStrAppend(Pr_String *, Pr_CStringRef);
    extern PRISM_API Pr_Strresult   Pr_StringCharAppend(Pr_String *, char);
    extern PRISM_API Pr_String *    Pr_StringAppendResult(Pr_CStrRef, Pr_CStringRef);
    extern PRISM_API Pr_Strresult   Pr_StringStrInsert(Pr_String *, unsigned long, Pr_CStringRef);
    extern PRISM_API Pr_Strresult   Pr_StringCharInsert(Pr_String *, unsigned long, char);
    extern PRISM_API Pr_Strresult   Pr_StringCut(Pr_String *, unsigned long);
    extern PRISM_API Pr_String *    Pr_StringSubStr(Pr_String *, unsigned long, unsigned long);
    extern PRISM_API Pr_String *    Pr_StringNSubStr(Pr_String *, unsigned long, unsigned long);
    extern PRISM_API Pr_Strresult   Pr_StringRemove(Pr_String *, unsigned long, unsigned long);
    extern PRISM_API Pr_Strresult   Pr_StringNRemove(Pr_String *, unsigned long, unsigned long);
    extern PRISM_API void           Pr_StringClear(Pr_String *);
    extern PRISM_API unsigned long  Pr_StringSize(Pr_StringRef);
    extern PRISM_API unsigned long  Pr_StringCapacity(Pr_StringRef);
    extern PRISM_API Pr_CStringRef  Pr_StringCStr(Pr_StringRef);
    extern PRISM_API unsigned long  Pr_StringReplace(Pr_String *, Pr_CStringRef, Pr_CStringRef);
    extern PRISM_API long           Pr_StringFind(Pr_StringRef, Pr_CStringRef);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_STRING_INCLUDED */
