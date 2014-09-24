#pragma once

#include "ofMain.h"
#include "Scene.h"


enum DrawStrategy { RECTS, DIAMONDS, CIRCLES };


class Subdivision
{
public:
    Subdivision(int generation,
                int x, int y,
                int width, int height,
                ofColor *color,
                ofColor *varColor,
                int *circleResolution,
                bool *isLerp);
    Subdivision(int generation,
                Subdivision *parent,
                bool topleft,
                int *circleResolution,
                bool *isLerp);
    void draw(DrawStrategy drawStrategy);

private:
    
    void subdivide();
    void update();
    
    void drawRect();
    void drawDiamond();
    void drawCircle();

    Subdivision *parent, *child1, *child2;
    int generation;
    float ratio;
    bool topleft, horizontal;

    int x, y;
    int width, height;
    
    ofColor *color;
    ofColor *varColor;
    int *circleResolution;
    bool *isLerp;
    ofVec3f offset;
};


class Subdivide : public Scene
{
public:
    void setup();
    void update();
    void draw();

private:
    void setDrawType(string & s);
    
    DrawStrategy drawStrategy = CIRCLES;

    int ndivisions;
    ofColor color;
    ofColor varColor;
    int circleResolution;
    bool isLerp;
    Subdivision *start;
};
