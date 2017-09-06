#include <Prism/RainbowSDL/texture.h>

#include <Prism/RainbowSDL/gfxwindow.h>

#include <SDL2/SDL_image.h>

SDL_Texture * Pr_LoadSDLTexture(SDL_Renderer * ap_rnd, char const * ap_file)
{
    SDL_Texture * lp_out;
    SDL_Surface * lp_surface;

    if (!ap_rnd) {
        return NULL;
    }

    lp_surface = IMG_Load(ap_file);
    if (!lp_surface) {
        return NULL;
    }

    lp_out = SDL_CreateTextureFromSurface(ap_rnd, lp_surface);

    SDL_FreeSurface(lp_surface);

    if (lp_out) {
        return lp_out;
    }

    return NULL;
}

struct {
    Pr_ResourceLoader * loader;
    pr_u32_t            textureCount;
} static s_textureLoader = { NULL, 0 };

static pr_bool_t s_Pr_ExtensionGetter(pr_cstring_t ap_ext)
{
    pr_u32_t l_i;

    static pr_cstring_t lp_exts[5] = {
        "bmp",
        "png",
        "jpg", "jpeg", /* ?? */
        "tga"
        /* and so on */
    };

    for (l_i=0 ; l_i<5 ; l_i++) { /* rework */
        if (strcmp(ap_ext, lp_exts[l_i]) == 0) {
            return PR_TRUE;
        }
    }

    return PR_FALSE;
}

static pr_bool_t s_Pr_FileLoader(Pr_ResourceRef ap_rsc, pr_cstring_t ap_file)
{
    Pr_TextureRef lp_tex = (Pr_TextureRef) ap_rsc;
    SDL_Surface * lp_surface;

    lp_surface = IMG_Load(ap_file);
        if (!lp_surface) return PR_FALSE;

    lp_tex->impl = SDL_CreateTextureFromSurface(Pr_GetCurrentRenderer(), lp_surface);
    
    SDL_FreeSurface(lp_surface);

    return (lp_tex->impl) ? PR_TRUE : PR_FALSE;
}

static Pr_ResourceLoader * s_Pr_GetTextureLoader(void)
{
    if (!s_textureLoader.loader) {
        s_textureLoader.loader = Pr_NewResourceLoader();
            if (!s_textureLoader.loader) return NULL;

        if (!Pr_RegisterResourceLoader(s_textureLoader.loader, s_Pr_ExtensionGetter, s_Pr_FileLoader, NULL)) {
            return NULL;
        }
    }

    return s_textureLoader.loader;
}

static pr_bool_t s_Pr_ConstructTexture(Pr_ObjectRef ap_obj)
{
    Pr_TextureRef ap_tex = (Pr_TextureRef) ap_obj;
    Pr_ResourceRef ap_rsc = (Pr_ResourceRef) ap_obj;

    ap_tex->impl = NULL;

    ap_rsc->loader = s_Pr_GetTextureLoader();
        if (!ap_rsc->loader) return PR_FALSE;

    s_textureLoader.textureCount++;

    return PR_TRUE;
}

static void s_Pr_DestructTexture(Pr_ObjectRef ap_obj)
{
    Pr_TextureRef ap_tex = (Pr_TextureRef) ap_obj;

    SDL_DestroyTexture(ap_tex->impl);

    s_textureLoader.textureCount--;
    if (s_textureLoader.textureCount == 0) {
        Pr_DeleteResourceLoader(s_textureLoader.loader);
        s_textureLoader.loader = NULL;
    }
}

Pr_Class Pr_TextureClass = {
    "Pr_Texture",
    sizeof(Pr_Texture),
    0,
    &Pr_ResourceClass,
    NULL,
    0,
    PR_FALSE,
    s_Pr_ConstructTexture,
    s_Pr_DestructTexture
};

Pr_TextureRef  Pr_NewTexture(pr_cstring_t ap_file)
{
    Pr_TextureRef lp_out;

    if (!ap_file || !ap_file[0]) return NULL;

    lp_out = Pr_New(&Pr_TextureClass);
        if (!lp_out) return NULL;

    if (Pr_LoadResourceFile((Pr_ResourceRef) lp_out, ap_file)) {
        return lp_out;
    }

    Pr_Delete((Pr_ObjectRef) lp_out);

    return NULL;
}

