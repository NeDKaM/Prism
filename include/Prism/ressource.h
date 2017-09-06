#ifndef H_PRISM_RESSOURCE_INCLUDED
#define H_PRISM_RESSOURCE_INCLUDED

#include <Prism/objects.h>
#include <Prism/string.h>

PR_CPP_PROTECT_ON


    PR_STRUCT(pr_ressourceloader_t, Pr_ResourceLoader);

    PR_OBJECT(pr_ressource_t, Pr_Resource) {
        PR_INHERIT(Pr_Object);

        Pr_String *         filePath;
        Pr_ResourceLoader * loader;
    };

    extern PRISM_API Pr_Class Pr_ResourceClass;

    typedef pr_bool_t   (*Pr_ExtensionGetter)(pr_cstring_t);
    typedef pr_bool_t   (*Pr_FileLoader)(Pr_ResourceRef, pr_cstring_t);
    typedef pr_bool_t   (*Pr_MemoryLoader)(Pr_ResourceRef, void *, pr_u32_t);

    extern PRISM_API Pr_ResourceLoader *    Pr_NewResourceLoader(void);
    extern PRISM_API void                   Pr_DeleteResourceLoader(Pr_ResourceLoader *);
    extern PRISM_API pr_bool_t              Pr_RegisterResourceLoader(Pr_ResourceLoader *, Pr_ExtensionGetter, Pr_FileLoader, Pr_MemoryLoader);
    extern PRISM_API pr_bool_t              Pr_LoadResourceFile(Pr_ResourceRef, pr_cstring_t);
    extern PRISM_API pr_bool_t              Pr_LoadResourceMemory(Pr_ResourceRef, void *, pr_u32_t);
    extern PRISM_API pr_bool_t              Pr_LoaderExtensionSupported(Pr_ResourceLoader *, pr_cstring_t);

    PR_STRUCT(pr_ressourcemanager_t, Pr_ResourceManager);

    extern PRISM_API Pr_ResourceManager *   Pr_NewResourceManager(pr_cstring_t);
    extern PRISM_API void                   Pr_DeleteResourceManager(Pr_ResourceManager *);
    extern PRISM_API pr_bool_t              Pr_RegisterResource(Pr_ResourceManager *, Pr_ResourceRef, pr_cstring_t);
    extern PRISM_API void                   Pr_UnregisterResource(Pr_ResourceManager *, pr_cstring_t);
    extern PRISM_API Pr_ResourceRef         Pr_GetResource(Pr_Class *, Pr_ResourceManager *, pr_cstring_t);
    

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_RESSOURCE_INCLUDED */
