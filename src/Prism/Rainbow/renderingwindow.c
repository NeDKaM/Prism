#include <Prism/Rainbow/renderingwindow.h>
#include <Prism/Rainbow/renderingtarget.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <Prism/array.h>
#include <string.h>
#include <stdio.h>
#include <Prism/Rainbow/opengl.h>

/* friend Pr_Window */ struct s_pr_window_t {
    SDL_Window *            wnd;
    unsigned long           id;
    Pr_Array(Pr_Signal *)   signals;
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

static void s_Pr_InitTransformable(Pr_RenderingWindow * ap_wnd)
{
    Pr_Rect(float) l_rect;

    l_rect.x = 0.f;
    l_rect.y = 0.f;
    Pr_GetWindowSize(ap_wnd->window, (pr_u32_t *)&l_rect.width, (pr_u32_t *)&l_rect.height);

    Pr_ResetView(&ap_wnd->renderTarget.defaultView, &l_rect);
    Pr_GetViewCopy(&ap_wnd->renderTarget.defaultView, &ap_wnd->renderTarget.view);

    ap_wnd->renderTarget.cache.statesSet = PR_FALSE;
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
    
    if (!ap_obj)    return;
    if (!ap_args)   return;

    lp_wnd = ap_obj;
    
    lp_wnd->renderTarget.size.x = va_arg(ap_args, unsigned int);
    lp_wnd->renderTarget.size.y = va_arg(ap_args, unsigned int);

    Pr_SetRndTargetView(&lp_wnd->renderTarget, &lp_wnd->renderTarget.view);

    Pr_Emit(Pr_WindowPainted(lp_wnd->window));
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
        );

        if (lp_wnd->wnd) {
            lp_wnd->id = SDL_GetWindowID(lp_wnd->wnd);
            lp_out->context = SDL_GL_CreateContext(lp_wnd->wnd);
#ifdef PRISM_SYSTEM_WINDOWS
            glewExperimental = GL_TRUE; 
#endif
            SDL_GL_MakeCurrent(lp_wnd->wnd, lp_out->context);
            
            if (lp_out->context
#ifdef PRISM_SYSTEM_WINDOWS
                && glewInit() == GLEW_OK
#endif          
                && Pr_Connect(Pr_WindowPainted((Pr_WindowRef)lp_wnd), lp_out, s_Pr_RenderPresent_Slot)  
                && Pr_Connect(Pr_WindowUpdated((Pr_WindowRef)lp_wnd), lp_out, s_Pr_ActivateContext_Slot) 
                && Pr_Connect(Pr_WindowSizeChanged((Pr_WindowRef)lp_wnd), lp_out, s_Pr_Resize_Slot)
                && Pr_Connect(Pr_WindowOnDelete((Pr_WindowRef)lp_wnd), lp_out, s_Pr_Delete_Slot)
            ) {
                glClearColor(1.f, 1.f, 1.f, 1.f);
                glClear(GL_COLOR_BUFFER_BIT);
                SDL_GL_SwapWindow(lp_wnd->wnd);
                lp_out->window = (Pr_Window *)lp_wnd;
                s_Pr_InitTransformable(lp_out);
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
