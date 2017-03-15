#include "ofxKuTextGuiRemote.h"


//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::setup(int in_port, ofBaseApp *app, ofxKuTextGui *gui) {
    receiver_.setup(in_port);
    app_ = app;
    gui_ = gui;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::update() {
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage m;
#ifdef OFXKUTEXTGUI_074
		receiver_.getNextMessage(&m);
#else
		receiver_.getNextMessage(m);
#endif
        processMessage(m);
    }
    
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::exit() {

}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::processMessage(ofxOscMessage &m) {
    //guiRequest(int) - argument is port for returning the result
    //keyPressed(int)
    //keyReleased(int)
    //mousePressed(float,float,int)
    //mouseDragged(float,float,int)
    //mouseReleased(float,float,int)
    string addr = m.getAddress();
    if (addr == "/guiRequest") {
        int port = m.getArgAsInt(0);
        answerGuiRequest( m.getRemoteIp(), port );
    }
    if (app_) {
        if (addr == "/keyPressed") {
            app_->keyPressed(m.getArgAsInt(0));
        }
        if (addr == "/keyReleased") {
            app_->keyReleased(m.getArgAsInt(0));
        }
        if (addr == "/mousePressed") {
            app_->mousePressed(m.getArgAsFloat(0) * ofGetWidth(),
                               m.getArgAsFloat(1) * ofGetHeight(),
                               m.getArgAsInt(0));
        }
        if (addr == "/mouseDragged") {
            app_->mouseDragged(m.getArgAsFloat(0) * ofGetWidth(),
                               m.getArgAsFloat(1) * ofGetHeight(),
                               m.getArgAsInt(0));
        }
        if (addr == "/mouseReleased") {
            app_->mouseReleased(m.getArgAsFloat(0) * ofGetWidth(),
                               m.getArgAsFloat(1) * ofGetHeight(),
                               m.getArgAsInt(0));
        }
    }
}


//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::answerGuiRequest(string ip, int port) {
    ofxOscSender sender;
    sender.setup(ip, port);
    ofxOscMessage m;
    m.setAddress("/guiRequestReturn");
    if (gui_) {
        m.addStringArg(gui_->drawToString());
    }
    sender.sendMessage(m);
    
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Connection to Gui, draws it and allows to send mouse and keyboard transmitting
void ofxKuTextGuiRemoteClient::setup(string out_host, int out_port, int in_port) {
    in_port_ = in_port;
    time0_ = ofGetElapsedTimef();
    sender_.setup(out_host, out_port);
    receiver_.setup(in_port);
    
    fast_time = 0.1;
    slow_time = 0.5;
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::update() {
    float time = ofGetElapsedTimef();
    float delta = time-time0_;
    if ( (need_fast_update_ && delta >= fast_time)
        || delta >= slow_time ) {
        askGuiRequest();
        time0_ = time;
        need_fast_update_ = false;
    }
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage m;
#ifdef OFXKUTEXTGUI_074
		receiver_.getNextMessage(&m);
#else
		receiver_.getNextMessage(m);
#endif
        processMessage(m);
    }

}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::exit() {
    
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::draw(float x, float y) {
    gui_.drawFromString(gui_string_, x, y);
}


//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::keyPressed(int key) {
    ofxOscMessage m;
    m.setAddress("/keyPressed");
    m.addIntArg(key);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::keyReleased(int key) {
    ofxOscMessage m;
    m.setAddress("/keyReleased");
    m.addIntArg(key);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::mousePressed(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mousePressed");
    m.addFloatArg(float(x) / ofGetWidth());
    m.addFloatArg(float(y) / ofGetHeight());
    m.addIntArg(button);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::mouseDragged(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mouseDragged");
    m.addFloatArg(float(x) / ofGetWidth());
    m.addFloatArg(float(y) / ofGetHeight());
    m.addIntArg(button);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::mouseReleased(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mouseReleased");
    m.addFloatArg(float(x) / ofGetWidth());
    m.addFloatArg(float(y) / ofGetHeight());
    m.addIntArg(button);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::askGuiRequest() {
    ofxOscMessage m;
    m.setAddress("/guiRequest");
    m.addIntArg(in_port_);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::processMessage(ofxOscMessage &m) {
    if (m.getAddress() == "/guiRequestReturn") {
        gui_string_ = m.getArgAsString(0);
    }
}

//------------------------------------------------------------------------







