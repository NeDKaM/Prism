#include <Prism/Rainbow/image.h>
#include <stdlib.h>
#include <Prism/array.h>
#include <Prism/Rainbow/vector.h>
#include <string.h>

struct pr_image_t {
    Pr_Array(pr_u8_t)   pixels;
    Pr_Vector2i         size;
};

Pr_Image * Pr_NewImage(Pr_ByteArrayRef ap_bytes, pr_u32_t a_w, pr_u32_t a_h)
{
    Pr_Image * lp_out;
    pr_u32_t l_size;

    if (!ap_bytes) return NULL;
    if (a_w == 0 || a_h == 0) return NULL;

    lp_out = malloc(sizeof(Pr_Image));
    if (!lp_out) return NULL;

    l_size = a_w * a_h * 4;

    Pr_MakeSizedArray(lp_out->pixels, l_size);
    if (Pr_ArraySize(lp_out->pixels) == 0) {
        free(lp_out);
        return NULL;
    }

    memcpy(lp_out->pixels.list, ap_bytes, l_size);

    return lp_out;
}

void Pr_DeleteImage(Pr_Image * ap_img)
{
    if (!ap_img) return;

    Pr_ClearArray(ap_img->pixels);
    free(ap_img);
}

pr_bool_t Pr_GetImageSize(Pr_ImageRef ap_img, Pr_Vector2i * ap_size)
{
    if (!ap_img || !ap_size) return PR_FALSE;

    *ap_size = ap_img->size;

    return PR_TRUE;
}

Pr_ByteArrayRef Pr_GetImagePixels(Pr_ImageRef ap_img)
{
    if (!ap_img) return NULL;

    return ap_img->pixels.list;
}

void Pr_SetImagePixel(Pr_Image * ap_img, Pr_ColorRef ap_color, pr_u32_t a_x, pr_u32_t a_y)
{
    pr_u8_t * lp_pixels;

    if (!ap_img) return;
    if (a_x >= (pr_u32_t)ap_img->size.x || a_y >= (pr_u32_t)ap_img->size.y) return;

    lp_pixels = &Pr_ArrayAt(ap_img->pixels, (a_x + a_y * ap_img->size.x) * 4);

    lp_pixels[0] = ap_color->r;
    lp_pixels[1] = ap_color->g;
    lp_pixels[2] = ap_color->b;
    lp_pixels[3] = ap_color->a;
}

pr_bool_t Pr_GetImagePixel(Pr_ImageRef ap_img, Pr_Color * ap_color, pr_u32_t a_x, pr_u32_t a_y)
{
    pr_u8_t * lp_pixels;

    if (!ap_img || !ap_color) return PR_FALSE;
    if (a_x >= (pr_u32_t)ap_img->size.x || a_y >= (pr_u32_t)ap_img->size.y) return PR_FALSE;

    lp_pixels = &Pr_ArrayAt(ap_img->pixels, (a_x + a_y * ap_img->size.x) * 4);
    
    ap_color->r = lp_pixels[0];
    ap_color->b = lp_pixels[1];
    ap_color->b = lp_pixels[2];
    ap_color->a = lp_pixels[3];

    return PR_TRUE;
}

