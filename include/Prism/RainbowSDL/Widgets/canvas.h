#ifndef H_RAINBOWSDL_CANVAS_INCLUDED
#define H_RAINBOWSDL_CANVAS_INCLUDED

#include <Prism/Polyhedron/widget.h>

#include <Prism/ECS/world.h>

PR_CPP_PROTECT_ON

    PR_OBJECT(pr_canvas_t, Pr_Canvas) {
        PR_INHERIT(Pr_WidgetContainer);

        pr_entityid_t (*createEntity)(Pr_CanvasRef);

        Pr_World *  world;
    };

    extern PRISM_API Pr_WidgetRef   Pr_AddCanvasWidget(Pr_CanvasRef, Pr_Class *);

    extern PRISM_API Pr_Class Pr_CanvasClass;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_CANVAS_INCLUDED */
