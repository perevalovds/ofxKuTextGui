#include "ofxKuTextGui.h"


//------------------------------------------------------------------------
ofxKuTextGui::ofxKuTextGui() {

	set_tab_w(180, 20);
	set_tab_h(35, 10, -18);

	//set_tab_w(140, 10, -10.5);
	//set_tab_h(20, 2, -14.5);

    
    drawSliderMode_ = true;

	editing_strings_ = true;
	mouse_enabled_ = true;

	drawn_x_ = -10000;
	drawn_y_ = -10000;
	mouse_step_ = 10; //15; //5;
	mouse_dragging_ = false;

	set_font(0, 0, 0);

	clear();
}

//------------------------------------------------------------------------
void ofxKuTextGui::clear() {
	selPage = 0;
	needRebuild_ = true;
	mouse_reset();

	page_.clear();
	vars_.clear();
}


//------------------------------------------------------------------------
void ofxKuTextGui::set_tab_w(float w, float indentx, float cell_dx) {
	cellW = w;
	draw_tabW = cellW + indentx;
	cellDx = cell_dx;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_tab_h(float h, float indenty, float cell_dy) {
	draw_yStep = h;
	cellH = draw_yStep - indenty;
	cellDy = cell_dy;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_var_color(const string &var_name, const ofColor &color) {	//sets variable color through all pages
	vector<Var *> vars = findVars(var_name);
	for (int i = 0; i<vars.size(); i++) {
		vars[i]->setColor(color);
	}
}

//------------------------------------------------------------------------
vector<ofxKuTextGui::Var *> ofxKuTextGui::getVars() {
	rebuildVars();
	vector<Var *> vars;
	for (StringVarMap::const_iterator iter = vars_.begin(); iter != vars_.end(); iter++) {
		vars.push_back( iter->second );
	}
	return vars;
}

//------------------------------------------------------------------------
vector<ofxKuTextGui::Var *> ofxKuTextGui::getPageVars() {
    vector<ofxKuTextGui::Var *> vars;
    ofxKuTextGui::Page *page = currentPagePointer();
    if (page) {
        for (int i=0; i<page->tab.size(); i++) {
            ofxKuTextGui::Tab &tab = page->tab[i];
            for (int j=0; j<tab.var.size(); j++) {
                ofxKuTextGui::Var &var = tab.var[j];
                vars.push_back(&var);
            }
        }
    }
    return vars;
}

//------------------------------------------------------------------------
string ofxKuTextGui::toString() {
    string s;
    vector<Var *> vars = getVars();
    for (int i=0; i<vars.size(); i++) {
        if (i>0) s += " ";
        s += vars[i]->name() + "=" + vars[i]->value();
    }
    return s;
}

//------------------------------------------------------------------------
void ofxKuTextGui::setFromString(const string &s) {
    vector<Var *> vars = getVars();
    vector<string> lines = ofSplitString(s," ");
    for (int k=0; k<lines.size(); k++) {
        vector<string> item = ofSplitString(lines[k], "=", true, true);
        if (item.size() >= 2) {
            string name		= item[0];
            string value	= item[1];
            Var *var = findVar(name);
            if (var) {
                var->setValue(value);
            }
        }
    }
}

//------------------------------------------------------------------------
void ofxKuTextGui::loadFromFile(const string &fileName) {
	if (fileName.empty()) {
		return;
	}
	//read lines
	vector<string> lines;
    ifstream f(ofToDataPath(fileName).c_str(),ios::in | ios::binary);
    string line;
    while (getline(f,line)) {
        if (line=="") continue;
        else {
            //remove '\r' for proper reading windows-files in linux
			while ( line[line.length()-1] == '\r' ) {
                line = line.substr( 0, line.length() - 1 );
            }
            lines.push_back( line );
        }
    }
	//parse lines
	vector<Var *> vars = getVars();
	for (int k=0; k<lines.size(); k++) {
		vector<string> item = ofSplitString(lines[k], "=", true, true);
		if (item.size() >= 2) {
			string name		= item[0];
			string value	= item[1];
			Var *var = findVar(name);
			if (var) {
				var->setValue(value);
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::saveToFile(const string &fileName) {
	vector<string> file;
	vector<Var *> vars = getVars();
	for (int i=0; i<vars.size(); i++) {
		if (vars[i]->need_save()) {
			string line = vars[i]->name() + "=" + vars[i]->value();
			file.push_back(line);
			//cout << "SAVE PARAM " << line << endl;
		}
	}
	ofstream f(ofToDataPath(fileName).c_str(),ios::out);
    for ( size_t i=0; i<file.size(); i++ ) {
        f << file[i] << endl;
    }
    f.close();
}

//------------------------------------------------------------------------
/*
Saving format:
{
  "-camera"="OFF",
  "FPS"="30",
  "[page]"="Camera",
  "depth_fps"="30",
  "depth_res"="640x480",
  "rgb_fps"="30"
}
*/
string ofxKuTextGui::saveToJSON() {
	vector<string> file;
	file.push_back("{");
	vector<Var *> vars = getVars();
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i]->need_save()) {
			string name = vars[i]->name();
			if (!name.empty()) {
				string line = "  \"" + vars[i]->name() + "\"" + ": " + "\"" + vars[i]->value() + "\"";
				file.push_back(line);
			}
		}
	}
	file.push_back("}");
	for (int i = 1; i < int(file.size()) - 2; i++) {
		file[i] += ",";
	}
	return ofJoinString(file, "\n");
}

//------------------------------------------------------------------------
//JSON using documentation: https://github.com/nlohmann/json#examples

bool ofxKuTextGui::loadFromJSON(const string &s) {
	ofJson j = ofJson::parse(s);

	vector<Var *> vars = getVars();

	for (auto it = j.begin(); it != j.end(); ++it) {
		//std::cout << it.key() << " : " << it.value() << "\n";
		Var *var = findVar(it.key());
		if (var) {
			var->setValue(it.value());
		}
	}
	return true;
}


//------------------------------------------------------------------------
//Generate names and values separated by separator: x;10;y;20...
string ofxKuTextGui::saveToLine(const string &separator) {
	string line;
	vector<Var *> vars = getVars();
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i]->need_save()) {
			string name = vars[i]->name();
			if (!name.empty()) {
				if (!line.empty()) line += separator;
				line += vars[i]->name() + separator + vars[i]->value();
			}
		}
	}
	return line;
}

