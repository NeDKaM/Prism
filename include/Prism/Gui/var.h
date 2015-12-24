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
#include <Prism/Gui/class.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct pr_var_t Pr_Variable;

    Pr_Variable *   Pr_NewVariable(char const *, Pr_Class *);
    void            Pr_DeleteVariable(Pr_Variable *);
    char const *    Pr_GetVariableName(Pr_Variable *);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_VAR_INCLUDED */
