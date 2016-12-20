#include <Prism/Rainbow/blendmode.h>

Pr_BlendMode * Pr_BlendModeDefault(void)
{
    return Pr_BlendModeAlpha();
}

Pr_BlendMode * Pr_BlendModeAlpha(void)
{
    Pr_BlendMode * lp_out;
    static Pr_BlendMode l_out = {
        PR_BLENDF_SRC_ALPHA,
        PR_BLENDF_1MINSRC_ALPHA,
        PR_BLENDE_ADD,
        PR_BLENDF_1,
        PR_BLENDF_1MINSRC_ALPHA,
        PR_BLENDE_ADD
    };

    lp_out = &l_out;

    return lp_out;
}

Pr_BlendMode * Pr_BlendModeAdd(void)
{
    Pr_BlendMode * lp_out;
    static Pr_BlendMode l_out = {
        PR_BLENDF_SRC_ALPHA,
        PR_BLENDF_1,
        PR_BLENDE_ADD,
        PR_BLENDF_1,
        PR_BLENDF_1,
        PR_BLENDE_ADD
    };

    lp_out = &l_out;

    return lp_out;
}

Pr_BlendMode * Pr_BlendModeMultiply(void)
{
    Pr_BlendMode * lp_out;
    static Pr_BlendMode l_out = {
        PR_BLENDF_DST,
        PR_BLENDF_ZERO,
        PR_BLENDE_ADD,
        PR_BLENDF_DST,
        PR_BLENDF_ZERO,
        PR_BLENDE_ADD
    };

    lp_out = &l_out;

    return lp_out;
}

Pr_BlendMode * Pr_BlendModeNone(void)
{
    Pr_BlendMode * lp_out;
    static Pr_BlendMode l_out = {
        PR_BLENDF_1,
        PR_BLENDF_ZERO,
        PR_BLENDE_ADD,
        PR_BLENDF_1,
        PR_BLENDF_ZERO,
        PR_BLENDE_ADD
    };

    lp_out = &l_out;

    return lp_out;
}

pr_bool_t Pr_BlendModeEquals(Pr_BlendMode * ap_1, Pr_BlendMode * ap_2)
{
    if (!ap_1 || !ap_2) return PR_FALSE;

    return (pr_bool_t)(
        ap_1->colorSrcFactor    == ap_2->colorSrcFactor &&
        ap_1->colorEquation     == ap_2->colorEquation  &&
        ap_1->colorDstFactor    == ap_2->colorDstFactor &&
        ap_1->alphaSrcFactor    == ap_2->alphaSrcFactor &&
        ap_1->alphaEquation     == ap_2->alphaEquation  &&
        ap_1->alphaDstFactor    == ap_2->alphaDstFactor
    );
}
