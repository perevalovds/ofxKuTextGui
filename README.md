# ofxKuTextGui - openFrameworks addon for creating textual GUI controlled by keyboard and mouse

It supports integers, floats, strings, stringlists (represented as integer, with string output in GUI) and buttons. 

Values can be edited using keyboard or mouse, and stored in a file.

![example-basic](https://github.com/perevalovds/ofxKuTextGui/raw/master/example-basic.png "example-basic screenshot")

## Installation

Download and unzip as *ofxKuTextGui* to *addons* folder of your openFrameworks installation.

Addon works in openFrameworks 0.10.1 and older, Windows/MacOS/Linux.

For working with addon, you need only four files in your project:

- *ofxKuGextGui.h/cpp* - its GUI itself, see *example-basic* example and 

- *ofxKuTextGuiGen.h/cpp* - this module allows to generate C++ code from GUI script, see *example-gui-generate* example.

Files *ofxKuTextGuiEnvelope* and *ofxKuTextGuiEnvelopeEditor* adds functionality for working with envelopes.


The other files requires adding additional addons to the  project, so just exlude this files  from your project this functionality is not required
and you don't want to add addinional addons:

- *ofxKuTextGuiPreset* works with presets and requires ofxKu addon,

- *ofxKuTextGuiRemote* is used for remote control GUI via OSC and requires ofxOsc addon, see *example-remote* and also 
https://github.com/perevalovds/KuRemote app which indended to control such GUIs remotely.



## Usage

1. *example-basic* example demonstrates creating GUI manually: you need to define variables and then link them with GUI.

2. *example-gui-generate* example demonstrates generation of H/CPP files for constructing GUI from a GUI script file (see below).
By pressing Shift+G it creates source files, which contain GUI creation and also variables definition.

Note: variables names started with "*" means constants, such as *FPS, and defined in C++ code as `PRM FPS` - constant, `PRM _FPS_` - its current value in GUI.
Variables names started with "-" means output values, such as -fps, and defined in C++ code as `PRM fps_`.


3. *example-gui-dyncreate" example shows how to create GUI during runtime from a script file.
In this case, GUI stores values itself, and to get access to value, use the following functions:

`gui.int_()`, `gui.float_()`, `gui_.stringlist_()`, `gui.string_()`, `gui.button_()`, for example:

`gui.int_("backR")`, `gui.float_("-fps")`


## GUI script file

A GUI script file looks as the following:

`PAGE screen`

`int *FPS=30 1:100 1,10`

`dummy Screen size:`

`int *w=1024 1:2000 1,10`

`int *h=768 1:2000 1,10`

`stringlist enabled=OFF [OFF,ON]


`TAB`

`COLOR 255,140,140`

`float -fps=30 0:100 100,10`

`RESET_COLOR`

Then in the code you just link it to gui object:

`PRM setup_gui(gui);`

And for access to parameters use PRM:

`ofSetFrameRate(PRM _FPS_);`

`PRM fps_ = ofGetFrameRate();`


