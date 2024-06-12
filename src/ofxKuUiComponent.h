#pragma once

// Base UI Component Class
#include "ofMain.h"
#include <cstdarg>

void KuUiExitWithMessage(const string& message);

enum class KuUiType : int {
	VUndefined = -1,
	VFloat = 0,
	VInt = 1,
	VStringList = 2,
	VString = 3,
	VDummy = 4
};

// General draw parameters
struct KuUiDrawData {
	bool drawSliderMode = true;

	bool enabled = true;
	int alpha_text = 255;
	int alpha_slider = 255;

	float alpha_text_f = 0;

	ofColor dummy_color = ofColor(127);
	ofColor dummy_back = ofColor(0, 0);

	ofTrueTypeFont* custom_font = nullptr;
	float font_shift_x = 0;
	float font_shift_y = 0;
};

// Parameters for drawing particular element
struct KuUiDrawComponentData {
	bool selected = false;

	float w = 0;
	float h = 0;

	float x = 0;
	float y = 0;
	float x0 = 0;
	float y0 = 0;
};

class KuUiComponent {
public:
	static void draw_string(const KuUiDrawData& dd, const string& s, float x, float y);

	string name_;
	string title_;

	KuUiType type = KuUiType::VUndefined;
	bool visible = true;
	bool editable = true;	// Can user edit it
	bool marked = false;	// Special mark, for example, to show the variable controlled from presets

	virtual void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc);

	bool is_editable() const { return visible && editable; }

	//Note: dummy's title stored in vstring.title !!!!
	//So, when we will optimize Vars - check this

	bool need_save() { //we save only not-dummy, not-button, not-output vars (name doesn't start with "-")
		return (type != KuUiType::VDummy && !is_button() && (title().empty() || title()[0] != '-'));
	}

	virtual bool is_string() { return false; }
	virtual bool is_button() { return false; }
	virtual bool is_checkbox() { return false; }
	virtual bool is_toggled() { return false; }

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

	void setupSmoothValue();
	//smoothing function
	void updateSmoothedValue(float dt, float time_whole_change);
	//get smoothed value
	float getSmoothedValue();
	//should we draw smoothed value
	void setDrawSmoothed(bool v);

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
