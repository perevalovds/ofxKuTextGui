#pragma once

#include "ofxKuUiComponent.h"

struct KuUiStringList : public KuUiComponent {
public:
	shared_ptr<int> autovar;
	int* var = nullptr;
	int step[2] = { 0 };
	int def = 0;
	vector<string> titles;

	KuUiStringList() {}
	KuUiStringList(string name0, int& var0, int defV,
		int step1, int step2, vector<string> titles0) {
		type = KuUiType::VStringList;
		name_ = name0;
		buildTitle();

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<int>(new int);
			var = autovar.get();
		}
		step[0] = step1;
		step[1] = step2;
		setValueInt(defV);
		def = defV;
		titles = titles0;
	}

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

	int* intVarPtr() override { return var; }
	int intDef() override { return def; }

	// Can return true if need to be in model mode, and do preparations - for stringlist
	bool start_modal_on_click() override;
	bool is_modal() override;
	void modalKeyPressed(int key) override;
	void modalMousePressed(int x, int y, int button) override;


	void setValue(const string& v) override {
		setValueString(v);
	}
	string value() override {
		if (*var >= 0 && *var <= int(titles.size()) - 1) return titles[*var];
		return "";
	}

	// -1 if s can't be converted into index
	int stringToIndex(const string& s) {
		for (int i = 0; i < titles.size(); i++) {
			if (titles[i] == s) {	// Can be optimized using unordered_map
				return i;
			}
		}
		return -1;
	}
	void setValueString(const string& s) {
		int i = stringToIndex(s);
		if (i >= 0) {
			setValueInt(i);
		}
	}
	void setValueInt(int v) override {
		*var = min(max(v, 0), maxV());
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
		return (maxV() > 0) ? ofMap(*var, 0, maxV(), 0, 1) : 0;
	}

	float normalizedToValue(float normalizedValue) override {
		return ofMap(normalizedValue, 0, 1, 0, maxV());
	}

	int intValue() override {
		return *var;
	}

	float floatValue() override {
		return *var;
	}

	void setTitles(const vector<string>& titles) {
		this->titles = titles;
	}
private:
	bool is_modal_ = false;
	bool modal_inited_ = false;
	ofRectangle modalRect_;	// rectangle for modal selection
	ofRectangle modalRectBackground_;
	float modalItemHeight_ = 1.f;
	float modalTextYValue_ = 0;
	void setupModal(const KuUiDrawData& dd, const KuUiDrawComponentData& dc);
	void stopModal();
	int getModalSelectedIndex(const glm::vec2& mousePos);

	int maxV() { return int(titles.size()) - 1; }
};
