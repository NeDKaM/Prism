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

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct pr_var_t Pr_Variable;
    struct pr_var_t {
        Pr_String * name;
        Pr_Class *  cls;
        void * obj;
    };

    Pr_Variable *   Pr_NewVariable(char const *, Pr_Class *);
    void            Pr_DeleteVariable(Pr_Variable *);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_VAR_INCLUDED */
