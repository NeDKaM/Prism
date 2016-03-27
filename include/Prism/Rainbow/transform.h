#ifndef H_RAINBOW_TRANSFORM_INCLUDED
#define H_RAINBOW_TRANSFORM_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/Rainbow/vector.h>
#include <Prism/Rainbow/rect.h>

PR_CPP_PROTECT_ON

    typedef float           Pr_Transform[16];
    typedef const float *   Pr_TransformRef;

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
    extern PRISM_API pr_bool_t      Pr_TransformInverse(Pr_TransformRef, Pr_Transform);
    extern PRISM_API pr_bool_t      Pr_TransformPoint(Pr_TransformRef, float, float, Pr_Vector2f *);
    extern PRISM_API pr_bool_t      Pr_TransformRect(Pr_TransformRef, Pr_RectRef(float), Pr_Rect(float) *);
    extern PRISM_API pr_bool_t      Pr_TransformCombine(Pr_TransformRef, Pr_Transform);
    extern PRISM_API void           Pr_TranslateTransform(Pr_Transform, float, float);
    extern PRISM_API void           Pr_RotateTransform(Pr_Transform, float, Pr_Vector2Ref(float));
    extern PRISM_API void           Pr_ScaleTransform(Pr_Transform, float, float);
    extern PRISM_API pr_bool_t      Pr_CopyTransform(Pr_TransformRef, Pr_Transform);
    extern PRISM_API pr_bool_t      Pr_MakeTransform(Pr_Transform, float, float, float, float, float, float, float, float, float);

    extern PRISM_API pr_bool_t      Pr_GetTransformableTransform(Pr_Transformable *, Pr_Transform);


PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_TRANSFORM_INCLUDED */