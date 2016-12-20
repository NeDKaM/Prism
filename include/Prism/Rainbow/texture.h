#ifndef H_RAINBOW_TEXTURE_INCLUDED
#define H_RAINBOW_TEXTURE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/Rainbow/vector.h>
#include <Prism/Rainbow/image.h>
#include <Prism/Rainbow/rect.h>

PR_CPP_PROTECT_ON

    typedef enum pr_texturecoordinate_t Pr_TextureCoordinate;

    PR_STRUCT(pr_texture_t, Pr_Texture);

    enum pr_texturecoordinate_t {
        PR_TEXCOORD_NORMALIZED = 0,
        PR_TEXCOORD_PIXELS,
    };

    extern PRISM_API Pr_Texture *   Pr_NewTexture(Pr_Image *, Pr_IntRect *);
    extern PRISM_API pr_bool_t      Pr_GetTextureSize(Pr_Texture *, Pr_Vector2i *);
    extern PRISM_API void           Pr_DeleteTexture(Pr_Texture *);
    extern PRISM_API void           Pr_BindTexture(Pr_Texture *, Pr_TextureCoordinate);
    extern PRISM_API void           Pr_SetTextureSmooth(Pr_Texture *, pr_bool_t);
    extern PRISM_API void           Pr_SetTextureRepeated(Pr_Texture *, pr_bool_t);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_TEXTURE_INCLUDED */
