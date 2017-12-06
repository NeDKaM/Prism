# Introduction
Prism is a library written in C which provides multiple basic data structure implementations (such as String, Table, etc...) and more advanced features like window management & input handling (with [SDL2](https://www.libsdl.org)), but also a working entity based component system for real time rendering and management.

It is inspired by the C++ library [NazaraEngine](https://github.com/DigitalPulseSoftware/NazaraEngine).

### Setting up a rendering window

```c
#include <Prism/RainbowSDL/gfxwindow.h>

int main(int argc, char ** argv) 
{
    union {
        Pr_SystemWindowRef  sys;
        Pr_GfxWindowRef     gfx;
    } window;

    Pr_InitApp();

    window.gfx = Pr_New(&Pr_GfxWindowClass);

    Pr_SetWindowSize(window.sys, 100, 100);
    Pr_SetWindowTitle(window.sys, "Prism application");

    Pr_Connect(Pr_WindowClosed(window.sys), NULL, PR_SLOT(Pr_QuitApp));
    Pr_Connect(Pr_AppStarted(), window.sys, PR_SLOT(Pr_ShowWindow));

    Pr_ExecApp();

    return 0;
}
```

This code sets up the Prism application and creates a window visible on screen. 

Before all, we'll need to init Prism application (which basically allows us to use windows). 

Then we create our window object : Prism has its own object representation which tries to imitate OO behavior (construction, inheritance). Those objects **need** to some extend inherit from the very-base object `Pr_Object`. 
For instance, `Pr_GfxWindow` inherits from `Pr_SystemWindow`, which, appart from wrapping a `SDL_Window`, directly inherits from `Pr_Object`. That being said, every objects shall be associated with a `Pr_Class` structure that describes internal information of an object, its constructor/destructor, etc...

`Pr_New(class)` allows us to allocate and construct objects, by passing the associated class as argument. Here we use `Pr_GfxWindowClass` adress, the Prism provided global structure for SDL based rendering window.
Because we'll need to access the `Pr_SystemWindow` form withtout making the code less readable, we use an union containing the references. Reference of an object **is** a pointer to that object in memory, used to highlight that the object actually is a Prism constructed object.

We set window size & title, using `Pr_SystemWindow` form.

Get a look at `Pr_Connect()` calls ; Prism also provides signal/slot implemation with `Pr_Signal` structure. On signal emission (`Pr_Emit(signal, args...)`), connected slot (`Pr_Slot`) will be called with previously associated data adress & emitted arguments.
Prism provides a `PR_SLOT(fn)` macro to simplify slots syntax, and signal getters function for opaque objects. For instance, `PR_SLOT(Pr_QuitApp)` **is** strictly equivalent to `Pr_QuitApp_Slot`, `Pr_WindowClosed(ref)` returns the appropriate signal.

Windows are not visible by default, so we tell application that window should be visible on screen **when** application starts executing with `Pr_ExecApp()` call.

**Done**.

Note that objects **should** be removed with `Pr_Delete(ref)`. Prism windows are not subject to that because application do the job when quitting.

