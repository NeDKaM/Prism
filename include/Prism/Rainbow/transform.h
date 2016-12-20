#ifndef H_RAINBOW_TRANSFORM_INCLUDED
#define H_RAINBOW_TRANSFORM_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/Rainbow/vector.h>
#include <Prism/Rainbow/rect.h>

PR_CPP_PROTECT_ON

    typedef float           Pr_Transform[16];

    PR_STRUCT(pr_transformable_t, Pr_Transformable) {
        Pr_Transform    transform;
        Pr_Transform    transformInverse;
        Pr_Vector2f     origin;
        Pr_Vector2f     position;
        Pr_Vector2f     scale;
        float           rotation;
        pr_bool_t       update;
        pr_bool_t       updateInverse;
    }; 

    extern PRISM_API pr_bool_t      Pr_MakeTransformIdentity(Pr_Transform);
    extern PRISM_API pr_bool_t      Pr_TransformInverse(Pr_Transform, Pr_Transform);
    extern PRISM_API pr_bool_t      Pr_TransformPoint(Pr_Transform, float, float, Pr_Vector2f *);
    extern PRISM_API pr_bool_t      Pr_TransformRect(Pr_Transform, Pr_FloatRect *, Pr_FloatRect *);
    extern PRISM_API pr_bool_t      Pr_TransformCombine(Pr_Transform, Pr_Transform);
    extern PRISM_API void           Pr_TranslateTransform(Pr_Transform, float, float);
    extern PRISM_API void           Pr_RotateTransform(Pr_Transform, float, Pr_Vector2f *);
    extern PRISM_API void           Pr_ScaleTransform(Pr_Transform, float, float);
    extern PRISM_API pr_bool_t      Pr_CopyTransform(Pr_Transform, Pr_Transform);
    extern PRISM_API pr_bool_t      Pr_MakeTransform(Pr_Transform, float, float, float, float, float, float, float, float, float);

    extern PRISM_API pr_bool_t      Pr_InitTransformable(Pr_Transformable *);
    extern PRISM_API pr_bool_t      Pr_GetTransformableTransform(Pr_Transformable *, Pr_Transform);
    extern PRISM_API pr_bool_t      Pr_SetTransformablePosition(Pr_Transformable *, float, float);
    extern PRISM_API pr_bool_t      Pr_SetTransformableScale(Pr_Transformable *, float, float);
    extern PRISM_API pr_bool_t      Pr_SetTransformableOrigin(Pr_Transformable *, float, float);
    extern PRISM_API pr_bool_t      Pr_SetTransformableRotation(Pr_Transformable *, float);


PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_TRANSFORM_INCLUDED */