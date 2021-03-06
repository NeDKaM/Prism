#ifndef H_RAINBOW_SPRITE_INCLUDED
#define H_RAINBOW_SPRITE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/Rainbow/vertex.h>
#include <Prism/Rainbow/rect.h>
#include <Prism/Rainbow/texture.h>
#include <Prism/Rainbow/transform.h>
#include <Prism/Rainbow/renderingtarget.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_sprite_t, Pr_Sprite) {
        Pr_Texture *       texture;
        Pr_IntRect          sourceRect;
        Pr_Vertex           vertices[4];
        Pr_Transformable    transformable;
    };

    extern PRISM_API void       Pr_SetSpriteTexture(Pr_Sprite *, Pr_Texture *, pr_bool_t);
    extern PRISM_API void       Pr_SetSpriteSrcRect(Pr_Sprite *, Pr_IntRect *);
    extern PRISM_API void       Pr_RenderDrawSprite(Pr_Sprite *, Pr_RenderingTarget *, Pr_RenderingStates *);
    extern PRISM_API pr_bool_t  Pr_GetSpriteLocalBounds(Pr_Sprite *, Pr_FloatRect *);
    extern PRISM_API pr_bool_t  Pr_GetSpriteGlobalBounds(Pr_Sprite *, Pr_FloatRect *);
    extern PRISM_API void       Pr_SetSpritePosition(Pr_Sprite *, float, float);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_SPRITE_INCLUDED */
