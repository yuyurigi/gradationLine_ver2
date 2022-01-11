#include "Polyline.h"

Polyline::Polyline(){
    bClose = false;
    bWire = false;
    bGradation = true;
    dangle = PI/18;
}

//--------------------------------------------------------------
void Polyline::draw(){
    //頂点がひとつだけのとき、点を描く
    if(pos.size() == 1){
        if(!b2Color){
            ofSetColor(lineColor[colCount]);
        } else { //２色カラーのとき
            ofSetColor(lineColor[0]);
        }
        ofSetLineWidth(10);
        ofNoFill();
        ofDrawEllipse(pos[0].x, pos[0].y, 10, 10);
    }
    //基本のライン、頂点に◯をつける
    /*
    if(pos.size() > 1){
        ofSetColor(0);
        ofSetLineWidth(1);
        ofBeginShape();
        for (int i = 0; i < pos.size(); i++) {
            ofVertex(pos[i].x, pos[i].y);
        }
        if(bClose) ofEndShape(true);
        else ofEndShape();
    }
    //中間点に円を描画
    for (int i = 0; i < mid.size(); i++) {
        ofSetColor(0);
        ofDrawEllipse(mid[i].x, mid[i].y, 5, 5);
    }
     */
    
    int lc;
    ofColor lColor[3];
    
    if (pos.size()>=2 && !bClose) {
        lc = ((int)pos.size()-1)%3;
        
        switch (linecap) {
            case 0:
                drawCap_butt();
                break;
            case 1:
                drawCap_round_first(pos[0], pos[1]);
                break;
            case 2:
                drawCap_square_first(pos[0], pos[1]);
                break;
        }
        drawLine_first();
    }  
    
    if (pos.size() >= 3) {
        switch (linejoin) {
            case 0:
                if(!bClose){
                    for (int i = 0; i < mid.size()-1; i++){
                        lc = colorNumber(i);
                        setLineColor(lc, lColor);
                        drawLine_miter(mid[i], pos[i+1], mid[i+1], lColor);
                    }
                } else {
                    for (int i = 0; i < mid.size()-2; i++){
                        lc = colorNumber(i);
                        setLineColor(lc, lColor);
                        drawLine_miter(mid[i], pos[i+1], mid[i+1], lColor);
                    }
                    lc = colorNumber((int)mid.size()-2);
                    setLineColor2(lc, lColor);
                    drawLine_miter(mid[mid.size()-2], pos[pos.size()-1], mid[mid.size()-1], lColor);
                    lc = colorNumber((int)mid.size()-2);
                    setLineColor3(lc, lColor);
                    drawLine_miter(mid[mid.size()-1], pos[0], mid[0], lColor);
                }
                break;
            case 1:
                if(!bClose){ //ラインを閉じてないとき
                    for (int i = 0; i < mid.size()-1; i++){
                        lc = colorNumber(i);
                        setLineColor(lc, lColor);
                        drawLine_bevel(mid[i], pos[i+1], mid[i+1], lColor);
                    }
                } else {  //ラインを閉じたとき
                    for (int i = 0; i < mid.size()-2; i++){
                        lc = colorNumber(i);
                        setLineColor(lc, lColor);
                        drawLine_bevel(mid[i], pos[i+1], mid[i+1], lColor);
                    }
                    lc = colorNumber((int)mid.size()-2);
                    setLineColor2(lc, lColor);
                    drawLine_bevel(mid[mid.size()-2], pos[pos.size()-1], mid[mid.size()-1], lColor);
                    lc = colorNumber((int)mid.size()-2);
                    setLineColor3(lc, lColor);
                    drawLine_bevel(mid[mid.size()-1], pos[0], mid[0], lColor);
                }
                break;
            case 2:
                if(!bClose){
                for (int i = 0; i < mid.size()-1; i++){
                    lc = colorNumber(i);
                    setLineColor(lc, lColor);
                    drawLine_round(mid[i], pos[i+1], mid[i+1], lColor);
                }
                } else {
                    for (int i = 0; i < mid.size()-2; i++){
                        lc = colorNumber(i);
                        setLineColor(lc, lColor);
                        drawLine_round(mid[i], pos[i+1], mid[i+1], lColor);
                    }
                    lc = colorNumber((int)mid.size()-2);
                    setLineColor2(lc, lColor);
                    drawLine_round(mid[mid.size()-2], pos[pos.size()-1], mid[mid.size()-1], lColor);
                    lc = colorNumber((int)mid.size()-2);
                    setLineColor3(lc, lColor);
                    drawLine_round(mid[mid.size()-1], pos[0], mid[0], lColor);
                }
                break;
        }
    }
    
    if (pos.size()>=2 && !bClose) {
        if(!b2Color){
            lc = (colCount + (int)pos.size()-1)%3;
        } else {
            lc = ((int)pos.size()-1)%2;
        }
        
        drawLine_last(lc);
        
        switch (linecap) {
            case 0:
                drawCap_butt();
                break;
            case 1:
                drawCap_round_last(pos[pos.size()-2], pos[pos.size()-1], lc);
                break;
            case 2:
                drawCap_square_last(pos[pos.size()-2], pos[pos.size()-1], lc);
                break;
        }
    }
}

