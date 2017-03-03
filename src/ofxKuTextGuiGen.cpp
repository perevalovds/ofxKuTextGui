#include "ofxKuTextGuiGen.h"

/*C++ code generator for ofxKuTextGui addon
 You preparing text description of the GUI, see gui-script.ini,
 and this algorithm generates .H and .CPP files, which implements this gui in the C++ code.
 
 Example of GUI script:
 
 ------
 PAGE screen
 int *FPS=30 1:100 1,10
 int *w=1024 1:2000 1,10
 int *h=768 1:2000 1,10
 TAB
 float -fps=30 0:100 100,10
 
 PAGE osc
 string send_host=localhost
 int send_port=12345 1:65535 1,10
 
 PAGE all
 var -fps
 stringlist list=a [a,b,c]
 ------
 Here
 "*" means parameter used at start, "*FPS" is "FPS",
 but changes only after restart
 "-" means read-only parameter, "-fps" is "fps_" in code
 
 
 Example of generator calling:
 generateCPP("../../src/gui-script.ini", "../src/params",
 "Parameters", "params",
 "P");
 "P" is the abbreviation of the parameters, you can use them in such a way:
 sender.setup(P send_host, P send_port);
 */

//------------------------------------------------------------------------
void ofxKuTextGuiGen::put(string line, vector<string> &lines) {
    lines.push_back(line);
}

//------------------------------------------------------------------------
void ofxKuTextGuiGen::saveFile(vector<string> &lines, string fileName) {
    ofstream f(ofToDataPath(fileName).c_str(),ios::out);
    for ( size_t i=0; i<lines.size(); i++ ) {
        f << lines[i] << endl;
    }
    f.close();
}

//------------------------------------------------------------------------
void ofxKuTextGuiGen::insert(vector<string> &lines, vector<string> &what_to_insert) {
    for (int i=0; i<what_to_insert.size(); i++) {
        lines.push_back(what_to_insert[i]);
    }
}

