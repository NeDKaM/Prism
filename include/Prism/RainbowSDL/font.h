#ifndef H_RAINBOWSDL_FONT_INCLUDED
#define H_RAINBOWSDL_FONT_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    #define PR_GLYPH_MAX        128
    #define PR_FONTSIZE_DEFAULT 12

    PR_STRUCT(pr_font_t, Pr_Font);

    PR_STRUCT(pr_glyphmetrics_t, Pr_GlyphMetrics) {
        SDL_Rect    texRect;
        long        horiBearingX;
        long        horiBearingY;
        long        horiAdvance;
        long        vertBearingX;
        long        vertBearingY;
        long        vertAdvance;
    };

    extern PRISM_API Pr_Font *      Pr_NewFont(SDL_Renderer * renderer, char const * file, pr_u32_t size, pr_bool_t fancy);
    extern PRISM_API void           Pr_DeleteFont(Pr_Font * font);
    extern PRISM_API pr_bool_t      Pr_GetFontKerning(Pr_Font * font, pr_u32_t left, pr_u32_t right, long * delta);
    extern PRISM_API pr_bool_t      Pr_GetFontGlyphMetrics(Pr_Font * font, Pr_GlyphMetrics * metrics, pr_u32_t charcode);
    extern PRISM_API SDL_Texture *  Pr_GetFontTexture(Pr_Font * font);  
    extern PRISM_API pr_bool_t      Pr_GetFontSize(Pr_Font * font, pr_u32_t * size); 

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_FONT_INCLUDED */
