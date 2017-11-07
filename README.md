# vtkAntTweakBar
This allows to integrate AntTweakBar with VTK,
providing a quick and easy way to add small user interfaces to VTK applications.

<img src="../assets/demo.png" alt="Demo Application">

## Requirements

* [AntTweakBar](http://anttweakbar.sourceforge.net)
* [VTK](https://www.vtk.org/) >=7.0.0
* [CMake](https://cmake.org/) >= 2.8

## Usage

A demo application can be found in the directory `demo`.

This eventually needs to be made into a proper cmake module. As of now, there are two possiblities to build vtkAntTweakBar:

### Method 1

* Add `src/vtkAntTweakBar.cpp` to your sources
* Add the `include` directory to your include path

### Method 2

Add the entire vtkAntTweakBar tree as subdirectory and add these lines to your `CMakeLists.txt`:

````cmake
include_directories("${PROJECT_SOURCE_DIR}/vtkAntTweakBar/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/vtkAntTweakBar")
````

### Using AntTweakBar in your VTK project

After setting up your VTK render window interactor (called `renderWindowInteractor` here), create a `vtkAntTweakBar` object and setup anttweakbar as follows:

````cpp

#include <AntTweakBar.h>
#include "vtkAntTweakBar.hpp"

// ...

auto twCallback = vtkSmartPointer<vtkAntTweakBar>::New();
twCallback->Initialize(renderWindowInteractor);

// Setup your TweakBars here
TwBar* bar = TwNewBar("TweakBar");
TwAddVarCB(...);
...

renderWindowInteractor->Start();

````

## Todo

* Set this up as a cmake module
* Add methods to hide/show AntTweakBar
* Add tests

## Known Issues

* VTK Widgets that capture input will always get preference over AntTweakBar
* vtkAntTweakBar will listen to the render window's RenderEvent for drawing AntTweakBar. If you do any rendering on your own using this event, make sure to not mess up the rendering order
* AntTweakBar expects `TwDraw` to be called in the event loop, while VTK only redraws if there was a change. By default, vtkAntTweakBar will call `renderWindow->Render()` only when an event (keyboard/mouse input) was handled by AntTweakBar. If you want to change this to render on any event (e.g. when the user moves the mouse outside of a TweakBar), call `vtkAntTweakBar::EnableAlwaysRenderOnEvent()`.


