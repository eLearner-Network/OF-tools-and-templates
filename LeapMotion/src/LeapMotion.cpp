#include "LeapMotion.h"

//-----------
void LeapMotion::setup(){
    leap.open();
    cam.setOrientation(ofPoint(-20, 0, 0));
    fbo.allocate(1024, 768, GL_RGBA);
    
    control.registerParameter("trackVelocity", &trackVelocity);
    control.registerParameter("numFrames", &numFrames, 1, 60);
    trackVelocity.addListener(this, &LeapMotion::trackVelocityChanged);
    numFrames.addListener(this, &LeapMotion::numFramesChanged);
    trackVelocity = false;
    numFrames = 15;
}

//-----------
void LeapMotion::setGuiPosition(int x, int y) {
    control.setGuiPosition(x, y);
}

//-----------
void LeapMotion::setVelocityTracking(bool trackVelocity, int numFrames) {
    this->trackVelocity = trackVelocity;
    if (trackVelocity) {
        this->numFrames = numFrames;
        frameWeight = 1.0f / (float) numFrames;
        idxFrame = 0;
        prevHandPosition[0] = ofPoint(0,0,0);
        prevHandPosition[1] = ofPoint(0,0,0);
        handVelocityList[0].resize(numFrames);
        handVelocityList[1].resize(numFrames);
        handVelocity[0] = ofPoint(0, 0);
        handVelocity[1] = ofPoint(0, 0);
        for (int i=0; i<numFrames; i++) {
            handVelocityList[0][i] = ofPoint(0, 0);
            handVelocityList[1][i] = ofPoint(0, 0);
        }
    }
}

//-----------
void LeapMotion::update(){
    simpleHands = leap.getSimpleHands();
    
    // update velocity
    if (trackVelocity) {
        for (int i = 0; i < simpleHands.size(); i++) {
            int idx = simpleHands[i].isLeft ? 0 : 1;
            handVelocity[idx] -= frameWeight * handVelocityList[idx][idxFrame];
            handVelocity[idx] += frameWeight * (simpleHands[i].handPos - prevHandPosition[idx]);
            handVelocityList[idx][idxFrame] = simpleHands[i].handPos - prevHandPosition[idx];
            prevHandPosition[idx] = simpleHands[i].handPos;
        }
        idxFrame = (idxFrame+1) % numFrames;
    }
}

//-----------
vector<ofPoint> LeapMotion::getFingerTips(Handedness hand, bool isNormalized) {
    vector<ofPoint> fingerTips;
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            fingerTips.push_back( simpleHands[i].fingers[THUMB].tip );
            fingerTips.push_back( simpleHands[i].fingers[INDEX].tip );
            fingerTips.push_back( simpleHands[i].fingers[MIDDLE].tip );
            fingerTips.push_back( simpleHands[i].fingers[RING].tip );
            fingerTips.push_back( simpleHands[i].fingers[PINKY].tip );
            if (isNormalized) {
                for (int j=0; j<fingerTips.size(); j++) {
                    fingerTips[j] -= simpleHands[i].handPos;
                }
            }
        }
    }
    return fingerTips;
}

//-----------
ofPoint LeapMotion::getFingerTip(Handedness hand, fingerType finger) {
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            return simpleHands[i].fingers[finger].tip;
        }
    }
}

//-----------
ofPoint LeapMotion::getHandVelocity(Handedness hand) {
    return handVelocity[ (hand == LEFT) ? 0 : 1 ];
}

//-----------
float LeapMotion::getOpenHandSize(Handedness hand) {
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            ofPoint minPos = ofPoint(999, 999, 999);
            ofPoint maxPos = ofPoint(-999, -999, -999);
            for (int j=0; j<5; j++) {
                ofPoint f = simpleHands[i].fingers[fingerTypes[j]].tip;
                if (f.x < minPos.x)         minPos.x = f.x;
                else if (f.x > maxPos.x)    maxPos.x = f.x;
                if (f.y < minPos.y)         minPos.y = f.y;
                else if (f.y > maxPos.y)    maxPos.y = f.y;
                if (f.z < minPos.z)         minPos.z = f.z;
                else if (f.z > maxPos.z)    maxPos.z = f.z;
            }
            return (maxPos.x-minPos.x) * (maxPos.y-minPos.y) * (maxPos.z-minPos.z);
        }
    }
}

//-----------
ofPoint LeapMotion::getHandNormal(Handedness hand) {
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            return simpleHands[i].handNormal;
        }
    }
}

//-----------
ofPoint LeapMotion::getHandDirection(Handedness hand) {
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            return simpleHands[i].direction;
        }
    }
}

//-----------
float LeapMotion::getHandRoll(Handedness hand) {
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            return simpleHands[i].roll;
        }
    }
}

//-----------
float LeapMotion::getHandPitch(Handedness hand) {
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            return simpleHands[i].pitch;
        }
    }
}

//-----------
float LeapMotion::getHandYaw(Handedness hand) {
    for (int i = 0; i < simpleHands.size(); i++) {
        if ((hand==LEFT) == simpleHands[i].isLeft) {
            return simpleHands[i].yaw;
        }
    }
}

//-----------
void LeapMotion::drawVelocityGraph(Handedness hand, int x, int y, int w, int h) {
    ofPoint velocity = getHandVelocity(hand);
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x, y);
    
    ofSetColor(0);
    
    if (hand == LEFT)
        ofDrawBitmapString("Left velocity XY", 0, -4);
    else
        ofDrawBitmapString("Right velocity XY", 0, -4);
    ofDrawBitmapString("Z", w+2, -4);
    ofNoFill();
    ofSetLineWidth(2);

    ofRect(0, 0, w, h);
    ofLine(-5+w/2, h/2, 5+w/2, h/2);
    ofLine(w/2, -5+h/2, w/2, 5+h/2);

    
    ofFill();
    float vx = ofMap(velocity.x, -10, 10, 0, w);
    float vy = ofMap(velocity.y, -10, 10, h, 0);
    float vz = ofMap(velocity.z, -10, 10, 0, h);
    ofEllipse(vx, vy, 16, 16);
    ofEllipse(w+8, vz, 16, 16);
    
    ofPopStyle();
    ofPopMatrix();
}

//-----------
void LeapMotion::draw(int x, int y, int w, int h){
    fbo.begin();
    
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	ofSetColor(200);
	ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
	cam.begin();
    
	ofPushMatrix();
    ofRotate(90, 0, 0, 1);
    ofSetColor(20);
    ofDrawGridPlane(800, 20, false);
	ofPopMatrix();
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
            ofDrawSphere(pip.x, pip.y, pip.z, 12);
            ofDrawSphere(dip.x, dip.y, dip.z, 12);
            ofDrawSphere(tip.x, tip.y, tip.z, 12);
            
            ofSetColor(255, 0, 0);
            ofSetLineWidth(20);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
        }
    }
    ofSetColor(255);
    
	cam.end();
    fbo.end();
    fbo.draw(x, y, w, h);
}

//-----------
LeapMotion::~LeapMotion(){
    trackVelocity.removeListener(this, &LeapMotion::trackVelocityChanged);
    numFrames.removeListener(this, &LeapMotion::numFramesChanged);
}
