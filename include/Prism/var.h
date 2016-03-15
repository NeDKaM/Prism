/* File name : var.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Variable implementation
 * Dependencies :   <Prism/config.h>,
                    <Prism/class.h>,
                    <Prism/string.h>
 */

#ifndef H_PRISM_VAR_INCLUDED
#define H_PRISM_VAR_INCLUDED

#include <Prism/config.h>
#include <Prism/class.h>
#include <Prism/string.h>

PR_CPP_PROTECT_ON

	PR_STRUCT(pr_variable_t, Pr_Variable) {
        Pr_String * name;
        Pr_ClassRef cls;
        void *      obj;
    };

    Pr_Variable *   Pr_NewVariable(char const *, Pr_ClassRef);
    void            Pr_DeleteVariable(Pr_Variable *);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_VAR_INCLUDED */
