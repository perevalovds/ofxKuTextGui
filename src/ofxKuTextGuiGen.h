﻿#pragma once

/*C++ code generator for ofxKuTextGui addon
 You preparing text description of the GUI, see gui-script.ini,
 and this algorithm generates .H and .CPP files, which implements this gui in the C++ code.
 
 Example of GUI script:
 
 ------
 PAGE screen
 COLOR 255,0,0
 int *FPS=30 1:100 1,10
 int *w=1024 1:2000 1,10
 int *h=768 1:2000 1,10
 TAB
 COLOR 64
 float -fps=30 0:100 100,10
 RESET_COLOR

 PAGE osc
 string send_host=localhost
 int send_port=12345 1:65535 1,10
 
 PAGE all
 var -fps
 dummy
 dummy Stringlist:
 stringlist list=a [a,b,c]
 button render
 #button - it's int variable, which is set when button is pressed, needs to set to 0 after reading

 ------
 Here
 "*" means constant parameter used at start, "*FPS" is "FPS",
 but changes only after restart
 "-" means read-only parameter, so user changes will affect nothing, "-fps" is "fps_" in code


Example of generator calling:
 generateCPP("../../src/gui-script.ini", "../src/params",
                        "Parameters", "params",
                         "PRM");
"P" is the abbreviation of the parameters, you can use them in such a way:
  sender.setup(PRM send_host, PRM send_port);
 
*/

#include "ofMain.h"
#include "ofxKuTextGui.h"


struct ofxKuTextGuiGen {
    //generateCPP("gui-script.ini",
    //"../../src/", "gui_generated",
    //"Parameters", "params", "PRM");
    //extern_var_name = "" means no extern variable for parameters
    static void generateCPP(string gui_file_in = "gui-script.ini",
                            string c_path = "../../src/",
                            string c_file_out = "gui_generated",
                            string class_name = "Parameters",
                            string extern_var_name = "params",
                            string define_prefix_name = "PRM");

protected:
    static void put(string lin, vector<string> &lines);
    static void saveFile(vector<string> &lines, string fileName);
    static void insert(vector<string> &lines, vector<string> &what_to_insert);
    
    struct Pair {
        string a,b;
        Pair(string v, string delim) {
            vector<string> item = ofSplitString(v, delim);
            if (item.size()>=1) a = item[0];
            if (item.size()>=2) b = item[1];
        }
    };
    struct Name {
        string code_name, screen_name;
        bool is_const;
        string const_name;
        Name(string name) {
            screen_name = name;
            string prefix = (name.size()>=1)?name.substr(0,1):"";
            code_name = name;
            string short_name = (name.size()>=2)?name.substr(1):"";  //shorten
            is_const = false;
            if (prefix == "*") {
                code_name = "_" + short_name + "_";
                const_name = short_name;
                is_const = true;
            }
            if (prefix == "-") code_name = short_name + "_";
        }
    };
    static vector<string> parse_stringlist_values(string v) {
        //[a,b,c]
        vector<string> list;
        if (v.size() >= 2) {
            v = v.substr(1,v.size()-2);
            list = ofSplitString(v, ",");
        }
        return list;
    }
    static string stringlist_values(vector<string> &list) {
        string s = "";
        for (int i=0; i<list.size(); i++) {
            if (i>0) s += ",";
            s += "\"" + list[i] + "\"";
        }
        return s;
    }
};

