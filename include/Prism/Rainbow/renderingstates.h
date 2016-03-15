#ifndef H_RAINBOW_RENDERINGSTATES_INCLUDED
#define H_RAINBOW_RENDERINGSTATES_INCLUDED

#include <Prism/config.h>
#include <Prism/Rainbow/blendmode.h>
#include <Prism/Rainbow/transform.h>
#include <Prism/Rainbow/texture.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_renderingstates_t, Pr_RenderingStates) {
        Pr_BlendMode    blenMode;
        /* Pr_ShaderRef */ void * shader; 
        Pr_TextureRef   texture;
        Pr_Transform    transform;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_RENDERINGSTATES_INCLUDED */
