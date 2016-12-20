#ifndef H_RAINBOW_RECT_INCLUDED
#define H_RAINBOW_RECT_INCLUDED

#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_floatrect_t, Pr_FloatRect) {
        float x;
        float y;
        float width;
        float height;
    };

    PR_STRUCT(pr_intrect_t, Pr_IntRect) {
        long        x;
        long        y;
        pr_u32_t    width;
        pr_u32_t    height;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_RECT_INCLUDED */
