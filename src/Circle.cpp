#include "Circle.h"

//--------------------------------------------------------------
void Circle::setup(int _centerX, int _centerY, int _maxWidth){
    centerX = _centerX;
    centerY = _centerY;
    maxWidth = _maxWidth;
    strokeWidth = 5;
    initTime = 0;
    circlePath.setCircleResolution(50);
    circlePath.setFilled(true);
    circlePath.setFillColor(lineColor);
    circlePath.setPolyWindingMode(OF_POLY_WINDING_ODD);
}
//--------------------------------------------------------------
void Circle::update(){
    initTime += 1;
    auto duration = 800;
    auto endTime = initTime + duration;
    auto now = ofGetElapsedTimef();
    radius = initTime > duration ? maxWidth: easeOutExpo(initTime, 0, maxWidth, duration);
    radius2 = initTime > duration ? (maxWidth-strokeWidth): easeOutExpo(initTime, 0, (maxWidth-strokeWidth), duration);
    if(initTime>duration){
        initTime = 0;
    }
}
//--------------------------------------------------------------
void Circle::draw(){
    circlePath.clear();
    for (int ang = 0; ang <= 360; ang+=5) {
        float rad = ofDegToRad(ang);
        float x = centerX + (radius * cos(rad));
        float y = centerY + (radius * sin(rad));
        circlePath.lineTo(x, y);
    }
    circlePath.close();
    
    for (int ang = 360; ang >= 0; ang-=5) {
        float rad = ofDegToRad(ang);
        float x = centerX + (radius2 * cos(rad));
        float y = centerY + (radius2 * sin(rad));
        if(ang == 360){
            circlePath.moveTo(x, y);
        }
        circlePath.lineTo(x, y);
    }
    
    circlePath.draw();
    
}
//--------------------------------------------------------------
float Circle::easeOutExpo(float t, float b, float c, float d){
  return t == d ? b + c: c * (-pow(2, -10 * t / d) + 1) + b;
}
//--------------------------------------------------------------
void Circle::setColor(ofColor _lineColor){
    lineColor = _lineColor;
    lineColor.a = 200;
}
//--------------------------------------------------------------
void Circle::setInitTime(float _initTime){
    initTime = _initTime;
}
//--------------------------------------------------------------
glm::vec2 Circle::getFirstPos(){
    return glm::vec2(centerX, centerY);
}
