#ifndef H_RAINBOW_COLOR_INCLUDED
#define H_RAINBOW_COLOR_INCLUDED

#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_color_t, Pr_Color) {
        pr_u8_t r;
        pr_u8_t g;
        pr_u8_t b;
        pr_u8_t a;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_COLOR_INCLUDED */
