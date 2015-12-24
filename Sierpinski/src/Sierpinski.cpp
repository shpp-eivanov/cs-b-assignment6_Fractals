/********************************************************************************************
* File: Sierpinski.cpp
* ----------------------
* v.3 2015/12/24
* - fields are renamed,
* - code is reformatted
*
* Implementation of the Sierpinski triangle drawing.
********************************************************************************************/

#include <iostream>
#include "console.h"
#include "simpio.h"
#include "gbufferedimage.h"

using namespace std;

/* Declarations
 * -----------------------------------------------------------------------------------------*/
GBufferedImage* img;                /* Main program image */
double const MAX_LENGTH = 1000;     /* Max side length value for 0-oreder triangle */
double const MIN_SIDE = 2;          /* Minimal inserted triangle side to make nice picture */
double const MIN_WINDOW = 150;      /* Minimal size of main window */

/* -----------------------------------------------------------------------------------------//
 * Implementation section.
 * -----------------------
 * Program asks user to enter length
 * of the bigest, 0-order, triangle, and then asks
 * to enter desireable order for Sierpinski triangle
 * drawing.
 * Then it recursively draws Sierpinski triangle.
 * -----------------------------------------------------------------------------------------*/

/* Function: maxOrderLimit
 * ----------------------
 * Returns maximal order value, for param length, due
 * to minLine value.
 * MinLine value - is minimal side length for internal
 * triangle, which could be obtained due to recursive
 * 0-order triangle dividing.
 *
 * @param minLineLimit        Minimal valid triangle side length.
 * @param currentLength       User input 0-oreder triangle length.
 * @param currentOrder        Rcusion current order   */
int maxOrderLimit(double minLineLimit, double currentLength, int currentOrder) {
    if (currentLength <= minLineLimit) {
        return currentOrder;
    }else{
        return maxOrderLimit(minLineLimit, (currentLength / 2), ++currentOrder);
    }
}

/* Function: userInput
 * -------------------
 * Asks user to enter length
 * of the bigest, 0-order triangle, and then asks
 * to enter desireable order for Sierpinski triangle
 * drawing.
 * Initiates values of params - lenght, inputOrder.
 * Makes prompts for optimal user input.
 *
 * @param inputLength       User input 0-oreder triangle length.
 * @param inputOrder        User desirable Sierpinski order   */
void userInput(double& inputLength, int& inputOrder) {
    /* Asks for 0-oreder triangle length */
    while (true) {
        inputLength = (double)getInteger("ENTER 0-ORDER TRIANGLE SIDE LENGTH: ");
        if ((inputLength > 0) && (inputLength < MAX_LENGTH)) {
            break;
        }else                                                              { cout << "      VALID LENGTH RANGE SHOULD BE [0 - " << MAX_LENGTH
                                              << "]" << endl; }
    }

    /* Choses optimal order due to input length.
     * Such order makes triangle picture more clear, and
     * dosen't rise recursion level to much */
    int optimalOrder = maxOrderLimit(MIN_SIDE, inputLength, 0);

    /* Asks for desirable Sierpinski order */
    while (true) {
        inputOrder = (double)getInteger("ENTER SIERPINSKI ORDER: ");
        /* Asks user to make choose due to optimal conditions */
        if ((inputOrder > 0) && (inputOrder <= optimalOrder)) {
            break;
        }else if (inputOrder > optimalOrder) {
            cout << "      OPTIMAL ORDER IS LOWER THEN: "
                 << optimalOrder << endl;
        }else if (inputOrder < 0) {
            cout << "      NOT VALID ORDER" << endl;
        }
    }
}

/* Function: setTriangleTopPosition
 * --------------------------------
 * Choose 0-oreder triangle position in the
 * main window. Sets window sizes due to
 * triangle size.
 * Initiates triangle top vertex position
 * params - vertex_X, vertex_Y.
 *
 * @param gw          Main program window object.
 * @param length      User input 0-oreder triangle length.
 * @param vertex_X    0-oreder triangle top vertex position
 * @param vertex_Y                                         */
void setTriangleTopPosition(GWindow& gw,
                double length,
                double& vertex_X,
                double& vertex_Y) {

    double triangleHeight = length * 0.866;
    double gwWidth = length + (0.2 * length);
    double gwHeight = triangleHeight + (0.3 * triangleHeight);

    if (gwHeight < MIN_WINDOW) {
        gw.setSize(MIN_WINDOW, MIN_WINDOW);
        setConsoleLocation(MIN_WINDOW, 0);
    }else{
        gw.setSize(gwWidth, gwHeight);
        setConsoleLocation(gwWidth, 0);
    }

    vertex_X = gwWidth / 2;
    vertex_Y = 0.1 * triangleHeight;
}

