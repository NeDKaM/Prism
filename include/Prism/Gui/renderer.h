#ifndef H_PRISM_RENDERER_INCLUDED
#define H_PRISM_RENDERER_INCLUDED

#include <Prism/config.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct pr_texture_t     Pr_Texture;
    typedef struct pr_font_t        Pr_Font;
    typedef struct pr_renderer_t    Pr_Renderer;

    struct pr_renderer_t {

        void *  impl;

        int     (*loadTexture)(Pr_Renderer *, Pr_Texture *);
        void    (*freeTexture)(Pr_Renderer *, Pr_Texture *);
        void    (*renderTexture)(Pr_Renderer *, Pr_Texture *, void *, float, float, float, float);

        void    (*loadFont)(Pr_Renderer *, Pr_Font *);
        void    (*freeFont)(Pr_Renderer *, Pr_Font *);
        void    (*renderText)(Pr_Renderer *, Pr_Font *, int, int, char const *);
        void    (*textSize)(Pr_Renderer *, Pr_Font *, void *);

        void    (*drawLine)(Pr_Renderer *, int, int, int, int);
        void    (*drawPoint)(Pr_Renderer *, int, int);
        void    (*drawRect)(Pr_Renderer *, int, int, unsigned int, unsigned int);
        void    (*drawFilledRect)(Pr_Renderer *, int, int, unsigned int, unsigned int);

        void    (*setDrawColor)(Pr_Renderer *, int, int, int, int);
        void    (*setDrawBlendMode)(Pr_Renderer *, int);

        void    (*renderClear)(Pr_Renderer *);
        void    (*renderDisplay)(Pr_Renderer *);
    };

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_RENDERER_INCLUDED */