#include <Prism/Rainbow/sprite.h>
#include <stdlib.h>

static void s_Pr_UpdatePositions(Pr_Sprite * ap_spr) 
{
    Pr_FloatRect l_rect;

    Pr_GetSpriteLocalBounds(ap_spr, &l_rect);

    ap_spr->vertices[0].position.x = 0.f;
    ap_spr->vertices[0].position.y = 0.f;

    ap_spr->vertices[1].position.x = 0.f;
    ap_spr->vertices[1].position.y = l_rect.height;

    ap_spr->vertices[2].position.x = l_rect.width;
    ap_spr->vertices[2].position.y = 0.f;

    ap_spr->vertices[3].position.x = l_rect.width;
    ap_spr->vertices[3].position.y = l_rect.height;
}

static void s_Pr_UpdateTexCoords(Pr_Sprite * ap_spr)
{
    float l_left    = (float)ap_spr->sourceRect.x;
    float l_right   = l_left + ap_spr->sourceRect.width;
    float l_top     = (float)ap_spr->sourceRect.y;
    float l_bottom  = l_top  + ap_spr->sourceRect.height;

    ap_spr->vertices[0].texCoords.x = l_left;
    ap_spr->vertices[0].texCoords.y = l_top;

    ap_spr->vertices[1].texCoords.x = l_left;
    ap_spr->vertices[1].texCoords.y = l_bottom;

    ap_spr->vertices[2].texCoords.x = l_right;
    ap_spr->vertices[2].texCoords.y = l_top;

    ap_spr->vertices[3].texCoords.x = l_right;
    ap_spr->vertices[3].texCoords.y = l_bottom;
}

static void s_Pr_InitVertices(Pr_Sprite * ap_spr)
{
    int l_i;
    Pr_Vector2f pos = {0.f, 0.f};
    Pr_Color color = {255, 255, 255, 255};

    for (l_i=0 ; l_i<4 ; l_i++) {
        ap_spr->vertices[l_i].color     = color;
        ap_spr->vertices[l_i].position  = pos;
        ap_spr->vertices[l_i].texCoords = pos;
    }
}

void Pr_SetSpriteTexture(Pr_Sprite * ap_spr, Pr_TextureRef ap_tex, pr_bool_t a_uptd)
{
    if (!ap_spr) return;

    Pr_InitTransformable(&ap_spr->transformable);
    s_Pr_InitVertices(ap_spr);

    ap_spr->texture = ap_tex;

    if (a_uptd) {
        Pr_SetSpriteSrcRect(ap_spr, NULL);
    }
}

void Pr_SetSpriteSrcRect(Pr_Sprite * ap_spr, Pr_IntRectRef ap_rect)
{
    if (!ap_spr) return;

    if (ap_rect) {
        ap_spr->sourceRect = *ap_rect;  
    } else {
        Pr_Vector2i l_size;
        ap_spr->sourceRect.x = 0;
        ap_spr->sourceRect.y = 0;
        if (Pr_GetTextureSize(ap_spr->texture, &l_size)) {
            ap_spr->sourceRect.width    = l_size.x;
            ap_spr->sourceRect.height   = l_size.y;
        } else {
            ap_spr->sourceRect.width    = 0;
            ap_spr->sourceRect.height   = 0;
        }
    }

    s_Pr_UpdatePositions(ap_spr);
    s_Pr_UpdateTexCoords(ap_spr);
}

void Pr_RenderDrawSprite(Pr_Sprite * ap_spr, Pr_RenderingTarget * ap_target, Pr_RenderingStatesRef ap_states)
{
    Pr_Transform        lp_tr;
    Pr_RenderingStates  l_states;

    if (!ap_spr || !ap_target) return;
    if (!ap_spr->texture) return;

    l_states.blenMode   = *Pr_BlendModeAlpha();
    l_states.shader     = NULL;
    l_states.texture    = ap_spr->texture;
    Pr_MakeTransformIdentity(l_states.transform);

    if (ap_states) {
        l_states.blenMode   = ap_states->blenMode;
        l_states.shader     = ap_states->shader;
    } 

    Pr_GetTransformableTransform(&ap_spr->transformable, lp_tr);
    Pr_TransformCombine(l_states.transform, lp_tr);

    Pr_RndTargetDraw(ap_target, ap_spr->vertices, 4, PR_PRIMITIVE_TRIANGLESTRIP, &l_states);
}

pr_bool_t Pr_GetSpriteLocalBounds(Pr_SpriteRef ap_spr, Pr_FloatRect * ap_dst)
{
    if (!ap_spr || !ap_dst) return PR_FALSE;

    ap_dst->x       = 0.f;
    ap_dst->y       = 0.f;
    ap_dst->width   = (float)ap_spr->sourceRect.width;
    ap_dst->height  = (float)ap_spr->sourceRect.height;

    return PR_TRUE;
}

pr_bool_t Pr_GetSpriteGlobalBounds(Pr_Sprite * ap_spr, Pr_FloatRect * ap_dst)
{
    Pr_Transform    lp_tr;
    Pr_FloatRect    l_rect;

    if (!ap_spr || !ap_dst) return PR_FALSE;

    Pr_GetTransformableTransform(&ap_spr->transformable, lp_tr);
    Pr_GetSpriteLocalBounds(ap_spr, &l_rect);
    Pr_TransformRect(lp_tr, &l_rect, ap_dst);

    return PR_TRUE;
}

void Pr_SetSpritePosition(Pr_Sprite * ap_spr, float a_x, float a_y)
{  
    if (!ap_spr) return;

    Pr_SetTransformablePosition(&ap_spr->transformable, a_x, a_y);
}
