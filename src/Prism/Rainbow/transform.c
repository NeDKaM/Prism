#include <Prism/Rainbow/transform.h>
#include <math.h>
#include <string.h>

pr_bool_t Pr_MakeTransform(Pr_Transform ap_tr, 
    float a_00, float a_01, float a_02, 
    float a_10, float a_11, float a_12,
    float a_20, float a_21, float a_22    
)
{
    if (!ap_tr) return PR_FALSE;

    ap_tr[0]    = a_00;
    ap_tr[1]    = a_10;
    ap_tr[2]    = 0.f;
    ap_tr[3]    = a_20;

    ap_tr[4]    = a_01;
    ap_tr[5]    = a_11;
    ap_tr[6]    = 0.f;
    ap_tr[7]    = a_21;

    ap_tr[8]    = 0.f;
    ap_tr[9]    = 0.f;
    ap_tr[10]   = 1.f;
    ap_tr[11]   = 0.f;

    ap_tr[12]   = a_02;
    ap_tr[13]   = a_12;
    ap_tr[14]   = 0.f;
    ap_tr[15]   = a_22;

    return PR_TRUE;
}

pr_bool_t Pr_MakeTransformIdentity(Pr_Transform ap_transform)
{
    if (!ap_transform) return PR_FALSE;

    ap_transform[0] = 1.f;
    ap_transform[1] = 0.f;
    ap_transform[2] = 0.f;
    ap_transform[3] = 0.f;

    ap_transform[4] = 0.f;
    ap_transform[5] = 1.f;
    ap_transform[6] = 0.f;
    ap_transform[7] = 0.f;

    ap_transform[8] =   0.f;
    ap_transform[9] =   0.f;
    ap_transform[10] =  1.f;
    ap_transform[11] =  0.f;

    ap_transform[12] =  0.f;
    ap_transform[13] =  0.f;
    ap_transform[14] =  0.f;
    ap_transform[15] =  1.f;

    return PR_TRUE;
}

pr_bool_t Pr_TransformInverse(Pr_TransformRef ap_src, Pr_Transform ap_dst)
{
    float l_det;
    
    if (!ap_src || !ap_dst) return PR_FALSE;

    l_det = (ap_src[0] * (ap_src[15] * ap_src[5] - ap_src[7] * ap_src[13])
        - ap_src[1] * (ap_src[15] * ap_src[4] - ap_src[7] * ap_src[12])
        + ap_src[3] * (ap_src[13] * ap_src[4] - ap_src[5] * ap_src[12])
    );

    if (l_det != 0.f) {
        Pr_MakeTransform(ap_dst,
            (ap_src[15] * ap_src[5] - ap_src[7] * ap_src[13]) / l_det,
            - (ap_src[15] * ap_src[4] - ap_src[7] * ap_src[12]) / l_det,
            (ap_src[13] * ap_src[4] - ap_src[5] * ap_src[12]) / l_det,
            - (ap_src[15] * ap_src[1] - ap_src[3] * ap_src[13]) /l_det,
            (ap_src[15] * ap_src[0] - ap_src[3] * ap_src[12]) / l_det,
            - (ap_src[13] * ap_src[0] - ap_src[1] * ap_src[12]) / l_det,
            (ap_src[7] * ap_src[1] - ap_src[3] * ap_src[5]) / l_det,
            - (ap_src[7] * ap_src[0] - ap_src[3] * ap_src[4]) / l_det,
            (ap_src[5] * ap_src[0] - ap_src[1] * ap_src[4]) / l_det
        );
    } else {
        Pr_MakeTransformIdentity(ap_dst);
    }

    return PR_TRUE;
}

pr_bool_t Pr_TransformPoint(Pr_TransformRef ap_src, float a_x, float a_y, Pr_Vector2f * ap_dst)
{
    if (!ap_src || !ap_dst) return PR_FALSE;

    ap_dst->x = ap_src[0] * a_x + ap_src[4] * a_y + ap_src[12];
    ap_dst->y = ap_src[1] * a_x + ap_src[5] * a_y + ap_src[13];

    return PR_TRUE;
}

