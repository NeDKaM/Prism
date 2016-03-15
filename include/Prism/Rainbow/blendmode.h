#ifndef H_RAINBOW_BLENDMODE_INCLUDED
#define H_RAINBOW_BLENDMODE_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    typedef enum pr_blendmodefactor_t   Pr_BlendModeFactor;
    typedef enum pr_blendmodeequation_t Pr_BlendModeEquation;

    enum pr_blendmodefactor_t {
        PR_BLENDF_ZERO,
        PR_BLENDF_1,
        PR_BLENDF_SRC,
        PR_BLENDF_1MINSRC,
        PR_BLENDF_DST,
        PR_BLENDF_1MINDST,
        PR_BLENDF_SRC_ALPHA,
        PR_BLENDF_1MINSRC_ALPHA,
        PR_BLENDF_DST_ALPHA,
        PR_BLENDF_1MINDST_ALPHA      
    };

    enum pr_blendmodeequation_t {
        PR_BLENDE_ADD,
        PR_BLENDE_SUB,
        PR_BLENDE_REVERSESUB
    };

    PR_STRUCT(pr_blendmode_t, Pr_BlendMode) {
        Pr_BlendModeFactor      colorSrcFactor;
        Pr_BlendModeFactor      colorDstFactor;
        Pr_BlendModeEquation    colorEquation;
        Pr_BlendModeFactor      alphaSrcFactor;
        Pr_BlendModeFactor      alphaDstFactor;
        Pr_BlendModeEquation    alphaEquation;
    };

    extern PRISM_API Pr_BlendModeRef    Pr_BlendModeAlpha(void);
    extern PRISM_API Pr_BlendModeRef    Pr_BlendModeAdd(void);
    extern PRISM_API Pr_BlendModeRef    Pr_BlendModeMultiply(void);
    extern PRISM_API Pr_BlendModeRef    Pr_BlendModeNone(void);
    extern PRISM_API pr_bool_t          Pr_BlendModeEquals(Pr_BlendModeRef, Pr_BlendModeRef);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_BLENDMODE_INCLUDED */
