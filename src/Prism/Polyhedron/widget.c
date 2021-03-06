#include <Prism/Polyhedron/widget.h>
#include <stdlib.h>
#include <string.h>

static void s_OnButtonPressed(Pr_WidgetRef ap_wid, long a_button, long a_x, long a_y)
{}
static void PR_SLOT(s_OnButtonPressed)(void * ap_obj, va_list ap_args)
{}

static void s_OnButtonReleased(Pr_WidgetRef ap_wid, long a_button, long a_x, long a_y)
{}
static void PR_SLOT(s_OnButtonReleased)(void * ap_obj, va_list ap_args)
{}

static void s_OnMouseEnter(Pr_WidgetRef ap_wid)
{}
static void PR_SLOT(s_OnMouseEnter)(void * ap_obj, va_list ap_args)
{}

static void s_OnMouseExit(Pr_WidgetRef ap_wid)
{}
static void PR_SLOT(s_OnMouseExit)(void * ap_obj, va_list ap_args)
{}

static void s_OnMouseMoved(Pr_WidgetRef ap_wid, long a_x, long a_y)
{}
static void PR_SLOT(s_OnMouseMoved)(void * ap_obj, va_list ap_args)
{}

static void s_Layout(Pr_WidgetRef ap_wid)
{}
static void PR_SLOT(s_Layout)(void * ap_obj, va_list ap_args)
{}

static void s_Show(Pr_WidgetRef ap_wid, pr_bool_t a_bool)
{}
static void PR_SLOT(s_Show)(void * ap_obj, va_list ap_args)
{}

static void s_onKeyPressed(Pr_WidgetRef ap_wid, long a_k)
{}
static void PR_SLOT(s_onKeyPressed)(void * ap_obj, va_list ap_args)
{}

static void s_onKeyReleased(Pr_WidgetRef ap_wid, long a_k)
{}
static void PR_SLOT(s_onKeyReleased)(void * ap_obj, va_list ap_args)
{}

static pr_bool_t s_Pr_ConstructWidget(Pr_ObjectRef ap_obj)
{
    Pr_WidgetRef lp_widget = (Pr_WidgetRef)ap_obj;

    lp_widget->moved = Pr_NewSignal();
    lp_widget->sizeChanged = Pr_NewSignal();

    if (!lp_widget->moved 
        || !lp_widget->sizeChanged
    ) {
        Pr_DeleteSignal(lp_widget->moved);
        Pr_DeleteSignal(lp_widget->sizeChanged);
        return PR_FALSE;
    }

    #define _SETSLOT(name, func) lp_widget->name = func, lp_widget->PR_SLOT(name) = PR_SLOT(func)
        _SETSLOT(onButtonPressed, s_OnButtonPressed);
        _SETSLOT(onButtonReleased, s_OnButtonReleased);
        _SETSLOT(onMouseEnter, s_OnMouseEnter);
        _SETSLOT(onMouseExit, s_OnMouseExit);
        _SETSLOT(onMouseMoved, s_OnMouseMoved);
        _SETSLOT(onKeyPressed, s_onKeyPressed);
        _SETSLOT(onKeyReleased, s_onKeyReleased);

        _SETSLOT(setSize, Pr_SetWidgetSize);
        _SETSLOT(setPosition, Pr_SetWidgetPosition);

        _SETSLOT(layout, s_Layout);
        _SETSLOT(show, s_Show);
    #undef _SETSLOT

    lp_widget->enabled = PR_FALSE;
    lp_widget->visible = PR_FALSE;
    lp_widget->hasFocus = PR_FALSE;

    lp_widget->boundingBox.x = 0;
    lp_widget->boundingBox.y = 0;
    lp_widget->boundingBox.w = 0;
    lp_widget->boundingBox.h = 0;

    lp_widget->parent = NULL;
        
    return PR_TRUE;
}

static void s_Pr_DestructWidget(Pr_ObjectRef ap_obj)
{
    Pr_WidgetRef lp_widget = (Pr_WidgetRef)ap_obj;

    Pr_DeleteSignal(lp_widget->moved);
    Pr_DeleteSignal(lp_widget->sizeChanged);
}

