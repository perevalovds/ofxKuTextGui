#pragma once

//Textual GUI
#include "ofMain.h"
#include <cstdarg>

//Comment it if you compile in oF < 0.9
#define OFXKUTEXTGUI_OF_09

//Checking oF version
#ifndef OFXKUTEXTGUI_OF_09
#define OFXKUTEXTGUI_074
#define ofDrawRectangle ofRect
#define getArgAsInt getArgAsInt32
#endif


struct ofxKuTextGui {
	ofxKuTextGui();

	void loadFromFile(const string &fileName);
	void saveToFile(const string &fileName);
    bool keyPressed(int key);       //generic keyPressed handler
	void update();					//for buttons processing
    void draw(float X, float Y, bool enabled=true, int alpha_text=255, int alpha_slider=255);	//generic draw
    float draw_tabW;	//Distance between tabs
    float draw_yStep;	//Distance between lines
	void set_tab_w(float w, float indentx = 10, float cell_dx = -10.5);
	void set_tab_h(float h, float indenty = 2, float cell_dy = -14.5);

	ofColor dummy_color_ = ofColor(127);
	void set_dummy_color(const ofColor &color) { dummy_color_ = color; }

	//using custom font - if not, using ofDrawBitmapString
	void set_font(ofTrueTypeFont *font, float shift_x, float shift_y);
	ofTrueTypeFont *custom_font_;
	float font_shift_x, font_shift_y;
	void draw_string(const string &s, float x, float y);


	//enable(default) or disable editing string values from keyboard. 
	//We disable it when remotely control GUI, in order not to loose the focus from the app.
	void set_editing_strings(bool v);	
	void set_mouse_enabled(bool v);	//enable/disable mouse control
	void set_mouse_step(int step);	//step for changing values

    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    string toString();
    void setFromString(const string &s);
    
    string drawToString();  //keeps current page, tabs, selected value
    void drawFromString(const string &s, float x, float y); //gui can be empty, just use cellW, cellH params

	int addPage(const string &pageName);    //returns page index
	void addTab();
    
    void setDrawSliderMode(bool value); //should we draw slider
	void set_var_color(const string &var_name, const ofColor &color);	//sets variable color through all pages

    struct Var;
    
	//adding to current page/tab
	Var *addFloat(string name, float &var, float defV, float minV, float maxV,
		int numStepsSlow, int numStepsFast);
	Var *addInt(string name, int &var, int defV, int minV, int maxV,
		int stepSlow, int stepFast);
	Var *addString(string name, string &var, const string &defV);
    Var *addStringList(string name, int &var, int defV, const vector<string> &title);
    Var *addStringList(string name, int &var, int defV, int count...);
	Var *addButton(string name, int &var);
	void addDummy(string title = "");
    

    
	Var *addVar(string name);	//adding existing var
    Var *findVar(const string &name);   //one var
    Var *findVarChecking(const string &name);   //one var, exits if no found
    float *findVarFloat(const string &name);
    int *findVarInt(const string &name);
    int *findVarStringList(const string &name);
    
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
			var = &var0;
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
		int *var;
		int minV, maxV;
		int step[2];
		int def;
		int is_button;		//buttons are just int values, but rendered specially
		VarInt() { var = 0; step[0]=step[1]=0; }
		VarInt(string name0, int &var0, int defV, int minV0, int maxV0,
		int step1, int step2) {
			name = name0;
            title = name;
            var = &var0;
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

		int just_fired_ = 0;
		void update_button() {
			if (button_fired_ && !(just_fired_==0 && *var)) {
				float delta = ofGetElapsedTimef() - button_time_;
				button_alpha_ = ofMap(delta, 0, 0.5, 1, 0, true);
				if (delta >= 0.5) {
					button_fired_ = 0;
				}
				*var = 0;		//here we reset *var!
				just_fired_ = 0;
			}
			else {
				if (*var) {
					button_fired_ = 1;
					button_time_ = ofGetElapsedTimef();
					button_alpha_ = 1;
					just_fired_ = 1;
					//here we keep *var non-zero to process it in the user code!
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
        string *var;
		string def;
		VarString() {}
		VarString(const string &name0, string &var0, const string &defV) {
			name = name0;
            title = name;
			var = &var0;
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
            var = &var0;
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
        static const int VFloat         = 0;
        static const int VInt           = 1;
        static const int VString        = 2;
        static const int VStringList    = 3;
        static const int VDummy         = 4;

		ofColor color;
		void setColor(const ofColor &color0) {
			color = color0;
		}

		bool is_button() {
			return (index == VInt) && vint.is_button;
		}
		void update_button() {
			if (is_button()) vint.update_button();
		}

        void setTitle(const string &title) {
            if (index == VFloat)    vfloat.title = title;
            if (index == VInt)      vint.title = title;
            if (index == VString)   vstring.title = title;
            if (index == VStringList) vstringlist.title = title;
        }
		void setValue( const string &v ) {
			if (index == 0) vfloat.setValue(ofToFloat(v));
			if (index == 1) vint.setValue(ofToInt(v));
			if (index == 2) vstring.setValue(v);
            if (index == 3) vstringlist.setValueString(v);
		}
		void inc(int dir, int speed) {
			if (index == 0) vfloat.inc(dir,speed);
			if (index == 1) vint.inc(dir,speed);
			if (index == 2) editStringValue(); //vstring.inc(dir,speed);
            if (index == 3) vstringlist.inc(dir,speed);
		}
		void editStringValue() {
			if (index == 2) vstring.editStringValue();
		}
		string value() {
			if (index == 0) return (fabs(*vfloat.var)>=0.00001)?ofToString(*vfloat.var):"0";	//crop very small values
			if (index == 1) return ofToString(*vint.var);
			if (index == 2) return *vstring.var;
            if (index == 3) return vstringlist.getValue();
			return "";
		}
        float valueNormalized() {
            if (index == 0) return (vfloat.minV != vfloat.maxV)?ofMap(*vfloat.var, vfloat.minV, vfloat.maxV,0,1):vfloat.minV;
            if (index == 1) return (vint.minV != vint.maxV)?ofMap(*vint.var, vint.minV, vint.maxV,0,1):vint.minV;
            if (index == 3) return (vstringlist.minV != vstringlist.maxV)?ofMap(*vstringlist.var, vstringlist.minV, vstringlist.maxV,0,1):vstringlist.minV;
            
            return 0;
            
        }
        int intValue() {
            if (index == 0) return 0;//int(*vfloat.var);
            if (index == 1) return *vint.var;
            if (index == 2) return 0;
            if (index == 3) return *vstringlist.var;
            return 0;
        }
		string name() {
			if (index == 0) return vfloat.name;
			if (index == 1) return vint.name;
			if (index == 2) return vstring.name;
            if (index == 3) return vstringlist.name;
			return "";
		}
        string title() {
            if (index == 0) return vfloat.title;
            if (index == 1) return vint.title;
            if (index == 2) return vstring.title;
            if (index == 3) return vstringlist.title;
            return "";
        }
		void reset() {
			if (index == 0) vfloat.reset();
			if (index == 1) vint.reset();
			if (index == 2) vstring.reset();
            if (index == 3) vstringlist.reset();
		}
        void setTitles(vector<string> &titles) {
            if (index == 3) vstringlist.titles = titles;
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
	vector<Page> page_;
	int selPage;	//selected page
    vector<string> pageTitles();

	typedef map<string, Var *> StringVarMap;
	StringVarMap vars_;	//index of vars

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

