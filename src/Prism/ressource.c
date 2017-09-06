#include <Prism/ressource.h>

#include <ctype.h>

#include <SDL2/SDL.h>

struct pr_ressourceloader_t {
    Pr_Array *  loaders;
};

typedef struct {
    Pr_ExtensionGetter  extensionGetter;
    Pr_FileLoader       fileLoader;
    Pr_MemoryLoader     memoryLoader;
} Pr_LoaderInfo;

struct pr_ressourcemanager_t {
    Pr_ObjectLibrary *  ressources;
};

static pr_bool_t s_Pr_ConstructResource(Pr_ObjectRef ap_data)
{
    Pr_ResourceRef lp_rsc = (Pr_ResourceRef) ap_data;

    lp_rsc->loader = NULL;
    lp_rsc->filePath = Pr_NewString();

    if (lp_rsc->filePath) {
        return PR_TRUE;
    }

    return PR_FALSE;
}

static void s_Pr_DestructResource(Pr_ObjectRef ap_data)
{
    Pr_ResourceRef lp_rsc = (Pr_ResourceRef) ap_data;

    Pr_DeleteString(lp_rsc->filePath);
}

Pr_Class Pr_ResourceClass = {
    "Pr_Resource",
    sizeof(Pr_Resource),
    0,
    &Pr_ObjectClass,
    NULL,
    0,
    PR_TRUE,
    s_Pr_ConstructResource,
    s_Pr_DestructResource
};

static pr_bool_t s_Pr_LoaderTypeInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_LoaderInfo * lp_inf = ap_data;

    lp_inf->extensionGetter = NULL;
    lp_inf->fileLoader = NULL;
    lp_inf->memoryLoader = NULL;

    return PR_TRUE;
}

Pr_ResourceLoader *   Pr_NewResourceLoader(void)
{
    Pr_ResourceLoader * lp_out;

    lp_out = malloc(sizeof(Pr_ResourceLoader));
        if (!lp_out) return NULL;

    lp_out->loaders = Pr_NewArray(sizeof(Pr_LoaderInfo), s_Pr_LoaderTypeInitializer);
    if (lp_out->loaders) {
        return lp_out;
    }

    free(lp_out);

    return NULL;
}

void    Pr_DeleteResourceLoader(Pr_ResourceLoader * ap_loader)
{
    if (!ap_loader) return;

    Pr_DeleteArray(ap_loader->loaders);

    free(ap_loader);
}

pr_bool_t  Pr_RegisterResourceLoader(Pr_ResourceLoader * ap_loader, 
    Pr_ExtensionGetter  ap_extg, 
    Pr_FileLoader       ap_fl, 
    Pr_MemoryLoader     ap_ml
)
{
    Pr_LoaderInfo l_inf;
    Pr_LoaderInfo * lp_loaders;
    pr_u32_t l_i;

    if (!ap_loader) return PR_FALSE;

    if (!ap_fl && !ap_ml) return PR_FALSE;

    l_inf.extensionGetter = ap_extg;
    l_inf.fileLoader = ap_fl;
    l_inf.memoryLoader = ap_ml;

    lp_loaders = Pr_GetArrayData(ap_loader->loaders);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_loader->loaders) ; l_i++) {
        if (lp_loaders[l_i].extensionGetter == ap_extg) {
            return PR_FALSE;
        }
    }

    if (Pr_SetArrayAt(ap_loader->loaders, Pr_ArraySize(ap_loader->loaders), &l_inf)) {
        return PR_TRUE;
    }

    return PR_FALSE;
}

pr_bool_t  Pr_LoadResourceFile(Pr_ResourceRef ap_rsc, pr_cstring_t ap_file)
{
    Pr_LoaderInfo * lp_loaders;
    pr_u32_t l_i;
    char * lp_ext;

    if (!ap_rsc || !ap_file) return PR_FALSE;

    if (!ap_rsc->loader) return PR_FALSE;

    lp_ext = strrchr(ap_file, '.');
        if (!lp_ext) return PR_FALSE;

    lp_loaders = Pr_GetArrayData(ap_rsc->loader->loaders);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_rsc->loader->loaders) ; l_i++) {
        Pr_LoaderInfo * lp_inf = &lp_loaders[l_i];

        if (!lp_inf->extensionGetter || !lp_inf->extensionGetter(lp_ext)) {
            continue;
        }

        if (lp_inf->fileLoader) {
            if (lp_inf->fileLoader(ap_rsc, ap_file)) {
                Pr_SetStringStr(ap_rsc->filePath, ap_file);
                return PR_TRUE;
            }
        }
    }

    return PR_FALSE;
}

