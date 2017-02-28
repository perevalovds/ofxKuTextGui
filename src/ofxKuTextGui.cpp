#include "ofxKuTextGui.h"


//------------------------------------------------------------------------
ofxKuTextGui::ofxKuTextGui() {
	selPage = 0;

	draw_tabW = 150;
	draw_yStep = 20;

	needRebuild_ = true;
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
void ofxKuTextGui::loadFromFile(const string &fileName) {
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
		string line = vars[i]->name() + "=" + vars[i]->value();
		file.push_back(line);
		cout << line << endl;
	}
	ofstream f(ofToDataPath(fileName).c_str(),ios::out);
    for ( size_t i=0; i<file.size(); i++ ) {
        f << file[i] << endl;
    }
    f.close();
}

//------------------------------------------------------------------------
void ofxKuTextGui::addPage(const string &pageName) {
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
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::addTab() {
	if (page_.empty()) addPage("");
	Tab tab;
	page_[page_.size()-1].addTab();	
	needRebuild_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGui::addVar(Var &var) {
	page_[page_.size()-1].addVar(var);
	needRebuild_ = true;
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
		for (int p = 0; p<page_.size(); p++) {
			Page &page = page_[p];
			for (int t=0; t<page.tab.size(); t++) {
				Tab &tab = page.tab[t];
				for (int i=0; i<tab.var.size(); i++) {
					Var &var_ = tab.var[i];
					vars_[var_.name()] = &var_;
				}
			}
		}
	}
}

//------------------------------------------------------------------------
//adding to current page
void ofxKuTextGui::addFloat(string name, float &var, float defV, float minV, float maxV, 
	int numSteps1, int numSteps2) 
{
	if (page_.empty()) addPage("");
	Var var_;
	var_.index=0;
	var_.vfloat = VarFloat(name, var, defV, minV, maxV, numSteps1, numSteps2);
	addVar(var_);

}

//------------------------------------------------------------------------
void ofxKuTextGui::addInt(string name, int &var, int defV, int minV, int maxV,
		int step1, int step2) 
{
	if (page_.empty()) addPage("");
	Var var_;
	var_.index=1;
	var_.vint = VarInt(name, var, defV, minV, maxV, step1, step2);
	addVar(var_);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addString(string name, string &var, const string &defV) {
	if (page_.empty()) addPage("");
	Var var_;
	var_.index=2;
	var_.vstring = VarString(name, var, defV);
	addVar(var_);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addStringList(string name, int &var, int defV, const vector<string> &title) {
    if (page_.empty()) addPage("");
    Var var_;
    var_.index=3;
    var_.vstringlist = VarStringList(name, var, defV, 0, int(title.size())-1, 1, 10, title);
    addVar(var_);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addStringList(string name, int &var, int defV, int count...) {
    vector<string> title(count);
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        title[i] = va_arg(args, char*);
    }
    va_end(args);
    addStringList(name,var,defV,title);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addVar(string name) {	//adding existing var
	Var *var = findVar(name);
	if (var) {
		addVar(*var);
	}
	else {
		cout << "ERROR ofxKuTextGui::addVar, no variable " << name << endl;
	}
}


//------------------------------------------------------------------------
void ofxKuTextGui::setPage( int index ) {
    if (index >= 0 && index < page_.size()) {
        selPage = index;
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
void ofxKuTextGui::decreaseValue(int speed) {	//0-slow,1-fast
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar].inc(-1,speed);
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::increaseValue(int speed) {	//0-slow,1-fast
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar].inc(+1,speed);
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::editStringValue() {
	if (validPage()) {
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar].editStringValue();
		}
	}
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
void ofxKuTextGui::draw(float X, float Y, bool enabled) {	//generic draw
	if (validPage()) {
		Page &page = page_[selPage];
		for (int t=0; t<page.tab.size(); t++) {
			Tab &tab = page.tab[t];
			for (int i=0; i<tab.var.size(); i++) {
				Var &var = tab.var[i];
				bool selected = (enabled && page.selTab==t && tab.selVar==i);
				string name = var.name();
				if ( selected ) name = ">" + name;
				else name = " " + name;
				ofDrawBitmapStringHighlight(name+" "+var.value(),
					X + draw_tabW * t, Y + draw_yStep * i);
			}
		}
	}
}

//------------------------------------------------------------------------
bool ofxKuTextGui::keyPressed(int key) {       //generic keyPressed handler
    if (key == '1')             { gotoPrevPage(); return true; }
    if (key == '2')             { gotoNextPage(); return true; }
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
	StringVarMap::iterator p = vars_.find( name );
    return ( p != vars_.end() )?vars_[name]:0;
}

//------------------------------------------------------------------------
vector<ofxKuTextGui::Var *> ofxKuTextGui::findVars(const string &name) {   //all instances
    vector<ofxKuTextGui::Var *> vars;
    for (int i=0; i<page_.size(); i++) {
        Page &page = page_[i];
        for (int j=0; j<page.tab.size(); j++) {
            Tab &tab = page.tab[j];
            for (int k=0; j<tab.var.size(); j++) {
                if (tab.var[k].name() == name) {
                    vars.push_back(&tab.var[k]);
                }
            }
        }
    }
    return vars;
    
}

//------------------------------------------------------------------------
