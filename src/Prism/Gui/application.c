#include <Prism/Gui/application.h>
#include <SDL2/SDL.h>

static int s_initialized = 0;

int Pr_InitApp(void)
{
	if (s_initialized) return 1;

	if (SDL_Init(SDL_INIT_VIDEO)) return 0;

	s_initialized = 1;

	return 1;
}

void Pr_QuitApp(void)
{
	if (!s_initialized) return;

	SDL_Quit();

	s_initialized = 0;
}

void Pr_Slot_QuitApp(va_list ap_args)
{
	(void)ap_args;

	Pr_QuitApp();
}

void Pr_UpdateApp(void)
{

}