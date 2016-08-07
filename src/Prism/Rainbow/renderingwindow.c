#include <Prism/Rainbow/renderingwindow.h>
#include <Prism/Rainbow/renderingtarget.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <Prism/list.h>
#include <string.h>
#include <stdio.h>
#include <Prism/Rainbow/opengl.h>

#ifdef PRISM_SYSTEM_WINDOWS
static pr_bool_t s_glewOn = PR_FALSE;
#endif

/* friend Pr_Window */ struct s_pr_window_t {
    SDL_Window *    wnd;
    unsigned long   id;
    Pr_List *       signals;
};

struct pr_renderingwindow_t {
    Pr_Window *         window;
    Pr_RenderingTarget  renderTarget;
    SDL_GLContext       context;
};

static void s_Pr_ActivateContext_Slot(void * ap_obj, va_list ap_args)
{
    Pr_RenderingWindow *    lp_rndwnd;
    struct s_pr_window_t *  lp_wnd;

    if (!ap_obj) return;

    lp_rndwnd   = ap_obj;
    lp_wnd      = (struct s_pr_window_t *)lp_rndwnd->window;

    SDL_GL_MakeCurrent(lp_wnd->wnd, lp_rndwnd->context); 
}

static void s_Pr_InitRenderingTraget(Pr_RenderingWindow * ap_wnd)
{
    Pr_FloatRect l_rect;
    pr_u32_t l_w = 0;
    pr_u32_t l_h = 0;

    Pr_GetWindowSize(ap_wnd->window, &l_w, &l_h);
    l_rect.x        = 0.f;
    l_rect.y        = 0.f;
    l_rect.width    = (float)l_w;
    l_rect.height   = (float)l_h;

    Pr_ResetView(&ap_wnd->renderTarget.defaultView, NULL);
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

static void s_Pr_PrepareContext(Pr_ContextSettingsRef ap_settings)
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

static void s_Pr_RenderPresent_Slot(void * ap_obj, va_list ap_args)
{
    Pr_RenderingWindow * lp_rnd;
    struct s_pr_window_t * lp_wnd;

    if (!ap_obj) return;

    lp_rnd = ap_obj;
    lp_wnd = (struct s_pr_window_t *)lp_rnd->window;

    SDL_GL_SwapWindow(lp_wnd->wnd);
}

static void s_Pr_Resize_Slot(void * ap_obj, va_list ap_args)
{
    Pr_RenderingWindow *    lp_wnd;  
    Pr_Vector2i             l_size;
    
    if (!ap_obj)    return;
    if (!ap_args)   return;

    lp_wnd = ap_obj;

    l_size.x = (long)va_arg(ap_args, pr_u32_t);
    l_size.y = (long)va_arg(ap_args, pr_u32_t);

    lp_wnd->renderTarget.size = l_size;

    Pr_SetRndTargetView(&lp_wnd->renderTarget, &lp_wnd->renderTarget.view);
}

static void s_Pr_Delete_Slot(void * ap_obj, va_list ap_args)
{
    Pr_RenderingWindow * lp_wnd;

    if (!ap_obj) return;

    lp_wnd = ap_obj;

    SDL_GL_DeleteContext(lp_wnd->context);
    free(lp_wnd);
}

Pr_RenderingWindow * Pr_NewRndWindow(Pr_ContextSettingsRef ap_settings)
{
    Pr_RenderingWindow * lp_out;
    struct s_pr_window_t * lp_wnd;

    lp_out = malloc(sizeof(Pr_RenderingWindow));

    if (!lp_out) return NULL;

    lp_wnd = (struct s_pr_window_t *)Pr_NewWindow();

    if (lp_wnd) {
        SDL_DestroyWindow(lp_wnd->wnd);
        lp_wnd->wnd = NULL;
        s_Pr_PrepareContext(ap_settings);
        lp_wnd->wnd = SDL_CreateWindow("",
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            0, 0,
            SDL_WINDOW_OPENGL
            | SDL_WINDOW_HIDDEN
        );

        if (lp_wnd->wnd) {
#ifdef PRISM_SYSTEM_WINDOWS
            pr_bool_t l_glew = PR_FALSE;
#endif
            lp_wnd->id = SDL_GetWindowID(lp_wnd->wnd);
            lp_out->context = SDL_GL_CreateContext(lp_wnd->wnd);

             SDL_GL_MakeCurrent(lp_wnd->wnd, lp_out->context);

#ifdef PRISM_SYSTEM_WINDOWS
            glewExperimental = GL_TRUE; 
            if (!s_glewOn) {
                l_glew = (glewInit() == GLEW_OK);
            } else {
                l_glew = PR_TRUE;
            }
#endif

            if (lp_out->context
#ifdef PRISM_SYSTEM_WINDOWS
                && l_glew
#endif          
                && Pr_Connect(Pr_WindowPainted((Pr_WindowRef)lp_wnd), lp_out, s_Pr_RenderPresent_Slot)  
                && Pr_Connect(Pr_WindowUpdated((Pr_WindowRef)lp_wnd), lp_out, s_Pr_ActivateContext_Slot) 
                && Pr_Connect(Pr_WindowSizeChanged((Pr_WindowRef)lp_wnd), lp_out, s_Pr_Resize_Slot)
                && Pr_Connect(Pr_WindowOnDelete((Pr_WindowRef)lp_wnd), lp_out, s_Pr_Delete_Slot)
            ) {
                lp_out->window = (Pr_Window *)lp_wnd;
                s_Pr_InitRenderingTraget(lp_out);
                return lp_out;
            }

            SDL_GL_DeleteContext(lp_out->context);
        } 

        Pr_DeleteWindow((Pr_Window *)lp_wnd);
    }

    free(lp_out);

    return NULL;
}

void Pr_DeleteRndWindow(Pr_RenderingWindow * ap_wnd)
{
    if (!ap_wnd) return;

    Pr_DeleteWindow(ap_wnd->window);
}

Pr_WindowRef Pr_GetRndWindowRef(Pr_RenderingWindowRef ap_wnd)
{
    return (ap_wnd) ? ap_wnd->window : NULL;
}

Pr_RenderingTargetRef Pr_GetRndWindowTargetRef(Pr_RenderingWindowRef ap_wnd)
{
    return (ap_wnd) ? &ap_wnd->renderTarget : NULL;
}
