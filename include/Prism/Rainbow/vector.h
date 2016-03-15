#ifndef H_RAINBOW_VECTOR_INCLUDED
#define H_RAINBOW_VECTOR_INCLUDED

#include <Prism/config.h>

PR_CPP_PROTECT_ON

    #define Pr_Vector2(type)    Pr_Vector2##type
    #define Pr_Vector2Ref(type) Pr_Vector2##type##Ref

    #define PR_VECTOR2(type) \
        typedef struct pr_vector2##type##_t Pr_Vector2##type; \
        typedef const Pr_Vector2##type *    Pr_Vector2##type##Ref; \
        struct pr_vector2##type##_t { \
            type x; \
            type y; \
        }

    PR_VECTOR2(long);   typedef Pr_Vector2(long)    Pr_Vector2i;
    PR_VECTOR2(float);  typedef Pr_Vector2(float)   Pr_Vector2f;
    PR_VECTOR2(double); typedef Pr_Vector2(double)  Pr_Vector2d;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_VECTOR_INCLUDED */

