#include <Prism/Rainbow/view.h>
#include <math.h>
#include <string.h>

void Pr_SetViewCenter(Pr_View * ap_view, float a_x, float a_y)
{
    if (!ap_view) return;

    ap_view->center.x = a_x;
    ap_view->center.y = a_y;

    ap_view->isTransformUpdated     = PR_FALSE;
    ap_view->isInvTransformUpdated  = PR_FALSE;
}

void Pr_SetViewSize(Pr_View * ap_view, float a_w, float a_h)
{
    if (!ap_view) return;

    ap_view->size.x = a_w;
    ap_view->size.y = a_h;

    ap_view->isTransformUpdated     = PR_FALSE;
    ap_view->isInvTransformUpdated  = PR_FALSE;
}

void Pr_SetViewRotation(Pr_View * ap_view, float a_degrees)
{
    if (!ap_view) return;

    a_degrees = fmodf(a_degrees, 360.f);

    if (a_degrees < 0.f) {
        a_degrees += 360.f;
    }

    ap_view->isTransformUpdated     = PR_FALSE;
    ap_view->isInvTransformUpdated  = PR_FALSE;
}

void Pr_SetViewRect(Pr_View * ap_view, Pr_FloatRect * ap_rect)
{
    if (!ap_view) return;

    ap_view->rotation = 0.f;

    ap_view->isTransformUpdated     = PR_FALSE;
    ap_view->isInvTransformUpdated  = PR_FALSE;
void Pr_ResetView(Pr_View * ap_view, Pr_FloatRect * ap_rect)

    if (!ap_rect) { /* sets default view */
        ap_view->center.x           = 0.f;
        ap_view->center.y           = 0.f;
        ap_view->size.x             = 1000.f;
        ap_view->size.y             = 1000.f;
        ap_view->viewport.x         = 0.f;
        ap_view->viewport.y         = 0.f;
        ap_view->viewport.width     = 1.f;
        ap_view->viewport.height    = 1.f;
        Pr_MakeTransformIdentity(ap_view->transform);
        Pr_MakeTransformIdentity(ap_view->inverseTransform);
        return;
    }

    ap_view->center.x   = ap_rect->x + ap_rect->width * .5f;
    ap_view->center.y   = ap_rect->y + ap_rect->height * .5f;
    ap_view->size.x     = ap_rect->width;
    ap_view->size.y     = ap_rect->height;
}

pr_bool_t Pr_GetViewTransform(Pr_View * ap_view, Pr_Transform ap_dst)
{
    if (!ap_view || !ap_dst) return PR_FALSE;

    if (!ap_view->isTransformUpdated) {
        float l_angle   = ap_view->rotation * 3.141592654f / 180.f;
        float l_cosine  = cosf(l_angle);
        float l_sine    = sinf(l_angle);
        float tx        = -ap_view->center.x * l_cosine - ap_view->center.y * l_sine + ap_view->center.x;
        float ty        = ap_view->center.x * l_sine - ap_view->center.y * l_cosine + ap_view->center.y;

        float a = 2.f / ap_view->size.x;
        float b = -2.f /  ap_view->size.y;
        float c = -a * ap_view->center.x;
        float d = -b * ap_view->center.y;

        Pr_MakeTransform(ap_view->transform, 
            a * l_cosine,   a * l_sine,     a * tx + c,
            -b * l_sine,    b * l_cosine,   b * ty + d,
            0.f,            0.f,            1.f
        );

        ap_view->isTransformUpdated = PR_TRUE;
    }

    Pr_CopyTransform(ap_view->transform, ap_dst);

    return PR_TRUE;
}

pr_bool_t Pr_GetViewInverseTransform(Pr_View * ap_view, Pr_Transform ap_dst)
{
    if (!ap_view || !ap_dst) return PR_FALSE;

    if (!ap_view->isInvTransformUpdated) {
        Pr_GetViewTransform(ap_view, ap_view->transform);
        Pr_TransformInverse(ap_view->transform, ap_view->inverseTransform);
        ap_view->isInvTransformUpdated = PR_TRUE;
    }

    Pr_CopyTransform(ap_view->inverseTransform, ap_dst);

    return PR_TRUE;
}

pr_bool_t Pr_GetViewCopy(Pr_View * ap_view, Pr_View * ap_dst)
{
    if (!ap_view || !ap_dst) return PR_FALSE;

    Pr_CopyTransform(ap_view->transform, ap_dst->transform);
    Pr_CopyTransform(ap_view->inverseTransform, ap_dst->inverseTransform);

    ap_dst->viewport    = ap_view->viewport;
    ap_dst->center      = ap_view->center;
    ap_dst->size        = ap_view->size;
    ap_dst->rotation    = ap_view->rotation;

    ap_dst->isInvTransformUpdated   = ap_view->isInvTransformUpdated;
    ap_dst->isTransformUpdated      = ap_view->isTransformUpdated;

    return PR_TRUE;
}
