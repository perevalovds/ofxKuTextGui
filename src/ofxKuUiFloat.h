#pragma once

#include "ofxKuUiComponent.h"

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
		buildTitle();

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

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

	float* floatVarPtr() override { return var; }
	float floatDef() override { return def; }

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

