#pragma once

/*
 Optional module for integrating with presets.
 Requires ofxKuPreset and ofxKuFile modules (part of ofxKu addon).
*/

#include "ofMain.h"
#include "ofxKuTextGui.h"
#include "ofxKuPreset.h"


struct ofxKuPresetLoaderParams {
	bool markVars = true;
};

//function loads to preset system variables listed in file_var_list
void ofxKuPreset_load_vars_to_preset_system(string file_var_list, ofxKuTextGui &gui, ofxKuPreset &presets, 
	const ofxKuPresetLoaderParams& params);









