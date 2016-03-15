#include <Prism/Rainbow/renderingwindow.h>
#include <Prism/Rainbow/renderingtarget.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <Prism/array.h>
#include <string.h>
#include <Prism/Rainbow/opengl.h>

/* friend Pr_Window */ struct s_pr_window_t {
    SDL_Window * wnd;
    unsigned long id;
    Pr_Array(Pr_Signal *) signals;
};

struct pr_renderingwindow_t {
    Pr_Window *         window;
    Pr_RenderingTarget  renderTarget;
    SDL_GLContext       context;
    SDL_Renderer *      renderer;
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

static void s_Pr_InitTransformable(Pr_RenderingWindow * ap_wnd)
{
    Pr_Rect(float) l_rect;

    ap_wnd->renderTarget.cache.statesSet = PR_FALSE;

    l_rect.x = 0.f;
    l_rect.y = 0.f;
    Pr_GetWindowSize(ap_wnd->window, &((pr_u32_t)l_rect.width), &((pr_u32_t)l_rect.height));

    Pr_ResetView(&ap_wnd->renderTarget.defaultView, &l_rect);
    Pr_GetViewCopy(&ap_wnd->renderTarget.defaultView, &ap_wnd->renderTarget.view);
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

Pr_RenderingWindow * Pr_NewRndWindow(Pr_ContextSettingsRef ap_settings)
{
    Pr_RenderingWindow * lp_out;
    struct s_pr_window_t * lp_wnd;

    lp_out = malloc(sizeof(Pr_RenderingWindow));

    if (!lp_out) return NULL;

    lp_wnd = (struct s_pr_window_t *)Pr_NewWindow();

    if (lp_wnd) {
        SDL_DestroyWindow(lp_wnd->wnd);
        lp_wnd->wnd = SDL_CreateWindow("Prism Application" ,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, 
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        );

        if (lp_wnd->wnd) {
            s_Pr_PrepareContext(ap_settings);
            lp_wnd->id = SDL_GetWindowID(lp_wnd->wnd);
            lp_out->context = SDL_GL_CreateContext(lp_wnd->wnd);
            lp_out->renderer = SDL_CreateRenderer(lp_wnd->wnd,-1, SDL_RENDERER_ACCELERATED);

            if (lp_out->context && lp_out->renderer && glewInit() == GLEW_OK && 
                Pr_Connect(Pr_WindowPainted((Pr_WindowRef)lp_wnd),lp_wnd,s_Pr_RenderPresent_Slot) &&
                Pr_Connect(Pr_WindowUpdated((Pr_WindowRef)lp_wnd),lp_wnd,s_Pr_ActivateContext_Slot))
            {
                glewExperimental = GL_TRUE; 
                lp_out->window = (Pr_Window *)lp_wnd;
                s_Pr_InitTransformable(lp_out);
                SDL_GL_SetSwapInterval(1);
                return lp_out;
            }

            SDL_GL_DeleteContext(lp_out->context);
            SDL_DestroyRenderer(lp_out->renderer);
        } 

        Pr_DeleteWindow((Pr_Window *)lp_wnd);
    }

    free(lp_out);

    return NULL;
}

void Pr_DeleteRndWindow(Pr_RenderingWindow * ap_wnd)
{
    if (!ap_wnd) return;

    SDL_GL_DeleteContext(ap_wnd->context);
    SDL_DestroyRenderer(ap_wnd->renderer);
    Pr_DeleteWindow(ap_wnd->window);
    free(ap_wnd);
}

Pr_WindowRef Pr_GetRndWindowRef(Pr_RenderingWindowRef ap_wnd)
{
    return (ap_wnd) ? ap_wnd->window : NULL;
}

Pr_RenderingTargetRef Pr_GetRndWindowTargetRef(Pr_RenderingWindowRef ap_wnd)
{
    return (ap_wnd) ? &ap_wnd->renderTarget : NULL;
}
