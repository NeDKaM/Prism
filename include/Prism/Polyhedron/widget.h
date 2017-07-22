#ifndef H_POLYHEDRON_WIDGET_INCLUDED
#define H_POLYHEDRON_WIDGET_INCLUDED

#include <Prism/objects.h>
#include <Prism/list.h>
#include <Prism/window.h>

PR_CPP_PROTECT_ON

    PR_OBJECT(pr_widgetcontainer_t, Pr_WidgetContainer);

    extern PRISM_API Pr_Class Pr_WidgetClass;

    PR_OBJECT(pr_widget_t, Pr_Widget) {
        PR_INHERIT(Pr_Object);

        Pr_WidgetContainerRef parent;

        struct {
            long x;
            long y;
            pr_u32_t w;
            pr_u32_t h;
        } boundingBox;

        Pr_SlotFunction(onMouseMoved)(Pr_WidgetRef, long, long);
        Pr_SlotFunction(onButtonPressed)(Pr_WidgetRef, long, long, long);
        Pr_SlotFunction(onButtonReleased)(Pr_WidgetRef, long, long, long);
        Pr_SlotFunction(onMouseEnter)(Pr_WidgetRef);
        Pr_SlotFunction(onKeyPressed)(Pr_WidgetRef, long);
        Pr_SlotFunction(onKeyReleased)(Pr_WidgetRef, long);
        Pr_SlotFunction(onMouseExit)(Pr_WidgetRef);

        Pr_SlotFunction(layout)(Pr_WidgetRef);
        Pr_SlotFunction(show)(Pr_WidgetRef);

        Pr_SlotFunction(setSize)(Pr_WidgetRef, pr_u32_t, pr_u32_t);
        Pr_Signal * sizeChanged;

        Pr_SlotFunction(setPosition)(Pr_WidgetRef, long, long);
        Pr_Signal * moved;

        pr_bool_t enabled;
        pr_bool_t visible;
        pr_bool_t hasFocus;
    };

    PR_SLOT_EXTERN_API(Pr_SetWidgetSize)(Pr_WidgetRef, pr_u32_t, pr_u32_t);
    PR_SLOT_EXTERN_API(Pr_SetWidgetPosition)(Pr_WidgetRef, long, long);


    extern PRISM_API Pr_Class Pr_WidgetContainerClass;

    struct pr_widgetcontainer_t {
        PR_INHERIT(Pr_Widget);

        Pr_List * children;

        Pr_WidgetRef    hoveredWidget;
        Pr_WidgetRef    clickedWidget;
        Pr_WidgetRef    focusedWidget;

        struct {
            Pr_Signal * mouseMoved;
            Pr_Signal * buttonPressed;
            Pr_Signal * buttonReleased;
            Pr_Signal * keyPressed;
            Pr_Signal * keyReleased; 
        } input;
    };

    extern PRISM_API pr_bool_t  Pr_SetWidgetParent(Pr_WidgetRef, Pr_WidgetContainerRef);
    extern PRISM_API pr_bool_t  Pr_WidgetParent(Pr_WidgetRef, Pr_WidgetContainerRef);

PR_CPP_PROTECT_OFF

#endif /* H_POLYHEDRON_WIDGET_INCLUDED */

