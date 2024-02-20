#pragma once

// Textual GUI
#include "ofMain.h"
#include <cstdarg>

struct ofxKuTextGui {
	ofxKuTextGui();

	void clear();
	void loadFromFile(const string &fileName);  //if fileName=="", then do nothing
	void saveToFile(const string &fileName);

	bool loadFromJSON(const string &s);
	string saveToJSON();

	bool wasModified();		// Currently slow, because comparing JSON serializations

	//Generate names and values separated by separator: x;10;y;20...
	string saveToLine(const string &separator);
	bool loadFromLine(const string &s, const string &separator);


    bool keyPressed(int key);       //generic keyPressed handler
	void update();					//for buttons processing
    void draw(float X, float Y, bool enabled=true, int alpha_text=255, int alpha_slider=255);	//generic draw
    float draw_tabW;	//Distance between tabs
    float draw_yStep;	//Distance between lines
	void set_tab_w(float w, float indentx = 10, float cell_dx = -10.5);
	void set_tab_h(float h, float indenty = 2, float cell_dy = -14.5);

	ofColor dummy_color_ = ofColor(127);
	ofColor dummy_back_ = ofColor(0, 0);
	void set_dummy_color(const ofColor &color) { dummy_color_ = color; }
	void set_dummy_back(const ofColor &color) { dummy_back_ = color; }

	//using custom font - if not, using ofDrawBitmapString
	void set_font(ofTrueTypeFont *font, float shift_x, float shift_y);
	ofTrueTypeFont *custom_font_;
	float font_shift_x, font_shift_y;
	void draw_string(const string &s, float x, float y);


	//Enable(default) or disable editing string values from keyboard
	//This disabling feature t may be useful, because currently editing strings blocks main app loop in oF,
	//and for remote controlled GUI it's undesirable to be blocked
	void set_editing_strings(bool v);	//true by default

	void set_mouse_enabled(bool v);	//enable/disable mouse control, true by default
	void set_mouse_step(int step);	//step for changing values

    bool mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
	bool is_mouse_dragging() { return mouse_dragging_; }
	void mouseReleased();

    string toString();
    void setFromString(const string &s);
    
    string drawToString();  //keeps current page, tabs, selected value
    void drawFromString(const string &s, float x, float y); //gui can be empty, just use cellW, cellH params

	int addPage(const string &pageName);    //returns page index
	void addTab(int n = 1);
    
    void setDrawSliderMode(bool value); //should we draw slider
	void set_var_color(const string &var_name, const ofColor &color);	//sets variable color through all pages
	void set_var_visibility(const string& var_name, bool visible);
	void set_var_editable(const string& var_name, bool editable);
	void set_pagelist_visibility(bool visible);	// allows to hide pages list

    struct Var;
    
	//adding to current page/tab
	//if &var = NULL, then GUI parameter is linked to its own value - useful for dynamic GUI creation
	Var *addFloat(string name, float &var, float defV, float minV, float maxV,
		int numStepsSlow, int numStepsFast);
	Var *addInt(string name, int &var, int defV, int minV, int maxV,
		int stepSlow, int stepFast);
	Var *addString(string name, string &var, const string &defV);
    Var *addStringList(string name, int &var, int defV, const vector<string> &title);
    Var *addStringList(string name, int &var, int defV, int count...);
	Var *addButton(string name, int &var);
	Var* addCheckbox(string name, int& var);
	void addDummy(string title = "", int n=1);
    

    
	Var *addVar(string name);	//adding existing var
    Var *findVar(const string &name);   //one var
    Var *findVarChecking(const string &name);   //one var, exits if no found
    float *findVarFloat(const string &name);
    int *findVarInt(const string &name);
    int *findVarStringList(const string &name);
	string *findVarString(const string &name);
	int *findVarButton(const string &name);
	void setToggled(const string& name, int t);		// Call this to toggle buttons (with duplicates)

	void exit_with_message(const string &message);


	//short access to vars
	float &float_(const string &name);
	int &int_(const string &name);
	int &stringlist_(const string &name);
	string &string_(const string &name);
	int &button_(const string &name);

