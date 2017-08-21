#include <Prism/RainbowSDL/Widgets/basebutton.h>
#include <Prism/RainbowSDL/Widgets/canvas.h>

#include <Prism/RainbowSDL/Components/geometrycomponent.h>
#include <Prism/RainbowSDL/Components/rendercomponent.h>

static void s_Pr_Layout(Pr_WidgetRef ap_wid)
{
    Pr_BaseButtonRef    lp_btn = (Pr_BaseButtonRef) ap_wid;
    Pr_CanvasRef        lp_canvas = (Pr_CanvasRef) ap_wid->parent;
    Pr_Entity *         lp_textEntity = Pr_GetWorldEntity(lp_canvas->world, lp_btn->textEntity);
    Pr_Entity *         lp_rndEntity = Pr_GetWorldEntity(lp_canvas->world, lp_btn->renderableEntity);
    Pr_GeometryComponent * lp_geo;
    SDL_Rect l_bbox = Pr_GetTextBoundingBox(lp_btn->textSprite);

    lp_geo = Pr_GetEntityComponent(lp_textEntity, &Pr_GeometryComponentInfo);
        lp_geo->position.x = ap_wid->boundingBox.x + ap_wid->boundingBox.w / 2 - l_bbox.w / 2;
        lp_geo->position.y = ap_wid->boundingBox.y + ap_wid->boundingBox.h / 2 - l_bbox.h / 2;
        lp_geo->size.w = l_bbox.w;
        lp_geo->size.h = l_bbox.h;

    lp_geo = Pr_GetEntityComponent(lp_rndEntity, &Pr_GeometryComponentInfo);
        lp_geo->position.x = ap_wid->boundingBox.x;
        lp_geo->position.y = ap_wid->boundingBox.y;
        lp_geo->size.w = ap_wid->boundingBox.w;
        lp_geo->size.h = ap_wid->boundingBox.h;
}
    static void PR_SLOT(s_Pr_Layout)(void * ap_obj, va_list ap_args) {
        s_Pr_Layout(ap_obj);
}

void Pr_OnBaseButtonPressed(Pr_WidgetRef ap_wid, long a_x, long a_y, long a_btn)
{
    Pr_BaseButtonRef lp_btn = (Pr_BaseButtonRef) ap_wid;

    Pr_Emit(lp_btn->pushed);
} 
    void PR_SLOT(Pr_OnBaseButtonPressed)(void * ap_obj, va_list ap_args) {
        long l_x = va_arg(ap_args, long);
        long l_y = va_arg(ap_args, long);
        long l_btn = va_arg(ap_args, long);

        Pr_OnBaseButtonPressed(ap_obj, l_x, l_y, l_btn);
}

void Pr_OnBaseButtonReleased(Pr_WidgetRef ap_wid, long a_x, long a_y, long a_btn)
{
    Pr_BaseButtonRef lp_btn = (Pr_BaseButtonRef) ap_wid;

    Pr_Emit(lp_btn->clicked);
} 
    void PR_SLOT(Pr_OnBaseButtonReleased)(void * ap_obj, va_list ap_args) {
        long l_x = va_arg(ap_args, long);
        long l_y = va_arg(ap_args, long);
        long l_btn = va_arg(ap_args, long);

        Pr_OnBaseButtonReleased(ap_obj, l_x, l_y, l_btn);
}

