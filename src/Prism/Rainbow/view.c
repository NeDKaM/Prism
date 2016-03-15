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

void Pr_ResetView(Pr_View * ap_view, Pr_RectRef(float) ap_rect)
{
    if (!ap_view) return;

    ap_view->isTransformUpdated     = PR_FALSE;
    ap_view->isInvTransformUpdated  = PR_FALSE;

    if (!ap_rect) { /* sets default view */
        ap_view->center.x           = 0.f;
        ap_view->center.y           = 0.f;
        ap_view->size.x             = 0.f;
        ap_view->size.y             = 0.f;
        ap_view->rotation           = 0.f;
        ap_view->viewport.x         = 0.f;
        ap_view->viewport.y         = 0.f;
        ap_view->viewport.width     = 1.f;
        ap_view->viewport.height    = 1.f;
        return;
    }

    ap_view->center.x   = ap_rect->x + ap_rect->width * .5f;
    ap_view->center.y   = ap_rect->y + ap_rect->height *.5f;
    ap_view->size.x     = ap_rect->width;
    ap_view->size.y     = ap_rect->height;
    ap_view->rotation   = 0.f;
}

pr_bool_t Pr_GetViewTransform(Pr_View * ap_view, Pr_Transform ap_dst)
{
    if (!ap_view || !ap_dst) return PR_FALSE;

    memcpy(ap_dst, ap_view->transform, 16 * sizeof(float));

    return PR_TRUE;
}

pr_bool_t Pr_GetViewCopy(Pr_ViewRef ap_view, Pr_View * ap_dst)
{
    if (!ap_view || !ap_dst) return PR_FALSE;

    memcpy(ap_dst, ap_view, sizeof(Pr_View));

    return PR_TRUE;
}
