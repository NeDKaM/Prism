#ifndef H_PRISM_TYPES_INCLUDED
#define H_PRISM_TYPES_INCLUDED

PR_CPP_PROTECT_ON

    typedef char pr_bool_t;
    #define PR_TRUE  1
    #define PR_FALSE 0

    typedef unsigned long   pr_u32_t;
    typedef unsigned char   pr_u8_t;
    typedef unsigned short  pr_u16_t;
    typedef const pr_u8_t * Pr_ByteArrayRef;
    typedef const char *    Pr_CStrRef;

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_TYPES_INCLUDED */ 
