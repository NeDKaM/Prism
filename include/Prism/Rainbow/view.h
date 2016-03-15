#ifndef H_RAINBOW_VIEW_INCLUDED
#define H_RAINBOW_VIEW_INCLUDED

#include <Prism/config.h>
#include <Prism/Rainbow/transform.h>
#include <Prism/Rainbow/rect.h>
#include <Prism/Rainbow/vector.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_view_t, Pr_View) {
        Pr_Transform    transform;
        Pr_Transform    inverseTransform;
        Pr_Rectf        viewport;
        Pr_Vector2f     center;
        Pr_Vector2f     size;
        float           rotation;
        pr_bool_t       isTransformUpdated;
        pr_bool_t       isInvTransformUpdated;
    };

    extern PRISM_API void       Pr_SetViewCenter(Pr_View *, float, float);
    extern PRISM_API void       Pr_SetViewSize(Pr_View *, float, float);
    extern PRISM_API void       Pr_SetViewRotation(Pr_View *, float);
    extern PRISM_API void       Pr_ResetView(Pr_View *, Pr_RectRef(float));
    extern PRISM_API pr_bool_t  Pr_GetViewTransform(Pr_View *, Pr_Transform);
    extern PRISM_API pr_bool_t  Pr_GetViewCopy(Pr_ViewRef, Pr_View *);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_VIEW_INCLUDED */

