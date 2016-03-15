#ifndef H_RAINBOW_CONTEXTSETTINGS_INCLUDED
#define H_RAINBOW_CONTEXTSETTINGS_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    typedef enum pr_contextattribute_t Pr_ContextAttribute;

    enum pr_contextattribute_t {
        PR_CONTEXTATTRIBUTE_DEFAULT = 0,
        PR_CONTEXTATTRIBUTE_CORE    = 1
    };

    PR_STRUCT(pr_contextsettings_t, Pr_ContextSettings) {
        pr_u32_t            depthBits;
        pr_u32_t            stencilBits;
        pr_u32_t            antialiasingLevel; 
        pr_u32_t            majorVersion;      
        pr_u32_t            minorVersion;
        Pr_ContextAttribute attribute;
    };

PR_CPP_PROTECT_OFF

#endif H_RAINBOW_CONTEXTSETTINGS_INCLUDED

