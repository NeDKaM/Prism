/* File name : var.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Variable implementation
 * Dependencies : -
 * Last modifications : 23/7/2015
 */

#ifndef H_PRISM_VAR_INCLUDED
#define H_PRISM_VAR_INCLUDED

#include <Prism/config.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct pr_var_t Pr_Variable;

	typedef enum pr_variabletype_t Pr_VariableType;
	enum pr_variabletype_t {
		PR_VAR_INT,
		PR_VAR_FLOAT,
		PR_VAR_STRING
	};

	extern PRISM_API Pr_Variable *  Pr_NewVariable(Pr_VariableType, char const *);
	extern PRISM_API void           Pr_DeleteVariable(Pr_Variable *);
	extern PRISM_API void           Pr_SetVariableInt(Pr_Variable *, long);
	extern PRISM_API void           Pr_SetVariableFloat(Pr_Variable *, float);
	extern PRISM_API void           Pr_SetVariableString(Pr_Variable *, char  *);
	extern PRISM_API void           Pr_GetVariableInt(Pr_Variable *, long *);
	extern PRISM_API void           Pr_GetVariableFloat(Pr_Variable *, float *);
	extern PRISM_API char *         Pr_GetVariableString(Pr_Variable *);
	extern PRISM_API char *         Pr_GetVariableName(Pr_Variable *);


#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_VAR_INCLUDED */
