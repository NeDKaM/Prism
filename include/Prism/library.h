/* File name : library.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : Prism class library implementation
 * Dependencies :   <Prism/config.h>,
                    <Prism/class.h>
*/


#ifndef H_PRISM_LIBRARY_INCLUDED
#define H_PRISM_LIBRARY_INCLUDED

#include <Prism/config.h>
#include <Prism/class.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct pr_library_t Pr_Library;

    extern PRISM_API Pr_Library *   Pr_NewLibrary(char const *);
    extern PRISM_API void           Pr_DeleteLibrary(Pr_Library *);
    extern PRISM_API int            Pr_RegisterClass(Pr_Library *, Pr_Class *);
    extern PRISM_API int            Pr_ClassRegistered(Pr_Library *, char const *);
    extern PRISM_API int            Pr_ImportLibrary(Pr_Library *, Pr_Library *);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_LIBRARY_INCLUDED */
