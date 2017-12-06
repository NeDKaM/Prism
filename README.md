# Introduction
Prism is a library written in C which provides multiple basic data structure implementations (such as String, Table, etc...) and more advanced features like window management & input handling (with [SDL2](https://www.libsdl.org)), but also a working component based entity system for real time management and rendering.

It is inspired by the [NazaraEngine](https://github.com/DigitalPulseSoftware/NazaraEngine) C++ library.

Documentation/tutorials available in the [Wiki](https://github.com/NeDKaM/Prism/wiki) section tab.

# Getting Started

Make sure you've correctly linked the library to your project and included the DLL files.

## Example : setting up a rendering window

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

This sample code sets up the Prism application and creates a visible window on screen. 

Before all, we need to init Prism application (which basically allows us to use windows). 

Then we create our window object : Prism has its own object representation which tries to imitate OO behavior (construction, inheritance). Those objects **need** to inherit from the very-base object `Pr_Object`. 
For instance, `Pr_GfxWindow` inherits from `Pr_SystemWindow`, which, appart from wrapping a `SDL_Window`, directly inherits from `Pr_Object`. That being said, every objects shall be associated with a `Pr_Class` structure that describes internal information of an object, its constructor/destructor, etc...

`Pr_New(class)` allows us to allocate and construct objects, by passing the associated class as argument. Here we use `Pr_GfxWindowClass` adress, the Prism provided global structure for SDL based rendering window.
Because we want to access the `Pr_SystemWindow` form without making the code less readable, we use an union containing the references. Reference to an object **is** a pointer to that object in memory, and is used to highlight that the object actually is a Prism constructed object.

We set window size & title, using `Pr_SystemWindow` form.

Get a look at `Pr_Connect()` calls ; Prism also provides signal/slot implemantation with `Pr_Signal` structure. On signal emission (`Pr_Emit(signal, args...)`), connected slot (`Pr_Slot`) will be called with previously associated data adress & emitted arguments.
Prism provides a `PR_SLOT(fn)` macro to simplify slots syntax, and signal getters function for opaque objects. For instance, `PR_SLOT(Pr_QuitApp)` is **strictly equivalent** to `Pr_QuitApp_Slot` ; `Pr_WindowClosed(ref)` returns the appropriate signal.

First we specify that application should quit when we hit the system window closing button.
Windows are not visible by default, so we tell application that window should be visible on screen when application **starts executing** with `Pr_ExecApp()` call.

**Done**.

Note that objects **should** be removed with `Pr_Delete(ref)`. Prism windows are not subject to that because application does the job when quitting.

# About

This project is developped by Alexandre "**NeDKaM**" Dubujet for educational purpose mainly.