//--------------------------------------------------------------

int Polyline::colorNumber(int cn){
    if(!b2Color){ //３色
        int returnNum = (colCount+cn)%3;
        if (returnNum == 0) {
            returnNum = 1;
        } else if (returnNum == 1){
            returnNum = 2;
        } else {
            returnNum = 0;
        }
        return returnNum;
    } else { //２色
        int returnNum = (1+cn)%2;
        return returnNum;
    }
}

//--------------------------------------------------------------
void Polyline::setLineColor(int midColor, ofColor returnColor[3]){
    int prevColor, nextColor;
    if(!b2Color){ //３色
        prevColor = midColor-1;
        if (prevColor == -1)  prevColor = 2;
        nextColor = midColor+1;
        if (nextColor == 3) nextColor = 0;
        
    } else { //２色
        prevColor = midColor-1;
        if (prevColor == -1) prevColor = 1;
        nextColor = midColor+1;
        if (nextColor == 2) nextColor = 0;
    }
    
    ofColor lerpColor0 = lineColor[prevColor];
    lerpColor0.lerp(lineColor[midColor], 0.5);
    ofColor lerpColor1 = lineColor[nextColor];
    lerpColor1.lerp(lineColor[midColor], 0.5);
    returnColor[0] = lerpColor0;
    returnColor[1] = lineColor[midColor];
    returnColor[2] = lerpColor1;
}
//--------------------------------------------------------------

void Polyline::setLineColor2(int midColor, ofColor returnColor[3]){
    int prevColor, nextColor;
    if(!b2Color){ //３色
        prevColor = midColor-1;
        if (prevColor == -1) prevColor = 2;
        nextColor = colCount;
    } else { //２色
        prevColor = midColor-1;
        if (prevColor == -1) prevColor = 1;
        nextColor = 0;
    }
        
    ofColor lerpColor0 = lineColor[prevColor];
    lerpColor0.lerp(lineColor[midColor], 0.5);
    ofColor lerpColor1 = lineColor[nextColor];
    lerpColor1.lerp(lineColor[midColor], 0.5);
    returnColor[0] = lerpColor0;
    returnColor[1] = lineColor[midColor];
    returnColor[2] = lerpColor1;
}
//--------------------------------------------------------------
void Polyline::setLineColor3(int prevColor, ofColor returnColor[3]){
    int midColor, nextColor;
    if(!b2Color){ //３色
        midColor = colCount;
        nextColor = (colCount+1)%3;
    } else { //２色
        midColor = 0;
        nextColor = 1;
    }
        
    ofColor lerpColor0 = lineColor[prevColor];
    lerpColor0.lerp(lineColor[midColor], 0.5);
    ofColor lerpColor1 = lineColor[nextColor];
    lerpColor1.lerp(lineColor[midColor], 0.5);
    returnColor[0] = lerpColor0;
    returnColor[1] = lineColor[midColor];
    returnColor[2] = lerpColor1;
}
//--------------------------------------------------------------
glm::vec2 Polyline::calcMidPoint(glm::vec2 end1, glm::vec2 end2){
    float mx, my;
    if(end1.x > end2.x){
        mx = end2.x + ((end1.x - end2.x)/2);
    } else {
        mx = end1.x + ((end2.x - end1.x)/2);
    }
    if (end1.y > end2.y) {
        my = end2.y + ((end1.y - end2.y)/2);
    } else {
        my = end1.y + ((end2.y - end1.y)/2);
    }
    return glm::vec2(mx, my);
}

//--------------------------------------------------------------
void Polyline::setPos(glm::vec2 _pos) {
    //配列に頂点を追加
    pos.push_back(_pos);
    
    //頂点と頂点の中間点を計算・追加
    if(pos.size()>=2){
        glm::vec2 thisMP = calcMidPoint(pos[pos.size()-2], pos[pos.size()-1]);
        mid.push_back(thisMP);
    }
}

//--------------------------------------------------------------
glm::vec2 Polyline::getFirstPos(){
    return pos[0];
}
//--------------------------------------------------------------
int Polyline::getPosNum(){
    return (int)pos.size();
}
//--------------------------------------------------------------
void Polyline::setColCount(int _colCount){
    colCount = _colCount;
}
//--------------------------------------------------------------
void Polyline::set2Color(bool _b2Color){ //線を２色カラーにするかどうかのbool値
    b2Color = _b2Color;
}
//--------------------------------------------------------------
void Polyline::setColor(ofColor _lineColor0, ofColor _lineColor1){ //２色カラー
    lineColor[0] = _lineColor0;
    lineColor[1] = _lineColor1;
    lineColor[2] = ofColor(255); //ダミー
}
//--------------------------------------------------------------
void Polyline::setColor(ofColor _lineColor0, ofColor _lineColor1, ofColor _lineColor2){ //３色
    lineColor[0] = _lineColor0;
    lineColor[1] = _lineColor1;
    lineColor[2] = _lineColor2;
}

