#pragma once

// Textual GUI
#include "ofMain.h"
#include <cstdarg>


struct KuUiFloat {
	string name;
	string title;
	shared_ptr<float> autovar;	//used when creating dynamic GUI, and variable stored right here
	float* var;
	float minV, maxV;
	int numSteps1, numSteps2;
	float step[2];
	float def;
	KuUiFloat() { var = 0; step[0] = step[1] = 0; }
	KuUiFloat(string name0, float& var0, float defV, float minV0, float maxV0,
		int numSteps1_0, int numSteps2_0) {
		name = name0;
		title = name;
		ofStringReplace(title, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<float>(new float);
			var = autovar.get();
		}
		minV = minV0;
		maxV = maxV0;
		numSteps1 = numSteps1_0;
		numSteps2 = numSteps2_0;
		update_steps();
		def = defV;
		setValue(defV);
	}
	void update_steps() {
		if (maxV != minV) {
			step[0] = (maxV - minV) / numSteps1;
			step[1] = (maxV - minV) / numSteps2;
		}
		else {
			step[0] = step[1] = 0;
		}
	}
	void setValue(float v) {
		*var = min(max(v, minV), maxV);
	}
	void inc(int dir, int speed) { //speed=0,1
		speed = min(max(speed, 0), 1);
		setValue(*var + dir * step[speed]);
	}
	void reset() {
		setValue(def);
	}
};

struct KuUiInt {
	string name;
	string title;

	shared_ptr<int> autovar;
	int* var;
	int minV = 0, maxV = 0;
	int step[2];
	int def = 0;
	int is_button = 0;		//buttons are just int values, but rendered specially
	int is_checkbox = 0;	// checkbox is int, which reads 0,1,OFF,ON for older compatibility
	//Note: Buttons fires only once after pressing, and then set to 0 automatically
	int is_toggled_ = 0;	// setToggled() should be used to manually toggle the button. This not saves to INI

	KuUiInt() { var = 0; step[0] = step[1] = 0; }
	KuUiInt(string name0, int& var0, int defV, int minV0, int maxV0,
		int step1, int step2) {
		name = name0;
		title = name;
		ofStringReplace(title, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<int>(new int);
			var = autovar.get();
		}
		step[0] = step1;
		step[1] = step2;
		minV = minV0;
		maxV = maxV0;
		setValue(defV);
		def = defV;
		is_button = 0;
	}

	//button ---------------------
	int button_fired_ = 0;
	float button_time_ = 0;
	float button_alpha_ = 0;

	void setButton(int b) {	//make int rendered as button
		is_button = b;
	}

	void setCheckbox() {	// make int as checkbox
		is_checkbox = true;
	}

	void setToggled(int t) {
		is_toggled_ = t;
	}

	void update_button() {
		// It's expected gui.update is the last line of the code,
		// so we cleaning *var here
		if (*var) {
			button_fired_ = 1;
			button_time_ = ofGetElapsedTimef();
			button_alpha_ = 1;
			*var = 0; // reset *var
		}
		if (button_fired_) {
			float delta = ofGetElapsedTimef() - button_time_;
			const float ButtonFireTimeSec = 0.5f;
			button_alpha_ = ofMap(delta, 0, ButtonFireTimeSec, 1, 0, true);
			if (delta >= ButtonFireTimeSec) {
				button_fired_ = 0;
			}
		}
	}

	//----------------------------
	void setValue(int v) {
		*var = min(max(v, minV), maxV);
	}
	void inc(int dir, int speed) { //speed=0,1
		speed = min(max(speed, 0), 1);
		setValue(*var + dir * step[speed]);
	}
	void reset() {
		setValue(def);
	}
};

struct KuUiString {
	string name;
	string title;
	shared_ptr<string> autovar;
	string* var;
	string def;

	KuUiString() {}
	KuUiString(const string& name0, string& var0, const string& defV) {
		name = name0;
		title = name;
		ofStringReplace(title, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<string>(new string);
			var = autovar.get();
		}

		def = defV;
		setValue(defV);
	}
	void setValue(string v) {
		*var = v;
	}
	void inc(int dir, int speed) { //speed=0,1
	}
	void reset() {
		setValue(def);
	}
	void editStringValue() {
		*var = ofSystemTextBoxDialog(name, *var);
	}
};

