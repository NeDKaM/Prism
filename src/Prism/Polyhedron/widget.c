#include <Prism/Polyhedron/widget.h>
#include <stdlib.h>
#include <string.h>

static void s_OnButtonPressed(Pr_WidgetRef ap_wid, long a_button, long a_x, long a_y)
{}
static void s_OnButtonPressed_Slot(void * ap_obj, va_list ap_args)
{}

static void s_OnButtonReleased(Pr_WidgetRef ap_wid, long a_button, long a_x, long a_y)
{}
static void s_OnButtonReleased_Slot(void * ap_obj, va_list ap_args)
{}

static void s_OnMouseEnter(Pr_WidgetRef ap_wid)
{}
static void s_OnMouseEnter_Slot(void * ap_obj, va_list ap_args)
{}

static void s_OnMouseExit(Pr_WidgetRef ap_wid)
{}
static void s_OnMouseExit_Slot(void * ap_obj, va_list ap_args)
{}

static void s_OnMouseMoved(Pr_WidgetRef ap_wid, long a_x, long a_y)
{}
static void s_OnMouseMoved_Slot(void * ap_obj, va_list ap_args)
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

    #define _SETSLOT(name, func, slot) lp_widget->name = func, lp_widget->slot_##name = slot;
        _SETSLOT(onButtonPressed, s_OnButtonPressed, s_OnButtonPressed_Slot);
        _SETSLOT(onButtonReleased, s_OnButtonReleased, s_OnButtonReleased_Slot);
        _SETSLOT(onMouseEnter, s_OnMouseEnter, s_OnMouseEnter_Slot);
        _SETSLOT(onMouseExit, s_OnMouseExit, s_OnMouseExit_Slot);
        _SETSLOT(onMouseMoved, s_OnMouseMoved, s_OnMouseMoved_Slot);
        _SETSLOT(onKeyPressed, NULL, NULL);
        _SETSLOT(onKeyReleased, NULL, NULL);

        _SETSLOT(setSize, Pr_SetWidgetSize, Pr_SetWidgetSize_Slot);
        _SETSLOT(setPosition, Pr_SetWidgetPosition, Pr_SetWidgetPosition_Slot);

        _SETSLOT(layout, NULL, NULL);
        _SETSLOT(show, NULL, NULL);
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
    "Pr_WidgetClass",
    sizeof(Pr_Widget),
    0,
    &Pr_ObjectClass,
    NULL,
    0,
    PR_TRUE,
    s_Pr_ConstructWidget,
    s_Pr_DestructWidget
};

#define PR_WIDGET_SLOT(name) void name##_Slot(void * ap_obj, va_list ap_args)

PR_WIDGET_SLOT(Pr_SetWidgetSize)
{   
    pr_u32_t l_w = va_arg(ap_args, pr_u32_t);
    pr_u32_t l_h = va_arg(ap_args, pr_u32_t);

    Pr_SetWidgetSize((Pr_WidgetRef)ap_obj, l_w, l_h);
}

void Pr_SetWidgetSize(Pr_WidgetRef ap_wid, pr_u32_t a_w, pr_u32_t a_h)
{
    ap_wid->boundingBox.w = a_w;
    ap_wid->boundingBox.h = a_h;

    Pr_Emit(ap_wid->sizeChanged, a_w, a_h);
}


PR_WIDGET_SLOT(Pr_SetWidgetPosition)
{
    long l_x = va_arg(ap_args, long);
    long l_y = va_arg(ap_args, long);

    Pr_SetWidgetPosition((Pr_WidgetRef)ap_obj, l_x, l_y);
}

void Pr_SetWidgetPosition(Pr_WidgetRef ap_wid, long a_x, long a_y)
{
    ap_wid->boundingBox.x = a_x;
    ap_wid->boundingBox.y = a_y;

    Pr_Emit(ap_wid->moved, a_x, a_y);
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
}

static void s_ContainerOnMouseMoved(Pr_WidgetRef ap_wid, long a_x, long a_y)
{
    Pr_WidgetContainerRef lp_cont = (Pr_WidgetContainerRef)ap_wid;
    pr_bool_t l_collide = PR_FALSE;
    Pr_ListIterator lp_it;
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

static pr_bool_t s_ConstructWidgetContainer(Pr_ObjectRef ap_obj)
{
    Pr_WidgetRef lp_wid = (Pr_WidgetRef)ap_obj;
    Pr_WidgetContainerRef lp_cont = (Pr_WidgetContainerRef)ap_obj;

    lp_cont->hoveredWidget = NULL;
    lp_cont->clickedWidget = NULL;

    lp_cont->children = Pr_NewList();
    if (!lp_cont->children) {
        return PR_FALSE;
    }

    lp_wid->onMouseMoved            = s_ContainerOnMouseMoved;
    lp_wid->slot_onMouseMoved       = s_ContainerOnMouseMoved_Slot; 

    lp_wid->onButtonPressed         = s_ContainerOnButtonPressed;
    lp_wid->slot_onButtonPressed    = s_ContainerOnButtonPressed_Slot;

    lp_wid->onButtonReleased        = s_ContainerOnButtonReleased;
    lp_wid->slot_onButtonReleased   = s_ContainerOnButtonReleased_Slot;

    Pr_Connect(Pr_MouseMoved(), lp_wid, lp_wid->slot_onMouseMoved);
    Pr_Connect(Pr_MouseButtonDown(), lp_wid, lp_wid->slot_onButtonPressed);
    Pr_Connect(Pr_MouseButtonUp(), lp_wid, lp_wid->slot_onButtonReleased);

    return PR_TRUE;
}

Pr_Class Pr_WidgetContainerClass = {
    "_Pr_WidgetContainerClass",
    sizeof(Pr_WidgetContainer),
    0,
    &Pr_WidgetClass,
    NULL,
    0,
    PR_FALSE,
    s_ConstructWidgetContainer,
    s_DestructWidgetContainer
};

static void s_Pr_DeleteWidget_Slot(void * ap_obj, va_list ap_args)
{
    Pr_Delete((Pr_ObjectRef)ap_obj);
}

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

    if (!ap_w || !ap_p) {
        return PR_FALSE;
    }

    if (s_Pr_FindParent(ap_w, ap_p)) {
        return PR_FALSE;
    }

    if (!Pr_PushBackListData(ap_p->children, ap_w)) {
        return PR_FALSE;
    }

    ap_w->parent = ap_p;

    Pr_Connect(lp_pobj->onDelete, ap_w, s_Pr_DeleteWidget_Slot);

    return PR_TRUE;
}

