#include "PaintTextureShaderFill.hpp"


ofShader PaintTextureShaderFill::shader;
ofShader PaintTextureShaderFill::noiseShader;
ofFbo PaintTextureShaderFill::noise;
ofFbo PaintTextureShaderFill::dummy;
bool PaintTextureShaderFill::isInitialized = false;

PaintTextureShaderFill::PaintTextureShaderFill(ofColor _colorA, ofColor _colorB, ofVec2f _dir) {
    if (!isInitialized) {
        initialize();
        isInitialized = true;
    }
    
    colorA = _colorA;
    colorB = _colorB;
    dir = _dir;
}

void PaintTextureShaderFill::initialize() {
    shader.load("shader/basic.vs", "shader/paintTexture/paintTexture.fs");
    noiseShader.load("shader/basic.vs", "shader/paintTexture/noise.fs");
    noise.allocate(BUFF_WIDTH, BUFF_HEIGHT, fboDepth, samplingDepth);
    dummy.allocate(BUFF_WIDTH, BUFF_HEIGHT, fboDepth, samplingDepth);
    noise.begin();
    ofClear(0, 0, 0);
    noiseShader.begin();
    noiseShader.setUniform2f("res", BUFF_WIDTH, BUFF_HEIGHT);
    dummy.draw(0, 0);
    noiseShader.end();
    noise.end();
}

void PaintTextureShaderFill::begin() {
    shader.begin();
    shader.setUniform4f("colorA", colorA);
    shader.setUniform4f("colorB", colorB);
    shader.setUniform2f("dir", dir);
    shader.setUniformTexture("noise", noise, 2);
}

void PaintTextureShaderFill::end() {
    shader.end();
};

ofShader& PaintTextureShaderFill::getShader() {
    return shader;
}
