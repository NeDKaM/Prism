/* File name : factory.c
*  File type : Source
*  Author : Alexandre DUBUJET
*  Creation : 2015
*  Desc. : Prism class implementation
*  Dependencies :	-
*/

#include <stdlib.h>
#include <string.h>
#include <Prism/string.h>
#include <Prism/list.h>
#include <Prism/Gui/class.h>
#include <Prism/Gui/var.h>

struct pr_class_t {
    Pr_String *     name;
    Pr_List *       members;
    Pr_ClassId      id;
    Pr_Constructor  constructor;
    Pr_Deleter      deleter;
    Pr_Class *      inherit;
    unsigned long   sizealloc;
};

Pr_Class * Pr_NewClass(char const * ap_name, Pr_Constructor ap_con, Pr_Deleter ap_del, Pr_Class * ap_inh)
{
    Pr_Class * lp_out = NULL;

    if (!ap_name) return NULL;
    if (!ap_name[0]) return NULL;

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
    if (!ap_cls) return;

    Pr_DeleteString(ap_cls->name);
    Pr_DeleteList(ap_cls->members);
    free(ap_cls);
}

int Pr_SetClassParameter(Pr_Class * ap_that, Pr_Class * ap_class, char const * ap_name)
{
    Pr_ListIterator lp_it;
    Pr_Class * lp_cls;
    Pr_Variable * lp_var;

    if (!ap_that) return 0;
    if (!ap_class) return 0;
    if (!ap_name) return 0;
    if (!ap_name[0]) return 0;

    PR_LIST_FOREACH(ap_that->members, lp_it) {
        lp_var = Pr_ListIteratorData(lp_it);
        if (strcmp(Pr_GetVariableName(lp_var), ap_name) == 0) {
            return 0;
        }
    }

    lp_cls = ap_that->inherit;

    while (lp_cls != NULL) {
        PR_LIST_FOREACH(lp_cls->members, lp_it) {
            lp_var = Pr_ListIteratorData(lp_it);
            if (strcmp(Pr_GetVariableName(lp_var), ap_name) == 0) {
                return 0;
            }
        }

        lp_cls = lp_cls->inherit;
    }

    lp_var = Pr_NewVariable(ap_name, ap_class);
    if (!lp_var) return 0;

    if (Pr_PushBackListData(ap_that->members, lp_var)) return 1;

    Pr_DeleteVariable(lp_var);
    return 0;
}

Pr_ClassId Pr_GetClassId(Pr_Class * ap_class)
{
    return (ap_class) ? ap_class->id : 0;
}

char * Pr_GetClassName(Pr_Class * ap_class)
{
    return (ap_class) ? Pr_StringCStr(ap_class->name) : NULL;
}

