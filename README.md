# ofxKuTextGui - openFrameworks addon for creating textual GUI controlled by keyboard and mouse

It supports integers, floats, strings, stringlists (represented as integer, with string output in GUI) and buttons. 

Values can be edited using keyboard or mouse, and stored in a file.

![example-basic](https://github.com/perevalovds/ofxKuTextGui/raw/master/example-basic.png "example-basic screenshot")

## Installation

Download and unzip as *ofxKuTextGui* to *addons* folder of your openFrameworks installation.

Addon works in openFrameworks 0.9.3, OSC/Windows/Linux.

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

See example *example-basic* included in addon for creating and using addon manually.

See *example-gui-generate* example which demonstrates auto-generation of GUI from script file.
Such GUI description look as the following:

`PAGE screen`

`int *FPS=30 1:100 1,10`

`dummy`

`int *w=1024 1:2000 1,10`

`int *h=768 1:2000 1,10`

`stringlist enabled=OFF [OFF,ON]


`TAB`

`COLOR 255,140,140

`float -fps=30 0:100 100,10`

`RESET_COLOR`

Then in the code you just link it to gui object:

`PRM setup_gui(gui);`

And for access to parameters use PRM:

`ofSetFrameRate(PRM _FPS_);`

`PRM fps_ = ofGetFrameRate();`


