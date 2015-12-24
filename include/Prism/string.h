/* File name : string.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C string implementation
 * Dependencies : -
 * Last modifications : 18/7/2015
 */

#ifndef H_PRISM_STRING_INCLUDED
#define H_PRISM_STRING_INCLUDED

#include <Prism/config.h>

#ifdef __cplusplus
extern "C"
{
#endif

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

	typedef struct pr_string_t Pr_String;

    extern PRISM_API Pr_String *    Pr_NewString(void);
    extern PRISM_API Pr_String *    Pr_NewStringStr(char const * ap_str);
    extern PRISM_API void           Pr_DeleteString(Pr_String * ap_this);
    extern PRISM_API Pr_Strresult   Pr_SetStringStr(Pr_String * ap_this, char const * ap_str);
    extern PRISM_API Pr_Strresult   Pr_SetStringChar(Pr_String * ap_this, char a_c);
    extern PRISM_API Pr_Strresult   Pr_SetString(Pr_String * ap_this, Pr_String * ap_str);
    extern PRISM_API Pr_Strcmpvalue Pr_StringSizeCmp(Pr_String * ap_str1, Pr_String * ap_str2);
    extern PRISM_API Pr_Strresult   Pr_StringStrAppend(Pr_String * ap_this, char const * ap_str);
    extern PRISM_API Pr_Strresult   Pr_StringCharAppend(Pr_String * ap_this, char a_c);
    extern PRISM_API Pr_String *    Pr_StringAppendResult(char const * ap_str1, char const * ap_str2);
    extern PRISM_API Pr_Strresult   Pr_StringStrInsert(Pr_String * ap_this, unsigned long a_at, char const * ap_str);
    extern PRISM_API Pr_Strresult   Pr_StringCharInsert(Pr_String * ap_this, unsigned long a_at, char a_c);
    extern PRISM_API Pr_Strresult   Pr_StringCut(Pr_String * ap_this, unsigned long a_at);
    extern PRISM_API Pr_String *    Pr_StringSubStr(Pr_String * ap_str, unsigned long a_begin, unsigned long a_end);
    extern PRISM_API Pr_String *    Pr_StringNSubStr(Pr_String * ap_str, unsigned long a_at, unsigned long a_size);
    extern PRISM_API Pr_Strresult   Pr_StringRemove(Pr_String * ap_this, unsigned long a_begin, unsigned long a_end);
    extern PRISM_API Pr_Strresult   Pr_StringNRemove(Pr_String * ap_this, unsigned long a_at, unsigned long a_size);
    extern PRISM_API void           Pr_StringClear(Pr_String * ap_this);
    extern PRISM_API unsigned long  Pr_StringSize(Pr_String * ap_this);
    extern PRISM_API unsigned long  Pr_StringCapacity(Pr_String * ap_this);
    extern PRISM_API char *         Pr_StringCStr(Pr_String * ap_this);
    extern PRISM_API unsigned long  Pr_StringReplace(Pr_String * ap_this, char const * ap_str, char const * ap_by);
    extern PRISM_API long           Pr_StringFind(Pr_String * ap_this, char const * ap_str);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_STRING_INCLUDED */