pr_bool_t Pr_TransformCombine(Pr_Transform ap_dst, Pr_TransformRef ap_src)
{
    Pr_TransformRef lp_a;
    Pr_TransformRef lp_b;
    
    if (!ap_dst || !ap_src) return PR_FALSE;

    lp_a = ap_dst;
    lp_b = ap_src;

    Pr_MakeTransform(ap_dst,
        lp_a[0] * lp_b[0]  + lp_a[4] * lp_b[1]  + lp_a[12] * lp_b[3],
        lp_a[0] * lp_b[4]  + lp_a[4] * lp_b[5]  + lp_a[12] * lp_b[7],
        lp_a[0] * lp_b[12] + lp_a[4] * lp_b[13] + lp_a[12] * lp_b[15],
        lp_a[1] * lp_b[0]  + lp_a[5] * lp_b[1]  + lp_a[13] * lp_b[3],
        lp_a[1] * lp_b[4]  + lp_a[5] * lp_b[5]  + lp_a[13] * lp_b[7],
        lp_a[1] * lp_b[12] + lp_a[5] * lp_b[13] + lp_a[13] * lp_b[15],
        lp_a[3] * lp_b[0]  + lp_a[7] * lp_b[1]  + lp_a[15] * lp_b[3],
        lp_a[3] * lp_b[4]  + lp_a[7] * lp_b[5]  + lp_a[15] * lp_b[7],
        lp_a[3] * lp_b[12] + lp_a[7] * lp_b[13] + lp_a[15] * lp_b[15]
    );

    return PR_TRUE;
}

void Pr_TranslateTransform(Pr_Transform ap_transform, float a_x, float a_y)
{
    Pr_Transform lp_translation;

    if (!ap_transform) return;

    Pr_MakeTransform(lp_translation,1.f,0.f,a_x,0.f,1.f,a_y,0.f,0.f,1.f);

    Pr_TransformCombine(ap_transform, lp_translation);
}

void Pr_RotateTransform(Pr_Transform ap_transform, float a_angle, Pr_Vector2Ref(float) ap_center)
{
    Pr_Transform lp_rotation;
    float l_rad;
    float l_cos;
    float l_sin;

    if (!ap_transform) return;
    
    l_rad = a_angle * 3.141592654f / 180.f;
    l_cos = cosf(l_rad);
    l_sin = sinf(l_rad);

    if (ap_center) {
        Pr_MakeTransform(lp_rotation,
            l_cos, -l_sin, ap_center->x * (1.f - l_cos) + ap_center->y * l_sin,
            l_sin, l_cos, ap_center->y * (1.f - l_cos) - ap_center->x * l_sin,
            0.f, 0.f, 1.f
        );
    } else {
        Pr_MakeTransform(lp_rotation,
            l_cos, -l_sin, 0.f,
            l_sin, l_cos, 0.f,
            0.f, 0.f, 1.f
        );
    }

    Pr_TransformCombine(ap_transform, lp_rotation);
}

void Pr_ScaleTransform(Pr_Transform ap_transform, float a_x, float a_y)
{
    Pr_Transform lp_scaling;

    if (!ap_transform) return;

    Pr_MakeTransform(lp_scaling,
        a_x, 0.f, 0.f,
        0.f, a_y, 0.f,
        0.f, 0.f, 1.f
    );

   Pr_TransformCombine(ap_transform, lp_scaling);
}

pr_bool_t Pr_CopyTransform(Pr_TransformRef ap_src, Pr_Transform ap_dst)
{
    if (!ap_src || !ap_dst) return PR_FALSE;

    memcpy(ap_dst, ap_src, 16 * sizeof(float));

    return PR_TRUE;
}

pr_bool_t Pr_GetTransformableTransform(Pr_Transformable * ap_trsf, Pr_Transform ap_dst)
{
    if (!ap_trsf || !ap_dst) return PR_FALSE;

    if (ap_trsf->update) {
        float l_angle  = -ap_trsf->rotation * (3.141592654f / 180.f);
        float l_cosine = cosf(l_angle);
        float l_sine   = sinf(l_angle);
        float l_sxc    = ap_trsf->scale.x * l_cosine;
        float l_syc    = ap_trsf->scale.y * l_cosine;
        float l_sxs    = ap_trsf->scale.x * l_sine;
        float l_sys    = ap_trsf->scale.y * l_sine;
        float l_tx     = -ap_trsf->origin.x * l_sxc - ap_trsf->origin.y * l_sys + ap_trsf->position.x;
        float l_ty     =  ap_trsf->origin.x * l_sxs - ap_trsf->origin.y * l_syc + ap_trsf->position.y;

        Pr_MakeTransform(ap_trsf->transform, 
            l_sxc,  l_sys,  l_tx, 
            -l_sxs, l_syc,  l_ty,
             0.f,   0.f,    1.f
        );

        ap_trsf->update = PR_FALSE;
    }

    Pr_CopyTransform(ap_trsf->transform, ap_dst);

    return PR_TRUE;
}

