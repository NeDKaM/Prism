/* File name : library.c
*  File type : Source
*  Author : Alexandre DUBUJET
*  Creation : 2015
*  Desc. : Prism class library implementation
*  Dependencies :	-
*/

#include <Prism/Gui/library.h>
#include <Prism/string.h>
#include <Prism/array.h>
#include <Prism/list.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct pr_library_t {
    Pr_String * name;
    PR_ARRAY(Pr_Class *) classList;
    unsigned long lastId;
};

Pr_Library * Pr_NewLibrary(char const * ap_name)
{
    Pr_Library * lp_out;

    if (!ap_name) return NULL;
    if (!ap_name[0]) return NULL;

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

int Pr_ClassRegistered(Pr_Library * ap_lib, char const * ap_name)
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

    /* friend Pr_Class */ struct pr_class_ {
        Pr_String *     name;
        Pr_List *       members;
        Pr_ClassId      id;
        Pr_Constructor  constructor;
        Pr_Deleter      deleter;
        Pr_Class *      inherit;
        unsigned long   sizealloc;
    } * lp_classDef;

    if (!ap_lib) return 0;
    if (!ap_class) return 0;

    lp_classDef = (struct pr_class_ *)ap_class; /* cast to reinterpret data */

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

int Pr_LoadLibrary(char const * ap_file)
{
    if (!ap_file) return 0;
    if (!ap_file[0]) return 0;

    return 0;
}

int Pr_SaveLibrary(Pr_Library * ap_lib, char const * ap_file)
{
    Pr_String * lp_src;

    if (!ap_lib) return 0;
    if (!ap_file) return 0;
    if (!ap_file[0]) return 0;

    /* Create a file like this */
    /* 
        #import "PrGuiLib"
        
        [StrangeButton] = PrButton {
            PrInteger : counter;
        }
    */

    return 0;
}
