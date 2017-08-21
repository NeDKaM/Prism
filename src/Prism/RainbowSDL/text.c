#include <Prism/RainbowSDL/text.h>

struct pr_simpletextrendererinfo_t Pr_SimpleTextRendererInfo = {
    { 255, 255, 255, SDL_ALPHA_OPAQUE },
    NULL,
    1.f
};

struct pr_textsprite_t {
    Pr_TextRenderer renderer;
    void *          renderData;
    Pr_Array *      renderables;
    Pr_String *     text;
};

static void s_Pr_InitializeGlyph(Pr_Renderable * ap_rnd, struct pr_simpletextrendererinfo_t * ap_info, SDL_Rect * ap_texRect)
{
    *ap_rnd = Pr_MakeRenderableSprite(
        Pr_GetFontTexture(ap_info->font), 
        *ap_texRect, 
        (pr_u32_t)((float)ap_texRect->w * ap_info->scale), 
        (pr_u32_t)((float)ap_texRect->h * ap_info->scale)
    );

    ap_rnd->colorMod = ap_info->colorMod;
}

static void s_Pr_SimpleTextRenderer(void * ap_data, Pr_Array * ap_renderables, pr_cstring_t ap_text)
{
    struct pr_simpletextrendererinfo_t * lp_info = ap_data;
    
    pr_cstring_t    lp_ptr;
    pr_u32_t        l_i = 0;
    long            l_x = 0;
    pr_u32_t        l_y = 0;

    if (!lp_info) return;

    if (!lp_info->font) return;
    if (!SDL_strlen(ap_text)) return;

    lp_ptr = ap_text;

    Pr_GetFontSize(lp_info->font, &l_y);

    while (*ap_text) {
        Pr_GlyphMetrics l_metrics;
        Pr_Renderable l_renderable; 

        Pr_GetFontGlyphMetrics(lp_info->font, &l_metrics, *ap_text);

        s_Pr_InitializeGlyph(&l_renderable, lp_info, &l_metrics.texRect);

        if (ap_text != lp_ptr) {
            long l_delta = 0;
            Pr_GetFontKerning(lp_info->font, *(ap_text-1), *ap_text, &l_delta);
            l_x += (long)(l_delta * lp_info->scale);
        }

        l_renderable.origin.x = l_x;
        l_renderable.origin.y = (long)(((long)l_y - l_metrics.horiBearingY) * lp_info->scale);

        Pr_SetArrayAt(ap_renderables, l_i, &l_renderable);

        l_x += (long)(l_metrics.horiAdvance * lp_info->scale);

        l_i++;
        ap_text++;
    }
}

Pr_TextRenderer const Pr_SimpleTextRenderer = s_Pr_SimpleTextRenderer;

Pr_TextSprite *    Pr_NewTextSprite(void)
{
    Pr_TextSprite * lp_out;

    lp_out = malloc(sizeof(Pr_TextSprite));
    if (!lp_out) return NULL;

    lp_out->renderables = Pr_NewArray(sizeof(Pr_Renderable), NULL);
    lp_out->text = Pr_NewString();

    if (lp_out->renderables) {
        lp_out->renderer = Pr_SimpleTextRenderer;
        lp_out->renderData = &Pr_SimpleTextRendererInfo;
        return lp_out;
    }

    Pr_DeleteTextSprite(lp_out);

    return NULL;
}

void               Pr_DeleteTextSprite(Pr_TextSprite * ap_spr)
{
    if (!ap_spr) return;

    Pr_DeleteArray(ap_spr->renderables);
    Pr_DeleteString(ap_spr->text);

    free(ap_spr);
}

void               Pr_SetTextSpriteRenderer(Pr_TextSprite * ap_spr, Pr_TextRenderer ap_rnd, void * ap_data)
{
    if (!ap_spr || !ap_rnd) return;

    ap_spr->renderer = ap_rnd;
    ap_spr->renderData = ap_data;
}

Pr_String * const  Pr_GetTextSpriteStr(Pr_TextSprite * ap_spr)
{
    return (ap_spr) ? ap_spr->text : NULL;
}

Pr_Array * const   Pr_UpdateTextSprite(Pr_TextSprite * ap_spr)
{
    if (!ap_spr) return NULL;

    ap_spr->renderer(ap_spr->renderData, ap_spr->renderables, Pr_StringCStr(ap_spr->text));

    return ap_spr->renderables;
}