pr_bool_t  Pr_LoadResourceMemory(Pr_ResourceRef ap_rsc, void * ap_mem, pr_u32_t a_size)
{
    Pr_LoaderInfo * lp_loaders;
    pr_u32_t l_i;

    if (!ap_rsc || !ap_mem || !a_size) return PR_FALSE;

    if (!ap_rsc->loader) return PR_FALSE;

    lp_loaders = Pr_GetArrayData(ap_rsc->loader->loaders);
    for (l_i=0 ; l_i<Pr_ArraySize(ap_rsc->loader->loaders) ; l_i++) {
        Pr_LoaderInfo * lp_inf = &lp_loaders[l_i];

        if (lp_inf->memoryLoader) {
            if (lp_inf->memoryLoader(ap_rsc, ap_mem, a_size)) {
                return PR_TRUE;
            }
        }
    }

    return PR_FALSE;
}

pr_bool_t   Pr_LoaderExtensionSupported(Pr_ResourceLoader * ap_loader, pr_cstring_t ap_ext)
{
    pr_u32_t l_i;
    Pr_LoaderInfo * lp_loaders;

    lp_loaders = Pr_GetArrayData(ap_loader->loaders);

    for (l_i=0 ; l_i<Pr_ArraySize(ap_loader->loaders) ; l_i++) {
        Pr_ExtensionGetter lp_getter = lp_loaders[l_i].extensionGetter;

        if (lp_getter && lp_getter(ap_ext)) {
            return PR_TRUE;
        }
    }

    return PR_FALSE;
}

Pr_ResourceManager *  Pr_NewResourceManager(pr_cstring_t ap_name)
{
    Pr_ResourceManager * lp_out;

    lp_out = malloc(sizeof(Pr_ResourceManager));
        if (!lp_out) return NULL;

    lp_out->ressources = Pr_NewObjectLibrary(ap_name);
    if (lp_out->ressources) {
        return lp_out;
    }

    free(lp_out);

    return NULL;
}

void    Pr_DeleteResourceManager(Pr_ResourceManager * ap_man)
{
    if (!ap_man) return;

    Pr_DeleteObjectLibrary(ap_man->ressources);

    free(ap_man);
}

pr_bool_t   Pr_RegisterResource(Pr_ResourceManager * ap_man, Pr_ResourceRef ap_rsc, pr_cstring_t ap_str)
{
    pr_bool_t l_out;

    if (!ap_man) return PR_FALSE;

    l_out = Pr_RegisterObject(ap_man->ressources, (Pr_ObjectRef) ap_rsc, ap_str);

    return l_out;
}

void    Pr_UnregisterResource(Pr_ResourceManager * ap_man, pr_cstring_t ap_str)
{
    if (!ap_man) return;

    Pr_UnregisterObject(ap_man->ressources, ap_str);
}

Pr_ResourceRef  Pr_GetResource(Pr_Class * ap_class, Pr_ResourceManager * ap_man, pr_cstring_t ap_str)
{
    Pr_ResourceRef lp_out;

    if (!ap_man) return NULL;

    if (!Pr_IsBaseOf(ap_class, &Pr_ResourceClass)) return NULL;

    lp_out = (Pr_ResourceRef) Pr_GetObject(ap_man->ressources, ap_str);

    if (!lp_out) {
        lp_out = Pr_New(ap_class);
            if (!lp_out) return NULL;
        
        if (!Pr_LoadResourceFile(lp_out, ap_str)) {
            Pr_Delete((Pr_ObjectRef) lp_out);
            return NULL;
        }

        if (!Pr_RegisterObject(ap_man->ressources, (Pr_ObjectRef) lp_out, ap_str)) {
            Pr_Delete((Pr_ObjectRef) lp_out);
            return NULL;
        }
    }

    return lp_out;
}

