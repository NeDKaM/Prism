/* File name : var.c
* File type : Source
* Author : Alexandre DUBUJET
* Creation : 2015
* Desc. : C Variable implementation
* Dependencies :	<Prism/var.h>,
					<Prism/string.h>,
					<stdlib.h>
*/

#include <Prism/Gui/var.h>
#include <Prism/string.h>
#include <stdlib.h>

struct pr_var_t {
	Pr_String * name;
    Pr_Class *  cls;
};

Pr_Variable * Pr_NewVariable(char const * ap_name, Pr_Class * ap_class)
{
    Pr_Variable * lp_out;

    if (!ap_class) return NULL;

    lp_out = calloc(1, sizeof(Pr_Variable));
    if (!lp_out) return NULL;

    lp_out->cls = ap_class;
    lp_out->name = Pr_NewStringStr(ap_name);

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

char const * Pr_GetVariableName(Pr_Variable * ap_var)
{
    return (ap_var) ? Pr_StringCStr(ap_var->name) : NULL;
}
