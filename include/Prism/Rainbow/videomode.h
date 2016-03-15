#ifndef H_RAINBOW_VIDEOMODE_INCLUDED
#define H_RAINBOW_VIDEOMODE_INLUCDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_videomode_t, Pr_VideoMode) {
        pr_u32_t width;
        pr_u32_t height;
        pr_u32_t bpp;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_VIDEOMODE_INLUCDED */

