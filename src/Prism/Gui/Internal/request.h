#ifndef H_PRISM_REQUEST_INCLUDED
#define H_PRISM_REQUEST_INCLUDED

#include <Prism/Gui/window.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* PRIVATE */ int   Pr_Request_NewWindow(Pr_Window *);
    /* PRIVATE */ int   Pr_Request_DeleteWindow(Pr_Window *);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_REQUEST_INCLUDED */