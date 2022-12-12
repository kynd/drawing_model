#include "SketchScenario02.hpp"


SketchScenario02::SketchScenario02() {
    colorSampler.load("img/26.jpg");
    
    canvas = shared_ptr<ofFbo>(new ofFbo());
    canvas->allocate(BUFF_WIDTH, BUFF_HEIGHT, fboDepth, samplingDepth);
 
    next();
};


void SketchScenario02::next() {
    canvas->begin();
    backgroundColor = colorSampler.getRandomColor();
    ofClear(backgroundColor);
    canvas->end();
    
    conductor.clear();
    shaderFillTest();
    shaderFillTest();
    shaderFillTest();
}


void SketchScenario02::shaderFillTest() {
    
    vector<ofRectangle> rects = createRandomSquareGrid(ofRandom(1, 4));
    
    for (int i = 0; i < rects.size(); i ++) {
        vector<ofPolyline> polylines = Illustrator::createBlobPath(rects[i]);
        auto tool = randomPathTool(canvas, polylines[0], ofRandom(10));
        conductor.addTool(tool);
    }
};

void SketchScenario02::lineTest() {
    vector<ofRectangle> rects = createRandomGrid(ofRandom(1,5), ofRandom(1,5));
    for (int i = 0; i < rects.size(); i ++) {
        vector<ofPolyline> polylines;
        float rnd = ofRandom(3.0);
        if (rnd < 1.0) {
            polylines = Illustrator::createSquigglePath(rects[i]);
        } else if (rnd < 2.0) {
            polylines = Illustrator::createFastDenseSquigglePath(rects[i]);
        } else {
            polylines = Illustrator::createGridPointPath(rects[i], 4, 4, ofRandom(4, 12));
        }
        
        if (ofRandom(1.0) < 0.25) {
            polylines[0] = PolyLineUtil::noiseWarp(polylines[0], 2, 8, 0.5, ofVec2f(1.f / BUFF_WIDTH), ofVec2f(200));
        }
        auto strokeTool = randomStrokeTool(canvas, polylines[0], 0, 1, 40);
        conductor.addTool(strokeTool);
    }
}

void SketchScenario02::lineTestSpecific() {
    vector<ofRectangle> rects = createRandomGrid(ofRandom(1,5), ofRandom(1,5));
    rects.push_back(ofRectangle(0, 0, BUFF_WIDTH, BUFF_HEIGHT));
    ofPolyline taper = PolyLineUtil::createTaperNtoN(1, 0.2);
    for (int i = 0; i < rects.size(); i ++) {
        vector<ofPolyline> polylines;
        polylines = Illustrator::createGridPointPath(rects[i], 4, 4, ofRandom(4, 12));
        //createSquigglePath(rects[i]);
        float w = ofRandom(16, 128) * 2;
        float freq = w / ofRandom(8, 64);
        auto style = shared_ptr<StrokeStyle>(new StripeStrokeStyle(colorSampler.getRandomColor(), colorSampler.getRandomColor(), freq));
        auto tool = shared_ptr<Tool>(new RoundStrokeTool(canvas, ofRandom(0,10), style, polylines[0], w, taper));
        conductor.addTool(tool);
    }
}


void SketchScenario02::maskTest() {
    vector<ofRectangle> rects = createRandomSquareGrid(ofRandom(1, 16));
    for (int i = 0; i < rects.size(); i ++) {
        MaskTool* mt = new MaskTool(canvas, ofRandom(10));
        auto maskTool = shared_ptr<Tool>(mt);
        auto _fill = new NoiseGradientShaderFill(ofFloatColor(1,1,1,1),ofFloatColor(0,0,0,1));
        _fill->setRadial(ofVec2f(ofRandom(1.0), ofRandom(1.0)), ofRandom(1.0));
        shared_ptr<ShaderFill> fill = shared_ptr<ShaderFill>(_fill);
        
        vector<ofPolyline> polylines = Illustrator::createBlobPath(rects[i]);
        auto tool = shared_ptr<Tool>(new ShaderFillTool(mt->getMask(), -1, fill, polylines[0]));
        mt->addMask(tool);
        
        for (int j = 0; j < 2; j ++) {
            vector<ofPolyline> polylines = Illustrator::createBlobPath(rects[i]);
            auto tool = randomPathTool(mt->getLocalCanvas(), polylines[0]);
            if (ofRandom(1.0) < 0.5) {
                mt->addTool(tool);
            }
            auto strokeTool = randomStrokeTool(mt->getLocalCanvas(), polylines[0], 0, 10, 40);
            if (ofRandom(1.0) < 0.5) {
                mt->addTool(strokeTool);
            }
        }
        {
            vector<ofPolyline> polylines = Illustrator::createSquigglePath(rects[i]);
            auto strokeTool = randomStrokeTool(mt->getLocalCanvas(), polylines[0], 0, 10, 40);
            mt->addTool(strokeTool);
        }
        conductor.addTool(maskTool);
    }
}


void SketchScenario02::groupTest() {
    
    ofPolyline bgRect = PolyLineUtil::rectangle(0, 0, BUFF_WIDTH, BUFF_HEIGHT);
    conductor.addTool(randomPathTool(canvas, bgRect, 20));
    
    int n = ofRandom(16, 24);
    for (int i = 0; i < n; i ++) {
        GroupTool* gt = new GroupTool(canvas, ofRandom(10));
        auto groupTool = shared_ptr<Tool>(gt);
        auto _fill = new NoiseGradientShaderFill(colorSampler.getRandomColor(),colorSampler.getRandomColor());
        _fill->setRadial(ofVec2f(ofRandom(1.0), ofRandom(1.0)), ofRandom(1.0));
        shared_ptr<ShaderFill> fill = shared_ptr<ShaderFill>(_fill);
        
        ofVec3f center(ofRandom(BUFF_WIDTH), ofRandom(BUFF_HEIGHT), 0);
        for (int j = 0; j < 48; j ++) {
            float angA = ofRandom(TWO_PI);
            float angB = angA + ofRandom(TWO_PI) / 16;
            float distA = ofRandom(BUFF_WIDTH / 4);
            float distB = ofRandom(BUFF_WIDTH / 4);
            ofVec3f pA = center + ofVec3f(cos(angA) * distA, sin(angA) * distA);
            
            ofVec3f pB = center + ofVec3f(cos(angB) * distB, sin(angB) * distB);
            ofPolyline polyline;
            polyline.addVertex(center);
            polyline.addVertex(pA);
            polyline.addVertex(pB);
            
            
            polyline = PolyLineUtil::noiseWarp(polyline, 2, 8, 0.5, ofVec2f(1.f / BUFF_WIDTH), ofVec2f(200));
            
            
            auto tool = shared_ptr<Tool>(new ShaderFillTool(canvas, 0, fill, polyline));
            gt->addTool(tool);
        }
        conductor.addTool(groupTool);
    }
}
