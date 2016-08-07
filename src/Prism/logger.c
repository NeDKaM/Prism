/* File name : logger.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2016
 * Desc. : Logger implementation
 * Dependencies :   <Prism/string.h>,
                    <Prism/list.h>,
                    <Prism/logger.h>,
                    <stdlib.h>
*/

#include <Prism/string.h>
#include <Prism/list.h>
#include <Prism/logger.h>
#include <stdlib.h>

struct pr_logger_t {
    Pr_List * entries;
    Pr_Signal * sigUpdated;
    unsigned long capacity;
};

Pr_Logger * Pr_NewLogger(void)
{
    Pr_Logger * lp_out = malloc(sizeof(Pr_Logger));

    if (!lp_out) return NULL;

    lp_out->entries = Pr_NewList();
    lp_out->sigUpdated = Pr_NewSignal();
    lp_out->capacity = 0;

    if (lp_out->entries && lp_out->sigUpdated) return lp_out;

    Pr_DeleteSignal(lp_out->sigUpdated);
    Pr_DeleteList(lp_out->entries);
    free(lp_out);

    return NULL;
}

void Pr_DeleteLogger(Pr_Logger * ap_log)
{
    if (!ap_log) return;

    Pr_ClearLog(ap_log);
    Pr_DeleteList(ap_log->entries);

    free(ap_log);
}

Pr_Signal * Pr_LogUpdated(Pr_LoggerRef ap_log)
{
    return (ap_log) ? ap_log->sigUpdated : NULL;
}

void Pr_WriteLog(Pr_Logger * ap_log, char * ap_txt)
{
    Pr_String * lp_ent;

    if (!ap_log || !ap_txt) return;

    lp_ent = Pr_NewStringStr(ap_txt);

    if (!lp_ent) return;

    if (!Pr_PushBackListData(ap_log->entries, lp_ent)) return;

    if (Pr_ListSize(ap_log->entries) != 0) {
        if (Pr_ListSize(ap_log->entries) > ap_log->capacity) {
            Pr_DeleteString(Pr_FrontList(ap_log->entries));
            Pr_PopFrontList(ap_log->entries);
        }   
    }
    
    Pr_Emit(Pr_LogUpdated(ap_log), ap_txt);
}

void Pr_WriteLog_Slot(void * ap_obj, va_list ap_args)
{
    if (!ap_args) return;

    Pr_WriteLog(ap_obj, va_arg(ap_args, char *));
}

void Pr_ClearLog(Pr_Logger * ap_log)
{
    if (!ap_log) return;

    while (Pr_ListSize(ap_log->entries)) {
        Pr_DeleteString(Pr_FrontList(ap_log->entries));
        Pr_PopFrontList(ap_log->entries);
    }

    Pr_Emit(Pr_LogUpdated(ap_log), NULL);
}

void Pr_ClearLog_Slot(void * ap_obj, va_list ap_args)
{
    Pr_ClearLog(ap_obj);

    (void)ap_args;
}

void Pr_SetLogCapacity(Pr_Logger * ap_log, unsigned long a_size)
{
    int l_diff;

    if (!ap_log) return;

    if (a_size == 0 || a_size >= ap_log->capacity) {
        ap_log->capacity = a_size;
        return;
    }

    l_diff = ap_log->capacity - a_size;

    while (l_diff--) {
        Pr_DeleteString(Pr_FrontList(ap_log->entries));
        Pr_PopFrontList(ap_log->entries);
    }

    ap_log->capacity = a_size;

    Pr_Emit(ap_log->sigUpdated, NULL);
}

void Pr_SetLogCapacity_Slot(void * ap_obj, va_list ap_args)
{
    if (!ap_args) return;

    Pr_SetLogCapacity(ap_obj, va_arg(ap_args, unsigned long));
}

Pr_ListRef Pr_GetLoggerEntries(Pr_LoggerRef ap_log)
{
    if (!ap_log) return NULL;

    return ap_log->entries;
}
