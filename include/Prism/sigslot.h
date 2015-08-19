/* File name : sigslots.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C signal/slot implementation
 * Dependencies :	<stdarg.h>
 * Last modifications : 06/7/2015
 */

#ifndef H_PRISM_SIGSLOT_INCLUDED
#define H_PRISM_SIGSLOT_INCLUDED

#include <Prism/config.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct pr_signal_t Pr_Signal;
	typedef void (*Pr_Slot)(va_list);

	extern PRISM_API	Pr_Signal * Pr_NewSignal(void);
	extern PRISM_API	void        Pr_DeleteSignal(Pr_Signal *);
	extern PRISM_API	int         Pr_Connect(Pr_Signal *, Pr_Slot);
	extern PRISM_API	void        Pr_Emit(Pr_Signal *, ...);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_SIGSLOT_INCLUDED */
