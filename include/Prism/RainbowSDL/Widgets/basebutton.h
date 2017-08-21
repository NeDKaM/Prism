#ifndef H_RAINBOWSDL_BASEBUTTON_INCLUDED
#define H_RAINBOWSDL_BASEBUTTON_INCLUDED

#include <Prism/Polyhedron/widget.h>

#include <Prism/ECS/entity.h>

#include <Prism/RainbowSDL/text.h>

PR_CPP_PROTECT_ON

    PR_OBJECT(pr_basebutton_t, Pr_BaseButton) {
        PR_INHERIT(Pr_Widget);

        Pr_Signal *     pushed;
        Pr_Signal *     clicked;

        Pr_SlotFunction(setText)(Pr_BaseButtonRef, pr_cstring_t const);

        pr_entityid_t   renderableEntity;
        
        pr_entityid_t   textEntity;
        Pr_TextSprite * textSprite;
    };

    PR_SLOT_EXTERN_API(Pr_SetBaseButtonText)(Pr_BaseButtonRef, pr_cstring_t const);

    PR_SLOT_EXTERN_API(Pr_OnBaseButtonPressed)(Pr_WidgetRef, long, long, long);
    PR_SLOT_EXTERN_API(Pr_OnBaseButtonReleased)(Pr_WidgetRef, long, long, long);

    extern PRISM_API Pr_Class Pr_BaseButtonClass;

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_BASEBUTTON_INCLUDED */
