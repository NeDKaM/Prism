#include <Prism/RainbowSDL/renderable.h>
#include <string.h>

Pr_Renderable Pr_MakeRenderable(void)
{
    Pr_Renderable l_out;

    l_out.texture = NULL;

    memset(&l_out.colorMod, 0xff, sizeof(SDL_Color));

    memset(&l_out.textureCoords, 0, sizeof(SDL_Rect));

    l_out.origin.x = 0;
    l_out.origin.y = 0;
    l_out.originOnly = PR_FALSE;

    l_out.blendMode = SDL_BLENDMODE_BLEND;

    l_out.shape.type = PR_SHAPE_NONE;

    memset(&l_out.shape.data, 0, sizeof(l_out.shape.data));

    return l_out;
}

Pr_Renderable Pr_MakeRenderableLine(SDL_Point a_p1, SDL_Point a_p2)
{
    Pr_Renderable l_out;

    l_out = Pr_MakeRenderable();

    l_out.shape.type = PR_SHAPE_LINE;
    l_out.shape.data.line.start = a_p1;
    l_out.shape.data.line.end = a_p2;

    return l_out;
}

Pr_Renderable Pr_MakeRenderableRect(pr_u32_t a_w, pr_u32_t a_h, pr_bool_t a_filled) 
{
    Pr_Renderable l_out;

    l_out = Pr_MakeRenderable();

    l_out.shape.type = (a_filled) ? PR_SHAPE_FILLEDRECT : PR_SHAPE_RECT;
    l_out.shape.data.size.w = a_w;
    l_out.shape.data.size.h = a_h;

    return l_out;
}

Pr_Renderable Pr_MakeRenderableSprite(SDL_Texture * a_tex, SDL_Rect * ap_texRect, pr_u32_t a_w, pr_u32_t a_h)
{
    Pr_Renderable l_out;

    l_out = Pr_MakeRenderableRect(a_w, a_h, PR_TRUE);

    if (!a_tex) return l_out;

    if (!ap_texRect) {
        SDL_Rect l_texRect;
            l_texRect.x = 0;
            l_texRect.y = 0;
        SDL_QueryTexture(a_tex, NULL, NULL, &l_texRect.w, &l_texRect.h);
        l_out.textureCoords = l_texRect;
    } else {
        l_out.textureCoords = *ap_texRect;
    }

    l_out.texture = a_tex;

    return l_out;
}