//--------------------------------------------------------------
void Polyline::setLineWidth(float _lineWidth){
    lineWidth = _lineWidth;
    half_lineWidth = lineWidth/2;
}
//--------------------------------------------------------------
void Polyline::setClose(bool _bClose){
    bClose = _bClose;
    
    //線を閉じたとき、最後の頂点と最初の頂点の中間点を追加
    if(bClose){
        glm::vec2 thisMP = calcMidPoint(pos[0], pos[pos.size()-1]);
        mid.push_back(thisMP);
    }
}
//--------------------------------------------------------------
void Polyline::setWireframe(bool _bWire){
    bWire = _bWire;
}
//--------------------------------------------------------------
void Polyline::setGradation(bool _bGradation){
    bGradation = _bGradation;
}
//--------------------------------------------------------------
void Polyline::setLineCap(int _linecap){
    linecap = _linecap;
}

//--------------------------------------------------------------
void Polyline::setLineJoin(int _linejoin){
    linejoin = _linejoin;
}
//--------------------------------------------------------------
void Polyline::deletePoint(int pattern){
    if(pattern == 1){
    pos.pop_back();
    mid.pop_back();
    } else if(pattern == 2){
        mid.pop_back();
    }
    
}
//--------------------------------------------------------------
bool Polyline::getClose(){
    return bClose;
}
//--------------------------------------------------------------
//stroke-linecap
//--------------------------------------------------------------
void Polyline::drawCap_butt(){
}
//--------------------------------------------------------------
void Polyline::drawCap_round_first(glm::vec2 p0, glm::vec2 p1){
    vector<glm::vec2> A;
    
    //最初のキャップ
    float angle1 = atan2(p1.y-p0.y, p1.x-p0.x);
    if (angle1 < 0) {angle1 = TWO_PI + angle1;}
    angle1 = angle1+HALF_PI;
    for (float ang = angle1; ang <= angle1 + PI; ang += dangle) {
        float x = p0.x + (half_lineWidth * cos(ang));
        float y = p0.y + (half_lineWidth * sin(ang));
        A.push_back(glm::vec2(x, y));
    }
    
    ofMesh mesh1;
    mesh1.addVertex(ofPoint(p0.x, p0.y));
    for (int i = 0; i < A.size(); i++) {
        mesh1.addVertex(ofPoint(A[i].x, A[i].y, 0));
    }
    
    for (int i = 1; i < A.size(); i++) {
        mesh1.addIndex(0);
        mesh1.addIndex(i);
        mesh1.addIndex(i+1);
    }
    
    if(!bWire){
        ofSetColor(lineColor[colCount]);
        mesh1.draw();
    } else {
        ofSetColor(lineColor[colCount]);
        mesh1.drawWireframe();
    }
}
//--------------------------------------------------------------
void Polyline::drawCap_round_last(glm::vec2 p2, glm::vec2 p3, int lColor){
    vector<glm::vec2> A2;
    //最後のキャップ
    float angle2 = atan2(p2.y-p3.y, p2.x-p3.x);
    if (angle2 < 0) {angle2 = TWO_PI + angle2;}
    angle2 = angle2 + HALF_PI;
    for (float ang = angle2; ang <= angle2+PI; ang += dangle) {
        float x = p3.x + (half_lineWidth * cos(ang));
        float y = p3.y + (half_lineWidth * sin(ang));
        A2.push_back(glm::vec2(x, y));
    }
    ofMesh mesh2;
    mesh2.addVertex(ofPoint(p3.x, p3.y));
    for (int i = 0; i < A2.size(); i++) {
        mesh2.addVertex(ofPoint(A2[i].x, A2[i].y, 0));
    }
    
    for (int i = 1; i < A2.size(); i++) {
        mesh2.addIndex(0);
        mesh2.addIndex(i);
        mesh2.addIndex(i+1);
    }
    
    if(!bWire){
        ofSetColor(lineColor[lColor]);
        mesh2.draw();
    } else {
        ofSetColor(lineColor[lColor]);
        mesh2.drawWireframe();
    }
    
}
//--------------------------------------------------------------
void Polyline::drawCap_square_first(glm::vec2 p0, glm::vec2 p1){
    glm::vec2 P;
    glm::vec2 T[4];
    glm::vec2 base;
    //最初のキャップ
    base = p1-p0;
    base = glm::normalize(base);
    base *= half_lineWidth;
    P = p0 + inverse(base);
    
    T[0] = p0 + perpen(base);
    T[1] = p0 + inverse(perpen(base));
    T[2] = P + perpen(base);
    T[3] = P + inverse(perpen(base));
    
    ofMesh mesh1;
    mesh1.addVertex(ofPoint(T[0].x, T[0].y, 0));
    mesh1.addVertex(ofPoint(T[1].x, T[1].y, 0));
    mesh1.addVertex(ofPoint(T[2].x, T[2].y, 0));
    mesh1.addVertex(ofPoint(T[3].x, T[3].y, 0));
    mesh1.addIndex(0);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(3);
    
    if(!bWire){
        ofSetColor(lineColor[0]);
        mesh1.draw();
    } else {
        ofSetColor(lineColor[0]);
        mesh1.drawWireframe();
    }
}
//--------------------------------------------------------------
void Polyline::drawCap_square_last(glm::vec2 p2, glm::vec2 p3, int lColor){
    glm::vec2 P;
    glm::vec2 T[4];
    glm::vec2 base;
    //最後のキャップ
    base = p2-p3;
    base = glm::normalize(base);
    base *= half_lineWidth;
    P = p3 + inverse(base);
    
    T[0] = p3 + perpen(base);
    T[1] = p3 + inverse(perpen(base));
    T[2] = P + perpen(base);
    T[3] = P + inverse(perpen(base));
    
    ofMesh mesh2;
    mesh2.addVertex(ofPoint(T[0].x, T[0].y, 0));
    mesh2.addVertex(ofPoint(T[1].x, T[1].y, 0));
    mesh2.addVertex(ofPoint(T[2].x, T[2].y, 0));
    mesh2.addVertex(ofPoint(T[3].x, T[3].y, 0));
    mesh2.addIndex(0);
    mesh2.addIndex(1);
    mesh2.addIndex(2);
    mesh2.addIndex(1);
    mesh2.addIndex(2);
    mesh2.addIndex(3);
    
    if(!bWire){
        ofSetColor(lineColor[lColor]);
        mesh2.draw();
    } else {
        ofSetColor(lineColor[lColor]);
        mesh2.drawWireframe();
    }
}
//--------------------------------------------------------------
//line-first-last
//--------------------------------------------------------------
void Polyline::drawLine_first(){
    glm::vec2 base;
    glm::vec2 T[4];
    //最初のライン
    base = mid[0]-pos[0];
    base = glm::normalize(base);
    base *= half_lineWidth;
    T[0] = pos[0] + perpen(base);
    T[1] = pos[0] + inverse(perpen(base));
    T[2] = mid[0] + perpen(base);
    T[3] = mid[0] + inverse(perpen(base));
        
    ofMesh mesh1;
    mesh1.addVertex(ofPoint(T[0].x, T[0].y, 0));
    mesh1.addVertex(ofPoint(T[1].x, T[1].y, 0));
    mesh1.addVertex(ofPoint(T[2].x, T[2].y, 0));
    mesh1.addVertex(ofPoint(T[3].x, T[3].y, 0));
    mesh1.addIndex(0);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(3);
    if(bGradation){
        mesh1.addColor(lineColor[colCount]);
        mesh1.addColor(lineColor[colCount]);
        ofColor lerpColor = lineColor[(colCount+1)%3];
        lerpColor.lerp(lineColor[colCount], 0.5);
        mesh1.addColor(lerpColor);
        mesh1.addColor(lerpColor);
    }
    
    if (!bGradation) {
        ofSetColor(lineColor[colCount]);
    }
    if(!bWire){
        mesh1.draw();
    } else {
        mesh1.drawWireframe();
    }
     
}
//--------------------------------------------------------------
void Polyline::drawLine_last(int lColor){
    glm::vec2 base;
    glm::vec2 T[4];
    //最後のライン
    base = mid[mid.size()-1]-pos[pos.size()-1];
    base = glm::normalize(base);
    base *= half_lineWidth;
    T[0] = mid[mid.size()-1] + perpen(base);
    T[1] = mid[mid.size()-1] + inverse(perpen(base));
    T[2] = pos[pos.size()-1] + perpen(base);
    T[3] = pos[pos.size()-1] + inverse(perpen(base));
    
    ofMesh mesh2;
    mesh2.addVertex(ofPoint(T[0].x, T[0].y, 0));
    mesh2.addVertex(ofPoint(T[1].x, T[1].y, 0));
    mesh2.addVertex(ofPoint(T[2].x, T[2].y, 0));
    mesh2.addVertex(ofPoint(T[3].x, T[3].y, 0));
    mesh2.addIndex(0);
    mesh2.addIndex(1);
    mesh2.addIndex(2);
    mesh2.addIndex(1);
    mesh2.addIndex(2);
    mesh2.addIndex(3);
    if(bGradation){
        int prevColor = lColor-1;
        if(prevColor == -1) {
            if (!b2Color) { //３色
                prevColor = 2;
            } else { //２色
                prevColor = 1;
            }
        }
        ofColor lerpColor0 = lineColor[prevColor];
        lerpColor0.lerp(lineColor[lColor], 0.5);
        mesh2.addColor(lerpColor0);
        mesh2.addColor(lerpColor0);
        mesh2.addColor(lineColor[lColor]);
        mesh2.addColor(lineColor[lColor]);
    }
    
    if(!bGradation){
        ofSetColor(lineColor[lColor]);
    }
    
    if(!bWire){
        mesh2.draw();
    } else {
        mesh2.drawWireframe();
    }
}
//--------------------------------------------------------------
//line-linejoin
//--------------------------------------------------------------
void Polyline::drawLine_miter(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2, ofColor lColor[3]){
    glm::vec2 T[4], aT[2], bT[2], vP[2];
    glm::vec2 base[3];
    base[0] = P1 - P0;
    base[0] = glm::normalize(base[0]);
    base[0] *= half_lineWidth;
    
    base[1] = P1 - P2;
    base[1] = glm::normalize(base[1]);
    base[1] *= half_lineWidth;
    
    float dist0 = ofDist(P0.x, P0.y, P1.x, P1.y);
    float dist1 = ofDist(P1.x, P1.y, P2.x, P2.y);
    
    bool clockwise = signed_area(P0, P1, P2);
    if (!clockwise) {
        base[0] = -base[0];
        base[1] = -base[1];
    }
    T[0] = P0 + perpen(base[0]);
    T[1] = P0 + inverse(perpen(base[0]));
    T[2] = P2 + inverse(perpen(base[1]));
    T[3] = P2 + perpen(base[1]);
        
    aT[0] = P1+perpen(base[0]);
    aT[1] = P1 + inverse(perpen(base[0]));
    bT[0] = P1+inverse(perpen(base[1]));
    bT[1] = P1+perpen(base[1]);
    
    int inter = intersect(T[1], aT[1], T[3], bT[1], vP[1]);
    
    //線と線が重なり合ってるとき、新しいvP[1]を計算する
    if ((inter == 3) || (inter == 4) || (inter == 5)) {
        if (clockwise) {
            if(dist1 < dist0){
                intersect(T[2], T[3], aT[1], T[1], vP[1]);
            }else {
                intersect(T[1], T[0], bT[1], T[3], vP[1]);
            }
        } else {
            if(dist0 < dist1){
                intersect(T[0], T[1], bT[1], T[3], vP[1]);
            } else {
                intersect(T[1], aT[1], T[3], T[2], vP[1]);
            }
        }
    }
    vP[0] = P1+P1-vP[1];
    
    ofMesh mesh;
    if(inter ==1 || inter == 2){
        mesh.addVertex(ofPoint(T[0].x, T[0].y, 0));   //0
        mesh.addVertex(ofPoint(T[1].x, T[1].y, 0));   //1
        mesh.addVertex(ofPoint(vP[1].x, vP[1].y, 0)); //2
        mesh.addVertex(ofPoint(T[3].x, T[3].y, 0));   //3
        mesh.addVertex(ofPoint(T[2].x, T[2].y, 0));   //4
        mesh.addVertex(ofPoint(bT[0].x, bT[0].y, 0)); //5
        mesh.addVertex(ofPoint(vP[0].x, vP[0].y, 0)); //6
        mesh.addVertex(ofPoint(aT[0].x, aT[0].y, 0)); //7
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(0);
        mesh.addIndex(2);
        mesh.addIndex(7);
        mesh.addIndex(2);
        mesh.addIndex(5);
        mesh.addIndex(7);
        mesh.addIndex(5);
        mesh.addIndex(6);
        mesh.addIndex(7);
        mesh.addIndex(2);
        mesh.addIndex(4);
        mesh.addIndex(5);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(4);
        if(bGradation){
            mesh.addColor(lColor[0]);   //0
            mesh.addColor(lColor[0]);   //1
            mesh.addColor(lColor[1]); //2
            mesh.addColor(lColor[2]);   //3
            mesh.addColor(lColor[2]);   //4
            mesh.addColor(lColor[1]); //5
            mesh.addColor(lColor[1]); //6
            mesh.addColor(lColor[1]); //7
        }
    } else {
        //線と線が重なり合ってるときはbevelの描き方をする
        mesh.addVertex(ofPoint(T[0].x, T[0].y, 0));   //0
        mesh.addVertex(ofPoint(T[1].x, T[1].y, 0));   //1
        mesh.addVertex(ofPoint(aT[0].x, aT[0].y, 0)); //2
        mesh.addVertex(ofPoint(bT[0].x, bT[0].y, 0)); //3
        mesh.addVertex(ofPoint(T[3].x, T[3].y, 0));   //4
        mesh.addVertex(ofPoint(T[2].x, T[2].y, 0));   //5
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(4);
        mesh.addIndex(3);
        mesh.addIndex(4);
        mesh.addIndex(5);
        if(bGradation){
            mesh.addColor(lColor[0]);   //0
            mesh.addColor(lColor[0]);   //1
            mesh.addColor(lColor[1]); //2
            mesh.addColor(lColor[1]);   //3
            mesh.addColor(lColor[2]);   //4
            mesh.addColor(lColor[2]); //5
        }
    }
    
    if (!bGradation) {
        ofSetColor(lColor[1]);
    }
    if(!bWire){
        mesh.draw();
    } else {
        mesh.drawWireframe();
    }
    
    //text(point position)
    /*
    ofSetColor(0, 0, 0);
    ofNoFill();
    ofSetLineWidth(1);
    ofTrueTypeFont font;
    font.load("NotoSansJP-Light.otf", 10);
    ofDrawEllipse(T[0].x, T[0].y, 5, 5);
    font.drawString("T[0]", T[0].x, T[0].y);
    ofDrawEllipse(T[1].x, T[1].y, 5, 5);
    font.drawString("T[1]", T[1].x, T[1].y);
    ofDrawEllipse(T[2].x, T[2].y, 5, 5);
    font.drawString("T[2]", T[2].x, T[2].y);
    ofDrawEllipse(T[3].x, T[3].y, 5, 5);
    font.drawString("T[3]", T[3].x, T[3].y);
    ofDrawEllipse(aT[0].x, aT[0].y, 5, 5);
    font.drawString("aT", aT[0].x, aT[0].y);
    ofDrawEllipse(bT[0].x, bT[0].y, 5, 5);
    font.drawString("bT", bT[0].x, bT[0].y);
    ofDrawEllipse(aT[1].x, aT[1].y, 5, 5);
    font.drawString("-aT", aT[1].x, aT[1].y);
    ofDrawEllipse(bT[1].x, bT[1].y, 5, 5);
    font.drawString("-bT", bT[1].x, bT[1].y);
    ofDrawEllipse(vP[0].x, vP[0].y, 5, 5);
    font.drawString("vP", vP[0].x, vP[0].y);
    ofDrawEllipse(vP[1].x, vP[1].y, 5, 5);
    font.drawString("-vP", vP[1].x, vP[1].y);
    string s = ofToString(inter);
    font.drawString(s, P1.x, P1.y);
     */
}
//--------------------------------------------------------------
void Polyline::drawLine_bevel(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2, ofColor lColor[3]){
    glm::vec2 T[4], aT[2], bT[2], vP[2];
    glm::vec2 base[3];
    base[0] = P1 - P0;
    base[0] = glm::normalize(base[0]);
    base[0] *= half_lineWidth;
    
    base[1] = P1 - P2;
    base[1] = glm::normalize(base[1]);
    base[1] *= half_lineWidth;
    
    float dist0 = ofDist(P0.x, P0.y, P1.x, P1.y);
    float dist1 = ofDist(P1.x, P1.y, P2.x, P2.y);
    
    //float clock = signed_area(P0, P1, P2);
    bool clockwise = signed_area(P0, P1, P2);
    if (!clockwise) {
        base[0] = -base[0];
        base[1] = -base[1];
    }
    T[0] = P0 + perpen(base[0]);
    T[1] = P0 + inverse(perpen(base[0]));
    T[2] = P2 + inverse(perpen(base[1]));
    T[3] = P2 + perpen(base[1]);
        
    aT[0] = P1+perpen(base[0]);
    aT[1] = P1 + inverse(perpen(base[0]));
    bT[0] = P1+inverse(perpen(base[1]));
    bT[1] = P1+perpen(base[1]);
    
    int inter = intersect(T[1], aT[1], T[3], bT[1], vP[1]);
    
    //線と線が重なり合ってるとき、新しいvP[1]を計算する
    if ((inter == 3) || (inter == 4) || (inter == 5)) {
        if (clockwise) {
            if(dist1 < dist0){
                intersect(T[2], T[3], aT[1], T[1], vP[1]);
            }else {
                intersect(T[1], T[0], bT[1], T[3], vP[1]);
            }
        } else {
            if(dist0 < dist1){
                intersect(T[0], T[1], bT[1], T[3], vP[1]);
            } else {
                intersect(T[1], aT[1], T[3], T[2], vP[1]);
            }
        }
    }
    
    vP[0] = P1+P1-vP[1];
    
    ofMesh mesh;
    if(inter ==1 || inter == 2){
        mesh.addVertex(ofPoint(T[0].x, T[0].y, 0));   //0
        mesh.addVertex(ofPoint(T[1].x, T[1].y, 0));   //1
        mesh.addVertex(ofPoint(vP[1].x, vP[1].y, 0)); //2
        mesh.addVertex(ofPoint(T[3].x, T[3].y, 0));   //3
        mesh.addVertex(ofPoint(T[2].x, T[2].y, 0));   //4
        mesh.addVertex(ofPoint(bT[0].x, bT[0].y, 0)); //5
        mesh.addVertex(ofPoint(aT[0].x, aT[0].y, 0)); //6
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(0);
        mesh.addIndex(2);
        mesh.addIndex(6);
        mesh.addIndex(2);
        mesh.addIndex(5);
        mesh.addIndex(6);
        mesh.addIndex(2);
        mesh.addIndex(4);
        mesh.addIndex(5);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(4);
        if(bGradation){
            mesh.addColor(lColor[0]); //0
            mesh.addColor(lColor[0]); //1
            mesh.addColor(lColor[1]); //2
            mesh.addColor(lColor[2]); //3
            mesh.addColor(lColor[2]); //4
            mesh.addColor(lColor[1]); //5
            mesh.addColor(lColor[1]); //6
        }
    } else {
        //線と線が重なり合ってるとき
        mesh.addVertex(ofPoint(T[0].x, T[0].y, 0));   //0
        mesh.addVertex(ofPoint(T[1].x, T[1].y, 0));   //1
        mesh.addVertex(ofPoint(aT[0].x, aT[0].y, 0)); //2
        mesh.addVertex(ofPoint(bT[0].x, bT[0].y, 0)); //3
        mesh.addVertex(ofPoint(T[3].x, T[3].y, 0));   //4
        mesh.addVertex(ofPoint(T[2].x, T[2].y, 0));   //5
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(4);
        mesh.addIndex(3);
        mesh.addIndex(4);
        mesh.addIndex(5);
        if(bGradation){
            mesh.addColor(lColor[0]); //0
            mesh.addColor(lColor[0]); //1
            mesh.addColor(lColor[1]); //2
            mesh.addColor(lColor[1]); //3
            mesh.addColor(lColor[2]); //4
            mesh.addColor(lColor[2]); //5
        }
    }
    
    if (!bGradation) {
        ofSetColor(lColor[1]);
    }
    if(!bWire){
        mesh.draw();
    } else {
        mesh.drawWireframe();
    }
}

