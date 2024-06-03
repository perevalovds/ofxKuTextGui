#pragma once

// Textual GUI
#include "ofMain.h"
#include <cstdarg>


enum class KuUiType : int {
	VUndefined = -1,
	VFloat = 0,
	VInt = 1,
	VString = 2,
	VStringList = 3,
	VDummy = 4
};

class KuUiComponent {
public:
	string name_;
	string title_;

	KuUiType type = KuUiType::VUndefined;
	bool visible = true;
	bool editable = true;	// Can user edit it
	bool marked = false;	// Special mark, for example, to show the variable controlled from presets

	bool is_editable() const { return visible && editable; }

	//Note: dummy's title stored in vstring.title !!!!
	//So, when we will optimize Vars - check this

	bool need_save() { //we save only not-dummy, not-button, not-output vars (name doesn't start with "-")
		return (type != KuUiType::VDummy && !is_button() && (title().empty() || title()[0] != '-'));
	}

	virtual bool is_string() { return false; }
	bool is_button() { return false; }
	bool is_checkbox() { return false; }

	ofColor color;
	void setColor(const ofColor& color0) {
		color = color0;
	}

	virtual void update_button() {}

	void setTitle(const string& title) { title_ = title; }

	virtual void setToggled(int t) {}
	virtual void setValueInt(int v) {}
	virtual void setValueFloat(float v) {}
	virtual void setValue(const string& v) {}
	virtual string value() { return ""; }
	virtual int intValue() { return 0; }
	virtual float floatValue() { return 0.f; }
	virtual float valueNormalized() { return 0.f; }
	virtual float normalizedToValue(float normalizedValue) { return 0.f; }

	virtual void inc(int dir, int speed) {}
	virtual void editStringValue() {}
	string name() { return name_; }
	string title() { return title_; }
	virtual void reset() {}

	virtual void setTitles(vector<string>& titles) {};

	//Smoothing values 
	//applicable to float and int vars
	float smoothed_value_normalized_ = 0;
	float smoothed_value_ = 0;
	bool draw_smoothed_value_ = false;

	void setupSmoothValue() {
		smoothed_value_normalized_ = valueNormalized();
		smoothed_value_ = floatValue();
	}
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
};

//------------------------------------------------------------------------
class KuUiFloat: public KuUiComponent {
public:
	shared_ptr<float> autovar;	//used when creating dynamic GUI, and variable stored right here
	float* var = nullptr;

	float minV = 0;
	float maxV = 0;
	int numSteps1 = 0;
	int numSteps2 = 0;
	float step[2] = { 0 };
	float def = 0;

	KuUiFloat() {}
	KuUiFloat(string name0, float& var0, float defV, float minV0, float maxV0,
		int numSteps1_0, int numSteps2_0) {
		type = KuUiType::VFloat;
		name_ = name0;
		title_ = name0;
		ofStringReplace(title_, "_", " ");

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
		setValueFloat(defV);
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

	void setValue(const string& value) override {
		setValueFloat(ofToFloat(value));
	}
	string value() override {
		return ofToString(*var);
	}
	void setValueFloat(float v) override {
		*var = min(max(v, minV), maxV);
		setupSmoothValue();
	}
	void inc(int dir, int speed) override { //speed=0,1
		speed = min(max(speed, 0), 1);
		setValueFloat(*var + dir * step[speed]);
	}
	void reset() override {
		setValueFloat(def);
	}

	float valueNormalized() override {
		return (minV != maxV) ? ofMap(*var, minV, maxV, 0, 1) : minV;
	}
	float normalizedToValue(float normalizedValue) override {
		return ofMap(normalizedValue, 0, 1, minV, maxV);
	}
	float floatValue() override {
		return *var;
	}
};

//------------------------------------------------------------------------
class KuUiInt: public KuUiComponent {
public:
	shared_ptr<int> autovar;
	int* var = nullptr;
	int minV = 0, maxV = 0;
	int step[2] = { 0 };
	int def = 0;
	int is_button_ = 0;		//buttons are just int values, but rendered specially
	int is_checkbox_ = 0;	// checkbox is int, which reads 0,1,OFF,ON for older compatibility
	//Note: Buttons fires only once after pressing, and then set to 0 automatically
	int is_toggled_ = 0;	// setToggled() should be used to manually toggle the button. This not saves to INI

	KuUiInt() {}
	KuUiInt(string name0, int& var0, int defV, int minV0, int maxV0,
		int step1, int step2) {
		type = KuUiType::VInt;
		name_ = name0;
		title_ = name0;
		ofStringReplace(title_, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<int>(new int);
			var = autovar.get();
		}
		step[0] = step1;
		step[1] = step2;
		minV = minV0;
		maxV = maxV0;
		setValueInt(defV);
		def = defV;
		is_button_ = 0;
	}

