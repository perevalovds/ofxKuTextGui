# ofxKuTextGui - openFrameworks addon for creating textual GUI controlled by keyboard and mouse

It supports integers, floats, strings, stringlists (represented as integer, with string output in GUI) and buttons. 

Values can be edited using keyboard or mouse, and stored in a file.

Supported smoothing values (read details below and see example of it at  **example-app-template**).

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

1. **example-app-template2** - is the most up-to-date example.

2. **example-basic** demonstrates creating GUI manually: you need to define variables and then link them with GUI.

3. **example-gui-generate** demonstrates generation of H/CPP files for constructing GUI from a GUI script file (see below).
By pressing Shift+G it creates source files, which contain GUI creation and also variables definition.

Note: variables names started with "*" means constants, such as *FPS, and defined in C++ code as `PRM FPS` - constant, `PRM _FPS_` - its current value in GUI.
Variables names started with "-" means output values, such as -fps, and defined in C++ code as `PRM fps_`.


4. **example-gui-dyncreate** shows how to create GUI during runtime from a script file.
In this case, GUI stores values itself, and to get access to value, use the following functions:

`gui.int_()`, `gui.float_()`, `gui_.stringlist_()`, `gui.string_()`, `gui.button_()`, for example:

`gui.int_("backR")`, `gui.float_("-fps")`

5. **example-app-template** example is a template for creating full-fledge applications. Just use App class for performing your custom actions.

Also it shows how to use feature of smoothing values (see technical details below).

 

## Technical details

Please take care on the following notes:

* 'stringlist' is int value.

* 'button' is int value. Button fires only once after pressing, and then set to 0 automatically.

## GUI script file

A GUI script file looks as the following:

`PAGE Screen`

`int *FPS=30 1:100 1,10`

`dummy Screen size:`

`int *w=1024 1:2000 1,10`

`int *h=768 1:2000 1,10`

`stringlist enabled=OFF [OFF,ON]

`button Press_Me`

`checkbox Toggle_Me=0`

`TAB`

`COLOR 255,140,140`

`float -fps=30 0:100 100,10`

`RESET_COLOR`

### Conditional UI

`# "IF" may contain integers (int, checkbox) and stringlist`

`# "IF"s can be nested`

`stringlist option1=UseA [UseA,UseB,UseC]`

`IF option1==UseA,UseB`

`checkbox Enable_AB=0`

`ENDIF`

`IF option1==UseC`

`checkbox Enable_C=0`

`ENDIF`

### Templates

`TEMPLATE ITEM X Y`

`dummy {X}{Y}`

`int Pos{X}{Y}=0 1:10 1,10`

`ENDTEMPLATE`

`ITEM 1 1`

`ITEM 1 2`

`ITEM 2 1`

`ITEM 2 1`


# GUI C++ Usage
Then in the code you just link it to gui object:

`PRM setup_gui(gui);`

And for access to parameters use PRM:

`ofSetFrameRate(PRM _FPS_);`

`PRM fps_ = ofGetFrameRate();`

Access to page:

`if (PRM pageEnum() == Parameters::PageEnum::Settings) {`

`        ofBackground(100, 60, 60);`

`    }`

`    else {`

`        ofBackground(60, 100, 60);`

`    }`



Note about constants: you can get access to changed valued of constants by adding "_" at the beginning and end.
For example, for '*FPS' value, PRM FPS means constant value, not changed after program start, and PRM _FPS_ is value,
possible changed by user.

## Smoothing values

Float and int values can be smoothed, so they change that smooth value with given period of time for whole range.

Smoothed values drawn automatically as think lines at the bottom of value's widget.

Code for smoothing currently binded to string names of variables:
```
//Smoothing values
float time_smooth = PRM time_smoothing;
float smoothed1 = gui.updateSmoothedValue("smooth_float", dt, time_smooth);
float smoothed2 = gui.updateSmoothedValue("smooth_int", dt, time_smooth);

//Access to a smoothed value
cout << "smooth_float smoothed = " << gui.getSmoothedValue("smooth_float") << endl;
```

The code above is a part of **example-app-template** example, so see it code for the more details.

