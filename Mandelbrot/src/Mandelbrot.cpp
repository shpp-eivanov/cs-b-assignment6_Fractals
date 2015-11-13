#include <iostream>
#include "console.h"
#include <stdlib.h>
#include "queue.h"
#include <Vector>
#include "gbufferedimage.h"


using namespace std;

GBufferedImage* img;
double const radius = 400;
int const MAX_DEPTH = 500;
double const GW_WIDTH = radius * 2;
double const GW_HEIGHT = radius * 2;
double const WIDTH_KOEF = 4/GW_WIDTH;
double const HEIGHT_KOEF = 4/GW_HEIGHT;

int equationProcess(double a,
                    double b,
                    double zR,
                    double zI,
                    int iter){
    double zLength;
    double buf = (zR * zR) - (zI * zI);
    zI = (2 * zR * zI) + b;
    zR = buf + a;
    zLength = (zR * zR) + (zI * zI);
    iter++;

    if((zLength < 4) && (iter < MAX_DEPTH)){
        return equationProcess(a, b, zR, zI, iter);
    }else{
        return iter;
    }
}

int getIterationDepth(double col, double row){
    /* Current complex area point (a, b) */
    double a = WIDTH_KOEF * col - 2;
    double b = 2 - HEIGHT_KOEF * row;
    double zR = 0;
    double zI = 0;
    int iter = 0;

    int depth = equationProcess(a, b, zR, zI, iter);

    return depth;
}

int main() {
    GWindow gw;
    gw.setSize(GW_WIDTH, GW_HEIGHT);
    img = new GBufferedImage(GW_WIDTH,GW_HEIGHT,0x00ffff);
    gw.add(img, 0, 0);
    for(int row = 0; row < GW_HEIGHT; row++){
        for(int col = 0; col < GW_WIDTH; col++){
            int currentDepth = getIterationDepth(col, row);
            int color = 255*(MAX_DEPTH - currentDepth)/MAX_DEPTH;
            int pixelColor = color * color *color;
            img->setRGB(col, row, pixelColor);
        }
    }
   return 0;
}

