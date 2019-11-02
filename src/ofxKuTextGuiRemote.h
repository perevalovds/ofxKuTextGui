#pragma once

/*
 Optional module for controlling ofApp and GUI over OSC networking protocol.
 It requires ofxOsc addon.
 If you are not need this capability - just exclude this from the project.
*/

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxKuTextGui.h"


//Remote Server
struct ofxKuTextGuiRemoteServer {
	//Setup with starting OSC
    void setup(int in_port, ofBaseApp *app, ofxKuTextGui *gui);
	//Setup without starting OSC - call processMessage for processing messages
	void setup(ofBaseApp *app, ofxKuTextGui *gui);	

    void update();
    void exit();
    
	//Parsing message and return true, if it's known command
    bool processMessage(ofxOscMessage &m);    

    void answerGuiRequest(string ip, int port);

	//Here receiver stored unparsed messages - use it for custom parsing
	//Note: it is cleaned on each update; so you should parse it right after "update"
	vector<ofxOscMessage> &unparsed_messages();

protected:
    ofxOscReceiver receiver_;
    ofxKuTextGui *gui_;
    ofBaseApp *app_;

	vector<ofxOscMessage> unparsed_messages_;

};


//Remote Client
struct ofxKuTextGuiRemoteClient {
    void setup(string out_host, int out_port, int in_port);
    void update();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void draw(float x, float y);
    
    void askGuiRequest();
    void processMessage(ofxOscMessage &m);

	string get_var_value(string var_name);	//returns var value if it's visible
protected:
    ofxOscReceiver receiver_;
    int in_port_;

    ofxOscSender sender_;
    float time0_;
    
    string gui_string_;
    ofxKuTextGui gui_;
    
    bool need_fast_update_; //нужно быстро обновить
    
    float fast_time, slow_time; //время быстрого и медленного запроса GUI
    
};










