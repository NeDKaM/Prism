#ifndef H_RAINBOWSDL_RENDERABLE_INCLUDED
#define H_RAINBOWSDL_RENDERABLE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

#include <SDL2/SDL.h>

PR_CPP_PROTECT_ON

    PR_ENUM(pr_shapetype_t, Pr_ShapeType) {
        PR_SHAPE_NONE,
        PR_SHAPE_LINE,
        PR_SHAPE_RECT,
        PR_SHAPE_FILLEDRECT,

        PR_SHAPE_COUNT
    };

    PR_STRUCT(pr_shape_t, Pr_Shape) {
        Pr_ShapeType    type;
        union {
            struct { 
                pr_u32_t w, h;
            } size;
            struct {
                SDL_Point start, end;
            } line;
        }               data;
    };

    PR_STRUCT(pr_renderable_t, Pr_Renderable) {
        Pr_Shape                    shape;
        SDL_Rect                    textureCoords;
        SDL_Texture *               texture;
        SDL_Point                   origin;
        SDL_Color                   colorMod;
        SDL_BlendMode               blendMode;
        pr_bool_t                   originOnly;
    };

    extern PRISM_API Pr_Renderable Pr_MakeRenderable(void);
    extern PRISM_API Pr_Renderable Pr_MakeRenderableLine(SDL_Point, SDL_Point);
    extern PRISM_API Pr_Renderable Pr_MakeRenderableRect(pr_u32_t, pr_u32_t, pr_bool_t);
    extern PRISM_API Pr_Renderable Pr_MakeRenderableSprite(SDL_Texture *, SDL_Rect *, pr_u32_t, pr_u32_t);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_RENDERABLE_INCLUDED */

/*


*/
