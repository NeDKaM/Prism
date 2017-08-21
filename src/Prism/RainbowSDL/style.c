#include <Prism/RainbowSDL/style.h>

#include <Prism/string.h>

#include <stdlib.h>

struct pr_styleheader_t {
    Pr_String *     name;
    Pr_StyleInfo *  info;
    Pr_Class *      class;
    Pr_List *       attributes;
    Pr_Style *      style;
    Pr_List *       states;
};

struct pr_skin_t {
    Pr_List *   styleHeaders;
    Pr_List *   commonAttributes;
};

Pr_Skin * Pr_NewSkin(void)
{
    Pr_Skin * lp_out;

    lp_out = malloc(sizeof(Pr_Skin));
    if (!lp_out) return NULL;

    lp_out->styleHeaders = Pr_NewList();

    if (lp_out->styleHeaders) {
        return lp_out;
    }

    free(lp_out);

    return NULL;
}

void Pr_DeleteSkin(Pr_Skin * ap_skin)
{
    if (!ap_skin) return;

    Pr_DeleteList(ap_skin->styleHeaders);
    free(ap_skin);
}

static void s_Pr_DeleteStyle(Pr_Style * ap_style)
{
    free(ap_style->data);
}

static Pr_Style * s_Pr_NewStyle(Pr_StyleHeader * ap_h, pr_bool_t * a_allocError)
{
    Pr_Style * lp_out;

    lp_out = malloc(sizeof(Pr_Style));
    if (!lp_out) {
        *a_allocError = PR_TRUE;
        return NULL;
    }

    if (ap_h->info->dataSize) {
        lp_out->data = malloc(ap_h->info->dataSize);
        if (!lp_out) {
            *a_allocError = PR_TRUE;
            free(lp_out);
            return NULL;
        }
    } else {
        lp_out = NULL;
    }

    lp_out->header = ap_h;

    return lp_out;
}

Pr_StyleHeader * Pr_NewStyleHeader(pr_cstring_t const ap_name, Pr_StyleInfo * ap_info, Pr_Class * ap_class)
{
    Pr_StyleHeader *    lp_out = NULL;
    pr_bool_t           l_allocError = PR_FALSE;

    if (!ap_info || !ap_class) return NULL;

    lp_out = malloc(sizeof(Pr_StyleHeader));
    if (!lp_out) return NULL;

    lp_out->class = ap_class;
    lp_out->info = ap_info;

    lp_out->attributes = Pr_NewList();
    
    if (!ap_name || !ap_name[0]) {
        lp_out->name = Pr_NewStringStr("UnnamedStyle");
    } else {
        lp_out->name = Pr_NewStringStr(ap_name);
    }

    lp_out->style = s_Pr_NewStyle(lp_out, &l_allocError);

    lp_out->states = Pr_NewList();

    if (lp_out->attributes
        && lp_out->name
        && lp_out->states
        && !l_allocError
    ) {
        return lp_out;
    }

    Pr_DeleteStyleHeader(lp_out);

    return NULL;
}

void Pr_DeleteStyleHeader(Pr_StyleHeader * ap_h)
{
    if (!ap_h) return;

    Pr_DeleteList(ap_h->states);
    s_Pr_DeleteStyle(ap_h->style);
    Pr_DeleteString(ap_h->name);
    Pr_DeleteList(ap_h->attributes);
    free(ap_h);
}

static void s_Pr_MergeStyleAttributes(Pr_Skin * ap_skin, Pr_StyleHeader * ap_h)
{

}

static Pr_Style * s_Pr_CheckStyleExistence(Pr_Skin * ap_skin, pr_cstring_t const ap_name, Pr_Class * ap_class)
{
    Pr_ListIterator lp_it;

    PR_LIST_FOREACH(ap_skin->styleHeaders, lp_it) {
        Pr_StyleHeader * lp_h = Pr_ListIteratorData(lp_it);
        if (lp_h->class == ap_class && !strcmp(ap_name, Pr_StringCStr(lp_h->name))) {
            return lp_h->style;
        }
    }

    return NULL;
}

Pr_Style * Pr_RegisterSkinStyle(Pr_Skin * ap_skin, Pr_StyleHeader * ap_header) 
{
    if (!ap_header) return NULL;

    if (ap_skin) {
        if (!s_Pr_CheckStyleExistence(ap_skin, Pr_StringCStr(ap_header->name), ap_header->class)) {
            Pr_PushBackList(ap_skin->styleHeaders, ap_header);
            s_Pr_MergeStyleAttributes(ap_skin, ap_header);
            return ap_header->style;
        }
    }

    return NULL;
}

Pr_Style * Pr_GetSkinStyle(Pr_Skin * ap_skin, pr_cstring_t const ap_name, Pr_Class * ap_class)
{
    Pr_Style * lp_out;

    if (!ap_skin || !ap_class) return NULL;

    if (!ap_name || !ap_name[0]) {
        lp_out = s_Pr_CheckStyleExistence(ap_skin, "UnnamedStyle", ap_class);
    } else {
        lp_out = s_Pr_CheckStyleExistence(ap_skin, ap_name, ap_class);
    }

    return lp_out;
}

pr_bool_t Pr_SetWidgetStyle(Pr_WidgetRef ap_wid, Pr_Style * ap_style)
{
    if (!ap_wid || !ap_style) return PR_FALSE;

    return ap_style->header->info->setStyle(ap_style, ap_wid);
}

Pr_Style * Pr_GetHeaderStyle(Pr_StyleHeader * ap_header)
{
    return (ap_header) ? ap_header->style : NULL;
}

