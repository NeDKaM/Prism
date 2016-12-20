#ifndef H_RAINBOW_VECTOR_INCLUDED
#define H_RAINBOW_VECTOR_INCLUDED

#include <Prism/config.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_verctor2i_t, Pr_Vector2i) {
        pr_u32_t x;
        pr_u32_t y;
    };

    PR_STRUCT(pr_vector2f_t, Pr_Vector2f) {
        float x;
        float y;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_VECTOR_INCLUDED */