	//Smoothing vars values
	//Applicable to float and int vars, automatically enables drawing smoothed value
	//Currently it's not in an development stage (not matured), so access by smoothed values is by their string names
	float updateSmoothedValue(const string name, float dt, float time_whole_change);
	float getSmoothedValue(const string name);

    vector<ofxKuTextGui::Var *> findVars(const string &name);   //all instances

	bool setValue(const string &name, const string &value);

	//edit
    void gotoPrevPage();
    void gotoNextPage();
	void gotoPrevTab();
	void gotoNextTab();
	void gotoPrevValue();
	void gotoNextValue();
	void decreaseValue(int speed, int value=1);	//0-slow,1-fast
	void increaseValue(int speed, int value=1);	//0-slow,1-fast
	void editStringValue();

    void setTab(int index);     //select tab
    void setValue(int index);   //select value
    
	void setPage( const string &name ); //switch to page
    void setPage( int index ); //switch to page
    int pageIndex();
    int tabIndex();
    int varIndex();
    string pageTitle();
    bool validPage();   //is current page number valid
    struct Page;
    Page *currentPagePointer();
    
    
	//-------------------------------------------------------------------
	struct VarFloat {
		string name;
        string title;
		shared_ptr<float> autovar;	//used when creating dynamic GUI, and variable stored right here
		float *var;
		float minV, maxV;
        int numSteps1, numSteps2;
        float step[2];
		float def;
		VarFloat() { var = 0; step[0]=step[1]=0; }
		VarFloat(string name0, float &var0, float defV, float minV0, float maxV0,
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
			setValue( defV );
		}
        void update_steps() {
            if (maxV!=minV) {
                step[0] = (maxV-minV)/numSteps1;
                step[1] = (maxV-minV)/numSteps2;
            }
            else {
                step[0] = step[1] = 0;
            }
        }
		void setValue(float v) {
			*var = min(max(v,minV),maxV);
		}
		void inc(int dir, int speed) { //speed=0,1
			speed = min(max(speed,0),1);
			setValue( *var + dir*step[speed] );
		}		
		void reset() {
			setValue(def);
		}
	};
	
	struct VarInt {
		string name;
        string title;
		
		shared_ptr<int> autovar;
		int *var;
		int minV = 0, maxV = 0;
		int step[2];
		int def = 0;
		int is_button = 0;		//buttons are just int values, but rendered specially
		int is_checkbox = 0;	// checkbox is int, which reads 0,1,OFF,ON for older compatibility
		//Note: Buttons fires only once after pressing, and then set to 0 automatically
		int is_toggled_ = 0;	// setToggled() should be used to manually toggle the button. This not saves to INI

		VarInt() { var = 0; step[0]=step[1]=0; }
		VarInt(string name0, int &var0, int defV, int minV0, int maxV0,
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
			setValue( defV );
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
			*var = min(max(v,minV),maxV);
		}
		void inc(int dir, int speed) { //speed=0,1
			speed = min(max(speed,0),1);
			setValue( *var + dir*step[speed] );
		}		
		void reset() {
			setValue(def);
		}
	};
	struct VarString {
		string name;
        string title;
		shared_ptr<string> autovar;
		string *var;
		string def;

