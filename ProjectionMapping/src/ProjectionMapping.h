#pragma once

#include "ofMain.h"
#include "ofxQuad.h"
#include "ofxPointInPolyline.h"


class Quad
{
public:
    Quad();
    
    void drawDebug();
    
    void begin();
    void end();

    ofPoint getInputCorner(int idx);
    ofPoint getOutputCorner(int idx);
    
    void setInputCorner(int idx, int x, int y);
    void setOutputCorner(int idx, int x, int y);
    
    void moveOutputCorner(int x, int y);
    void grabOutputCorner(int x, int y);
    void releaseOutputCorner();
    
    void setDebug(bool debugging, bool debugMe) {
        this->debugging = debugging;
        this->debugMe = debugMe;
    }
    
private:
    bool debugging, debugMe;
    int selectedCorner;
    ofxQuad quad;
    ofxPointInPolyline pipl;
    ofPoint lastGrabbedPoint;
};



class ProjectionMapping 
{
public:
    ProjectionMapping();
    
    void setMouseResolution(int x, int y);
    
    void addQuad(int width, int height);

    void begin(int idx) { quads[idx]->begin(); }
    void end(int idx)   { quads[idx]->end(); }
    
    void mousePressed(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void keyPressed(ofKeyEventArgs &e);
    
    void toggleDebug();
    void toggleSelectedQuad(int shift=1);
    bool getDebug() { return debug; }
    
    void loadPreset(string path);
    void savePreset(string path);
    
private:
    vector<Quad *> quads;
    bool debug;
    int selected;
    ofPoint mouseResolution;
};

