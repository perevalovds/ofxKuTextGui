#pragma once

#include "ofxKuUiComponent.h"

// Button and checkbox
class KuUiButton: public KuUiComponent {
public:
	shared_ptr<int> autovar;
	int* var = nullptr;
	int def = 0;
	int is_checkbox_ = 0;	// checkbox is int, which reads 0,1,OFF,ON for older compatibility
	//Note: Buttons fires only once after pressing, and then set to 0 automatically
	int is_toggled_ = 0;	// setToggled() should be used to manually toggle the button. This not saves to INI

	KuUiButton() {}
	KuUiButton(string name0, int& var0, int defV) {
		type = KuUiType::VInt;
		name_ = name0;
		title_ = name0;
		ofStringReplace(title_, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<int>(new int);
			var = autovar.get();
		}
		setValueInt(defV);
		def = defV;
	}

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

	int* intVarPtr() override { return var; }
	int intDef() override { return def; }

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
		*var = min(max(v, 0), 1);
		setupSmoothValue();
	}
	void inc(int dir, int speed) { //speed=0,1
		setValueInt(*var + dir);
	}
	void reset() {
		setValueInt(def);
	}

	float normalizedToValue(float normalizedValue) override {
		return normalizedValue;
	}

	int intValue() override {
		return *var;
	}

	float floatValue() override {
		return *var;
	}
	
	float valueNormalized() {
		return *var;
	}

	// Mouse click
	bool acceptMouseClick() override { return true; }
	void mouseClick(int x, int y, int button) override {
		if (is_button()) {
			setValue("1");
		}
		if (is_checkbox())
		{
			setValueInt(1 - intValue());
		}
	}

	// button ---------------------
	int button_fired_ = 0;
	float button_time_ = 0;
	float button_alpha_ = 0;

	bool is_button() override { 
		return !is_checkbox_;
	}

	bool is_checkbox() override {
		return is_checkbox_;
	}
	bool is_toggled() override {
		return is_toggled_;
	}

	void setCheckbox() {	// make int as checkbox
		is_checkbox_ = true;
	}

	void setToggled(int t) override {
		is_toggled_ = t;
	}

	void update_button() override {
		if (!is_button()) {
			return;
		}
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

