/* File name : logger.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2016
 * Desc. : Logger implementation
 * Dependencies :   <Prism/config.h>,
                    <Prism/sigslot.h>
*/

#ifndef H_PRISM_LOGGER_INCLUDED
#define H_PRISM_LOGGER_INCLUDED

#include <Prism/config.h>
#include <Prism/sigslot.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_logger_t, Pr_Logger);

    extern PRISM_API Pr_Logger *    Pr_NewLogger(void);
    extern PRISM_API void           Pr_DeleteLogger(Pr_Logger *);

    extern PRISM_API PR_SIGNAL(Pr_LogUpdated)(Pr_Logger *);     /**< (pr_cstring_t) >*/

    PR_SLOT_EXTERN_API(Pr_WriteLog)(Pr_Logger *, char *);
    PR_SLOT_EXTERN_API(Pr_ClearLog)(Pr_Logger *);
    PR_SLOT_EXTERN_API(Pr_SetLogCapacity)(Pr_Logger *, pr_u32_t);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_LOGGER_INCLUDED */

