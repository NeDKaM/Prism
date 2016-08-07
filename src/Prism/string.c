/* File name : string.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C string implementation
 * Dependencies :	<Prism/string.h>
					<stdlib.h>, 
					<string.h>, 
					<stdio.h>, 
 */

#include <Prism/string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STRING_UNIT 32

struct pr_string_t {
    unsigned long size;
    unsigned long capacity;
    char * str;
};

Pr_String * Pr_NewString(void)
{
    Pr_String * lp_out = malloc(sizeof(Pr_String));

    if (lp_out) {
        lp_out->str = calloc(STRING_UNIT,sizeof(char));

        if (lp_out->str) {
            lp_out->size = 0;
            lp_out->capacity = STRING_UNIT;
        } else {
            free(lp_out);
            lp_out = NULL;
        }
    }
    return lp_out;
}

Pr_String * Pr_NewStringStr(Pr_CStringRef ap_str)
{
    Pr_String * lp_out = Pr_NewString();

    if (lp_out) {
        if (ap_str) {
            Pr_SetStringStr(lp_out,ap_str);
        }
    }

    return lp_out;
}

void Pr_DeleteString(Pr_String * ap_this)
{
    if (ap_this) {
        free(ap_this->str);
        free(ap_this);
    }
}

Pr_Strresult Pr_SetStringStr(Pr_String * ap_this, Pr_CStringRef ap_str)
{
    Pr_Strresult l_code = 0;

    if (ap_this) {
        if (ap_str && ap_str[0] != '\0') {
            unsigned long l_size = strlen(ap_str);

            if (l_size + 1 > ap_this->capacity) {
                unsigned long l_cap = ap_this->capacity + STRING_UNIT;
                char * lp_tmp;

                while (l_size + 1 > l_cap) {
                    l_cap += STRING_UNIT;
                }

                lp_tmp = calloc(l_cap,sizeof(char));

                if (lp_tmp) {
                    strcpy(lp_tmp,ap_str);
                    free(ap_this->str);
                    ap_this->str = lp_tmp;
                    ap_this->size = l_size;
                    ap_this->capacity = l_cap;
                    l_code = PR_STRING_ALLOCOK;
                } else {
                    l_code = PR_STRING_BADALLOC;
                }
            } else {
                ap_this->size = l_size;
                memcpy(ap_this->str,ap_str,ap_this->capacity);
                l_code = PR_STRING_NOALLOC;
            }
        } else {
            Pr_StringClear(ap_this);
            l_code = PR_STRING_UNMODIFIED;
        }
    }

    return l_code;
}

Pr_Strresult Pr_SetStringChar(Pr_String * ap_str, char a_c)
{
    char lp_tmp[2];

	lp_tmp[0] = a_c;
	lp_tmp[1] = '\0';

    return Pr_SetStringStr(ap_str,lp_tmp);
}

Pr_Strresult Pr_SetString(Pr_String * ap_this, Pr_String * ap_str)
{
    return Pr_SetStringStr(ap_this,(ap_str) ? ap_str->str : NULL);
}

Pr_Strcmpvalue Pr_StringSizeCmp(Pr_StringRef ap_str1, Pr_StringRef ap_str2)
{
    Pr_Strcmpvalue l_code;

    if (ap_str1->size > ap_str2->size) {
        l_code = PR_STRING_GREATER;
    } else if (ap_str1 < ap_str2) {
        l_code = PR_STRING_SMALLER;
    } else {
        l_code = PR_STRING_EQUAL;
    }

    return l_code;
}

Pr_Strresult Pr_StringStrAppend(Pr_String * ap_this, Pr_CStringRef ap_str)
{
    Pr_Strresult l_code = PR_STRING_NOSTRING;

    if (ap_this) {
        if (ap_str && ap_str[0] != '\0') {
            if (ap_this->size) {
                unsigned long l_size = strlen(ap_str) + ap_this->size + 1;
                char * lp_tmp = calloc(l_size,sizeof(char));

                if (lp_tmp) {
                    strcpy(lp_tmp,ap_this->str);
                    strcat(lp_tmp,ap_str);
                    Pr_SetStringStr(ap_this,lp_tmp);
                    l_code = PR_STRING_ALLOCOK;
                    free(lp_tmp);
                } else {
                    l_code = PR_STRING_BADALLOC;
                }
            } else {
                l_code = Pr_SetStringStr(ap_this,ap_str);
            }
        } else {
            l_code = PR_STRING_UNMODIFIED;
        }
    }
    return l_code;
}

Pr_Strresult Pr_StringCharAppend(Pr_String * ap_this, char a_c)
{
    char lp_tmp[2];

	lp_tmp[0] = a_c;
	lp_tmp[1] = '\0';

    return Pr_StringStrAppend(ap_this,lp_tmp);

}

Pr_String * Pr_StringAppendResult(Pr_CStringRef ap_str1, Pr_CStringRef ap_str2)
{
    Pr_String * lp_out = Pr_NewStringStr(ap_str1);

    if (lp_out) {
        Pr_StringStrAppend(lp_out,ap_str2);
    }
    return lp_out;
}

Pr_Strresult Pr_StringStrInsert(Pr_String * ap_this, unsigned long a_at, Pr_CStringRef ap_str)
{
    Pr_Strresult l_code = PR_STRING_NOSTRING;

    if (ap_this) {
        if (ap_str && ap_str[0] != '\0') {
            if (a_at >= ap_this->size) {
                l_code = Pr_StringStrAppend(ap_this,ap_str);
            } else {
                Pr_String * lp_tmp = Pr_StringSubStr(ap_this,a_at,ap_this->size);

                if (lp_tmp) {
                    l_code = Pr_StringCut(ap_this,a_at);

                    if (l_code > 0) {
                        l_code = Pr_StringStrAppend(ap_this,ap_str);

                        if (l_code > 0) {
                            l_code = Pr_StringStrAppend(ap_this,lp_tmp->str);
                        }
                    }
                    Pr_DeleteString(lp_tmp);
                }
            }
        } else {
            l_code = PR_STRING_UNMODIFIED;
        }
    }
    return l_code;
}

