#include <Prism/Rainbow/image.h>
#include <stdlib.h>
#include <Prism/array.h>
#include <Prism/Rainbow/vector.h>
#include <string.h>

struct pr_image_t {
    pr_u8_t *   pixels;
    Pr_Vector2i size;
};

Pr_Image * Pr_NewImage(pr_u32_t a_w, pr_u32_t a_h)
{
    Pr_Image * lp_out;
    pr_u32_t l_size;

    if (a_w == 0 || a_h == 0) return NULL;

    lp_out = malloc(sizeof(Pr_Image));
    if (!lp_out) return NULL;

    l_size = a_w * a_h * 4;

    lp_out->pixels = calloc(l_size, sizeof(pr_u8_t));
    if (!lp_out->pixels) {
        free(lp_out);
        return NULL;
    }

    lp_out->size.x = a_w;
    lp_out->size.y = a_h;

    return lp_out;
}

void Pr_DeleteImage(Pr_Image * ap_img)
{
    if (!ap_img) return;

    free(ap_img->pixels);
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

    return ap_img->pixels;
}

void Pr_SetImagePixel(Pr_Image * ap_img, Pr_ColorRef ap_color, pr_u32_t a_x, pr_u32_t a_y)
{
    pr_u8_t * lp_pixels;

    if (!ap_img) return;
    if (a_x >= (pr_u32_t)ap_img->size.x || a_y >= (pr_u32_t)ap_img->size.y) return;

    lp_pixels = ap_img->pixels + (a_x + a_y * ap_img->size.x) * 4;

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

    lp_pixels = ap_img->pixels + (a_x + a_y * ap_img->size.x) * 4;
    
    ap_color->r = lp_pixels[0];
    ap_color->b = lp_pixels[1];
    ap_color->b = lp_pixels[2];
    ap_color->a = lp_pixels[3];

    return PR_TRUE;
}

void Pr_FillImageRect(Pr_Image * ap_img, Pr_RectRef(long) ap_rect, Pr_ColorRef ap_color)
{
    Pr_Recti    l_rect;
    Pr_Color    l_color;
    pr_u8_t *   lp_pixels;
    pr_u32_t    l_w;

    if (!ap_img) return;

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

    if (ap_color) {
        memcpy(&l_color, ap_color, sizeof(Pr_Color));
    } else {
        memset(ap_img->pixels, 0, ap_img->size.x * ap_img->size.y * 4);
        return;
    }

    lp_pixels = ap_img->pixels + (l_rect.x + l_rect.y * ap_img->size.x) * 4;

    while (l_rect.height--) {
        l_w = l_rect.width;
        while (l_w--) {
            *lp_pixels++ = l_color.r;
            *lp_pixels++ = l_color.g;
            *lp_pixels++ = l_color.b;
            *lp_pixels++ = l_color.a;
        }

        lp_pixels += (ap_img->size.x - (l_rect.x + l_rect.width)) + l_rect.x;
    }
}