struct KuUiStringList {
	string name;
	string title;
	shared_ptr<int> autovar;
	int* var;
	int minV, maxV;
	int step[2];
	int def;
	vector<string> titles;

	KuUiStringList() { var = 0; step[0] = step[1] = 0; }
	KuUiStringList(string name0, int& var0, int defV, int minV0, int maxV0,
		int step1, int step2, vector<string> titles0) {
		name = name0;
		title = name;
		ofStringReplace(title, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<int>(new int);
			var = autovar.get();
		}
		step[0] = step1;
		step[1] = step2;
		minV = minV0;
		maxV = maxV0;
		setValue(defV);
		def = defV;
		titles = titles0;
	}
	void setValueString(const string& s) {
		//if (!s.empty()) {
			//if (s[0]>='0' && s[0]<='9') {  //TODO it's so easy check for is-number
			//    setValue(ofToInt(s));
			//    return;
			//}
		for (int i = 0; i < titles.size(); i++) {
			if (titles[i] == s) {
				setValue(i);
				return;
			}
		}
	}
	void setValue(int v) {
		*var = min(max(v, minV), maxV);
	}
	void inc(int dir, int speed) { //speed=0,1
		speed = min(max(speed, 0), 1);
		setValue(*var + dir * step[speed]);
	}
	void reset() {
		setValue(def);
	}
	string getValue() {
		if (*var >= 0 && *var <= int(titles.size()) - 1) return titles[*var];
		return "";
	}
};

struct KuUiComponent {
	KuUiFloat vfloat;
	KuUiInt vint;
	KuUiString vstring;
	KuUiStringList vstringlist;
	int index;	//0 - vfloat, 1-vint, 2-string, 3-stringlist, 4-dummy
	bool visible = true;
	bool editable = true;	// Can user edit it
	bool marked = false;	// Special mark, for example, to show the variable controlled from presets

	bool is_editable() const { return visible && editable; }

	//Note: dummy's title stored in vstring.title !!!!
	//So, when we will optimize Vars - check this
	static const int VFloat = 0;
	static const int VInt = 1;
	static const int VString = 2;
	static const int VStringList = 3;
	static const int VDummy = 4;

	bool need_save() { //we save only not-dummy, not-button, not-output vars (name doesn't start with "-")
		return (index != VDummy && !is_button() && (title().empty() || title()[0] != '-'));
	}

	bool is_string() {
		return (index == VString);
	}
	bool is_button() {
		return (index == VInt) && vint.is_button;
	}
	bool is_checkbox() {
		return (index == VInt) && vint.is_checkbox;
	}

	ofColor color;
	void setColor(const ofColor& color0) {
		color = color0;
	}

	void update_button() {
		if (is_button()) vint.update_button();
	}


	//Smoothing values 
	//applicable to float and int vars
	float smoothed_value_normalized_ = 0;
	float smoothed_value_ = 0;
	bool draw_smoothed_value_ = false;
	//smoothing function
	void updateSmoothedValue(float dt, float time_whole_change) {
		float v = valueNormalized();
		if (time_whole_change <= 0.00001) {
			//momentary change
			smoothed_value_normalized_ = v;
		}
		else {
			//smooth change
			float limit = dt / time_whole_change;
			smoothed_value_normalized_ += ofClamp(v - smoothed_value_normalized_, -limit, limit);
		}
		smoothed_value_ = normalizedToValue(smoothed_value_normalized_);
	}
	//get smoother value
	float getSmoothedValue() {
		return smoothed_value_;
	}

	//should we draw smoothed value
	void setDrawSmoothed(bool v) {
		draw_smoothed_value_ = v;
	}


	void setTitle(const string& title) {
		if (index == VFloat)    vfloat.title = title;
		if (index == VInt)      vint.title = title;
		if (index == VString)   vstring.title = title;
		if (index == VStringList) vstringlist.title = title;
	}

	void setToggled(int t) {
		if (index == VInt) vint.setToggled(t);
	}

