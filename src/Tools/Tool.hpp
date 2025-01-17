#pragma once

#include "const.h"
#include "ofMain.h"
#include "ShaderFill.hpp"
#include "PolyLineUtil.hpp"

class Tool {
public:
    enum States {STANDBY, ACTIVE, COMPLETE};
    
    Tool(shared_ptr<ofFbo> _canvas, float _priority = 0);
    
    virtual ofRectangle getBoundingBox();
    virtual float getPriority();
    virtual void setPriority(float p);
    virtual States getState();
    virtual void setup();
    virtual void setupMain();
    virtual void activate();
    virtual void activateMain();
    virtual void update();
    virtual void updateMain();
    virtual void finalize();
    virtual void finalizeMain();
    virtual void debugDraw();
    bool isMultiStep();
protected:
    bool multiStep = false;
    States state = STANDBY;
    shared_ptr<ofFbo> canvas;
    ofRectangle boundingBox;
    float priority;
    int id;
    static int idCnt;
};
