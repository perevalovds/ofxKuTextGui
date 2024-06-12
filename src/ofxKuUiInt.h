#pragma once

#include "ofxKuUiComponent.h"


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

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

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
	bool is_toggled() override {
		return is_toggled_;
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