Pr_Class Pr_WidgetClass = {
    "Pr_Widget",
    sizeof(Pr_Widget),
    0,
    &Pr_ObjectClass,
    NULL,
    0,
    PR_TRUE,
    s_Pr_ConstructWidget,
    s_Pr_DestructWidget
};

void Pr_SetWidgetSize(Pr_WidgetRef ap_wid, pr_u32_t a_w, pr_u32_t a_h)
{
    if (!ap_wid) return;

    ap_wid->boundingBox.w = a_w;
    ap_wid->boundingBox.h = a_h;

    ap_wid->layout(ap_wid);

    Pr_Emit(ap_wid->sizeChanged, a_w, a_h);
}
    void PR_SLOT(Pr_SetWidgetSize)(void * ap_obj, va_list ap_args) {   
        pr_u32_t l_w = va_arg(ap_args, pr_u32_t);
        pr_u32_t l_h = va_arg(ap_args, pr_u32_t);

        Pr_SetWidgetSize(ap_obj, l_w, l_h);
}

void Pr_SetWidgetPosition(Pr_WidgetRef ap_wid, long a_x, long a_y)
{
    if (!ap_wid) return;

    ap_wid->boundingBox.x = a_x;
    ap_wid->boundingBox.y = a_y;

    ap_wid->layout(ap_wid);

    Pr_Emit(ap_wid->moved, a_x, a_y);
}
    void PR_SLOT(Pr_SetWidgetPosition)(void * ap_obj, va_list ap_args) {
        long l_x = va_arg(ap_args, long);
        long l_y = va_arg(ap_args, long);

        Pr_SetWidgetPosition(ap_obj, l_x, l_y);
}

static pr_bool_t s_collide(Pr_WidgetRef ap_wid, long a_x, long a_y)
{
    return (a_x >= ap_wid->boundingBox.x 
        && a_x < ap_wid->boundingBox.x + (long)ap_wid->boundingBox.w
        && a_y >= ap_wid->boundingBox.y 
        && a_y < ap_wid->boundingBox.y + (long)ap_wid->boundingBox.h
    );
}

static void s_DestructWidgetContainer(Pr_ObjectRef ap_obj)
{
    Pr_WidgetContainerRef lp_cont = (Pr_WidgetContainerRef)ap_obj;

    Pr_DeleteList(lp_cont->children);

    Pr_DeleteSignal(lp_cont->input.mouseMoved);
    Pr_DeleteSignal(lp_cont->input.buttonPressed);
    Pr_DeleteSignal(lp_cont->input.buttonReleased);
    Pr_DeleteSignal(lp_cont->input.keyPressed);
    Pr_DeleteSignal(lp_cont->input.keyReleased);
}

static void s_ContainerOnMouseMoved(Pr_WidgetRef ap_wid, long a_x, long a_y)
{
    Pr_WidgetContainerRef lp_cont = (Pr_WidgetContainerRef)ap_wid;
    pr_bool_t l_collide = PR_FALSE;
    Pr_WidgetRef lp_entry = NULL;

    static pr_bool_t sl_mouseIn = PR_FALSE;

    if (!ap_wid) return;

    l_collide = s_collide(ap_wid, a_x, a_y);

    if (l_collide && !sl_mouseIn) {
        ap_wid->onMouseEnter(ap_wid);
        sl_mouseIn = PR_TRUE;
    }

    if (!l_collide && sl_mouseIn) {
        ap_wid->onMouseExit(ap_wid);
        sl_mouseIn = PR_FALSE;
    }

    if (sl_mouseIn) {
        Pr_ListIterator lp_it;
        PR_LIST_FOREACH(lp_cont->children, lp_it) {
            Pr_WidgetRef lp_tmp = Pr_ListIteratorData(lp_it);
            if (s_collide(lp_tmp, a_x, a_y)) {
                lp_entry = lp_tmp;
                break;
            }
        }
    }

    if (lp_entry) {
        if (lp_cont->hoveredWidget != lp_entry) {
            if (lp_cont->hoveredWidget) {
                lp_cont->hoveredWidget->onMouseExit(lp_cont->hoveredWidget);
            }

            lp_cont->hoveredWidget = lp_entry;
            lp_entry->onMouseEnter(lp_entry);
        }

        lp_entry->onMouseMoved(lp_entry, a_x, a_y);

    } else if (lp_cont->hoveredWidget) {
        lp_cont->hoveredWidget->onMouseExit(lp_cont->hoveredWidget);
        lp_cont->hoveredWidget = NULL;
    }
}

