/* File name : library.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism class library implementation
 * Dependencies :   --
*/

#include <Prism/library.h>
#include <Prism/string.h>
#include <Prism/array.h>
#include <Prism/list.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct pr_library_t {
    Pr_String * name;
    Pr_Array(Pr_Class *) classList;
    unsigned long lastId;
};

/* friend Pr_Class */ struct s_pr_class_t {
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

Pr_Library * Pr_NewLibrary(char const * ap_name)
{
    Pr_Library * lp_out;

    if (!ap_name) return NULL;
    if (!ap_name[0]) return NULL;
    if (!s_Pr_CheckName(ap_name)) return NULL;

    lp_out = malloc(sizeof(Pr_Library));
    if (!lp_out) return NULL;

    lp_out->name = Pr_NewStringStr(ap_name);
    Pr_MakeSizedArray(lp_out->classList,1);
    lp_out->lastId = 1;

    if (Pr_ArraySize(lp_out->classList)) {
        Pr_ArrayAt(lp_out->classList,0) = NULL;
    }

    if (lp_out->name && Pr_ArraySize(lp_out->classList)) return lp_out;

    Pr_DeleteString(lp_out->name);
    Pr_ClearArray(lp_out->classList);
    free(lp_out);
    return NULL;
}

void Pr_DeleteLibrary(Pr_Library * ap_lib)
{
    unsigned int i;

    if (!ap_lib) return;

    for (i = 1 ; i < Pr_ArraySize(ap_lib->classList) ; i++) {
        Pr_DeleteClass(Pr_ArrayAt(ap_lib->classList,i));
    }

    Pr_DeleteString(ap_lib->name);
    Pr_ClearArray(ap_lib->classList);
    free(ap_lib);
}

int Pr_ClassRegistered(Pr_LibraryRef ap_lib, char const * ap_name)
{
    unsigned int i;
    
    if (!ap_lib) return 0;
    if (!ap_name) return 1;
    if (!ap_name[0]) return 1;

    for (i = 1 ; i < Pr_ArraySize(ap_lib->classList) ; i++) {
        Pr_Class * lp_tmp = Pr_ArrayAt(ap_lib->classList,i);
        if (strcmp(Pr_GetClassName(lp_tmp), ap_name) == 0) {
            return 1;
        }
    }

    return 0;
}

int Pr_RegisterClass(Pr_Library * ap_lib, Pr_Class * ap_class)
{
    unsigned int l_size;
    struct s_pr_class_t * lp_classDef;

    if (!ap_lib) return 0;
    if (!ap_class) return 0;

    lp_classDef = (struct s_pr_class_t *)ap_class; 

    if (Pr_ClassRegistered(ap_lib, Pr_StringCStr(lp_classDef->name))) return 0;
    if (!Pr_ClassRegistered(ap_lib, Pr_GetClassName(lp_classDef->inherit))) return 0;

    l_size = Pr_ArraySize(ap_lib->classList);
    Pr_PushBackArray(ap_lib->classList);
    if (l_size == Pr_ArraySize(ap_lib->classList)) return 0;

    Pr_ArrayAt(ap_lib->classList,ap_lib->lastId) = ap_class;
    lp_classDef->id = ap_lib->lastId;

    ap_lib->lastId++;

    return 1;
}

static Pr_Class * s_Pr_CopyClass(Pr_ClassRef ap_class)
{
    Pr_Class * lp_out;
    struct s_pr_class_t * lp_classDef;
    struct s_pr_class_t * lp_classDef2;

    if (!ap_class) return NULL;

    lp_classDef = (struct s_pr_class_t *)ap_class;

    lp_out = Pr_NewClass(
        Pr_StringCStr(lp_classDef->name),
        lp_classDef->constructor,
        lp_classDef->deleter,
        lp_classDef->inherit
    );

    if (!lp_out) return NULL;

    lp_classDef2 = (struct s_pr_class_t *)lp_out;

    lp_classDef2->sizealloc = lp_classDef->sizealloc;

    lp_classDef2->members = Pr_CopyList(lp_classDef->members);
    if (lp_classDef2->members) return lp_out;

    Pr_DeleteClass(lp_out);

    return NULL;
}

int Pr_ImportLibrary(Pr_LibraryRef ap_from, Pr_Library * ap_to)
{
    unsigned int l_i;

    if (!ap_from || !ap_to) return 0;

    for (l_i = 0 ; l_i < Pr_ArraySize(ap_from->classList) ; l_i++) {
        if (Pr_ClassRegistered(ap_to, Pr_GetClassName(Pr_ArrayAt(ap_from->classList, l_i)))) {
            return 0;
        }
    }

    for (l_i = 0 ; l_i < Pr_ArraySize(ap_from->classList) ; l_i++) {
        Pr_Class * lp_tmp = s_Pr_CopyClass(Pr_ArrayAt(ap_from->classList, l_i));
        if (!Pr_RegisterClass(ap_to, lp_tmp)) {
            Pr_DeleteClass(lp_tmp);
            return 0;
        }
    }

    return 1;
}