		VarString() {}
		VarString(const string &name0, string &var0, const string &defV) {
			name = name0;
            title = name;
			ofStringReplace(title, "_", " ");

			if (&var0) var = &var0;
			else {
				autovar = shared_ptr<string>(new string);
				var = autovar.get();
			}

			def = defV;
			setValue( defV );
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
    struct VarStringList {
        string name;
        string title;
		shared_ptr<int> autovar;
		int *var;
        int minV, maxV;
        int step[2];
        int def;
        vector<string> titles;

        VarStringList() { var = 0; step[0]=step[1]=0; }
        VarStringList(string name0, int &var0, int defV, int minV0, int maxV0,
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
            setValue( defV );
            def = defV;
            titles = titles0;
        }
		void setValueString(const string &s) {
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
            *var = min(max(v,minV),maxV);
        }
        void inc(int dir, int speed) { //speed=0,1
            speed = min(max(speed,0),1);
            setValue( *var + dir*step[speed] );
        }
        void reset() {
            setValue(def);
        }
        string getValue() {
            if (*var >= 0 && *var <= int(titles.size())-1) return titles[*var];
            return "";
        }
    };
	
	struct Var {
		VarFloat vfloat;
		VarInt vint;
		VarString vstring;
        VarStringList vstringlist;
		int index;	//0 - vfloat, 1-vint, 2-string, 3-stringlist, 4-dummy
		bool visible = true;
		bool editable = true;	// Can user edit it

		bool is_editable() const { return visible && editable; }

		//Note: dummy's title stored in vstring.title !!!!
		//So, when we will optimize Vars - check this
        static const int VFloat         = 0;
        static const int VInt           = 1;
        static const int VString        = 2;
        static const int VStringList    = 3;
        static const int VDummy         = 4;

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
		void setColor(const ofColor &color0) {
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


        void setTitle(const string &title) {
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
			if (index == VFloat) vfloat.inc(dir,speed);
			if (index == VInt) vint.inc(dir,speed);
			if (index == VString) editStringValue(); //vstring.inc(dir,speed);
            if (index == VStringList) vstringlist.inc(dir,speed);
		}
		void editStringValue() {
			if (index == VString) vstring.editStringValue();
		}
		string value() {
			if (index == VFloat) return (fabs(*vfloat.var)>=0.00001)?ofToString(*vfloat.var):"0";	//crop very small values
			if (index == VInt) return ofToString(*vint.var);
			if (index == VString) return *vstring.var;
            if (index == VStringList) return vstringlist.getValue();
			return "";
		}
        float valueNormalized() {
            if (index == VFloat) return (vfloat.minV != vfloat.maxV)?ofMap(*vfloat.var, vfloat.minV, vfloat.maxV,0,1):vfloat.minV;
            if (index == VInt) return (vint.minV != vint.maxV)?ofMap(*vint.var, vint.minV, vint.maxV,0,1):vint.minV;
            if (index == VStringList) return (vstringlist.minV != vstringlist.maxV)?ofMap(*vstringlist.var, vstringlist.minV, vstringlist.maxV,0,1):vstringlist.minV;
            
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
        void setTitles(vector<string> &titles) {
            if (index == VStringList) vstringlist.titles = titles;
        }
	};

	struct Tab {
		vector<Var> var;
		int selVar;
		Tab() {
			selVar = 0;
		}
		bool validVar() { return selVar >= 0 && selVar < var.size(); }
	};

	struct Page {
		string name;
		vector<Tab> tab;
		int selTab;

		Page() {
			selTab = 0;
		}
		void addTab() {
			tab.push_back(Tab());
		}
		Var *addVar( Var &var ) {
			if ( tab.empty() ) addTab();
			vector<Var> &vars = tab[tab.size()-1].var;
			vars.push_back(var);
			return &vars[vars.size()-1];
		}
		bool validTab() { return selTab >= 0 && selTab < tab.size(); }
	};

	vector<Var *> getVars();    //without repetitions
    vector<Var *> getPageVars();
    
    //-------------------------------------------------------------------


protected:
	void storeState();		// for "wasModified"
	string stateJson_;

	const string PageVarName = "[page]";

	vector<Page> page_;
	int selPage;	//selected page
    vector<string> pageTitles();

	typedef map<string, Var *> StringVarMap;
	StringVarMap vars_;	//index of vars - sorted, for saving to file

	typedef unordered_map<string, vector<Var *> > StringVarHash;
	StringVarHash hash_vars_;	//index of vars - unsorted, multiple, for search and toggle buttons

	Var *addVar(Var &var);

	void rebuildVars();
	bool needRebuild_;
    float cellW, cellH, cellDx, cellDy;
    
    bool drawSliderMode_;    
	bool mouse_enabled_;    
	bool editing_strings_;

	float drawn_x_;
	float drawn_y_;
	ofRectangle drawn_cell_rect(int tab, int i);
	void get_cell_by_coords(float x, float y, int &tab, int &i);


    bool mouse_dragging_;
    int mouse_x_,mouse_y_;
	int mouse_step_;
	void mouse_reset();

};