//------------------------------------------------------------------------
void ofxKuTextGuiGen::generateCPP(string gui_file_in, string c_path, string c_file_out,
                                  string class_name, string extern_var_name,
                                  string define_prefix_name) {
    vector<string> lines;
    ifstream F(ofToDataPath(gui_file_in).c_str(),ios::in | ios::binary);
    string line;
    while (getline(F,line)) {
        ofTrim(line);
        if (line=="") continue;         //skip empty line
        if (line[0] == '#') continue;   //comment

        //remove '\r' for proper reading windows-files in linux
        while ( line[line.length()-1] == '\r' ) {
            line = line.substr( 0, line.length() - 1 );
        }
        lines.push_back( line );
    }
    vector<string> Decl;  //struct declaration
    vector<string> Constr;  //constructor
    vector<string> Setup;  //setup_gui
    vector<string> AfterLoad;  //after_load
    
    for (int i=0; i<lines.size(); i++) {
        string line = lines[i];
        ofStringReplace(line, "\t", " ");
        ofStringReplace(line, "  ", " ");
        //ofTrim(line);
        if (line.empty()) continue;
        vector<string> item = ofSplitString(line, " ");
        int n = item.size();
        string type_s = item[0];
        string name_s = (n>=2) ? item[1] : "";
        string range_s = (n>=3) ? item[2] : "";
        string step_s = (n>=4) ? item[3] : "";
        
        Pair name_pair(name_s,"=");
        Pair range_pair(range_s, ":");
        Pair step_pair(step_s, ",");
        Name name_code(name_pair.a);

        if (name_code.is_const) {
            put("\t" + name_code.const_name + " = " + name_code.code_name + ";", AfterLoad);
        }
        
        if (type_s == "PAGE" && n>=2) {
            put("\tgui.addPage(\"" + name_s + "\");", Setup);
        }
        if (type_s == "TAB") {
            put("\tgui.addTab();", Setup);
        }
        if (type_s == "int" || type_s == "float") {
            if (name_code.is_const) {
                put("\t" + type_s + " " + name_code.const_name + ";", Decl);
            }
            put("\t" + type_s + " " + name_code.code_name + ";", Decl);
            put("\t" + name_code.code_name + "=" + name_pair.b + ";", Constr);
            if (type_s == "int") {
                //int *FPS=30 1:100 1,10
                put("\tgui.addInt(\"" + name_code.screen_name + "\","
                    + name_code.code_name + "," + name_pair.b + ","
                    + range_pair.a + "," + range_pair.b + ","
                    + step_pair.a + "," + step_pair.b + ");",
                    Setup);
            }
            if (type_s == "float") {
                //float -fps=30 0:100 100,10
                put("\tgui.addFloat(\"" + name_code.screen_name + "\","
                    + name_code.code_name + "," + name_pair.b + ","
                    + range_pair.a + "," + range_pair.b + ","
                    + step_pair.a + "," + step_pair.b + ");",
                    Setup);
            }
        }
        if (type_s == "string") {
            if (name_code.is_const) {
                put("\t" + type_s + " " + name_code.const_name + ";", Decl);
            }
            //string send_host=localhost
            put("\t" + type_s + " " + name_code.code_name + ";", Decl);
            put("\t" + name_code.code_name + "=\"" + name_pair.b + "\";", Constr);
            put("\tgui.addString(\"" + name_code.screen_name + "\","
                + name_code.code_name + ",\"" + name_pair.b + "\");",
                Setup);
        }
        if (type_s == "stringlist") {
            if (name_code.is_const) {
                put("\tint " + name_code.const_name + ";", Decl);
            }

            //stringlist list=a [a,b,c]
            put("\tint " + name_code.code_name + ";", Decl);
            vector<string> list = parse_stringlist_values(range_s);
            string def_index = "0";
            for (int i=0; i<list.size(); i++) {
                if (list[i] == name_pair.b) {
                    def_index = ofToString(i);
                    break;
                }
            }
            put("\t" + name_code.code_name + "=" + def_index + ";", Constr);
            string line = "\tgui.addStringList(\"" + name_code.screen_name + "\","
            + name_code.code_name + "," + def_index + ","
            + ofToString(list.size());
            if (list.size()>0) {
                line += "," + stringlist_values(list) + ");";
            }
            put(line,Setup);
        }
    }
    
    
    //.H file
    vector<string> f_h;
    put("#pragma once", f_h);
    put("//Auto-generated GUI file for ofxKuTextGui, " + ofGetTimestampString(), f_h);
    put("", f_h);
    put("#include \"ofMain.h\"", f_h);
    put("#include \"ofxKuTextGui.h\"", f_h);
    
    put("", f_h);
    put("struct " + class_name + " {", f_h);
    insert(f_h, Decl);
    put("    " + class_name + "();", f_h);
    put("    void setup(ofxKuTextGui &gui, string fileName);", f_h);
    put("    void save();", f_h);
    put("    string fileName_;", f_h);
    put("    ofxKuTextGui *gui_;", f_h);
    put("};", f_h);
    put("", f_h);
    
    if (!extern_var_name.empty()) {
        put("extern " + class_name + " " + extern_var_name + ";", f_h);
        if (!define_prefix_name.empty()) {
            put("#define " + define_prefix_name + " " + extern_var_name + ".", f_h);
        }
    }

    saveFile(f_h, c_path + c_file_out + ".h");
    
    //.CPP file
    vector<string> f_cpp;
    
    put("#include \"" + c_file_out + ".h\"", f_cpp);
    put("//Auto-generated GUI file for ofxKuTextGui, " + ofGetTimestampString(), f_cpp);
    put("", f_cpp);
    if (!extern_var_name.empty()) {
        put(class_name + " " + extern_var_name + ";", f_cpp);
    }
    put("//--------------------------------------------------------------", f_cpp);
    put(class_name+"::"+class_name+"() {", f_cpp);
    insert(f_cpp,Constr);
    put("}", f_cpp);
    put("", f_cpp);
    put("//--------------------------------------------------------------", f_cpp);
    put("void " + class_name + "::setup(ofxKuTextGui &gui, string fileName) {", f_cpp);
    insert(f_cpp, Setup);
    put("\tfileName_ = fileName;", f_cpp);
    put("\tgui_ = &gui;", f_cpp);
    put("\tgui.loadFromFile(fileName);", f_cpp);
    insert(f_cpp, AfterLoad);
    put("}", f_cpp);
    put("", f_cpp);
    put("//--------------------------------------------------------------", f_cpp);
    put("void " + class_name + "::save() {", f_cpp);
    put("\tgui_->saveToFile(fileName_);", f_cpp);
    put("}", f_cpp);
    put("", f_cpp);
    put("//--------------------------------------------------------------", f_cpp);
    put("", f_cpp);
    saveFile(f_cpp, c_path + c_file_out + ".cpp");

    
    cout << "Code files for parameters " << c_file_out << " are generated" << endl;

}

//------------------------------------------------------------------------
