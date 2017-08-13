#ifndef H_RAINBOWSDL_TEXT_INCLUDED
#define H_RAINBOWSDL_TEXT_INCLUDED

#include <Prism/memory.h>
#include <Prism/string.h>

#include <Prism/RainbowSDL/font.h>
#include <Prism/RainbowSDL/renderable.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_textsprite_t, Pr_TextSprite);

    typedef void (*Pr_TextRenderer)(void * data, Pr_Array * renderables, pr_cstring_t);

    extern PRISM_API Pr_TextSprite *    Pr_NewTextSprite(void);
    extern PRISM_API void               Pr_DeleteTextSprite(Pr_TextSprite *);

    extern PRISM_API void               Pr_SetTextSpriteRenderer(Pr_TextSprite *, Pr_TextRenderer, void *);
    extern PRISM_API Pr_String * const  Pr_GetTextSpriteStr(Pr_TextSprite *);

    extern PRISM_API Pr_Array * const   Pr_UpdateTextSprite(Pr_TextSprite *);

    extern PRISM_API Pr_TextRenderer const Pr_SimpleTextRenderer;

    struct pr_simpletextrendererinfo_t {
        SDL_Color   colorMod;
        Pr_Font *   font;
        float       scale; 
    } extern PRISM_API Pr_SimpleTextRendererInfo;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_TEXT_INCLUDED */

/*



*/
