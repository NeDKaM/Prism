#ifndef H_RAINBOW_RENDERINGTARGET_INCLUDED
#define H_RAINBOW_RENDERINGTARGET_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>
#include <Prism/Rainbow/vertex.h>
#include <Prism/Rainbow/blendmode.h>
#include <Prism/Rainbow/view.h>
#include <Prism/Rainbow/rect.h>
#include <Prism/Rainbow/renderingstates.h>

PR_CPP_PROTECT_ON

    typedef enum pr_primitypetype_t Pr_PrimitiveType;

    enum pr_primitypetype_t {
        PR_PRIMITIVE_POINTS = 0,
        PR_PRIMITIVE_LINES,
        PR_PRIMITIVE_LINESSTRIP,
        PR_PRIMITIVE_TRIANGLES,
        PR_PRIMITIVE_TRIANGLESSTRIP,
        PR_PRIMITIVE_TRIANGLESFAN,
        PR_PRIMITIVE_QUADS
    };

    PR_STRUCT(pr_renderingtarget_t, Pr_RenderingTarget) {
        struct {
            #define PR_VERTEXCACHE_SIZE 4
            Pr_Vertex       vertexCache[PR_VERTEXCACHE_SIZE];
            pr_u32_t        oldTextureId;
            Pr_BlendMode    oldBlendMode;
            pr_bool_t       statesSet;
            pr_bool_t       useVertexCache;
        }           cache;
        Pr_View     defaultView;
        Pr_View     view;
        Pr_Vector2i size;
    };

    extern PRISM_API pr_bool_t  Pr_GetRndTargetViewport(Pr_RenderingTargetRef, Pr_ViewRef, Pr_Recti *);
    extern PRISM_API void       Pr_ClearRndTarget(Pr_RenderingTarget *, Pr_ColorRef);
    extern PRISM_API pr_bool_t  Pr_MapPixelToCoord(Pr_RenderingTargetRef, Pr_Vector2Ref(long), Pr_Vector2f *);
    extern PRISM_API pr_bool_t  Pr_MapCoordToPixel(Pr_RenderingTargetRef, Pr_Vector2Ref(float), Pr_Vector2i *);
    extern PRISM_API void       Pr_PushGLStates(Pr_RenderingTarget *);
    extern PRISM_API void       Pr_PopGLStates(Pr_RenderingTarget *);
    extern PRISM_API void       Pr_ResetGLStates(Pr_RenderingTarget *);
    extern PRISM_API void       Pr_SetRndTargetView(Pr_RenderingTarget *, Pr_ViewRef);
    extern PRISM_API void       Pr_RndTargetDraw(Pr_RenderingTarget *, Pr_Vertex const *, pr_u32_t, Pr_PrimitiveType, Pr_RenderingStatesRef);

PR_CPP_PROTECT_OFF

#endif /* H_RAINBOW_RENDERINGTARGET_INCLUDED */