/* Function: drawBigTriangle
 * ----------------------
 * Draws main big triangle of the image.
 * Start of drawing begins from topPt.
 * Initiates positions of triangle vertexes into
 * params - rightPt and leftPt.
 *
 * @param gw          Main program window object.
 * @param topPt       Top vertex - start of drawing
 * @param rightPt     righ vertex - value is set in function
 * @param leftPt      left vertex - value is set in function
 * @param length      Triangle side length.        */
void drawBigTriangle(GWindow& gw,
             GPoint& topPt,
             GPoint& rightPt,
             GPoint& leftPt,
             int length) {

    rightPt = gw.drawPolarLine(topPt, length, -60);
    leftPt = gw.drawPolarLine(rightPt, length, 180);
    gw.drawPolarLine(leftPt, length, 60);
}

/* Function: getMiddle
 * ----------------------
 * Returns point which is positioned
 * on the middle, between param points. */
GPoint getMiddle(GPoint& pt1, GPoint& pt2) {

    double x1 = pt1.getX();
    double y1 = pt1.getY();
    double x2 = pt2.getX();
    double y2 = pt2.getY();
    double middle_x = (x2 + x1) / 2;
    double middle_y = (y2 + y1) / 2;
    GPoint pt(middle_x, middle_y);
    return pt;
}

/* Function: drawSmallSide
 * -----------------------
 * Draws line between param points.
 * This line is side of the inserted triangle.  */
void drawSmallSide(GWindow& gw,
           GPoint& pt1,
           GPoint& pt2) {

    gw.drawLine(pt1.getX(), pt1.getY(), pt2.getX(), pt2.getY());
}

/* Function: smallTrianglesDrawing
 * -------------------------
 * Main program recursion. Insert three small
 * triangles inside external biger triangle, while
 * current oreder isn't smaller then input order.
 *
 * @param gw              Main program window object.
 * @param currentOrder    Order of triangles which are inserted yet.
 * @param inputOrder      User order.
 * @param topPt           Top, righ, left edge vertexes
 * @param rightPt         of external bigger triangle.
 * @param leftPt                                                   */
void smallTrianglesDrawing(GWindow& gw,
               int currentOrder,
               int inputOrder,
               GPoint& topPt,
               GPoint& rightPt,
               GPoint& leftPt) {

    if (currentOrder <= inputOrder) {
        /* Determines middle points of external triangle sides */
        GPoint rightSideMiddle = getMiddle(rightPt, topPt);
        GPoint bottomSideMiddle = getMiddle(rightPt, leftPt);
        GPoint leftSideMiddle = getMiddle(leftPt, topPt);
        /* Draws top shelf - side of the top inserted triangle */
        drawSmallSide(gw, leftSideMiddle, rightSideMiddle);
        /* Draws right shelf - side of the right inserted triangle */
        drawSmallSide(gw, rightSideMiddle, bottomSideMiddle);
        /* Draws left shelf - side of the left inserted triangle */
        drawSmallSide(gw, leftSideMiddle, bottomSideMiddle);
        /* Update current order for current triangle */
        currentOrder++;
        /* Recursively calls top triangle pocess */
        smallTrianglesDrawing(gw,
                      currentOrder,
                      inputOrder,
                      topPt,
                      rightSideMiddle,
                      leftSideMiddle);
        /* Recursively calls right triangle pocess */
        smallTrianglesDrawing(gw,
                      currentOrder,
                      inputOrder,
                      rightSideMiddle,
                      rightPt,
                      bottomSideMiddle);
        /* Recursively calls left triangle pocess */
        smallTrianglesDrawing(gw,
                      currentOrder,
                      inputOrder,
                      leftSideMiddle,
                      bottomSideMiddle,
                      leftPt);
    }
}

int main() {
    GWindow gw;
    gw.setLocation(0, 0);
    setConsoleLocation(0, 0);
    double bigTriangleSideLength;  /* 0-order triangle side length */
    int inputOrder;     /* User order request */
    /* Asks user to enter length and order */
    userInput(bigTriangleSideLength, inputOrder);

    /* Finds position of the top vertex of 0-order triangle */
    double topVertex_X;
    double topVertex_Y;

    /* Adjusts window and set triangle top vertex position */
    setTriangleTopPosition(gw, bigTriangleSideLength, topVertex_X, topVertex_Y);

    GPoint topPt(topVertex_X, topVertex_Y);/* Initiate top vertex */
    GPoint rightPt;              /* Right vertex */
    GPoint leftPt;               /* Left vertex */
    int currentOrder = 0;        /* Current drawing order */

    /* Draws 0-order triangle and initiate rightPt and leftPt  */
    drawBigTriangle(gw, topPt, rightPt, leftPt, bigTriangleSideLength);

    currentOrder++;

    /* Starts    recursion */
    smallTrianglesDrawing(gw,
                  currentOrder,
                  inputOrder,
                  topPt,
                  rightPt,
                  leftPt);

    return 0;
}