static void s_ContainerOnMouseMoved_Slot(void * ap_obj, va_list ap_args)
{
    long l_x = va_arg(ap_args, long);
    long l_y = va_arg(ap_args, long);

    s_ContainerOnMouseMoved(ap_obj, l_x, l_y);
}

static void s_ContainerOnButtonPressed(Pr_WidgetRef ap_wid, long a_button, long a_x, long a_y)
{
    Pr_WidgetContainerRef lp_wid = (Pr_WidgetContainerRef)ap_wid;

    if (!ap_wid) return;

    if (lp_wid->hoveredWidget) {
        lp_wid->clickedWidget = lp_wid->hoveredWidget;
        if (lp_wid->hoveredWidget != ap_wid) {
            lp_wid->hoveredWidget->onButtonPressed(lp_wid->hoveredWidget, a_button, a_x, a_y);
        }
    }
}

static void s_ContainerOnButtonPressed_Slot(void * ap_obj, va_list ap_args)
{
    long l_b = va_arg(ap_args, long);
    long l_x = va_arg(ap_args, long);
    long l_y = va_arg(ap_args, long);

    s_ContainerOnButtonPressed(ap_obj, l_b, l_x, l_y);
}

static void s_ContainerOnButtonReleased(Pr_WidgetRef ap_wid, long a_button, long a_x, long a_y)
{
    Pr_WidgetContainerRef lp_wid = (Pr_WidgetContainerRef)ap_wid;

    if (!ap_wid) return;

    if (!lp_wid->hoveredWidget) return;

    if (lp_wid->hoveredWidget == lp_wid->clickedWidget) {
        if (lp_wid->clickedWidget != ap_wid) {
            lp_wid->hoveredWidget->onButtonReleased(lp_wid->hoveredWidget, a_button, a_x, a_y);
        }
    }
}

static void s_ContainerOnButtonReleased_Slot(void * ap_obj, va_list ap_args)
{
    long l_b = va_arg(ap_args, long);
    long l_x = va_arg(ap_args, long);
    long l_y = va_arg(ap_args, long);

    s_ContainerOnButtonReleased(ap_obj, l_b, l_x, l_y);
}

static pr_bool_t s_MakeInput(Pr_WidgetContainerRef ap_cont)
{
    ap_cont->input.mouseMoved = Pr_NewSignal();
    ap_cont->input.buttonPressed = Pr_NewSignal();
    ap_cont->input.buttonReleased = Pr_NewSignal();
    ap_cont->input.keyPressed = Pr_NewSignal();
    ap_cont->input.keyReleased = Pr_NewSignal();

    if (ap_cont->input.mouseMoved
        && ap_cont->input.buttonPressed
        && ap_cont->input.buttonReleased
        && ap_cont->input.keyPressed
        && ap_cont->input.keyReleased
    ) {
        if (Pr_ConnectRelay(Pr_MouseMoved(), ap_cont->input.mouseMoved)
            && Pr_ConnectRelay(Pr_MouseButtonDown(), ap_cont->input.buttonPressed)
            && Pr_ConnectRelay(Pr_MouseButtonUp(), ap_cont->input.buttonReleased)
        ) {
            return PR_TRUE;
        }
    }

    Pr_DeleteSignal(ap_cont->input.mouseMoved);
    Pr_DeleteSignal(ap_cont->input.buttonPressed);
    Pr_DeleteSignal(ap_cont->input.buttonReleased);
    Pr_DeleteSignal(ap_cont->input.keyPressed);
    Pr_DeleteSignal(ap_cont->input.keyReleased);

    return PR_FALSE;
}

