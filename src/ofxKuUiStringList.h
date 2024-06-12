#pragma once

#include "ofxKuUiComponent.h"

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

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

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
