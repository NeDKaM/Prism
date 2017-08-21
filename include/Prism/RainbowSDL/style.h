#ifndef H_RAINBOWSDL_STYLE_H
#define H_RAINBOWSDL_STYLE_H

#include <Prism/config.h>
#include <Prism/types.h>

#include <Prism/Polyhedron/widget.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_styleheader_t, Pr_StyleHeader);

    PR_STRUCT(pr_style_t, Pr_Style) {
        Pr_StyleHeader *    header;
        void *              data;
    };

    PR_STRUCT(pr_styleinfo_t, Pr_StyleInfo) {
        pr_u32_t    dataSize;
        pr_bool_t   (*setStyle)(Pr_Style *, Pr_WidgetRef);
    };

    PR_STRUCT(pr_skin_t, Pr_Skin);

    extern PRISM_API Pr_Skin *  Pr_NewSkin(void);
    extern PRISM_API void       Pr_DeleteSkin(Pr_Skin *);
    extern PRISM_API Pr_Style * Pr_RegisterSkinStyle(Pr_Skin *, Pr_StyleHeader *);
    extern PRISM_API Pr_Style * Pr_GetSkinStyle(Pr_Skin *, pr_cstring_t const, Pr_Class *);

    extern PRISM_API pr_bool_t  Pr_SetWidgetStyle(Pr_WidgetRef, Pr_Style *);

    extern PRISM_API Pr_StyleHeader *   Pr_NewStyleHeader(pr_cstring_t const, Pr_StyleInfo *, Pr_Class *);
    extern PRISM_API void               Pr_DeleteStyleHeader(Pr_StyleHeader *);
    extern PRISM_API Pr_Style *         Pr_GetHeaderStyle(Pr_StyleHeader *);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOWSDL_STYLE_H */

/*
    pr_bool_t   Pr_AddStyleHeaderState(Pr_StyleHeader *, pr_cstring_t const, Pr_StyleState);
    pr_bool_t   Pr_AddStyleHeaderAttribute(Pr_StyleHeader *, pr_cstring const, pr_u32_t, pr_u32_t);
*/
