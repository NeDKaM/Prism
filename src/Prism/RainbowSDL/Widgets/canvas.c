#include <Prism/RainbowSDL/Widgets/canvas.h>

#include <stdlib.h>

static Pr_Entity * s_Pr_CreateEntity(Pr_CanvasRef ap_canvas)
{
    return Pr_CreateWorldEntity(ap_canvas->world);
}

static pr_bool_t s_Pr_ConstructCanvas(Pr_ObjectRef ap_obj)
{
    Pr_CanvasRef lp_canvas = (Pr_CanvasRef) ap_obj;

    lp_canvas->createEntity = s_Pr_CreateEntity;

    lp_canvas->world = NULL;

    return PR_TRUE;
}

static void s_Pr_DestructCanvas(Pr_ObjectRef ap_obj)
{
}

Pr_Class Pr_CanvasClass = {
    "Pr_Canvas",
    sizeof(Pr_Canvas),
    0,
    &Pr_WidgetContainerClass,
    NULL,
    0,
    PR_FALSE,
    s_Pr_ConstructCanvas,
    s_Pr_DestructCanvas
};

static void PR_SLOT(s_Pr_DeleteWidget)(void * ap_obj, va_list ap_args)
{
    Pr_Delete((Pr_ObjectRef)ap_obj);
}

static void PR_SLOT(s_Pr_SetCanvasAsParent)(void * ap_obj, va_list ap_args)
{
    Pr_CanvasRef lp_canvas = ap_obj;
    Pr_ObjectRef lp_obj = va_arg(ap_args, Pr_ObjectRef);
    Pr_Class * lp_class = va_arg(ap_args, Pr_Class *);

    if (lp_class != &Pr_ObjectClass && lp_class != &Pr_WidgetClass) {
        ((Pr_WidgetRef)lp_obj)->parent = (Pr_WidgetContainerRef)lp_canvas;
    }
}

Pr_WidgetRef   Pr_AddCanvasWidget(Pr_CanvasRef ap_canvas, Pr_Class * ap_class)
{
    Pr_WidgetRef lp_out;

    if (!ap_canvas || !ap_class) return NULL;

    lp_out = Pr_Alloc(ap_class);
    if (!lp_out) return NULL;

    lp_out->parent = NULL;

    Pr_Connect(((Pr_ObjectRef)lp_out)->onConstruction, ap_canvas, PR_SLOT(s_Pr_SetCanvasAsParent));

    if (Pr_SetWidgetParent(lp_out, (Pr_WidgetContainerRef) ap_canvas)) {
        if (Pr_Construct((Pr_ObjectRef)lp_out)) {
            if (Pr_Connect(((Pr_ObjectRef)ap_canvas)->onDelete, lp_out, PR_SLOT(s_Pr_DeleteWidget))) {
                return lp_out;
            }
        }

        Pr_SetWidgetParent(lp_out, NULL);
    }

    Pr_Delete((Pr_ObjectRef)lp_out);

    return NULL;
}