static pr_bool_t s_ConstructWidgetContainer(Pr_ObjectRef ap_obj)
{
    Pr_WidgetRef lp_wid = (Pr_WidgetRef)ap_obj;
    Pr_WidgetContainerRef lp_cont = (Pr_WidgetContainerRef)ap_obj;

    lp_cont->hoveredWidget = NULL;
    lp_cont->clickedWidget = NULL;
    lp_cont->focusedWidget = NULL;

    lp_cont->children = Pr_NewList();
    if (!lp_cont->children) {
        return PR_FALSE;
    }

    if (!s_MakeInput(lp_cont)) {
        Pr_DeleteList(lp_cont->children);
        return PR_FALSE;
    }

    lp_wid->onMouseMoved            = s_ContainerOnMouseMoved;
    lp_wid->PR_SLOT(onMouseMoved)       = PR_SLOT(s_ContainerOnMouseMoved); 

    lp_wid->onButtonPressed         = s_ContainerOnButtonPressed;
    lp_wid->PR_SLOT(onButtonPressed)    = PR_SLOT(s_ContainerOnButtonPressed);

    lp_wid->onButtonReleased        = s_ContainerOnButtonReleased;
    lp_wid->PR_SLOT(onButtonReleased)   = PR_SLOT(s_ContainerOnButtonReleased);

    Pr_Connect(lp_cont->input.mouseMoved, lp_wid, lp_wid->PR_SLOT(onMouseMoved));
    Pr_Connect(lp_cont->input.buttonPressed, lp_wid, lp_wid->PR_SLOT(onButtonPressed));
    Pr_Connect(lp_cont->input.buttonReleased, lp_wid, lp_wid->PR_SLOT(onButtonReleased));

    return PR_TRUE;
}

Pr_Class Pr_WidgetContainerClass = {
    "Pr_WidgetContainer",
    sizeof(Pr_WidgetContainer),
    0,
    &Pr_WidgetClass,
    NULL,
    0,
    PR_FALSE,
    s_ConstructWidgetContainer,
    s_DestructWidgetContainer
};

static pr_bool_t s_Pr_FindParent(Pr_WidgetRef ap_wid, Pr_WidgetContainerRef ap_parent)
{
    Pr_WidgetContainerRef lp_tmp = ap_wid->parent;
    Pr_WidgetRef lp_widtmp = (Pr_WidgetRef)lp_tmp;

    while(lp_tmp) {
        if (lp_tmp == ap_parent) {
            return PR_TRUE;
        }

        lp_tmp = lp_widtmp->parent;
    }

    return PR_FALSE;
}

pr_bool_t Pr_WidgetParent(Pr_WidgetRef ap_wid, Pr_WidgetContainerRef ap_parent)
{
    if (!ap_wid || !ap_parent) {
        return PR_FALSE;
    }

    return s_Pr_FindParent(ap_wid, ap_parent);
}

pr_bool_t Pr_SetWidgetParent(Pr_WidgetRef ap_w, Pr_WidgetContainerRef ap_p)
{
    Pr_ObjectRef lp_pobj = (Pr_ObjectRef)ap_p;

    if (!ap_w) {
        return PR_FALSE;
    }

    if (ap_w->parent == ap_p) return PR_TRUE;

    if (ap_w->parent) {
        Pr_ListIterator lp_it;
        PR_LIST_FOREACH(ap_w->parent->children, lp_it) {
            if (ap_w == Pr_ListIteratorData(lp_it)) {
                Pr_EraseListElement(ap_w->parent->children, lp_it);
                ap_w->parent = NULL;
                break;
            }
        }

        if (!ap_p) return PR_TRUE;
    }

    if (!Pr_PushBackList(ap_p->children, ap_w)) {
        return PR_FALSE;
    }

    ap_w->parent = ap_p;

    return PR_TRUE;
}

