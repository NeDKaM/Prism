#include <Prism/Rainbow/image.h>
#include <stdlib.h>
#include <Prism/array.h>
#include <Prism/Rainbow/vector.h>
#include <string.h>

struct pr_image_t {
    Pr_Array *  pixels;
    Pr_Vector2i size;
};

static pr_bool_t s_Pr_MakeImage(Pr_Image * ap_img, pr_u32_t a_w, pr_u32_t a_h)
{
    pr_u32_t l_size;
    
    l_size = a_w * a_h * 4;

    ap_img->pixels = Pr_NewArray(a_w * a_h * 4, sizeof(pr_u8_t));
    if (!ap_img->pixels) return PR_FALSE;

    ap_img->size.x = a_w;
    ap_img->size.y = a_h;

    return PR_TRUE;
}

Pr_Image * Pr_NewImageMatrix(pr_u32_t a_w, pr_u32_t a_h)
{
    Pr_Image * lp_out;

    if (a_w == 0 || a_h == 0) return NULL;

    lp_out = malloc(sizeof(Pr_Image));
    if (!lp_out) return NULL;

    if (s_Pr_MakeImage(lp_out, a_w, a_h)) return lp_out;

    free(lp_out);

    return NULL;
}

void Pr_DeleteImage(Pr_Image * ap_img)
{
    if (!ap_img) return;

    Pr_DeleteArray(ap_img->pixels);
    free(ap_img);
}

pr_bool_t Pr_GetImageSize(Pr_ImageRef ap_img, Pr_Vector2i * ap_size)
{
    if (!ap_img || !ap_size) return PR_FALSE;

    *ap_size = ap_img->size;

    return PR_TRUE;
}

pr_u8_t * Pr_GetImagePixels(Pr_ImageRef ap_img)
{
    if (!ap_img) return NULL;

    return (pr_u8_t *)Pr_GetArrayPointer(ap_img->pixels);
}

void Pr_SetImagePixel(Pr_Image * ap_img, Pr_ColorRef ap_color, pr_u32_t a_x, pr_u32_t a_y)
{
    pr_u8_t * lp_pixels;

    if (!ap_img) return;
    if (a_x >= (pr_u32_t)ap_img->size.x || a_y >= (pr_u32_t)ap_img->size.y) return;

    lp_pixels = Pr_GetArrayPointer(ap_img->pixels);
    lp_pixels += (a_x + a_y * ap_img->size.x) * 4;

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

    lp_pixels = Pr_GetArrayPointer(ap_img->pixels); 
    lp_pixels += (a_x + a_y * ap_img->size.x) * 4;
    
    ap_color->r = lp_pixels[0];
    ap_color->b = lp_pixels[1];
    ap_color->b = lp_pixels[2];
    ap_color->a = lp_pixels[3];

    return PR_TRUE;
}

pr_bool_t Pr_FillImageRect(Pr_Image * ap_img, Pr_IntRectRef ap_rect, Pr_ColorRef ap_color)
{
    Pr_IntRect    l_rect;
    pr_u32_t    l_x;
    pr_u32_t    l_y;

    if (!ap_img) return PR_FALSE;
    if (!ap_color) return PR_FALSE;

    if (ap_rect) {
        l_rect.x        = (ap_rect->x < 0) ? 0 : ap_rect->x;
        l_rect.y        = (ap_rect->y < 0) ? 0 : ap_rect->y;
        l_rect.width    = (ap_rect->width > ap_img->size.x) ? ap_img->size.x : ap_rect->width;
        l_rect.height   = (ap_rect->height > ap_img->size.y) ? ap_img->size.y : ap_rect->height;
    } else {
        l_rect.x        = 0;
        l_rect.y        = 0;
        l_rect.width    = ap_img->size.x;
        l_rect.height   = ap_img->size.y;
    }

    for (l_x = 0 ; l_x < l_rect.width ; l_x++) {
        for (l_y = 0 ; l_y < l_rect.height ; l_y++) {
            Pr_SetImagePixel(ap_img, ap_color, l_x, l_y);
        }
    }

    return PR_TRUE;
}

