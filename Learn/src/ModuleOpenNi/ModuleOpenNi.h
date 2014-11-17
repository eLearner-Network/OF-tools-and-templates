#pragma once

#include "ofMain.h"
#include "Learn.h"
#include "OpenNi.h"


class ModuleOpenNi : public Learn
{
public:
    ~ModuleOpenNi() {openNi.close();}
    ModuleOpenNi();
    
    void update();
    void draw();
    
    void addJointsAsInput();
    void addUpperBodyAsInput();
    
    void setGuiPosition(int x, int y) {
        this->x = x;
        this->y = y;
        openNi.setGuiPosition(x, y);
    }
    void toggleVisible();
    void toggleOpenNiVisible() {openNi.toggleGuiVisible();}
    
protected:
    
    OpenNi openNi;
    
    int x, y;

};
