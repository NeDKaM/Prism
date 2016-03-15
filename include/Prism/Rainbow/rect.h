#ifndef H_RAINBOW_RECT_INCLUDED
#define H_RAINBOW_RECT_INCLUDED

#include <Prism/config.h>

PR_CPP_PROTECT_ON

    #define Pr_Rect(type)       Pr_##type##Rect
    #define Pr_RectRef(type)    Pr_##type##RectRef

    #define PR_RECT(type) \
        typedef struct pr_##type##rect_t    Pr_##type##Rect; \
        typedef const Pr_##type##Rect *     Pr_##type##RectRef; \
        struct pr_##type##rect_t { \
            type x; \
            type y; \
            type width; \
            type height; \
        }

    PR_RECT(long);  typedef Pr_Rect(long)   Pr_Recti;
    PR_RECT(float); typedef Pr_Rect(float)  Pr_Rectf;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_RECT_INCLUDED */