//------------------------------------------------------------------------
bool ofxKuTextGui::loadFromLine(const string &s, const string &separator) {
	vector<Var *> vars = getVars();

	vector<string> items = ofSplitString(s, separator);
	int n = items.size();
	for (int i = 0; i < n; i+=2) {
		if (i + 1 < n) {
			Var *var = findVar(items[i]);
			if (var) {
				var->setValue(items[i + 1]);
			}
		}
	}
	return true;
}

//------------------------------------------------------------------------
int ofxKuTextGui::addPage(const string &pageName) {
	Page page;
	page.name = pageName;
	page_.push_back(page);
	addTab();
	needRebuild_ = true;

    //add var "[page]" at the top of the page
    string pageVar = "[page]";
    if (page_.size() == 1) {
        string page_var = pageVar;
        addStringList(page_var, selPage, selPage, 0);
    }
    else {
        addVar(pageVar);
    }
    vector<string> titles = pageTitles();
    vector<Var *> var = findVars(pageVar);
    for (int i=0; i<var.size(); i++) {
        var[i]->setTitles(titles);
        var[i]->vstringlist.maxV = int(titles.size())-1;
    }
    selPage = 0;
    return int(page_.size())-1;
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::addTab(int n) {
	for (int i = 0; i < n; i++) {
		if (page_.empty()) addPage("");
		Tab tab;
		page_[page_.size() - 1].addTab();
		needRebuild_ = true;
	}
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addVar(Var &var) {
	Var *var_ = page_[page_.size()-1].addVar(var);
	needRebuild_ = true;
    return var_;
}

//------------------------------------------------------------------------
bool ofxKuTextGui::setValue(const string &name, const string &value) {
	Var *var = findVar(name);
	if (var) {
		var->setValue(value);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void ofxKuTextGui::rebuildVars() {
	if (needRebuild_) {
		needRebuild_ = false;
		vars_.clear();
		hash_vars_.clear();
		for (int p = 0; p<page_.size(); p++) {
			Page &page = page_[p];
			for (int t=0; t<page.tab.size(); t++) {
				Tab &tab = page.tab[t];
				for (int i=0; i<tab.var.size(); i++) {
					Var &var_ = tab.var[i];
					vars_[var_.name()] = &var_;
					hash_vars_[var_.name()] = &var_;
				}
			}
		}
	}
}

//------------------------------------------------------------------------
//adding to current page
ofxKuTextGui::Var *ofxKuTextGui::addFloat(string name, float &var, float defV, float minV, float maxV,
	int numSteps1, int numSteps2) 
{
	if (page_.empty()) addPage("");
	Var var_;
	var_.index= Var::VFloat;
	var_.vfloat = VarFloat(name, var, defV, minV, maxV, numSteps1, numSteps2);
	return addVar(var_);
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addInt(string name, int &var, int defV, int minV, int maxV,
		int step1, int step2) 
{
	if (page_.empty()) addPage("");
	Var var_;
	var_.index= Var::VInt;
	var_.vint = VarInt(name, var, defV, minV, maxV, step1, step2);
	return addVar(var_);
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addButton(string name, int &var) {
	if (page_.empty()) addPage("");
	Var var_;
	var_.index = Var::VInt;
	var_.vint = VarInt(name, var, 0, 0, 1, 1, 1);
	var_.vint.setButton(1);
	return addVar(var_);
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addString(string name, string &var, const string &defV) {
	if (page_.empty()) addPage("");
	Var var_;
	var_.index= Var::VString;
	var_.vstring = VarString(name, var, defV);
	return addVar(var_);
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addStringList(string name, int &var, int defV, const vector<string> &title) {
    if (page_.empty()) addPage("");
    Var var_;
    var_.index= Var::VStringList;
    var_.vstringlist = VarStringList(name, var, defV, 0, int(title.size())-1, 1, 10, title);
    return addVar(var_);
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addStringList(string name, int &var, int defV, int count...) {
    vector<string> title(count);
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        title[i] = va_arg(args, char*);
    }
    va_end(args);
    return addStringList(name,var,defV,title);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addDummy(string title, int n) {
	for (int i = 0; i < n; i++) {
		if (page_.empty()) addPage("");
		Var var_;
		var_.index = Var::VDummy;
		var_.vstring.title = title;
		addVar(var_);
	}
}


//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::addVar(string name) {	//adding existing var
	Var *var = findVar(name);
	if (var) {
		addVar(*var);
	}
	else {
		cout << "ERROR ofxKuTextGui::addVar, no variable " << name << endl;
	}
    return var;
}


//------------------------------------------------------------------------
void ofxKuTextGui::setPage( int index ) {
    if (index >= 0 && index < page_.size()) {
        selPage = index;
		mouse_reset();
    }
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevPage() {
    setPage(selPage-1);
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextPage() {
    setPage(selPage+1);
}

//------------------------------------------------------------------------
void ofxKuTextGui::setTab(int index) {
    if (validPage()) {
        Page &page = page_[selPage];
        page.selTab = index;
        if (page.selTab >= page.tab.size()) {
            page.selTab = 0;
        }
    }
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevTab() {
	if (validPage()){
		Page &page = page_[selPage];
		page.selTab--;
		if (page.selTab < 0) {
			page.selTab = int(page.tab.size())-1;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextTab() {
	if (validPage()){
		Page &page = page_[selPage];
		page.selTab++;
		if (page.selTab >= page.tab.size()) {
			page.selTab = 0;
		}
	}
}



//------------------------------------------------------------------------
void ofxKuTextGui::setValue(int index) {
    if (validPage()){
        Page &page = page_[selPage];
        if (page.validTab()) {
            Tab &tab = page.tab[page.selTab];
            tab.selVar = index;
            if (tab.selVar<0) tab.selVar = int(tab.var.size())-1;
        }
    }
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevValue() {
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			tab.selVar--;
			if (tab.selVar<0) tab.selVar = int(tab.var.size())-1;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextValue() {
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			tab.selVar++;
			if (tab.selVar>=tab.var.size()) tab.selVar = 0;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::decreaseValue(int speed, int value) {	//0-slow,1-fast
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) {
				bool is_string = tab.var[tab.selVar].is_string();
				if (!is_string || (is_string && editing_strings_)) {
					tab.var[tab.selVar].inc(-value, speed);
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::increaseValue(int speed, int value) {	//0-slow,1-fast
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) {
				bool is_string = tab.var[tab.selVar].is_string();
				if (!is_string || (is_string && editing_strings_)) {
					tab.var[tab.selVar].inc(+value, speed);
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::editStringValue() {
	if (validPage() && editing_strings_) {
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar].editStringValue();
		}
	}
}


//------------------------------------------------------------------------
//Enable(default) or disable editing string values from keyboard
//This disabling feature t may be useful, because currently editing strings blocks main app loop in oF,
//and for remote controlled GUI it's undesirable to be blocked
void ofxKuTextGui::set_editing_strings(bool v) {	
	editing_strings_ = v;
}

//------------------------------------------------------------------------
void ofxKuTextGui::setPage( const string &name ) {
	for (int i=0; i<page_.size(); i++) {
		if (name == page_[i].name) {
			selPage = i;
			break;
		}
	}
}

//------------------------------------------------------------------------
int ofxKuTextGui::pageIndex() {
    return selPage;
}

//------------------------------------------------------------------------
int ofxKuTextGui::tabIndex() {
    if (validPage()){
        Page &page = page_[selPage];
        if (page.validTab()) {
            return page.selTab;
        }
    }
    return -1;
}

//------------------------------------------------------------------------
int ofxKuTextGui::varIndex() {
    if (validPage()){
        Page &page = page_[selPage];
        if (page.validTab()) {
            Tab &tab = page.tab[page.selTab];
            if (tab.validVar()) return tab.selVar;
        }
    }
    return -1;
}

//------------------------------------------------------------------------
string ofxKuTextGui::pageTitle() {
    if (validPage()) return page_[selPage].name;
    else return "";
}

//------------------------------------------------------------------------
bool ofxKuTextGui::validPage() {
	return (selPage >= 0 && selPage < page_.size());
}

//------------------------------------------------------------------------
ofxKuTextGui::Page *ofxKuTextGui::currentPagePointer() {
    if (validPage()) { return &page_[selPage]; }
    return 0;
}

//------------------------------------------------------------------------
vector<string> ofxKuTextGui::pageTitles() {
    int n = page_.size();
    vector<string> titles(n);
    for (int i=0; i<n; i++) {
        titles[i] = page_[i].name;
    }
    return titles;
}

//------------------------------------------------------------------------
ofRectangle ofxKuTextGui::drawn_cell_rect(int tab, int i) {
	float x = drawn_x_ + cellDx + draw_tabW * tab;
    float y = drawn_y_ + cellDy + draw_yStep * i;
	return ofRectangle(x,y,cellW,cellH);                
}

//------------------------------------------------------------------------
void ofxKuTextGui::get_cell_by_coords(float x, float y, int &tab, int &i) {
	tab = (x-drawn_x_-cellDx) / draw_tabW;
	i = (y-drawn_y_-cellDy) / draw_yStep;
}

//------------------------------------------------------------------------
void ofxKuTextGui::update() {					//for buttons processing
	for (int i = 0; i<page_.size(); i++) {
		Page &page = page_[i];
		for (int j = 0; j<page.tab.size(); j++) {
			Tab &tab = page.tab[j];
			for (int k = 0; k<tab.var.size(); k++) {
				tab.var[k].update_button();				//TODO optimize and check only button variables
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::draw(float X, float Y, bool enabled, int alpha_text, int alpha_slider) {	//generic draw
	if (validPage()) {
		float alpha_text_f = float(alpha_text) / 255.0;

		ofEnableAlphaBlending();
        
		drawn_x_ = X;
		drawn_y_ = Y;

        float w = cellW;
        float h = cellH;
        
		Page &page = page_[selPage];
		for (int t=0; t<page.tab.size(); t++) {
			Tab &tab = page.tab[t];
			for (int i=0; i<tab.var.size(); i++) {
				Var &var = tab.var[i];
				bool selected = (/*enabled &&*/ page.selTab==t && tab.selVar==i);
				string name = var.title();
				if ( selected ) name = ">" + name;
				else name = " " + name;
                
                float x = X + draw_tabW * t;
                float y = Y + draw_yStep * i;
                
				bool dummy = (var.index == Var::VDummy);

				if (!dummy) {
					bool button = var.is_button();
					const int button_ind = 8;
					const float button_round = 10;	//button rounding of corners

					if (!button) {
						ofFill();
						ofSetColor(0, alpha_slider);
						ofDrawRectangle(x + cellDx, y + cellDy, w, h);
					
						if (drawSliderMode_) {
							if (selected) {
								if (enabled) ofSetColor(200, 200, 0, alpha_slider);
								else ofSetColor(0, 200, 200, alpha_slider);
							}
							else ofSetColor(128, alpha_slider);
							ofNoFill();
							ofDrawRectangle(x + cellDx, y + cellDy, w, h);
						}
					}

					//button
					if (button) {
						float a = ofLerp(0.3, 1, var.vint.button_alpha_);	//NOTE: Parameters for button's background
						if (a > 0) {
							ofSetColor(180 * a, alpha_slider);
							ofFill();
							ofDrawRectRounded(x + cellDx + button_ind, y + cellDy, w - 2 * button_ind, h, button_round);
							//ofDrawRectangle(x + cellDx + button_ind, y + cellDy, w - 2 * button_ind, h);
						}

						if (drawSliderMode_) {
							if (selected) {
								if (enabled) ofSetColor(200, 200, 0, alpha_slider);
								else ofSetColor(0, 200, 200, alpha_slider);
							}
							else ofSetColor(128, alpha_slider);
							ofNoFill();
							ofDrawRectRounded(x + cellDx + button_ind, y + cellDy, w - 2 * button_ind, h, button_round);
						}

						ofColor &color = var.color;
						ofSetColor(color.r, color.g, color.b, color.a * alpha_text_f);
						draw_string(name, x, y);
						
					}
					else {
						//non-button
						ofColor &color = var.color;
						ofSetColor(color.r, color.g, color.b, color.a * alpha_text_f);
						draw_string(name + " " + var.value(), x, y);

						//Slider and smoothed value
						if (drawSliderMode_) {
							ofFill();
							ofSetColor(255, 60.0 / 255.0*alpha_slider);
							ofDrawRectangle(x + cellDx, y + cellDy, w*var.valueNormalized(), h);
							if (selected) {
								if (enabled) ofSetColor(255, 255, 0, alpha_slider);
								else ofSetColor(0, 200, 200, alpha_slider);
							}
							else ofSetColor(200, alpha_slider);
							//Slider
							ofNoFill();
							float val_pix = w * var.valueNormalized();
							ofDrawRectangle(x + cellDx, y + cellDy, val_pix, h);

							//Thick line
							ofSetLineWidth(3);
							ofDrawLine(x + cellDx + val_pix, y + cellDy, x + cellDx + val_pix, y + cellDy + h);
							ofSetLineWidth(1);

							//Bottom line - currently only for smoothed values
							if (var.draw_smoothed_value_) {	//comment to draw bottom line always
								float bottom_val_pix = (var.draw_smoothed_value_) ? w * var.smoothed_value_normalized_ : val_pix;
								ofFill();
								const int h1 = 3; //PARAM
								ofDrawRectangle(x + cellDx, y + cellDy + h - h1, bottom_val_pix, h1);
							}
						}
					}
                }
				else {
					//Dummy

					//dummy's text
					if (!var.vstring.title.empty()) {
						//back
						ofFill();
						ofSetColor(dummy_back_);
						//ofSetColor(0, alpha_slider);
						ofDrawRectangle(x + cellDx, y + cellDy, w, h);

						ofSetColor(dummy_color_);
						draw_string(var.vstring.title, x, y);
					}

					if (drawSliderMode_) {
						if (selected) {
							if (enabled) ofSetColor(150, 150, 0, alpha_slider);
							else ofSetColor(0, 120, 120, alpha_slider);
							//}
							//else ofSetColor(100, alpha_slider);
							ofNoFill();
							ofDrawRectangle(x + cellDx, y + cellDy, w, h);
						}
					}
					
				}
			}
		}
	}
}

//------------------------------------------------------------------------
string ofxKuTextGui::drawToString() {  //keeps current page, tabs, selected value
    string s = "";
    if (validPage()) {
        Page &page = page_[selPage];
        for (int t=0; t<page.tab.size(); t++) {
            if (t>0) s += ";";
            Tab &tab = page.tab[t];
            for (int i=0; i<tab.var.size(); i++) {
                Var &var = tab.var[i];
                bool selected = (page.selTab==t && tab.selVar==i);
                string name = var.name();
                if ( selected ) name = ">" + name;
                else name = " " + name;
                
                if (i>0) s+= ",";
                s += name + "=" + var.value() + "=" + ofToString(var.valueNormalized());
            }
        }
    }
    return s;
}

//------------------------------------------------------------------------
//using custom font - if not, using ofDrawBitmapString
void ofxKuTextGui::set_font(ofTrueTypeFont *font, float shift_x, float shift_y) {
	custom_font_ = font;
	font_shift_x = shift_x;
	font_shift_y = shift_y;

}

//------------------------------------------------------------------------
void ofxKuTextGui::draw_string(const string &s, float x, float y) {
	if (custom_font_) {
		custom_font_->drawString(s, x + font_shift_x, y + font_shift_y);
	}
	else {
		ofDrawBitmapString(s, x, y);
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::drawFromString(const string &s, float X, float Y) {
    vector<string> tabs = ofSplitString(s, ";");
    float w = cellW;
    float h = cellH;
        
    for (int t=0; t<tabs.size(); t++) {
        vector<string> tab = ofSplitString(tabs[t], ",");
        for (int i=0; i<tab.size(); i++) {
            vector<string> var = ofSplitString(tab[i],"=");
            if (var.size()>=3) {
                string name = var[0];
                string value = var[1];
                float valueNormalized = ofToFloat(var[2]);
                bool selected = (!name.empty() && name[0] == '>');
                float x = X + draw_tabW * t;
                float y = Y + draw_yStep * i;
                
                ofFill();
                ofSetColor(0);
                ofDrawRectangle(x+cellDx,y+cellDy,w,h);
                if (drawSliderMode_) {
                    if (selected) ofSetColor(200,200,0);
                    else ofSetColor(128);
                    ofNoFill();
                    ofDrawRectangle(x+cellDx,y+cellDy,w,h);
                }
                ofSetColor(255);
				draw_string(name+" "+value, x, y);
                if (drawSliderMode_) {
                    ofFill();
                    ofSetColor(255,60);
                    ofDrawRectangle(x+cellDx,y+cellDy,w*valueNormalized,h);
                    if (selected) ofSetColor(255,255,0);
                    else ofSetColor(200);
                    ofNoFill();
                    ofDrawRectangle(x+cellDx,y+cellDy,w*valueNormalized,h);
                }
            }
        }
    }
}

//------------------------------------------------------------------------
bool ofxKuTextGui::keyPressed(int key) {       //generic keyPressed handler
    if (key == '1')             { gotoPrevPage(); return true; }
    if (key == '2')             { gotoNextPage(); return true; }
	if (key == '!')				{ setPage(0); return true; }
	if (key == '@')				{ setPage(int(page_.size())-1); return true; }
    if (key == OF_KEY_LEFT)     { gotoPrevTab(); return true; }
    if (key == OF_KEY_RIGHT)    { gotoNextTab(); return true; }
    if (key == OF_KEY_UP)       { gotoPrevValue(); return true; }
    if (key == OF_KEY_DOWN)     { gotoNextValue(); return true; }
    if (key == '[')             { decreaseValue(0); return true; }
    if (key == ']')             { increaseValue(0); return true; }
    if (key == '{')             { decreaseValue(1); return true; }
    if (key == '}')             { increaseValue(1); return true; }

    return false;
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::findVar(const string &name) {
	rebuildVars();
	StringVarHash::iterator p = hash_vars_.find( name );
    return ( p != hash_vars_.end() )? hash_vars_[name]:0;
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::findVarChecking(const string &name) {   //one var, exits if no found
	Var *v = findVar(name);
	if (!v) {	//Note - it shouldn't put checking of v to "exit_with_message" to avoid constructing strings each calling
		exit_with_message("ofxKuTextGui error in findVarChecking, no var '" + name + "'");
	}
	return v;
}

//------------------------------------------------------------------------
void ofxKuTextGui::exit_with_message(const string &message) {
	cout << "ofxKuTextGui causes exiting now," << endl;
	cout << "    Reason: " << message << endl;
	ofSetFullscreen(false);
	ofSystemAlertDialog("ofxKuTextGui causes exiting now, reason:\n" + message);
	ofSleepMillis(2000);
	OF_EXIT_APP(0);
}

//------------------------------------------------------------------------
float *ofxKuTextGui::findVarFloat(const string &name) {
	float *v = findVarChecking(name)->vfloat.var;
	if (!v) exit_with_message("No float " + name);
    return v;
}

//------------------------------------------------------------------------
int *ofxKuTextGui::findVarInt(const string &name) {
	int *v = findVarChecking(name)->vint.var;
	if (!v) exit_with_message("No int " + name);
	return v;
}

//------------------------------------------------------------------------
int *ofxKuTextGui::findVarStringList(const string &name) {
	int *v = findVarChecking(name)->vstringlist.var;
	if (!v) exit_with_message("No stringlist " + name);
	return v;
}

//------------------------------------------------------------------------
string *ofxKuTextGui::findVarString(const string &name) {
	string *v = findVarChecking(name)->vstring.var;
	if (!v) exit_with_message("No string " + name);
	return v;
}

//------------------------------------------------------------------------
int *ofxKuTextGui::findVarButton(const string &name) {
	int *v = findVarChecking(name)->vint.var;
	if (!v) exit_with_message("No button " + name);
	return v;
}

//------------------------------------------------------------------------
float &ofxKuTextGui::float_(const string &name) {
	return *findVarFloat(name);
}

//------------------------------------------------------------------------
int &ofxKuTextGui::int_(const string &name) {
	return *findVarInt(name);
}

//------------------------------------------------------------------------
int &ofxKuTextGui::stringlist_(const string &name) {
	return *findVarStringList(name);
}

//------------------------------------------------------------------------
string &ofxKuTextGui::string_(const string &name) {
	return *findVarString(name);
}

//------------------------------------------------------------------------
int &ofxKuTextGui::button_(const string &name) {
	return *findVarButton(name);
}

//------------------------------------------------------------------------
//applicable to float and int vars, automatically enables drawing smoothed value
float ofxKuTextGui::updateSmoothedValue(const string name, float dt, float time_whole_change) {
	auto *var = findVarChecking(name);
	var->setDrawSmoothed(true);
	var->updateSmoothedValue(dt, time_whole_change);
	return var->getSmoothedValue();
}

//------------------------------------------------------------------------
float ofxKuTextGui::getSmoothedValue(const string name) {
	return findVarChecking(name)->getSmoothedValue();
}

//------------------------------------------------------------------------
vector<ofxKuTextGui::Var *> ofxKuTextGui::findVars(const string &name) {   //all instances
	//rebuildVars();
    vector<ofxKuTextGui::Var *> vars;
    for (int i=0; i<page_.size(); i++) {
        Page &page = page_[i];
        for (int j=0; j<page.tab.size(); j++) {
            Tab &tab = page.tab[j];
            for (int k=0; k<tab.var.size(); k++) {
                if (tab.var[k].name() == name) {
                    vars.push_back(&tab.var[k]);
                }
            }
        }
    }
    return vars;
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::setDrawSliderMode(bool value) { //should we draw slider
    drawSliderMode_ = value;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_mouse_enabled(bool v) {
	mouse_enabled_  = v;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_mouse_step(int step) {
	mouse_step_ = step;
}

//------------------------------------------------------------------------
bool ofxKuTextGui::mousePressed(int x, int y, int button) {
	if (mouse_enabled_) {
		if (validPage()) {
			Page *page = currentPagePointer();		
			if (page) {
				int t, i;
				get_cell_by_coords(x, y, t, i);
				if (t >= 0 && t < page->tab.size()) {
					Tab &tab = page->tab[t];
					if (i >= 0 && i < tab.var.size()) {
						page->selTab = t;
						tab.selVar = i;

						//check button
						if (tab.validVar() && tab.var[tab.selVar].is_button()) {
							tab.var[tab.selVar].setValue("1");
						}
						else {
							mouse_dragging_ = true;
							mouse_x_ = x;
							mouse_y_ = y;
						}
                        return true;
					}
				}
			}
		}
	}
    return false;
}

//------------------------------------------------------------------------
void ofxKuTextGui::mouseDragged(int x, int y, int button) {
	if (mouse_enabled_) {
		if (mouse_dragging_) {
			int speed = (button == 2)?1:0;

			int delta = x - mouse_x_;
			int a = abs(delta);
			if (a >= mouse_step_) {
				int shift = (a / mouse_step_) ;				
				if (delta>0) increaseValue(speed, shift);
				else decreaseValue(speed, shift);
				mouse_x_ += shift * mouse_step_ * ((delta>0)?1:-1);
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::mouseReleased(int x, int y, int button) {
	if (mouse_enabled_) {

	}
	mouse_reset();
}

//------------------------------------------------------------------------
void ofxKuTextGui::mouse_reset() {
	mouse_dragging_ = false;
}

//------------------------------------------------------------------------
