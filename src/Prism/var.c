/* File name : var.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Variable implementation
 * Dependencies :	<Prism/var.h>,
					<Prism/string.h>,
					<stdlib.h>
*/

#include <Prism/var.h>
#include <stdlib.h>
#include <string.h>

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

Pr_Variable * Pr_NewVariable(char const * ap_name, Pr_Class * ap_class)
{
    Pr_Variable * lp_out;

    if (!ap_class) return NULL;
    if (!ap_name) return NULL;
    if (!ap_name[0]) return NULL;
    if (!s_Pr_CheckName(ap_name)) return NULL;

    lp_out = calloc(1, sizeof(Pr_Variable));
    if (!lp_out) return NULL;

    lp_out->cls = ap_class;
    lp_out->name = Pr_NewStringStr(ap_name);
    lp_out->obj = NULL;

    if (lp_out->name) return lp_out;

    free(lp_out);
    return NULL;
}

void Pr_DeleteVariable(Pr_Variable * ap_var)
{
    if (!ap_var) return;

    Pr_DeleteString(ap_var->name);
    free(ap_var);
}