static pr_bool_t s_Pr_ConstructBaseButton(Pr_ObjectRef ap_obj)
{
    Pr_BaseButtonRef    lp_btn = (Pr_BaseButtonRef) ap_obj;
    Pr_WidgetRef        lp_wid = (Pr_WidgetRef) ap_obj;
    Pr_CanvasRef        lp_canvas;
    Pr_Entity *         lp_renderableEntity;
    Pr_Entity *         lp_textEntity;

    if (!lp_wid->parent) return PR_FALSE;

    lp_canvas = PR_CAST(Pr_CanvasClass, lp_wid->parent);
    if (!lp_canvas) return PR_FALSE;

    lp_btn->clicked = Pr_NewSignal();
    lp_btn->pushed = Pr_NewSignal();

    lp_renderableEntity = lp_canvas->createEntity(lp_canvas);
    lp_btn->renderableEntity = lp_renderableEntity->id;
        Pr_AddEntityComponent(lp_renderableEntity, &Pr_GeometryComponentInfo);
        Pr_AddEntityComponent(lp_renderableEntity, &Pr_RenderComponentInfo);

    lp_textEntity = lp_canvas->createEntity(lp_canvas);
    lp_btn->textEntity = lp_textEntity->id;
        Pr_AddEntityComponent(lp_textEntity, &Pr_GeometryComponentInfo);
        Pr_AddEntityComponent(lp_textEntity, &Pr_RenderComponentInfo);

    lp_btn->textSprite = Pr_NewTextSprite();

    lp_wid->layout = s_Pr_Layout;
    lp_wid->PR_SLOT(layout) = PR_SLOT(s_Pr_Layout);

    lp_btn->setText = Pr_SetBaseButtonText;
    lp_btn->PR_SLOT(setText) = PR_SLOT(Pr_SetBaseButtonText);

    lp_wid->onButtonPressed = Pr_OnBaseButtonPressed;
    lp_wid->PR_SLOT(onButtonPressed) = PR_SLOT(Pr_OnBaseButtonPressed);
    lp_wid->onButtonReleased = Pr_OnBaseButtonReleased;
    lp_wid->PR_SLOT(onButtonReleased) = PR_SLOT(Pr_OnBaseButtonReleased);

    return PR_TRUE;
}

static void s_Pr_DestructBaseButton(Pr_ObjectRef ap_obj)
{
    Pr_BaseButtonRef    lp_btn = (Pr_BaseButtonRef) ap_obj;
    Pr_Entity *         lp_renderableEntity;
    Pr_Entity *         lp_textEntity;
    Pr_CanvasRef        lp_canvas = (Pr_CanvasRef)((Pr_WidgetRef)lp_btn)->parent;

    Pr_DeleteTextSprite(lp_btn->textSprite);

    lp_renderableEntity = Pr_GetWorldEntity(lp_canvas->world, lp_btn->renderableEntity);
    Pr_RemoveWorldEntity(lp_renderableEntity);

    lp_textEntity = Pr_GetWorldEntity(lp_canvas->world, lp_btn->textEntity);
    Pr_RemoveWorldEntity(lp_textEntity);

    Pr_DeleteSignal(lp_btn->clicked);   
    Pr_DeleteSignal(lp_btn->pushed);
}

Pr_Class Pr_BaseButtonClass = {
    "Pr_BaseButton",
    sizeof(Pr_BaseButton),
    0,
    &Pr_WidgetClass,
    NULL,
    0,
    PR_FALSE,
    &s_Pr_ConstructBaseButton,
    &s_Pr_DestructBaseButton
};

void PR_SLOT(Pr_SetBaseButtonText)(void * ap_obj, va_list ap_args)
{
    pr_cstring_t const lp_text = va_arg(ap_args, pr_cstring_t const);

    Pr_SetBaseButtonText(ap_obj, lp_text);
}

void Pr_SetBaseButtonText(Pr_BaseButtonRef ap_btn, pr_cstring_t const ap_text)
{
    Pr_Entity * lp_textEntity;
    Pr_RenderComponent * lp_rnd;

    if (!ap_btn) return;

    lp_textEntity = Pr_GetWorldEntity(((Pr_CanvasRef)(((Pr_WidgetRef)ap_btn)->parent))->world, ap_btn->textEntity);

    lp_rnd = Pr_GetEntityComponent(lp_textEntity, &Pr_RenderComponentInfo); {
        Pr_Array * lp_array;
        Pr_String * lp_str = Pr_GetTextSpriteStr(ap_btn->textSprite);
            Pr_SetStringStr(lp_str, ap_text);
        lp_array = Pr_UpdateTextSprite(ap_btn->textSprite);
        Pr_AttachRenderableList(lp_rnd, Pr_GetArrayData(lp_array), Pr_ArraySize(lp_array));
    }

    ((Pr_WidgetRef)ap_btn)->layout((Pr_WidgetRef)ap_btn);
}
