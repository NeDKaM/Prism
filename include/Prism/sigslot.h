/* File name : sigslots.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C signal/slot implementation
 * Dependencies :	<Prism/config.h>,
                    <stdarg.h>
 */

#ifndef H_PRISM_SIGSLOT_INCLUDED
#define H_PRISM_SIGSLOT_INCLUDED

#include <Prism/config.h>
#include <stdarg.h>

PR_CPP_PROTECT_ON

	PR_STRUCT(pr_signal_t, Pr_Signal);

	typedef void (*Pr_Slot)(void *, va_list);

	extern PRISM_API Pr_Signal *    Pr_NewSignal(void);
	extern PRISM_API void           Pr_DeleteSignal(Pr_Signal *);
	extern PRISM_API int            Pr_Connect(Pr_Signal *, void *, Pr_Slot);
	extern PRISM_API void           Pr_Emit(Pr_Signal *, ...);

#define PR_SIGNAL(name)             Pr_Signal * name
#define PR_SLOT(name)               void name##_Slot(void *, va_list); \
                                    void name
#define PR_SLOT_EXTERN_API(name)    extern PRISM_API void name##_Slot(void *, va_list); \
                                    extern PRISM_API void name


PR_CPP_PROTECT_OFF

#endif /* H_PRISM_SIGSLOT_INCLUDED */
