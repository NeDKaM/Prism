#include <Prism/Rainbow/renderingtarget.h>
#include <stdlib.h>
#include <Prism/Rainbow/opengl.h>
#include <Prism/Rainbow/texture.h>
#include <Prism/Rainbow/blendmode.h>
#include <string.h>
#include <Prism/Rainbow/transform.h>

/* friend Pr_Texture */ struct s_pr_texture_t {
    Pr_Vector2i size;
    Pr_Vector2i realSize;
    pr_u32_t    textureId;
    pr_u32_t    cacheId;
    pr_bool_t   isSmooth;
    pr_bool_t   isRepeated;
    pr_bool_t   flipped;
    pr_bool_t   hasFBO;
};

static pr_u32_t s_Pr_GetGLFactor(Pr_BlendModeFactor a_factor)
{
    switch (a_factor) {
        case PR_BLENDF_ZERO:            return GL_ZERO;
        case PR_BLENDF_1:               return GL_ONE;
        case PR_BLENDF_SRC :            return GL_SRC_COLOR;
        case PR_BLENDF_1MINSRC:         return GL_ONE_MINUS_SRC_COLOR;
        case PR_BLENDF_DST:             return GL_DST_COLOR;
        case PR_BLENDF_1MINDST:         return GL_ONE_MINUS_DST_COLOR;
        case PR_BLENDF_SRC_ALPHA:       return GL_SRC_ALPHA;
        case PR_BLENDF_1MINSRC_ALPHA:   return GL_ONE_MINUS_SRC_ALPHA;
        case PR_BLENDF_DST_ALPHA :      return GL_DST_ALPHA;
        case PR_BLENDF_1MINDST_ALPHA:   return GL_ONE_MINUS_DST_ALPHA;
        default:                        return GL_ZERO;
    }
}

static void s_Pr_ApplyCurrentView(Pr_RenderingTarget * ap_target)
{
    Pr_IntRect      l_viewport;
    Pr_Transform    lp_tr;
    long l_top;

    Pr_GetRndTargetViewport(ap_target, &ap_target->view, &l_viewport);
    l_top = ap_target->size.y - (l_viewport.y + l_viewport.height);

    glViewport(l_viewport.x, l_top, l_viewport.width, l_viewport.height);

    glMatrixMode(GL_PROJECTION);
    Pr_GetViewTransform(&ap_target->view, lp_tr);
    glLoadMatrixf(lp_tr);
    glMatrixMode(GL_MODELVIEW);

    ap_target->cache.viewChanged = PR_FALSE;
}

static void s_Pr_ApplyTexture(Pr_RenderingTarget * ap_target, Pr_TextureRef ap_tex)
{
    struct s_pr_texture_t const * lp_tmp = (struct s_pr_texture_t const *)ap_tex;

    Pr_BindTexture(ap_tex, PR_TEXCOORD_PIXELS);

    ap_target->cache.oldTextureId = (ap_tex) ? lp_tmp->cacheId : 0;
}

static void s_Pr_ApplyBlendMode(Pr_RenderingTarget * ap_target, Pr_BlendModeRef ap_mode)
{
    glBlendFunc(s_Pr_GetGLFactor(ap_mode->colorSrcFactor), s_Pr_GetGLFactor(ap_mode->colorDstFactor));

    ap_target->cache.oldBlendMode = *ap_mode;

}

static void s_Pr_ApplyTransform(Pr_TransformRef ap_transform)
{
    glLoadMatrixf(ap_transform);
}

static void s_Pr_ApplyShader(Pr_RenderingTargetRef ap_target, void * ap_shader)
{
}