//--------------------------------------------------------------
void Polyline::drawLine_round(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2, ofColor lColor[3]){
    glm::vec2 T[4], aT[2], bT[2], vP[2];
    glm::vec2 base[3];
    vector<glm::vec2> arcP;
    base[0] = P1 - P0;
    base[0] = glm::normalize(base[0]);
    base[0] *= half_lineWidth;
    
    base[1] = P1 - P2;
    base[1] = glm::normalize(base[1]);
    base[1] *= half_lineWidth;
    
    bool clockwise = signed_area(P0, P1, P2);
    if (!clockwise) {
        base[0] = -base[0];
        base[1] = -base[1];
    }
    T[0] = P0 + perpen(base[0]);
    T[1] = P0 + inverse(perpen(base[0]));
    T[2] = P2 + inverse(perpen(base[1]));
    T[3] = P2 + perpen(base[1]);
        
    aT[0] = P1+perpen(base[0]);
    aT[1] = P1 + inverse(perpen(base[0]));
    bT[0] = P1+inverse(perpen(base[1]));
    bT[1] = P1+perpen(base[1]);
    
    int inter = intersect(T[1], aT[1], T[3], bT[1], vP[1]);
    float dist0 = ofDist(P0.x, P0.y, P1.x, P1.y);
    float dist1 = ofDist(P1.x, P1.y, P2.x, P2.y);
    
    //線と線が重なり合ってるとき、新しいvP[1]を計算する
    if ((inter == 3) || (inter == 4) || (inter == 5)) {
        if (clockwise) {
            if(dist1 < dist0){
                intersect(T[2], T[3], aT[1], T[1], vP[1]);
            }else {
                intersect(T[1], T[0], bT[1], T[3], vP[1]);
            }
        } else {
            if(dist0 < dist1){
                intersect(T[0], T[1], bT[1], T[3], vP[1]);
            } else {
                intersect(T[1], aT[1], T[3], T[2], vP[1]);
            }
        }
    }
    
    vP[0] = P1+P1-vP[1];
    
    basic_vectors_arc(P1, aT[0], bT[0], half_lineWidth, arcP, clockwise);
    
    ofMesh mesh;
    mesh.addVertex(ofPoint(T[0].x, T[0].y, 0));   //0
    mesh.addVertex(ofPoint(T[1].x, T[1].y, 0));   //1
    mesh.addVertex(ofPoint(vP[1].x, vP[1].y, 0)); //2
    mesh.addVertex(ofPoint(T[3].x, T[3].y, 0));   //3
    mesh.addVertex(ofPoint(T[2].x, T[2].y, 0));   //4
    for (int i = 0; i < arcP.size(); i++) {
        mesh.addVertex(ofPoint(arcP[i].x, arcP[i].y, 0)); //5~
    }
    
    int last = (int)mesh.getNumVertices()-1;
    mesh.addIndex(0);
    mesh.addIndex(1);
    mesh.addIndex(2);
    mesh.addIndex(0);
    mesh.addIndex(2);
    mesh.addIndex(5);
    for (int i = 0; i < arcP.size()-1; i++) {
        mesh.addIndex(i+5);
        mesh.addIndex(i+6);
        mesh.addIndex(2);
    }
    mesh.addIndex(last);
    mesh.addIndex(2);
    mesh.addIndex(4);
    mesh.addIndex(2);
    mesh.addIndex(3);
    mesh.addIndex(4);
    if (bGradation) {
        mesh.addColor(lColor[0]); //0
        mesh.addColor(lColor[0]); //1
        mesh.addColor(lColor[1]); //2
        mesh.addColor(lColor[2]); //3
        mesh.addColor(lColor[2]); //4
        for (int i = 0; i < arcP.size(); i++) {
            mesh.addColor(lColor[1]);
        }
    }
    
    if (!bGradation) {
        ofSetColor(lColor[1]);
    }
    if(!bWire){
        mesh.draw();
    } else {
        mesh.drawWireframe();
    }
}

