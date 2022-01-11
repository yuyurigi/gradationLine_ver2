#include "ofMain.h"

class Circle {
private:
    int centerX, centerY, maxWidth;
    float duration, radius, radius2, initTime, strokeWidth;
    ofPath circlePath;
    ofColor lineColor;
    float easeOutExpo(float t, float b, float c, float d);
public:
    void setup(int _centerX, int _centerY, int _maxWidth);
    void update();
    void draw();
    void setColor(ofColor _lineColor);
    void setInitTime(float _initTime);
    glm::vec2 getFirstPos();
};
