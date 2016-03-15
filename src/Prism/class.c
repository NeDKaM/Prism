/* File name : class.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism class implementation
 * Dependencies :   --
*/

#include <stdlib.h>
#include <string.h>
#include <Prism/string.h>
#include <Prism/list.h>
#include <Prism/class.h>
#include <Prism/var.h>

struct pr_class_t {
    Pr_String *     name;
    Pr_List *       members;
    Pr_ClassId      id;
    Pr_Constructor  constructor;
    Pr_Deleter      deleter;
    Pr_ClassRef     inherit;
    unsigned long   sizealloc;
};

static int s_Pr_CheckName(char const * ap_name)
{
    unsigned int l_i;
    unsigned int l_n;

    if (!ap_name) return 0;

    l_n = strlen(ap_name);

    for (l_i = 0 ; l_i < l_n ; l_i++) {
        char l_tmp = ap_name[l_i];
        if (l_tmp >= 'A' && l_tmp <= 'Z') continue;
        if (l_tmp >= 'a' && l_tmp <= 'z') continue;
        if (l_tmp >= '0' && l_tmp <= '9') continue;
        if (l_tmp == '_') continue;

        return 0;
    }

    return 1;
}

Pr_Class * Pr_NewClass(char const * ap_name, Pr_Constructor ap_con, Pr_Deleter ap_del, Pr_ClassRef ap_inh)
{
    Pr_Class * lp_out = NULL;

    if (!ap_name) return NULL;
    if (!ap_name[0]) return NULL;
    if (!s_Pr_CheckName(ap_name)) return NULL;

    lp_out = calloc(1, sizeof(Pr_Class));
    if (!lp_out) return NULL;

    lp_out->name = Pr_NewStringStr(ap_name);
    lp_out->members = Pr_NewList();

    lp_out->constructor = ap_con;
    lp_out->deleter = (ap_con) ? ap_del : NULL;
    lp_out->id = 0;
    lp_out->inherit = ap_inh;
    lp_out->sizealloc = (ap_inh) ? ap_inh->sizealloc : 0;

    if (lp_out->members && lp_out->name) return lp_out;

    Pr_DeleteClass(lp_out);

    return NULL;
}

void Pr_DeleteClass(Pr_Class * ap_cls)
{
    Pr_ListIterator lp_it;

    if (!ap_cls) return;

    PR_LIST_FOREACH(ap_cls->members, lp_it) {
        Pr_DeleteVariable(Pr_ListIteratorData(lp_it));
    }

    Pr_DeleteString(ap_cls->name);
    Pr_DeleteList(ap_cls->members);
    free(ap_cls);
}

int Pr_SetClassParameter(Pr_Class * ap_that, Pr_ClassRef ap_class, char const * ap_name)
{
    Pr_ListIterator lp_it;
    Pr_Class * lp_cls;
    Pr_Variable * lp_var;

    if (!ap_that) return 0;
    if (!ap_class) return 0;
    if (!ap_name) return 0;
    if (!ap_name[0]) return 0;

    if (ap_that->id) return 0;

    PR_LIST_FOREACH(ap_that->members, lp_it) {
        lp_var = Pr_ListIteratorData(lp_it);
        if (strcmp(Pr_StringCStr(lp_var->name), ap_name) == 0) {
            return 0;
        }
    }

    lp_cls = (Pr_Class *)ap_that->inherit;

    while (lp_cls != NULL) {
        PR_LIST_FOREACH(lp_cls->members, lp_it) {
            lp_var = Pr_ListIteratorData(lp_it);
            if (strcmp(Pr_StringCStr(lp_var->name), ap_name) == 0) {
                return 0;
            }
        }

        lp_cls = (Pr_Class *)lp_cls->inherit;
    }

    lp_var = Pr_NewVariable(ap_name, ap_class);
    if (!lp_var) return 0;

    if (Pr_PushBackListData(ap_that->members, lp_var)) return 1;

    Pr_DeleteVariable(lp_var);
    return 0;
}

Pr_ClassId Pr_GetClassId(Pr_ClassRef ap_class)
{
    return (ap_class) ? ap_class->id : 0;
}

char const * Pr_GetClassName(Pr_ClassRef ap_class)
{
    return (ap_class) ? Pr_StringCStr(ap_class->name) : NULL;
}