	void setValueInt(int v) {
		if (index == VInt) {
			vint.setValue(v);
		}
		else if (index == VStringList) {
			vstringlist.setValue(v);
		}
	}
	void setValue(const string& v) {
		if (index == VFloat) vfloat.setValue(ofToFloat(v));
		if (index == VInt) {
			if (is_checkbox()) {
				vint.setValue((v == "1" || v == "ON") ? 1 : 0);	// Compatibility with old "OFF/ON" checkboxes
			}
			else {
				vint.setValue(ofToInt(v));
			}
		}
		if (index == VString) vstring.setValue(v);
		if (index == VStringList) vstringlist.setValueString(v);

		//TODO this applied to all vars, even not smoothed - performance is not optimal
		smoothed_value_normalized_ = valueNormalized();
		smoothed_value_ = floatValue();
	}
	void inc(int dir, int speed) {
		if (index == VFloat) vfloat.inc(dir, speed);
		if (index == VInt) vint.inc(dir, speed);
		if (index == VString) editStringValue(); //vstring.inc(dir,speed);
		if (index == VStringList) vstringlist.inc(dir, speed);
	}
	void editStringValue() {
		if (index == VString) vstring.editStringValue();
	}
	string value() {
		if (index == VFloat) return (fabs(*vfloat.var) >= 0.00001) ? ofToString(*vfloat.var) : "0";	//crop very small values
		if (index == VInt) return ofToString(*vint.var);
		if (index == VString) return *vstring.var;
		if (index == VStringList) return vstringlist.getValue();
		return "";
	}
	float valueNormalized() {
		if (index == VFloat) return (vfloat.minV != vfloat.maxV) ? ofMap(*vfloat.var, vfloat.minV, vfloat.maxV, 0, 1) : vfloat.minV;
		if (index == VInt) return (vint.minV != vint.maxV) ? ofMap(*vint.var, vint.minV, vint.maxV, 0, 1) : vint.minV;
		if (index == VStringList) return (vstringlist.minV != vstringlist.maxV) ? ofMap(*vstringlist.var, vstringlist.minV, vstringlist.maxV, 0, 1) : vstringlist.minV;

		return 0;
	}
	float normalizedToValue(float normalizedValue) {
		if (index == VFloat) return ofMap(normalizedValue, 0, 1, vfloat.minV, vfloat.maxV);
		if (index == VInt) return ofMap(normalizedValue, 0, 1, vint.minV, vint.maxV);
		if (index == VStringList) return ofMap(normalizedValue, 0, 1, vstringlist.minV, vstringlist.maxV);

		return 0;
	}

	int intValue() {
		if (index == VFloat) return 0;//int(*vfloat.var);
		if (index == VInt) return *vint.var;
		if (index == VString) return 0;
		if (index == VStringList) return *vstringlist.var;
		return 0;
	}
	float floatValue() {
		if (index == VFloat) return *vfloat.var;
		if (index == VInt) return *vint.var;
		if (index == VString) return 0;
		if (index == VStringList) return *vstringlist.var;
		return 0;
	}
	string name() {
		if (index == VFloat) return vfloat.name;
		if (index == VInt) return vint.name;
		if (index == VString) return vstring.name;
		if (index == VStringList) return vstringlist.name;
		return "";
	}
	string title() {
		if (index == VFloat) return vfloat.title;
		if (index == VInt) return vint.title;
		if (index == VString) return vstring.title;
		if (index == VStringList) return vstringlist.title;
		return "";
	}
	void reset() {
		if (index == VFloat) vfloat.reset();
		if (index == VInt) vint.reset();
		if (index == VString) vstring.reset();
		if (index == VStringList) vstringlist.reset();
	}
	void setTitles(vector<string>& titles) {
		if (index == VStringList) vstringlist.titles = titles;
	}
};

struct KuUiTab {
	vector<KuUiComponent> var;
	int selVar;
	KuUiTab() {
		selVar = 0;
	}
	bool validVar() { return selVar >= 0 && selVar < var.size(); }
};

struct KuUiPage {
	string name;
	vector<KuUiTab> tab;
	int selTab;

	KuUiPage() {
		selTab = 0;
	}
	void addTab() {
		tab.push_back(KuUiTab());
	}
	KuUiComponent* addVar(KuUiComponent& var) {
		if (tab.empty()) addTab();
		vector<KuUiComponent>& vars = tab[tab.size() - 1].var;
		vars.push_back(var);
		return &vars[vars.size() - 1];
	}
	bool validTab() { return selTab >= 0 && selTab < tab.size(); }
};
