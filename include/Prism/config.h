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


#endif /* H_PRISM_CONFIG_INCLUDED */