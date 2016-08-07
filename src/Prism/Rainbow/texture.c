#include <Prism/Rainbow/texture.h>
#include <stdlib.h>
#include <Prism/Rainbow/opengl.h>
#include <Prism/Rainbow/transform.h>

struct pr_texture_t {
    Pr_Vector2i size;
    Pr_Vector2i realSize;
    pr_u32_t    textureId;
    pr_u32_t    cacheId;
    pr_bool_t   isSmooth;
    pr_bool_t   isRepeated;
    pr_bool_t   flipped;
    pr_bool_t   hasFBO;
};

static pr_u32_t s_Pr_GetUniqueTextureId(void)
{
    static pr_u32_t s_id = 1;

    return s_id++;
}

static pr_u32_t s_Pr_GetNextPowerOf2(pr_u32_t a_val)
{
    if (a_val == 0) return 0;

    /* Got it on lazyfoo.net */
    a_val--;
    a_val |= (a_val >> 1); 
    a_val |= (a_val >> 2); 
    a_val |= (a_val >> 4); 
    a_val |= (a_val >> 8); 
    a_val |= (a_val >> 16);
    a_val++;

    return a_val;
}

static Pr_Texture * s_Pr_NewEmptyTexture(pr_u32_t a_w, pr_u32_t a_h)
{
    Pr_Texture * lp_out = NULL;

    if (a_w == 0 || a_h == 0) return NULL;

    lp_out = calloc(1, sizeof(Pr_Texture));

    if (!lp_out) return NULL;

    lp_out->realSize.x  = s_Pr_GetNextPowerOf2(a_w);
    lp_out->realSize.y  = s_Pr_GetNextPowerOf2(a_h);
    lp_out->size.x      = a_w;
    lp_out->size.y      = a_h;
    lp_out->flipped     = PR_FALSE;
    lp_out->hasFBO      = PR_FALSE;
    lp_out->isSmooth    = PR_FALSE;
    lp_out->isRepeated  = PR_FALSE;

    /* NOT SAFE */ {
        GLuint l_tex;

        glGenTextures(1, &l_tex);
        lp_out->textureId = (pr_u32_t)l_tex;

        glBindTexture(GL_TEXTURE_2D, lp_out->textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
            lp_out->realSize.x, lp_out->realSize.y, 
            0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
        );

        /* no smoothness */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_NEAREST);
        /* not repeated */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,       GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,       GL_CLAMP);
    }

    return lp_out;
}

static void s_Pr_SetTextureImage(Pr_Texture * ap_tex, Pr_ImageRef ap_img)
{
    pr_u8_t * pixels = Pr_GetImagePixels(ap_img);

    glBindTexture(GL_TEXTURE_2D, ap_tex->textureId);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 
        0, 0, 
        ap_tex->size.x, ap_tex->size.y, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        pixels
    );

    glTexParameteri(GL_TEXTURE_2D, 
        GL_TEXTURE_MIN_FILTER, 
        ap_tex->isSmooth ? GL_LINEAR : GL_NEAREST
    );

    ap_tex->hasFBO  = PR_FALSE;
    ap_tex->flipped = PR_FALSE;
    ap_tex->cacheId = s_Pr_GetUniqueTextureId();
}

void Pr_BindTexture(Pr_TextureRef ap_tex, Pr_TextureCoordinate a_type)
{
    if (!ap_tex) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);

        return;
    }

    glBindTexture(GL_TEXTURE_2D, ap_tex->textureId);

    /* special texture matrix */
    if (a_type == PR_TEXCOORD_PIXELS || ap_tex->flipped) {
        GLfloat l_matrix[16] = {
            1.f,0.f,0.f,0.f, 0.f,1.f,0.f,0.f,
            0.f,0.f,1.f,0.f, 0.f,0.f,0.f,1.f
        };
    
        if (a_type == PR_TEXCOORD_PIXELS) {
            l_matrix[0] = 1.f / ap_tex->realSize.x;
            l_matrix[5] = 1.f / ap_tex->realSize.y;
        }

        if (ap_tex->flipped) {
            l_matrix[5]     = - l_matrix[5];
            l_matrix[16]    = (float)(ap_tex->size.y) / ap_tex->realSize.y;
        }

        glMatrixMode(GL_TEXTURE);
        glLoadMatrixf(l_matrix);

        glMatrixMode(GL_MODELVIEW);
    }
}

Pr_Texture * Pr_NewTexture(Pr_ImageRef ap_img, Pr_IntRectRef ap_rect)
{
    Pr_Texture *    lp_out = NULL;
    Pr_Vector2i     l_size;
    Pr_IntRect      l_rect;
    pr_u8_t *       lp_bytes;
    pr_u32_t        l_i;

    if (!ap_img) return NULL;

    Pr_GetImageSize(ap_img, &l_size);

    if (!ap_rect) {
        lp_out = s_Pr_NewEmptyTexture(l_size.x, l_size.y);
        if (!lp_out) return NULL;

        s_Pr_SetTextureImage(lp_out, ap_img);

        glFlush(); /* force context update */

        return lp_out;
    }

    l_rect.x        = (ap_rect->x < 0) ? 0 : ap_rect->x;
    l_rect.y        = (ap_rect->y < 0) ? 0 : ap_rect->y;
    l_rect.width    = (ap_rect->width  > (pr_u32_t)l_size.x) ? l_size.x : ap_rect->width;
    l_rect.height   = (ap_rect->height > (pr_u32_t)l_size.y) ? l_size.y : ap_rect->height;

    lp_out = s_Pr_NewEmptyTexture(l_rect.width, l_rect.width);
    if (!lp_out) return NULL;
    
    lp_bytes = Pr_GetImagePixels(ap_img) + 4 * (l_rect.x + l_size.x * l_rect.y);

    glBindTexture(GL_TEXTURE_2D, lp_out->textureId);

    for (l_i=0 ; l_i<l_rect.height ; l_i++) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, l_i, l_rect.width, 
            1, GL_RGBA, GL_UNSIGNED_BYTE, lp_bytes
        );
        lp_bytes += 4 * l_size.x;
    }

    glFlush(); /* force context update */

    return lp_out;
}

pr_bool_t Pr_GetTextureSize(Pr_TextureRef ap_tex, Pr_Vector2i * ap_dst)
{
    if (!ap_tex || !ap_dst) return PR_FALSE;

    *ap_dst = ap_tex->size;
    
    return PR_TRUE;
}

void Pr_DeleteTexture(Pr_Texture * ap_tex)
{
    if (!ap_tex) return;

    glDeleteTextures(1, (GLuint *)&ap_tex->textureId);

    free(ap_tex);
}

void Pr_SetTextureSmooth(Pr_Texture * ap_tex, pr_bool_t a_smooth)
{
    if (!ap_tex) return;
    if (ap_tex->isSmooth == a_smooth) return;

    ap_tex->isSmooth = a_smooth;

    glBindTexture(GL_TEXTURE_2D, ap_tex->textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (a_smooth) ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (a_smooth) ? GL_LINEAR : GL_NEAREST);
}

void Pr_SetTextureRepeated(Pr_Texture * ap_tex, pr_bool_t a_repeat)
{
    if (!ap_tex) return;
    if (a_repeat == ap_tex->isRepeated);

    ap_tex->isRepeated = a_repeat;

    glBindTexture(GL_TEXTURE_2D, ap_tex->textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (a_repeat) ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (a_repeat) ? GL_REPEAT : GL_CLAMP);
}