Pr_Strresult Pr_StringCharInsert(Pr_String * ap_this, unsigned long a_at, char a_c)
{
    char lp_tmp[2];

	lp_tmp[0] = a_c;
	lp_tmp[1] = '\0';

    return Pr_StringStrInsert(ap_this,a_at,lp_tmp);
}

Pr_Strresult Pr_StringCut(Pr_String * ap_this, unsigned long a_at)
{
    Pr_Strresult l_code = PR_STRING_NOSTRING;

    if (ap_this) {
        if (a_at == 0) {
            l_code = Pr_SetStringStr(ap_this,NULL);
        } else if (a_at >= ap_this->size) {
            l_code = PR_STRING_UNMODIFIED;
        } else {
            unsigned long l_i;

            if (a_at > ap_this->size) {
                a_at = ap_this->size;
            }
            for (l_i=0 ; l_i<ap_this->size-a_at ; l_i++) {
                ap_this->str[a_at+l_i] = '\0';
            }
            ap_this->size -= (ap_this->size - a_at);
            l_code = PR_STRING_NOALLOC;
        }
    }
    return l_code;
}

Pr_String * Pr_StringSubStr(Pr_String * ap_str, unsigned long a_begin, unsigned long a_end)
{
    Pr_String * lp_out = Pr_NewString();

    if (lp_out) {
        if (ap_str) {
            if (a_begin < ap_str->size && a_begin < a_end) {
                char * lp_tmp = NULL;
                unsigned long l_size = 0;

                if (a_end > ap_str->size) {
                    a_end = ap_str->size;
                }
                l_size = a_end - a_begin + 1;
                lp_tmp = calloc(l_size,sizeof(char));

                if (lp_tmp) {
                    memcpy(lp_tmp,ap_str->str + a_begin,l_size);
                    lp_tmp[l_size-1] = '\0';
                    Pr_SetStringStr(lp_out,lp_tmp);
                    free(lp_tmp);
                }
            }
        }
    }
    return lp_out;
}

Pr_String * Pr_StringNSubStr(Pr_String * ap_str, unsigned long a_at, unsigned long a_size)
{
    return Pr_StringSubStr(ap_str,a_at,a_at+a_size);
}

Pr_Strresult Pr_StringRemove(Pr_String * ap_this, unsigned long a_begin, unsigned long a_end)
{
    Pr_Strresult l_code = PR_STRING_NOSTRING;

    if (ap_this) {
        if (a_begin >= ap_this->size && a_begin > a_end) {
            l_code = PR_STRING_UNMODIFIED;
        } else {
            if (a_end >= ap_this->size) {
                l_code = Pr_StringCut(ap_this,a_begin);
            } else {
                Pr_String * lp_tmp = Pr_StringSubStr(ap_this,a_end+1,ap_this->size);

                if (lp_tmp) {
                    l_code = Pr_StringCut(ap_this,a_begin);

                    if (l_code) {
                        l_code = Pr_StringStrAppend(ap_this,lp_tmp->str);
                    }
                    Pr_DeleteString(lp_tmp);
                } else {
                    l_code = PR_STRING_BADALLOC;
                }
            }
        }
    }
    return l_code;
}

void Pr_StringClear(Pr_String * ap_this)
{
    if (ap_this) {
        memset(ap_this->str,0,ap_this->capacity);
        ap_this->size = 0;
    }
}

Pr_Strresult Pr_StringNRemove(Pr_String * ap_this, unsigned long a_at, unsigned long a_size)
{
    return Pr_StringRemove(ap_this,a_at,a_at+a_size-1);
}

unsigned long Pr_StringSize(Pr_StringRef ap_this)
{
    return (ap_this) ? ap_this->size : 0;
}

unsigned long Pr_StringCapacity(Pr_StringRef ap_this)
{
    return (ap_this) ? ap_this->capacity : 0;
}

char const * Pr_StringCStr(Pr_StringRef ap_this)
{
    return (ap_this) ? ap_this->str : NULL;
}

long Pr_StringFind(Pr_StringRef ap_this, Pr_CStringRef ap_str)
{
    char const * lp_tmp;

    if (!ap_this) return -1;
    if (!ap_str) return -1;
    if (!ap_str[0]) return -1;

    lp_tmp = strstr(ap_this->str, ap_str);

    return (lp_tmp) ? (lp_tmp - ap_this->str) : -1;
}

unsigned long Pr_StringReplace(Pr_String * ap_this, Pr_CStringRef ap_str, Pr_CStringRef ap_by)
{
    size_t l_count = 0;
    size_t l_i, l_strlen, l_bylen;
    
    if (!ap_this) return 0;
    if (!ap_str) return 0;
    if (!ap_str[0]) return 0;
    if (!ap_by) return 0;

    l_strlen = strlen(ap_str);
    l_bylen = (ap_by) ? strlen(ap_by) : 0;

    for (l_i = 0 ; l_i < ap_this->size ; l_i++) {
        long l_pos = Pr_StringFind(ap_this,ap_str);
        if (l_pos > -1) {
            Pr_StringNRemove(ap_this,l_pos,l_strlen);
            Pr_StringStrInsert(ap_this,l_pos,ap_by);
            l_i += l_bylen;
            l_count++;
        }
    }

    return l_count;
}  

#undef STRING_UNIT