//--------------------------------------------------------------
glm::vec2 Polyline::perpen(glm::vec2 P){
    //垂直、反時計回りに９０度
    return glm::vec2(P.y, -P.x);
}
//--------------------------------------------------------------
glm::vec2 Polyline::inverse(glm::vec2 P){
    //１８０度反転
    return glm::vec2(-P.x, -P.y);
}
//--------------------------------------------------------------
bool Polyline::signed_area(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2){
    //時計回りの順序であるか計算
    float check = (P1.x-P0.x)*(P2.y-P0.y)-(P2.x-P0.x)*(P1.y-P0.y);
    if(check > 0){
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
int Polyline::intersect(glm::vec2 P1, glm::vec2 P2, //line 1
                        glm::vec2 P3, glm::vec2 P4, //line 2
                        glm::vec2 & Pout)           //output point
{
    //2つの線の交点を計算する
    double mua,mub;
    double denom,numera,numerb;
    const double eps = 0.000000000001;

    denom  = (P4.y-P3.y) * (P2.x-P1.x) - (P4.x-P3.x) * (P2.y-P1.y);
    numera = (P4.x-P3.x) * (P1.y-P3.y) - (P4.y-P3.y) * (P1.x-P3.x);
    numerb = (P2.x-P1.x) * (P1.y-P3.y) - (P2.y-P1.y) * (P1.x-P3.x);

    if ( (-eps < numera && numera < eps) &&
             (-eps < numerb && numerb < eps) &&
             (-eps < denom  && denom  < eps) ) {
        Pout.x = (P1.x + P2.x) * 0.5;
        Pout.y = (P1.y + P2.y) * 0.5;
        return 2; //meaning the lines coincide
    }

    if (-eps < denom  && denom  < eps) {
        Pout.x = 0;
        Pout.y = 0;
        return 0; //meaning lines are parallel
    }

    mua = numera / denom;
    mub = numerb / denom;
    Pout.x = P1.x + mua * (P2.x - P1.x);
    Pout.y = P1.y + mua * (P2.y - P1.y);
    bool out1 = mua < 0 || mua > 1;
    bool out2 = mub < 0 || mub > 1;

    if ( out1 & out2) {
        return 5; //the intersection lies outside both segments
    } else if ( out1) {
        return 3; //the intersection lies outside segment 1
    } else if ( out2) {
        return 4; //the intersection lies outside segment 2
    } else {
        return 1; //the intersection lies inside both segments
    }
}

//--------------------------------------------------------------
void Polyline::basic_vectors_arc(glm::vec2 P, //orijin
                       glm::vec2 A, glm::vec2 B,
                       float r, //radius
                       vector<glm::vec2> & q,
                       bool clockwise){
    float angle1 = atan2(A.y-P.y, A.x-P.x);
    float angle2 = atan2(B.y-P.y, B.x-P.x);
    if (angle1 < 0) {angle1 = TWO_PI + angle1;}
    if (angle2 < 0) {angle2 = TWO_PI + angle2;}
    
    if ((clockwise == true) && (angle1 > angle2)) {
        angle2 = TWO_PI+angle2;
    }
    if ((clockwise == false) && (angle2 > angle1)) {
        angle2 = angle2-TWO_PI;
    }
    
    bool incremental = true;
    if (angle1 > angle2) {
        incremental = false; //means decremental(減少)
    }
    
    if (incremental) {
        for (float a = angle1; a < angle2; a += dangle) {
            float x = P.x + r * cos(a);
            float y = P.y + r * sin(a);
            q.push_back(glm::vec2(x, y));  //the current point on the arc
        }
    } else {
        for (float a = angle1; a > angle2; a -= dangle) {
            float x = P.x + r * cos(a);
            float y = P.y + r * sin(a);
            q.push_back(glm::vec2(x, y));
        }
    }
}
//--------------------------------------------------------------
