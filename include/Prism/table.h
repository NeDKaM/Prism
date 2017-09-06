#ifndef PRISM_TABLE_INCLUDED
#define PRISM_TABLE_INCLUDED

#include <Prism/list.h>
#include <string.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_table_t, Pr_Table);

    extern PRISM_API Pr_Table * Pr_NewTable(pr_u32_t (*hash)(void * key, pr_u32_t));
    extern PRISM_API void       Pr_DeleteTable(Pr_Table *);
    
    extern PRISM_API Pr_ListIterator    Pr_TableBegin(Pr_Table *);
    extern PRISM_API Pr_ListIterator    Pr_TableBack(Pr_Table *);
    extern PRISM_API Pr_ListIterator    Pr_GetTableIterator(Pr_Table *, void * key, pr_u32_t keySize);
    extern PRISM_API void               Pr_EraseTableElement(Pr_Table *, void * key, pr_u32_t keySize);

    extern PRISM_API pr_bool_t  Pr_TableElementExists(Pr_Table *, void * key, pr_u32_t keySize);

    #define PR_TKEY_STR(str) str, strlen(str)
    #define PR_TKEY_PTR(ptr) ptr, sizeof(*(ptr)) 

PR_CPP_PROTECT_OFF

#endif /* PRISM_TABLE_INCLUDED */

/*

Pr_GetTableIterator(table, PR_TKEY_STR("key"));
Pr_GetTableIterator(table, PR_TKEY_PTR(&color));

*/

