#include <Prism/RainbowSDL/font.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

struct {
    FT_Library  lib;
    pr_u32_t    faceCount;
} static s_ftlibrary = {NULL, 0};

PR_STRUCT(pr_glyph_t, Pr_Glyph) {
    Pr_GlyphMetrics metrics;
    pr_u32_t        index;
    pr_bool_t       loaded;
};

struct pr_font_t {
    Pr_Glyph        glyphs[PR_GLYPH_MAX];
    SDL_Texture *   texture;
    FT_Face         ftFace;
    pr_u32_t        size;
    pr_bool_t       hasKerning;        
};

static void s_Pr_ShouldQuitFreeType(void)
{
    if (s_ftlibrary.lib && s_ftlibrary.faceCount) {
        FT_Done_FreeType(s_ftlibrary.lib);
        s_ftlibrary.faceCount = 0;
    }
}

static void s_Pr_SetSurfacePixel(SDL_Surface * ap_surface, Uint32 a_color, pr_u32_t a_x, pr_u32_t a_y)
{
    Uint8 * lp_pix = (Uint8*)ap_surface->pixels + a_y * ap_surface->pitch + a_x * 4;

    *(Uint32*)lp_pix = a_color;
}

static void s_Pr_BlitGlyphOnSurface(SDL_Surface * ap_dst, FT_Bitmap * ap_bitmap)
{
    pr_u32_t l_i;
    pr_u32_t l_j;
    pr_u8_t l_pix;

    for (l_i = 0 ; l_i < ap_bitmap->width ; l_i++) {
        for (l_j = 0; l_j < ap_bitmap->rows ; l_j++) {
            l_pix = ap_bitmap->buffer[l_i + l_j * ap_bitmap->width];
            if (l_pix > 0) {
                s_Pr_SetSurfacePixel(ap_dst, SDL_MapRGBA(ap_dst->format, 255, 255, 255, l_pix), l_i, l_j);
            }
        }
    }
}

static void s_Pr_BlitPackedGlyphOnSurface(SDL_Surface * ap_dst, FT_Bitmap * ap_bitmap)
{
    pr_u32_t l_i;
    pr_u32_t l_j;
    int l_pitch;
    pr_u8_t * lp_byteRow;

    l_pitch = (ap_bitmap->pitch < 0) ? (-ap_bitmap->pitch) : ap_bitmap->pitch;

    for (l_i = 0 ; l_i < ap_bitmap->width ; l_i++) {
        for (l_j = 0 ; l_j < ap_bitmap->rows ; l_j++) {
            lp_byteRow = &ap_bitmap->buffer[l_pitch * l_j];
            if ((lp_byteRow[l_i >> 3] & (128 >> (l_i & 7))) != 0) {
                s_Pr_SetSurfacePixel(ap_dst, SDL_MapRGBA(ap_dst->format, 255, 255, 255, 255), l_i, l_j);
            }
        }
    }
}

