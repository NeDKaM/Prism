#ifndef H_RAINBOW_OPENGL_INCLUDED
#define H_RAINBOW_OPENGL_INCLUDED

#include <Prism/config.h>

#if defined(PRISM_SYSTEM_WINDOWS)
    #define GLEW_STATIC 
    #include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>

#endif /* H_RAINBOW_OPENGL_INCLUDED */