pr_bool_t Pr_GetTransformableInvTransform(Pr_Transformable * ap_trsf, Pr_Transform ap_dst)
{
    if (!ap_trsf || !ap_dst) return PR_FALSE;

    if (!ap_trsf->updateInverse) {
        Pr_GetTransformableTransform(ap_trsf->transform, ap_trsf->transformInverse);
        Pr_TransformInverse(ap_trsf->transformInverse, ap_trsf->transformInverse);
        ap_trsf->updateInverse = PR_FALSE;
    }

    memcpy(ap_dst, ap_trsf->transformInverse, 16 * sizeof(float));

    return PR_TRUE;
}

pr_bool_t Pr_TransformRect(Pr_TransformRef ap_transform, Pr_FloatRectRef ap_rect, Pr_FloatRect * ap_dst)
{
    Pr_Vector2f lp_points[4];
    float l_left;
    float l_right;
    float l_top;
    float l_bottom;
    int l_i;

    if (!ap_transform || !ap_rect || !ap_dst) return PR_FALSE;

    Pr_TransformPoint(ap_transform, ap_rect->x, ap_rect->y, &lp_points[0]);
    Pr_TransformPoint(ap_transform, ap_rect->x, ap_rect->y + ap_rect->height, &lp_points[1]);
    Pr_TransformPoint(ap_transform, ap_rect->x + ap_rect->width, ap_rect->y, &lp_points[2]);
    Pr_TransformPoint(ap_transform, ap_rect->x + ap_rect->width, ap_rect->y + ap_rect->height, &lp_points[3]);

    l_left      = lp_points[0].x;
    l_top       = lp_points[0].y;
    l_right     = lp_points[0].x;
    l_bottom    = lp_points[0].y;

    for (l_i=0 ; l_i<4 ; l_i++) {
        if (lp_points[l_i].x < l_left) {
            l_left = lp_points[l_i].x;
        } else if (lp_points[l_i].x > l_right) { 
            l_right = lp_points[l_i].x;
        }

        if (lp_points[l_i].y < l_top) { 
            l_top = lp_points[l_i].y;
        } else if (lp_points[l_i].y > l_bottom) {
            l_bottom = lp_points[l_i].y;
        }
    }

    ap_dst->x       = l_left;
    ap_dst->y       = l_top;
    ap_dst->width   = l_right - l_left;
    ap_dst->height  = l_bottom - l_top;

    return PR_TRUE;
}

pr_bool_t Pr_InitTransformable(Pr_Transformable * ap_trsf)
{
    if (!ap_trsf) return PR_FALSE;

    ap_trsf->origin.x = 0.f;
    ap_trsf->origin.y = 0.f;

    ap_trsf->position.x = 0.f;
    ap_trsf->position.y = 0.f;

    ap_trsf->rotation = 0.f;

    ap_trsf->scale.x = 1.f;
    ap_trsf->scale.y = 1.f;

    Pr_MakeTransformIdentity(ap_trsf->transform);
    Pr_MakeTransformIdentity(ap_trsf->transformInverse);

    ap_trsf->update        = PR_TRUE;
    ap_trsf->updateInverse = PR_TRUE;

    return PR_TRUE;
}

pr_bool_t Pr_SetTransformablePosition(Pr_Transformable * ap_src, float a_x, float a_y)
{
    if (!ap_src) return PR_FALSE;

    ap_src->position.x = a_x;
    ap_src->position.y = a_y;

    ap_src->update          = PR_TRUE;
    ap_src->updateInverse   = PR_TRUE;

    return PR_TRUE;
}

pr_bool_t Pr_SetTransformableScale(Pr_Transformable * ap_src, float a_x, float a_y)
{
    if (!ap_src) return PR_FALSE;

    ap_src->scale.x = a_x;
    ap_src->scale.y = a_y;

    ap_src->update          = PR_TRUE;
    ap_src->updateInverse   = PR_TRUE;

    return PR_TRUE;
}

pr_bool_t Pr_SetTransformableOrigin(Pr_Transformable * ap_src, float a_x, float a_y)
{
    if (!ap_src) return PR_FALSE;

    ap_src->origin.x = a_x;
    ap_src->origin.y = a_y;

    ap_src->update          = PR_TRUE;
    ap_src->updateInverse   = PR_TRUE;

    return PR_TRUE;
}

pr_bool_t Pr_SetTransformableRotation(Pr_Transformable * ap_src, float a_angle)
{
    if (!ap_src) return PR_FALSE;

    ap_src->rotation = fmodf(a_angle, 360.f);

    if (ap_src->rotation < 0) {
        ap_src->rotation += 360.f;
    }

    ap_src->update          = PR_TRUE;
    ap_src->updateInverse   = PR_TRUE;

    return PR_TRUE;
}

