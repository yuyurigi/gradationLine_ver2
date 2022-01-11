#pragma once

#include "ofMain.h"
#include "Polyline.h"
#include "Circle.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<Polyline> polylines;
    vector<bool> b2Colorline;
    int buttonPos; //マウスのボタン位置
    int count; //現在の線のかたまり数
    int vertexNum; //頂点数
    int prevCount, prevVertexNum, colCount;
    void deletePos();
    
    ofImage myImage;
    float dist, radius, initTime;
    Circle circle;
    
    // gui
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameter<int> lineWidth;
    ofParameter<bool> bWire;
    ofParameter<bool> bGradation;
    ofParameterGroup linecap;
    ofParameter<bool> capParameter[3];
    ofParameterGroup linejoin;
    ofParameter<bool> joinParameter[3];
    ofParameterGroup gcolor;
    ofParameter<ofColor> backColor;
    ofParameter<ofColor> color1;
    ofParameter<ofColor> color2;
    ofParameter<ofColor> color3;
    ofParameterGroup gcolor2;
    ofParameter<bool> b2Color;
    ofParameter<ofColor> color1_;
    ofParameter<ofColor> color2_;
    
    void setGui();
    void setLineCap();
    void checkLineCap();
    int cap, join, colNum;
    bool bHide, check[3], jcheck[3], colcheck[2];
    void setLineJoin();
    void checkLineJoin();
};
