#ifndef H_PRISM_CONFIG_INCLUDED
#define H_PRISM_CONFIG_INCLUDED

#if defined(_WIN32)
	#define PRISM_SYSTEM_WINDOWS
#else 
    #error("Not ready for Linux and co...")
#endif

#if defined(PRISM_SYSTEM_WINDOWS)
	#define PRISM_API_EXPORT __declspec(dllexport)
	#define PRISM_API_IMPORT __declspec(dllimport)
#endif

#if defined(PRISM_EXPORTS)
	#define PRISM_API PRISM_API_EXPORT
#else
	#define PRISM_API PRISM_API_IMPORT
#endif

#if defined(PRISM_STATIC)
    #undef  PRISM_API
    #define PRISM_API /* static */
#endif

#ifdef __cplusplus
    #define PR_CPP_PROTECT_ON  extern "C" {
    #define PR_CPP_PROTECT_OFF }
#else
    #define PR_CPP_PROTECT_ON
    #define PR_CPP_PROTECT_OFF
#endif

#define PR_STRUCT(name_t,name) \
    typedef struct name_t name; \
    struct name_t

#define PR_SLOT_EXTERN_API(name)    extern PRISM_API void name##_Slot(void *, va_list); \
                                    extern PRISM_API void name

#endif /* H_PRISM_CONFIG_INCLUDED */


