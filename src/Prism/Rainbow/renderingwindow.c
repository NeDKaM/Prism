#include <Prism/Rainbow/renderingwindow.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <Prism/list.h>
#include <string.h>
#include <stdio.h>
#include <Prism/Rainbow/opengl.h>

void Pr_ClearRenderWindow(Pr_RenderWindowRef ap_wnd)
{
    if (!ap_wnd) return;

    Pr_ClearRndTarget(&ap_wnd->renderTarget, &ap_wnd->clearColor);
}

#ifdef PRISM_SYSTEM_WINDOWS
    static pr_bool_t s_glewOn = PR_FALSE;
#endif

#define PR_RENDERWINDOW_SLOT(name) static void name(void * ap_obj, va_list ap_args)

/*
PR_RENDERWINDOW_SLOT(s_Pr_ActivateContext_Slot)
{
    Pr_RenderWindowRef lp_this = ap_obj;

    SDL_GL_MakeCurrent(lp_this->self.sdlWindow, lp_this->sdlglContext); 
}
*/

PR_RENDERWINDOW_SLOT(s_Pr_RenderPresent_Slot)
{
    Pr_RenderWindowRef lp_this = ap_obj;

    SDL_GL_SwapWindow(lp_this->self.sdlWindow);
}

PR_RENDERWINDOW_SLOT(s_Pr_Resize_Slot)
{
    Pr_RenderWindowRef  lp_this = ap_obj;  
    Pr_Vector2i         l_size;
    
    if (!ap_args)   return;

    l_size.x = (long)va_arg(ap_args, pr_u32_t);
    l_size.y = (long)va_arg(ap_args, pr_u32_t);

    lp_this->renderTarget.size = l_size;

    Pr_SetRndTargetView(&lp_this->renderTarget, &lp_this->renderTarget.view);
}

static void s_Pr_InitRenderingTraget(Pr_RenderWindowRef ap_wnd)
{
    Pr_FloatRect l_rect;
    pr_u32_t l_w = 0;
    pr_u32_t l_h = 0;

    ap_wnd->self.getSize(&ap_wnd->self, &l_w, &l_h);
    l_rect.x        = 0.f;
    l_rect.y        = 0.f;
    l_rect.width    = (float)l_w;
    l_rect.height   = (float)l_h;

    Pr_ResetView(&ap_wnd->renderTarget.defaultView, &l_rect);
    Pr_GetViewCopy(&ap_wnd->renderTarget.defaultView, &ap_wnd->renderTarget.view);

    ap_wnd->renderTarget.size.x = l_w;
    ap_wnd->renderTarget.size.y = l_h;

    ap_wnd->renderTarget.cache.statesSet        = PR_FALSE;
    ap_wnd->renderTarget.cache.useVertexCache   = PR_FALSE;
    ap_wnd->renderTarget.cache.oldBlendMode     = *Pr_BlendModeDefault();
    ap_wnd->renderTarget.cache.viewChanged      = PR_TRUE;
    ap_wnd->renderTarget.cache.oldTextureId     = 0;
    ap_wnd->renderTarget.cache.oldBlendMode     = *Pr_BlendModeDefault();
    memset(ap_wnd->renderTarget.cache.vertexCache, 0, PR_VERTEXCACHE_SIZE * sizeof(Pr_Vertex));

    Pr_ClearRndTarget(&ap_wnd->renderTarget, NULL);
}

static void s_Pr_PrepareContext(Pr_ContextSettings * ap_settings)
{
    if (ap_settings) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   ap_settings->majorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   ap_settings->minorVersion);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,              ap_settings->depthBits);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,            ap_settings->stencilBits);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,      1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,      ap_settings->antialiasingLevel);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,    
            ap_settings->attribute == PR_CONTEXTATTRIBUTE_DEFAULT
            ? SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
            : SDL_GL_CONTEXT_PROFILE_CORE
        );
    } else {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,   3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,            1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,              24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    }
}

static pr_bool_t s_Pr_ConstructWindow(Pr_ObjectRef ap_obj)
{
    Pr_RenderWindowRef lp_this = (Pr_RenderWindowRef)ap_obj;
    SDL_Window * lp_wnd;

    s_Pr_PrepareContext(NULL);

    lp_wnd = SDL_CreateWindow("",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        0, 0,
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_HIDDEN
    );

    if (lp_wnd) {
        Pr_SystemWindowRef lp_tmp = (Pr_SystemWindowRef)ap_obj;
#ifdef PRISM_SYSTEM_WINDOWS
        pr_bool_t l_glew = PR_FALSE;
#endif

        lp_this->sdlglContext = SDL_GL_CreateContext(lp_wnd);

#ifdef PRISM_SYSTEM_WINDOWS
        glewExperimental = GL_TRUE; 
        if (!s_glewOn) {
            l_glew = (glewInit() == GLEW_OK);
        } else {
            l_glew = PR_TRUE;
        }
#endif
        if (lp_this->sdlglContext
            && l_glew
            && Pr_Connect(Pr_WindowSizeChanged(lp_tmp), lp_tmp, s_Pr_Resize_Slot)
            && Pr_Connect(Pr_WindowOnDisplay(lp_tmp), lp_tmp, s_Pr_RenderPresent_Slot)
        ) {
            Pr_Color l_black = {0,0,0,255};

            SDL_DestroyWindow(lp_tmp->sdlWindow);
            lp_tmp->sdlWindow   = lp_wnd;
            lp_tmp->id          = SDL_GetWindowID(lp_tmp->sdlWindow);
            s_Pr_InitRenderingTraget(lp_this);
            lp_this->clearColor = l_black;

            return PR_TRUE;
        }

        SDL_GL_DeleteContext(lp_this->sdlglContext);
        lp_this->sdlglContext = NULL;
        SDL_DestroyWindow(lp_wnd);
    }

    return PR_FALSE;
}

static void s_Pr_DestructWindow(Pr_ObjectRef ap_obj)
{
    Pr_RenderWindowRef lp_this = (Pr_RenderWindowRef)ap_obj;

    SDL_GL_DeleteContext(lp_this->sdlglContext);
}

Pr_Class Pr_RenderWindowClass = {
    "Pr_RenderWindow",
    sizeof(Pr_RenderWindow),
    0,
    &Pr_SystemWindowClass,
    NULL,
    8,
    PR_FALSE,

    s_Pr_ConstructWindow,
    s_Pr_DestructWindow
};

