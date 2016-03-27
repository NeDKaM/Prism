#ifndef H_RAINBOW_IMAGE_INCLUDED
#define H_RAINBOW_IMAGE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/Rainbow/vector.h>
#include <Prism/Rainbow/rect.h>
#include <Prism/Rainbow/color.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_image_t, Pr_Image);

    extern PRISM_API Pr_Image *         Pr_NewImage(pr_u32_t, pr_u32_t);
    extern PRISM_API void               Pr_DeleteImage(Pr_Image *);
    extern PRISM_API pr_bool_t          Pr_GetImageSize(Pr_ImageRef, Pr_Vector2i *);
    extern PRISM_API void               Pr_SetImagePixel(Pr_Image *, Pr_ColorRef, pr_u32_t, pr_u32_t);
    extern PRISM_API pr_bool_t          Pr_GetImagePixel(Pr_ImageRef, Pr_Color *, pr_u32_t, pr_u32_t);
    extern PRISM_API Pr_ByteArrayRef    Pr_GetImagePixels(Pr_ImageRef);
    extern PRISM_API void               Pr_FillImageRect(Pr_Image *, Pr_RectRef(long), Pr_ColorRef);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_IMAGE_INCLUDED */