pr_bool_t Pr_GetRndTargetViewport(Pr_RenderingTargetRef ap_target, Pr_ViewRef ap_view, Pr_IntRect * ap_dst)
{
    float l_w;
    float l_h;
    Pr_FloatRect lp_vp;

    if (!ap_target || !ap_view || !ap_dst) return PR_FALSE;

    l_w = (float)ap_target->size.x;
    l_h = (float)ap_target->size.y;
    lp_vp = ap_view->viewport;

    ap_dst->x       = (long)(.5f + l_w * lp_vp.x);
    ap_dst->y       = (long)(.5f + l_h * lp_vp.y);
    ap_dst->width   = (pr_u32_t)(.5f + l_w * lp_vp.width);
    ap_dst->height  = (pr_u32_t)(.5f + l_h * lp_vp.height);

    return PR_TRUE;
}

void Pr_ClearRndTarget(Pr_RenderingTarget * ap_target, Pr_ColorRef ap_color)
{
    if (!ap_target) return;

    s_Pr_ApplyTexture(ap_target,NULL);

    if (ap_color) {
        glClearColor(ap_color->r / 255.f, 
            ap_color->g / 255.f, 
            ap_color->b / 255.f, 
            ap_color->a / 255.f
        );
    } else {
        glClearColor(0.f,0.f,0.f,1.f);
    }

    glClear(GL_COLOR_BUFFER_BIT);
}

void Pr_PushGLStates(Pr_RenderingTarget * ap_target)
{
    if (!ap_target) return;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();

    Pr_ResetGLStates(ap_target);
}

void Pr_PopGLStates(Pr_RenderingTarget * ap_target)
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

void Pr_SetRndTargetView(Pr_RenderingTarget * ap_target, Pr_ViewRef ap_view)
{
    if (!ap_target || !ap_view) return;

    Pr_GetViewCopy(ap_view, &ap_target->view);
    ap_target->cache.viewChanged = PR_TRUE;
}