	void setValue(const string& value) override {
		if (is_checkbox()) {
			setValueInt((value == "1" || value == "ON") ? 1 : 0);	// Compatibility with old "OFF/ON" checkboxes
		}
		else {
			setValueInt(ofToInt(value));
		}
	}
	string value() override {
		return ofToString(*var);
	}

	void setValueInt(int v) override {
		*var = min(max(v, minV), maxV);
		setupSmoothValue();
	}
	void inc(int dir, int speed) { //speed=0,1
		speed = min(max(speed, 0), 1);
		setValueInt(*var + dir * step[speed]);
	}
	void reset() {
		setValueInt(def);
	}

	float normalizedToValue(float normalizedValue) override {
		return ofMap(normalizedValue, 0, 1, minV, maxV);
	}

	int intValue() override {
		return *var;
	}

	float floatValue() override {
		return *var;
	}
	
	float valueNormalized() {
		return (minV != maxV) ? ofMap(*var, minV, maxV, 0, 1) : minV;
	}

	// button ---------------------
	int button_fired_ = 0;
	float button_time_ = 0;
	float button_alpha_ = 0;

	bool is_button() {
		return is_button_;
	}
	bool is_checkbox() {
		return is_checkbox_;
	}
	void setButton(int b) {	//make int rendered as button
		is_button_ = b;
	}

	void setCheckbox() {	// make int as checkbox
		is_checkbox_ = true;
	}

	void setToggled(int t) override {
		is_toggled_ = t;
	}

	void update_button() override {
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
};


//------------------------------------------------------------------------
struct KuUiStringList : public KuUiComponent {
public:
	shared_ptr<int> autovar;
	int* var = nullptr;
	int minV = 0;
	int maxV = 0;
	int step[2] = { 0 };
	int def = 0;
	vector<string> titles;

	KuUiStringList() {}
	KuUiStringList(string name0, int& var0, int defV, int minV0, int maxV0,
		int step1, int step2, vector<string> titles0) {
		type = KuUiType::VStringList;
		name_ = name0;
		title_ = name0;
		ofStringReplace(title_, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<int>(new int);
			var = autovar.get();
		}
		step[0] = step1;
		step[1] = step2;
		minV = minV0;
		maxV = maxV0;
		setValueInt(defV);
		def = defV;
		titles = titles0;
	}

	void setValue(const string& v) override {
		setValueString(v);
	}
	string value() override {
		if (*var >= 0 && *var <= int(titles.size()) - 1) return titles[*var];
		return "";
	}
	void setValueString(const string& s) {
		//if (!s.empty()) {
			//if (s[0]>='0' && s[0]<='9') {  //TODO it's so easy check for is-number
			//    setValue(ofToInt(s));
			//    return;
			//}
		for (int i = 0; i < titles.size(); i++) {
			if (titles[i] == s) {
				setValueInt(i);
				return;
			}
		}
	}
	void setValueInt(int v) override {
		*var = min(max(v, minV), maxV);
		setupSmoothValue();
	}
	void inc(int dir, int speed) override { //speed=0,1
		speed = min(max(speed, 0), 1);
		setValueInt(*var + dir * step[speed]);
	}
	void reset() override {
		setValueInt(def);
	}
	
	float valueNormalized() {
		return (minV != maxV) ? ofMap(*var, minV, maxV, 0, 1) : minV;
	}

	float normalizedToValue(float normalizedValue) override {
		return ofMap(normalizedValue, 0, 1, minV, maxV);
	}

	int intValue() override {
		return *var;
	}

	float floatValue() override {
		return *var;
	}

	void setTitles(vector<string>& titles) {
		this->titles = titles;
	}

};

//------------------------------------------------------------------------
class KuUiString: public KuUiComponent {
public:
	string name;
	string title;
	shared_ptr<string> autovar;
	string* var = nullptr;
	string def;

	KuUiString() {}
	KuUiString(const string& name0, string& var0, const string& defV) {
		type = KuUiType::VString;
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
	void setValue(const string& v) override {
		*var = v;
	}

	string value() override {
		return *var;
	}

	void inc(int dir, int speed) override { //speed=0,1
		editStringValue();
	}
	void reset() override {
		setValue(def);
	}

	void editStringValue() {
		*var = ofSystemTextBoxDialog(name, *var);
	}
	bool is_string() override {
		return true;
	}
};

//------------------------------------------------------------------------
struct KuUiTab {
	vector<KuUiComponent*> var;
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
	KuUiComponent* addVar(KuUiComponent* var) {
		if (tab.empty()) addTab();
		vector<KuUiComponent*>& vars = tab[tab.size() - 1].var;
		vars.push_back(var);
		return vars[vars.size() - 1];
	}
	bool validTab() { return selTab >= 0 && selTab < tab.size(); }
};
