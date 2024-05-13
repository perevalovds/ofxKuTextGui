#include "ofxKuTextGuiPreset.h"
#include "ofxKuFile.h"


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
		ofxKuTextGui::Var *var = gui.findVar(name);
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

			if (var->vfloat.var) {
				presets.add_float(name, var->vfloat.var, var->vfloat.def);
				//cout << "    added float " << name << endl;
			}
			if (var->vint.var) {
				presets.add_int(name, var->vint.var, var->vint.def);
				//cout << "    added int " << name << endl;
			}
			if (var->vstringlist.var) { 
				presets.add_int(name, var->vstringlist.var, var->vstringlist.def);
				//cout << "    added stringlist " << name << endl;
			}
		}
	}
}

//------------------------------------------------------------------------







