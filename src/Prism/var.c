/* File name : var.c
* File type : Source
* Author : Alexandre DUBUJET
* Creation : 2015
* Desc. : C Variable implementation
* Dependencies :	<Prism/var.h>,
					<Prism/string.h>,
					<stdlib.h>
* Last modifications : 27/7/2015
*/

#include <Prism/var.h>
#include <Prism/string.h>
#include <stdlib.h>

struct pr_var_t {
	Pr_VariableType type;
	Pr_String * name;
	union {
		long	d;
		float	f;
		char *	s;
	};
};

Pr_Variable * Pr_NewVariable(Pr_VariableType a_type, char const * ap_name)
{
	Pr_Variable * lp_out = NULL;

	if (!ap_name || ap_name[0] == '\0') return NULL;

	lp_out = malloc(sizeof(Pr_Variable));

	if (!lp_out) return NULL;

	lp_out->type = a_type;
	lp_out->name = Pr_NewStringStr(ap_name);

	if (!lp_out->name) {
		free(lp_out);
		lp_out = NULL;
	}

	return lp_out;
}

void Pr_DeleteVariable(Pr_Variable * ap_var)
{
	if (ap_var) {
		Pr_DeleteString(ap_var->name);
		free(ap_var);
	}
}

void Pr_SetVariableInt(Pr_Variable * ap_var, long a_d)
{
	if (!ap_var) return;

	if (ap_var->type == PR_VAR_INT) {
		ap_var->d = a_d;
	}
}

void Pr_SetVariableFloat(Pr_Variable * ap_var, float a_f)
{
	if (!ap_var) return;

	if (ap_var->type == PR_VAR_FLOAT) {
		ap_var->f = a_f;
	}
}

void Pr_SetVariableString(Pr_Variable * ap_var, char * ap_str)
{
	if (!ap_var) return;

	if (ap_var->type == PR_VAR_STRING) {
		ap_var->s = ap_str;
	}
}

void Pr_GetVariableInt(Pr_Variable * ap_var, long * ap_d)
{
	if (!ap_var || !ap_d) return;

	if (ap_var->type == PR_VAR_INT) {
		*ap_d = ap_var->d;
	}
}

void Pr_GetVariableFloat(Pr_Variable * ap_var, float * ap_f)
{
	if (!ap_var || !ap_f) return;

	if (ap_var->type == PR_VAR_FLOAT) {
		*ap_f = ap_var->f;
	}
}

char * Pr_GetVariableString(Pr_Variable * ap_var)
{
	if (!ap_var) return NULL;

	if (ap_var->type != PR_VAR_STRING) return NULL;

	return ap_var->s;
}

char * Pr_GetVariableName(Pr_Variable * ap_var)
{
	if (!ap_var) return NULL;

	return Pr_StringCStr(ap_var->name);
}