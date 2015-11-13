#include <iostream>
#include "console.h"
#include <stdlib.h>
#include "queue.h"
#include <Vector>
#include "gbufferedimage.h"


using namespace std;

GBufferedImage* img;

void printPoint(GPoint& pt){
    cout << pt.getX() << " " << pt.getY() << endl;
}

void drawTriangle(GWindow& gw,
                  GPoint& startPt,
                  GPoint& rightPt,
                  GPoint& leftPt,
                  GPoint& topPt,
                  int length){
    rightPt = gw.drawPolarLine(startPt, length, -60);
    leftPt = gw.drawPolarLine(rightPt, length, 180);
    topPt = gw.drawPolarLine(leftPt, length, 60);
}

GPoint getMiddle(GPoint& pt1, GPoint& pt2){
    double x1 = pt1.getX();
    double y1 = pt1.getY();
    double x2 = pt2.getX();
    double y2 = pt2.getY();
    double middle_x = (x2 + x1)/2;
    double middle_y = (y2 + y1)/2;
    GPoint pt(middle_x, middle_y);
    return pt;
}

void drawSmallSide(GWindow& gw,
                  GPoint& pt1,
                  GPoint& pt2){
    gw.drawLine(pt1.getX(), pt1.getY(), pt2.getX(), pt2.getY());
}

void triangleProcess(GWindow& gw,
                      int currentOrder,
                      int inputOrder,
                      GPoint& topPt,
                      GPoint& rightPt,
                      GPoint& leftPt){
    if(currentOrder <= inputOrder){
        GPoint rightSideMiddle = getMiddle(rightPt, topPt);
        GPoint bottomSideMiddle = getMiddle(rightPt, leftPt);
        GPoint leftSideMiddle = getMiddle(leftPt, topPt);
        /* Top  shelf */
        drawSmallSide(gw, leftSideMiddle, rightSideMiddle);
        /* Right shelf */
        drawSmallSide(gw, rightSideMiddle, bottomSideMiddle);
        /* Left  shelf */
        drawSmallSide(gw, leftSideMiddle, bottomSideMiddle);
        currentOrder++;
        /* Call top triangle pocess */
        triangleProcess(gw,
                        currentOrder,
                        inputOrder,
                        topPt,
                        rightSideMiddle,
                        leftSideMiddle);
        /* Call right triangle pocess */
        triangleProcess(gw,
                        currentOrder,
                        inputOrder,
                        rightSideMiddle,
                        rightPt,
                        bottomSideMiddle);
        /* Call left triangle pocess */
        triangleProcess(gw,
                        currentOrder,
                        inputOrder,
                        leftSideMiddle,
                        bottomSideMiddle,
                        leftPt);
    }
}
/*
 * - разобраться с высотой окна
 * - оформить ввод и вывод
 * - комментарии
 * - минимально возможный порядок прикинуть
 * - коммит
 *
*/

int main() {
    GWindow gw;
    double length = 300;
    int inputOrder = 6;
    double triangleHeight = length * 0.866;
    double gwWidth = length + (0.2 * length);
    double gwHeight = triangleHeight +(0.2 * triangleHeight);
    double vertex_X = gwWidth / 2;
    double vertex_Y = 0.1 * triangleHeight;

    gw.setSize(gwWidth, gwHeight);

    GPoint top(vertex_X, vertex_Y);
    GPoint topPt;
    GPoint rightPt;
    GPoint leftPt;
    int currentOrder = 0;
        drawTriangle(gw, top, rightPt, leftPt, topPt, length);
        currentOrder++;
        /* Start recursion */
        triangleProcess(gw,
                        currentOrder,
                        inputOrder,
                        topPt,
                        rightPt,
                        leftPt);
        /* Recursion stopped */

   return 0;
}

