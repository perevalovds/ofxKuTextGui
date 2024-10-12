#include "ofxKuTextGuiPreset.h"
#include "ofxKuFile.h"
#include "ofxKuUiComponent.h"

//------------------------------------------------------------------------
//function loads to preset system variables listed in file_var_list
void ofxKuPreset_load_vars_to_preset_system(string file_var_list, ofxKuTextGui &gui, ofxKuPreset &presets, 
	const ofxKuPresetLoaderParams& params) {
	cout << "Loading vars to preset system from " << file_var_list << endl;
	vector<string> vars = ofxKuFileReadStrings(file_var_list);
	for (int i=0; i<vars.size(); i++) {
		string name = vars[i];
		ofStringReplace(name, "\t", "");
		ofStringReplace(name, " ", "");
		ofStringReplace(name, "\r", "");
		ofStringReplace(name, "\n", "");
		if (name.empty()) continue;		//skip empty name
		if (name[0] == '#') continue;	//skip comment
		KuUiComponent* var = gui.findVar(name);
		if (!var) {
			string message = "Error while ofxKuPreset loading " + file_var_list + ": No variable " + name;
			cout << message << endl;
			ofSystemAlertDialog(message);
		}
		else {
			// Mark preset variables
			if (params.markVars) {
				gui.set_var_mark(name, true);
			}

			if (var->floatVarPtr()) {
				presets.add_float(name, var->floatVarPtr(), var->floatDef());
				//cout << "    added float " << name << endl;
			}
			else if (var->intVarPtr()) {
				presets.add_int(name, var->intVarPtr(), var->intDef());
				//cout << "    added int " << name << endl;
			}
			else if (var->stringVarPtr()) {
				presets.add_string(name, var->stringVarPtr(), var->stringDef());
			}
			else {
				string message = "Can't add variable '" + name + "' to presets, ofxKuPreset_load_vars_to_preset_system, " + file_var_list;
				cout << message << endl;
				ofSystemAlertDialog(message);

			}
		}
	}
}

//------------------------------------------------------------------------







