#ifndef H_RAINBOW_VERTEX_INCLUDED
#define H_RAINBOW_VERTEX_INCLUDED

#include <Prism/config.h>
#include <Prism/Rainbow/color.h>
#include <Prism/Rainbow/vector.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_vertex_t, Pr_Vertex) {
        Pr_Vector2f position;
        Pr_Vector2f texCoords;
        Pr_Color    color;
    };

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_VERTEX_INCLUDED */
