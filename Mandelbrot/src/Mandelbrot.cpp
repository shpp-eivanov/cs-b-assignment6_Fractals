/********************************************************************************************
 * File: Mandelbrot.cpp
 * ----------------------
 * v.1 2015/11/30
 * Implementation of the Mandelbrot set drawing.
 ********************************************************************************************/

#include <iostream>
#include "console.h"
#include <stdlib.h>
#include "gbufferedimage.h"

using namespace std;


/* Declarations
 * -----------------------------------------------------------------------------------------*/
GBufferedImage* img;
double const RADIUS = 300;              /* Represents radius value "2" on the complex area  */
int const MAX_DEPTH = 500;              /* Limit for iterations quantity  */

double const GW_WIDTH =  1.3 * RADIUS;  /* Main image parameters, koeficients make          */
double const GW_HEIGHT = 1.2 * RADIUS;  /* cardioide position being closer to window centre */

/*------------------------------------------------------------------------------------------//
 * Implementation section.
 * -----------------------
 * For current radius parameter program draws
 * Mandelbrot set image. It main part, cardioide,
 * is black area in the centre of the window.
 * Space of program window represent  complex space,
 * where:
 * - window top is "-1" imaginary complex value,
 * - window bottom is "1" imaginary value,
 * - window left edge is "-2" of real complex value.
 * User can enter RADIUS value in declarations part
 * of code to change sizes of program image.
 * -----------------------------------------------------------------------------------------*/

/* Function: equationProcess
 * ---------------------------
 * Returns iterations quantity for current c-point(params a, b),
 * due to previous Z-value (params zR, zI).
 *
 * @a, b       Current complex area c-point: c = a + jb
 * @zR, zI     Previous complex Z-value parameters: Z = zR + jzI
 * @iter       Counter of iterations have been made yet  */
int equationProcess(double a,
                    double b,
                    double zR,
                    double zI,
                    int iter){
    /* Main Z-value rised to power:
     * zLenght = Z^2 = zR^2 + jzI^2 */
    double zLength;
    /* Euaquations for zLength */
    double buf = (zR * zR) - (zI * zI);
    zI = (2 * zR * zI) + b;
    zR = buf + a;
    zLength = (zR * zR) + (zI * zI);
    iter++;

    /* Implementation notes
     * --------------------------------------------------------------------
     * In this program  we make such Mandelbrot apreciation:
     *   if (Z^2 < 4) for every iteration - this c-point belongs
     *   to Mandelbrot set. If we limit iterations quantity by MAX_DEPTH,
     *   and if for every iteration this apreciation will be true
     *   we can finish sequences for this c-point. This c-point is from
     *   Mandelbrot set then - it will be black on the picture.
     *
     *   Else, if we don't meet confition - so we find external point
     *   for Mandelbrot set, and we could choose color for it due
     *   to depth iterations value.
     * -------------------------------------------------------------------- */

    /* Main apreciation process */
    if((zLength < 4) && (iter < MAX_DEPTH)){
        return equationProcess(a, b, zR, zI, iter);//Continue calculations
    }else{
        return iter;                                //Stop calculations
    }
}

/* Function: getIterationDepth
 * ---------------------------
 * Returns iterations quantity for current pixel.
 * Position of Current pixel is modified into
 * complex area scale position. In this complex area we made
 * evaluations, to what set this point belongs. If depth value
 * rises  MAX_DEPTH - so it's internal point of
 * Manelbrot set, and it will be black on image. If we obtain
 * smaller value then MAX_DEPTH - determine that this is external point
 * and we can paint it in some other color.
 *
 * @col, row       Current image position  */
int getIterationDepth(double col, double row){

    /* Implementation notes
     * --------------------------------------------------------------------
     * The user radius represents R = 2 radius on the
     * complex area. The main cardioide of the Madelbrot
     * set is inserted in this circle. In this program
     * window image position is taken due to radius value,
     * so the image size depends of the radius and
     * always represents the same part of the complex circle.
     * This circle positioned in such way:
     *  - At the left side of the program image is the
     *    "-2" real complex value - left edge of the
     *    Mandelbrot set.
     *  - Top edge - is "-1" complex imaginary value
     *  - Image bottom edge - is "1" complex imaginary value
     *  - Image right side - is > "1" complex real value
     *  - The real value axis (b = 0) is on the middle of the
     *    image height.
     * So, modification of image position is strict, and
     * requires significant corrections in demand.
     * --------------------------------------------------------------------*/

    /* Modifies image pixel position into
     * complex area position (c = a + jb) due
     * to implementation describtion above   */
    double a = col *(2 / RADIUS) - 2;   /* a - real complex value */
    double b = row *(2 / RADIUS) - 1;   /* b - imaginary complex value */

    /* Implementation notes
     * --------------------------------------------------------------------
     * The main apreciation parameter of program is Z value,
     * which forms sequences like this:
     * Z0 = 0
     * Znext = Zpreviuos^2 + c
     * ...
     *
     * The main Mandelbrot idea is "|Z| < R" for every iterations depth,
     * where R could be taken as "2" for simplification.
     *
     * We try to discover if this c-point satisfy this condition
     * for any iteration. If it's true - it will be black point on the image.
     * --------------------------------------------------------------------*/

    /* Apreciation starts from Z0 = z0R + jz0I = 0.             */
    double z0R = 0;      /* Represents real value for complex Z0       */
    double z0I = 0;      /* Represents imaginary value for complex Z0  */
    int iter = 0;        /* Start value for iterations counter */

    /* Makes sequence of Z-value calculations for current c-point
     * to find out iterations quantity  */
    int depth = equationProcess(a, b, z0R, z0I, iter);

    return depth;
}

int main() {
    GWindow gw;
    gw.setSize(GW_WIDTH, GW_HEIGHT);
    img = new GBufferedImage(GW_WIDTH, GW_HEIGHT,0xffffff);
    gw.add(img, 0, 0);
    /* For every image pixel:
     * - modifies it into complex area scale;
     * - appreciates, if it satisfy Mandelbrot set condition
     * - choses color of pixel due to Mandelbrot set condition */
    for(int row = 0; row < GW_HEIGHT; row++){
        for(int col = 0; col < GW_WIDTH; col++){
            /* Determines main Mandelbrot condition (described above)
             * for this pixel */
            int currentDepth = getIterationDepth(col, row);
            /* Set color for pixel:
             * - if currentDepth = MAX_DEPTH - it's black pixel
             * - if currentDepth < MAX_DEPTH - set apropriate color */
            int color = 255 *(MAX_DEPTH - currentDepth)/MAX_DEPTH;
            int pixelColor = color * color * color;
            img->setRGB(col, row, pixelColor);
        }
    }
   return 0;
}

