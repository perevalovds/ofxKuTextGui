#pragma once

#include "ofxKuUiComponent.h"

class KuUiInt: public KuUiComponent {
public:
	shared_ptr<int> autovar;
	int* var = nullptr;
	int minV = 0, maxV = 0;
	int step[2] = { 0 };
	int def = 0;

	KuUiInt() {}
	KuUiInt(string name0, int& var0, int defV, int minV0, int maxV0,
		int step1, int step2) {
		type = KuUiType::VInt;
		name_ = name0;
		buildTitle();

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

};