void Pr_ResetGLStates(Pr_RenderingTarget * ap_target)
{
    Pr_Transform lp_identity;

    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glMatrixMode(GL_MODELVIEW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    ap_target->cache.statesSet = PR_TRUE;

    Pr_MakeTransformIdentity(lp_identity);
    s_Pr_ApplyTransform(lp_identity);
    
    s_Pr_ApplyBlendMode(ap_target, Pr_BlendModeAlpha());
    s_Pr_ApplyTexture(ap_target, NULL);
    s_Pr_ApplyShader(ap_target, NULL);

    ap_target->cache.useVertexCache = PR_FALSE;

    Pr_SetRndTargetView(ap_target, &ap_target->view);
}

pr_bool_t Pr_MapPixelToCoord(Pr_RenderingTargetRef ap_target, Pr_Vector2Ref(long) ap_point, Pr_Vector2f * ap_dst)
{
    Pr_Vector2f l_normalized;
    Pr_IntRect l_vp; 

    if (!ap_target || !ap_point || !ap_dst) return PR_FALSE;

    Pr_GetRndTargetViewport(ap_target, &ap_target->view, &l_vp);

    l_normalized.x = -1.f + 2.f * (ap_point->x - l_vp.x) / l_vp.width;
    l_normalized.y =  1.f - 2.f * (ap_point->y - l_vp.y) / l_vp.height;

    Pr_TransformPoint(ap_target->view.inverseTransform, l_normalized.x, l_normalized.y, ap_dst);

    return PR_TRUE;
}

pr_bool_t Pr_MapCoordToPixel(Pr_RenderingTargetRef ap_target, Pr_Vector2Ref(float) ap_point, Pr_Vector2i * ap_dst)
{
    Pr_Vector2f l_normalized;
    Pr_IntRect l_vp;

    if (!ap_target || !ap_point || !ap_dst) return PR_FALSE;

    Pr_TransformPoint(ap_target->view.transform, ap_point->x, ap_point->y, &l_normalized);
    Pr_GetRndTargetViewport(ap_target, &ap_target->view, &l_vp);

    ap_dst->x = (int)(( l_normalized.x + 1.f) * .5f * l_vp.width  + l_vp.x);
    ap_dst->y = (int)((-l_normalized.y + 1.f) * .5f * l_vp.height + l_vp.y);

    return PR_TRUE;
}

void Pr_RndTargetDraw(
    Pr_RenderingTarget *    ap_target, 
    Pr_Vertex const *       ap_vertices, 
    pr_u32_t                a_count, 
    Pr_PrimitiveType        a_type, 
    Pr_RenderingStatesRef   ap_states
    )
{
    pr_bool_t l_useVertexCache;
    pr_u32_t l_texId;
    GLenum l_mode;
    struct s_pr_texture_t * lp_tex;
    Pr_RenderingStates l_states;
    static GLuint s_modes[] = {
        GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
        GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS
    };
    
    if (!ap_target) return;
    if (!ap_vertices || a_count == 0) return;

    if (!ap_states) {
        l_states.shader = NULL;
        l_states.texture = NULL;
        Pr_MakeTransformIdentity(l_states.transform);
        l_states.blenMode = *Pr_BlendModeAlpha();
    } else {
        l_states.blenMode   = ap_states->blenMode;
        l_states.shader     = ap_states->shader;
        l_states.texture    = ap_states->texture;
        Pr_CopyTransform(ap_states->transform, l_states.transform);
    }

    if (!ap_target->cache.statesSet) {
        Pr_ResetGLStates(ap_target);
    }

    l_useVertexCache = (a_count <= PR_VERTEXCACHE_SIZE);
    if (l_useVertexCache) {
        pr_u32_t l_i;
        for (l_i=0 ; l_i<a_count ; l_i++) {
            Pr_Vertex * lp_tmp = &ap_target->cache.vertexCache[l_i];
            float l_x = ap_vertices[l_i].position.x;
            float l_y = ap_vertices[l_i].position.y;

            Pr_TransformPoint(l_states.transform, l_x, l_y, &lp_tmp->position);
            lp_tmp->color = ap_vertices[l_i].color;
            lp_tmp->texCoords = ap_vertices[l_i].texCoords;
        }

        if (!ap_target->cache.useVertexCache) {
            Pr_Transform lp_tmp;
            Pr_MakeTransformIdentity(lp_tmp);
            s_Pr_ApplyTransform(lp_tmp);
        }
    } else {
        s_Pr_ApplyTransform(l_states.transform);
    }

    if (ap_target->cache.viewChanged) {
        s_Pr_ApplyCurrentView(ap_target);
    }

    if (!Pr_BlendModeEquals(&l_states.blenMode, &ap_target->cache.oldBlendMode)) {
        s_Pr_ApplyBlendMode(ap_target, &l_states.blenMode);
    }

    lp_tex = (struct s_pr_texture_t *)l_states.texture;

    l_texId = (l_states.texture) ?  lp_tex->cacheId : 0;
    if (l_texId != ap_target->cache.oldTextureId) {
        s_Pr_ApplyTexture(ap_target, l_states.texture);
    }

    if (l_states.shader) {
        s_Pr_ApplyShader(ap_target, l_states.shader);
    }

    if (l_useVertexCache) {
        ap_vertices = (!ap_target->cache.useVertexCache) ? ap_target->cache.vertexCache : NULL;
    }

    if (ap_vertices) {
        pr_u8_t * lp_bytes = (pr_u8_t *)ap_vertices;
        glVertexPointer(2, GL_FLOAT, sizeof(Pr_Vertex), lp_bytes);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Pr_Vertex), lp_bytes + 8);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Pr_Vertex), lp_bytes + 12);
    }

    l_mode = s_modes[a_type];

    glDrawArrays(l_mode, 0, a_count);

    if (l_states.shader) {
        s_Pr_ApplyShader(ap_target, NULL);
    }

    if (l_states.texture && lp_tex->hasFBO) {
        s_Pr_ApplyTexture(ap_target, NULL);
    }

    ap_target->cache.useVertexCache = l_useVertexCache;
}
