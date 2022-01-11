//reference:https://www.codeproject.com/Articles/226569/Drawing-polylines-by-tessellation

#pragma once

#include "ofMain.h"

class Polyline {
private:
    vector<glm::vec2> pos;
    vector<glm::vec2> mid;
    float lineWidth, half_lineWidth; //線の太さ
    float dangle; //angle for each step (strokecap(round), strokejoin(round))
    ofColor lineColor[3];
    bool bClose, bWire, bGradation, b2Color;
    int linecap, linejoin, colCount;
    glm::vec2 calcMidPoint(glm::vec2 end1, glm::vec2 end2);
    void drawCap_butt();
    void drawCap_round_first(glm::vec2 p0, glm::vec2 p1);
    void drawCap_round_last(glm::vec2 p2, glm::vec2 p3, int lColor);
    void drawCap_square_first(glm::vec2 p0, glm::vec2 p1);
    void drawCap_square_last(glm::vec2 p2, glm::vec2 p3, int lColor);
    void drawLine_first();
    void drawLine_last(int lColor);
    void drawLine_miter(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2, ofColor lColor[3]);
    void drawLine_bevel(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2, ofColor lColor[3]);
    void drawLine_round(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2, ofColor lColor[3]);
    glm::vec2 perpen(glm::vec2 P);
    glm::vec2 inverse(glm::vec2 P);
    bool signed_area(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3);
    int intersect(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3, glm::vec2 P4, glm::vec2 & Pout);
    void basic_vectors_arc(glm::vec2 P, glm::vec2 A, glm::vec2 B, float r, vector<glm::vec2> & q, bool clockwise);
    int colorNumber(int cn);
    void setLineColor(int midColor, ofColor returnColor[3]);
    void setLineColor2(int midColor, ofColor returnColor[3]);
    void setLineColor3(int prevColor, ofColor returnColor[3]);

public:
    Polyline(); //コンストラクタ
    void draw();
    void setPos(glm::vec2 _pos);
    glm::vec2 getFirstPos();
    int getPosNum();
    void setColCount(int _colCount);
    void set2Color(bool _b2Color);
    void setColor(ofColor _lineColor0, ofColor _lineColor1);
    void setColor(ofColor _lineColor0, ofColor _lineColor1, ofColor _lineColor2);
    void setLineWidth(float _lineWidth);
    void setClose(bool _bClose);
    void setLineCap(int _linecap);
    void setLineJoin(int _linejoin);
    void setWireframe(bool _bWire);
    void setGradation(bool _bGradation);
    void deletePoint(int pattern);
    bool getClose();
};
