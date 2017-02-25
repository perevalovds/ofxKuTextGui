# ofxKuTextGui - openFrameworks addon for creating textual GUI controlled by keyboard

It supports integer, float, string values (string are read-only), and string list (represented as integer, with string output in GUI). 

Values can be edited using keyboard, and stored in a file.

[![Video](http://img.youtube.com/vi/jVFbEZ22iPc/0.jpg)](http://www.youtube.com/watch?v=jVFbEZ22iPc)

##Installation

Download and unzip as *ofxKuTextGui* to *addons* folder of your openFrameworks installation.

Addon works in openFrameworks 0.9.3, OSC/Windows/Linux.

##Usage

See example *example-basic* included in addon.

##Automatic generating .H and .CPP files for GUI

*example-gui-generate* shows how to use GUI description stored in text file
 for automatically generation .H and .CPP files containing this GUI.

For automatic generation, you are creating textual description of GUI in a separate file:

‘PAGE screen’
‘int *FPS=30 1:100 1,10’
‘int *w=1024 1:2000 1,10’
‘int *h=768 1:2000 1,10’
‘TAB’
‘float -fps=30 0:100 100,10’

Then in the code you just link it to gui object:

‘PRM setup_gui(gui);’

And for access to parameters use PRM:
‘ofSetFrameRate(PRM _FPS_);’
‘PRM fps_ = ofGetFrameRate();’