Pr_Font * Pr_NewFont(SDL_Renderer * ap_rnd, char const * ap_file, pr_u32_t a_size, pr_bool_t a_fancy)
{
    Pr_Font * lp_out;
    SDL_Surface * lp_surface;
    SDL_Surface * lp_glyphbitmaps[PR_GLYPH_MAX];
    FT_Face lp_face;
    FT_Error l_error;
    pr_u32_t l_i;
    pr_u32_t l_w;

    if (!ap_rnd) {
        return NULL;
    }

    lp_out = malloc(sizeof(Pr_Font));
    if (!lp_out) {
        return NULL;
    }

    if (!s_ftlibrary.lib) {
        l_error = FT_Init_FreeType(&s_ftlibrary.lib);
        if (l_error != FT_Err_Ok) {
            free(lp_out);
            return NULL;
        }
    }

    l_error = FT_New_Face(s_ftlibrary.lib, ap_file, 0, &lp_face);
    if (l_error != FT_Err_Ok) {
        s_Pr_ShouldQuitFreeType();
        free(lp_out);
        return NULL;
    }

    lp_out->ftFace = lp_face;

    lp_out->size = (a_size > 0) ? a_size : PR_FONTSIZE_DEFAULT;

    FT_Set_Pixel_Sizes(lp_face, 0, lp_out->size);

    lp_out->hasKerning = FT_HAS_KERNING(lp_out->ftFace);

    l_w = 0;

    for (l_i=0 ; l_i<PR_GLYPH_MAX ; l_i++) {
        Pr_Glyph * lp_tmpGlyph = &lp_out->glyphs[l_i];
        FT_GlyphSlot lp_slot = lp_out->ftFace->glyph;

        lp_tmpGlyph->loaded = PR_FALSE;

        if (a_fancy) {
            l_error = FT_Load_Char(lp_out->ftFace, l_i, FT_LOAD_RENDER);
        } else {
            l_error = FT_Load_Char(lp_out->ftFace, l_i, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
        }

        if (l_error != FT_Err_Ok) {
            continue;
        }

        lp_glyphbitmaps[l_i] = SDL_CreateRGBSurface(0, lp_slot->bitmap.width, lp_out->size, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
        );

        if (!lp_glyphbitmaps[l_i]) {
            continue;
        }

        SDL_FillRect(lp_glyphbitmaps[l_i], NULL, SDL_MapRGBA(lp_glyphbitmaps[l_i]->format, 0, 0, 0, 0));

        lp_tmpGlyph->metrics.texRect.x = l_w;
        lp_tmpGlyph->metrics.texRect.y = 0;
        lp_tmpGlyph->metrics.texRect.h = lp_slot->bitmap.rows;
        lp_tmpGlyph->metrics.texRect.w = lp_slot->bitmap.width;

        if (a_fancy) {
            s_Pr_BlitGlyphOnSurface(lp_glyphbitmaps[l_i], &lp_slot->bitmap);
        } else {
            s_Pr_BlitPackedGlyphOnSurface(lp_glyphbitmaps[l_i], &lp_slot->bitmap);
        }

        lp_tmpGlyph->metrics.vertAdvance    = lp_slot->metrics.vertAdvance >> 6;
        lp_tmpGlyph->metrics.horiAdvance    = lp_slot->metrics.horiAdvance >> 6;
        lp_tmpGlyph->metrics.horiBearingX   = lp_slot->metrics.horiBearingX >> 6;
        lp_tmpGlyph->metrics.horiBearingY   = lp_slot->metrics.horiBearingY >> 6;
        lp_tmpGlyph->metrics.vertBearingX   = lp_slot->metrics.vertBearingX >> 6;
        lp_tmpGlyph->metrics.vertBearingY   = lp_slot->metrics.vertBearingY >> 6;

        lp_tmpGlyph->index = FT_Get_Char_Index(lp_out->ftFace, l_i);

        lp_tmpGlyph->loaded = PR_TRUE;

        l_w += lp_tmpGlyph->metrics.texRect.w;
    }

    lp_surface = SDL_CreateRGBSurface(0, l_w, lp_out->size, 32, 
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
    );

    lp_out->texture = NULL;

    if (lp_surface) {
        SDL_FillRect(lp_surface, NULL, SDL_MapRGBA(lp_surface->format, 0, 0, 0, 0));
        for (l_i=0 ; l_i<PR_GLYPH_MAX ; l_i++) {
            SDL_BlitSurface(lp_glyphbitmaps[l_i], NULL, lp_surface, &lp_out->glyphs[l_i].metrics.texRect);
        }

        lp_out->texture = SDL_CreateTextureFromSurface(ap_rnd, lp_surface);

        SDL_FreeSurface(lp_surface);
    }

    for (l_i=0 ; l_i<PR_GLYPH_MAX ; l_i++) {
        if (lp_out->glyphs[l_i].loaded) {
            SDL_FreeSurface(lp_glyphbitmaps[l_i]);
        }
    }

    if (!lp_out->texture) {
        Pr_DeleteFont(lp_out);
        lp_out = NULL;
    }

    return lp_out;
}


void Pr_DeleteFont(Pr_Font * ap_f)
{
    if (ap_f) {
        SDL_DestroyTexture(ap_f->texture);

        FT_Done_Face(ap_f->ftFace);

        free(ap_f);

        s_ftlibrary.faceCount--;
    }

    s_Pr_ShouldQuitFreeType();
}

SDL_Texture * Pr_GetFontTexture(Pr_Font * ap_font)
{
    return (ap_font) ? ap_font->texture : NULL;
}

pr_bool_t Pr_GetFontGlyphMetrics(Pr_Font * ap_font, Pr_GlyphMetrics * ap_glyph, pr_u32_t a_charcode)
{
    if (!ap_font) {
        return PR_FALSE;
    }

    if (!ap_glyph) {
        return PR_FALSE;
    }

    if (a_charcode >= PR_GLYPH_MAX) {
        return PR_FALSE;
    }

    if (!ap_font->glyphs[a_charcode].loaded) {
        return PR_FALSE;
    }

    *ap_glyph = ap_font->glyphs[a_charcode].metrics;

    return PR_TRUE;
}

pr_bool_t Pr_GetFontKerning(Pr_Font * ap_f, pr_u32_t a_l, pr_u32_t a_r, long * a_d)
{
    if (!ap_f) return PR_FALSE;
    if (a_l >= PR_GLYPH_MAX || a_r >= PR_GLYPH_MAX) return PR_FALSE;
    if (!a_d) return PR_FALSE;

    if (!ap_f->glyphs[a_l].loaded) return PR_FALSE;
    if (!ap_f->glyphs[a_r].loaded) return PR_FALSE;

    if (ap_f->hasKerning) {
        FT_Vector l_delta;

        FT_Get_Kerning(
            ap_f->ftFace, 
            ap_f->glyphs[a_l].index, ap_f->glyphs[a_r].index, 
            FT_KERNING_DEFAULT, &l_delta
        );

        *a_d = l_delta.x >> 6;
    } else {
        *a_d = 0;
    }

    return PR_TRUE;
}

pr_bool_t Pr_GetFontSize(Pr_Font * ap_font, pr_u32_t * ap_size)
{
    if (!ap_font) return PR_FALSE;

    *ap_size = ap_font->size;

    return PR_TRUE;
}

