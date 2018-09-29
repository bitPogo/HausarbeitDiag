/**
 * @autor Matthias Geisler (bitpogo) <geislemx@informatik.hu-berlin.de>
 * @licence GNU General Public License
 */
#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>
#include <stdlib.h>
#include <string.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#define CMP_EQUAL 0
#define CMP_GREATER 1
#define CMP_LESS -1

#define SUCCESS 0x0
#define NO_DISTANCE_OF_CENTERS 0x1
#define NO_INTERSECTION 0x2
#define ONE_TOUCH 0x3
#define INFINTIY 0x4

#ifndef DIAG_LIB_H
#define DIAG_LIB_H

typedef struct Point
{
    mpfr_t X;
    mpfr_t Y;
} Point;

/*-------------------LibFunctions Declaration---------------*/
/**
 * Transform degree, minutes and seconds to minutes
 * @param Return | mpfr_t | the computed minutes
 * @param degree | mpfr_t | the start degrees
 * @param Minutes | mpfr_t | the start minutes
 * @param Seconds | mpfr_t | the start seconds
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void toSeconds(
        mpfr_t Return,
        mpfr_t Degree,
        mpfr_t Minutes,
        mpfr_t Seconds,
        mpfr_rnd_t Round
);
/**
 * Transform degree, minutes and seconds to seconds
 * @param Return | mpfr_t | the computed seconds
 * @param degree | mpfr_t | the start degrees
 * @param Minutes | mpfr_t | the start minutes
 * @param Seconds | mpfr_t | the start seconds
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void toMinutes(
        mpfr_t Return,
        mpfr_t Degree,
        mpfr_t Minutes,
        mpfr_t Seconds,
        mpfr_rnd_t Round
);
/**
 * Transform degree, minutes and seconds to degrees
 * @param Return | mpfr_t | the computed degree
 * @param degree | mpfr_t | the start degrees
 * @param Minutes | mpfr_t | the start minutes
 * @param Seconds | mpfr_t | the start seconds
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void toDegree(
        mpfr_t Return,
        mpfr_t Degree,
        mpfr_t Minutes,
        mpfr_t Seconds,
        mpfr_rnd_t Round
);
/**
 * Mirrows a point on x-axis
 * @param Return | Point | the mirrowed point
 * @param X | mpfr_t | Zeropoint of X
 * @param Point | Point | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPointOnX( Point* Return, mpfr_t X, Point* Punkt, mpfr_rnd_t Round );
/**
 * Mirrows a point on y-axis
 * @param Return | Point | the mirrowed point
 * @param X | mpfr_t | Zeropoint of X
 * @param Point | Point | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPointOnY( Point* Return, mpfr_t Y, Point* Punkt, mpfr_rnd_t Round );
/**
 * Mirrows a point on x- and y-axis
 * @param Return | Point | the mirrowed point
 * @param Mirrow | Point | the mirrow
 * @param Point | Point | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPoint( Point* Return, Point* Mirrow, Point* Punkt, mpfr_rnd_t Round );
/**
 * intersectLine computes the touchpoints of to given a line and a second line
 * @param Return | Point | the computed touchpoints
 * @param PointAStart | Point | the startpoint of the first line
 * @param PointAEnd | Point | the endpoint of the first line
 * @param PointBStart | Point | the startpoint of the second line
 * @param PointBEnd | Point | the endpoint of the second line
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return int | SUCCESS or NO_INTERSECTION
 */
extern unsigned short intersectLines(
        Point* Return,
        Point* PunktAStart,
        Point* PunktAEnd,
        Point* PunktBStart,
        Point* PunktBEnd,
        mpfr_rnd_t Round
);

/**
 * intersectLine computes the touchpoints of to given a circle and a line
 * @param Return | array of mpfr_t | n = 4 | the computed touchpoints
 * @param PointA | Point | startpoint of the given line
 * @param PointB | Point | startpoint of the given line
 * @param Center | Point | circlecenter of the given circle
 * @param Radius | mpfr_t | the radius of the given circle
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return int | SUCCESS or NO_INTERSECTION or ONE_TOUCH
 */
extern unsigned short intersectCircleLine(
        mpfr_t* Return,
        Point* PointA,
        Point* PointB,
        Point* Center,
        mpfr_t Radius,
        mpfr_rnd_t Round
);
/**
 * intersectCircles computes the touchpoints of to given circles
 * @param Return | array of mpfr_t | n = 4 | the computed touchpoints
 * @param ACenter | Point | x/y - coordinates of circle A
 * @param BCenter | Point | x/y - coordinates of circle B
 * @param ARadius | mpfr_t | the radius of circle A
 * @param BRadius | mpfr_t | the radius of circle A
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return | SUCCESS or NO_DISTANCE_OF_CENTERS or NO_INTERSECTION or ONE_TOUCH or INFINTIY
 */
extern unsigned short intersectCircles(
        mpfr_t* Return,
        Point* ACenter,
        Point* BCenter,
        mpfr_t ARadius,
        mpfr_t BRadius,
        mpfr_rnd_t Round
);
/**
 * Checks if a given char is a diget or not
 * @param Char | char | the given diget
 * @param IsHex | TRUE or FALSE | if hexdigets are allowed
 * @return | TRUE or FALSE
 */
extern unsigned short isDigit( char Char, unsigned  short IsHex );
/**
 * Caculate the degree by given radians and
 * @param Return | mpfr_t | the computed degree
 * @param Radians | mpfr_t | the given radians
 * @param Diameter | mpfr_t | the given diameter
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void getDegreeOnCircle( mpfr_t Return, mpfr_t Radians, mpfr_t Diameter, mpfr_rnd_t Round );
/**
 * Caculate the degree by given radians and
 * @param Return | mpfr_t | the computed radians
 * @param Degree | mpfr_t | the given degree
 * @param Diameter | mpfr_t | the given diameter
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void getRadiansOnCircle( mpfr_t Return, mpfr_t Degree, mpfr_t Diameter, mpfr_rnd_t Round );
/**
 * Converts degrees to rad
 * @param Return | mpfr_t | the computed radians
 * @param Degree | mpfr_t | the given degrees
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void degree2Rad(mpfr_t Return, mpfr_t Degree, mpfr_rnd_t Round );
/**
 * Converts rad to degree
 * @param Return | mpfr_t | the computed degree
 * @param Rad | mpfr_t | the given rads
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void rad2Degree (mpfr_t Return, mpfr_t Rad, mpfr_rnd_t Round );
/**
 * Rotates a point at a given Point
 * @param Return | Point | the computed rotated point
 * @param Point | Point | Point which should be rotated
 * @param Center | Point | Point which should be the center of rotation
 * @param Degree | mpfr_t | degree for rotation
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void rotatePoint( Point* Return, Point* Punkt, Point* Center, mpfr_t Degree, mpfr_rnd_t Round );
/**
 * Computtes the einheitsvektor
 * @param Return | Point | the computed einheitsvektor
 * @param Point | Point | Point which from which should be derrived
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void getEinheitsVector( Point* Return, Point* Punkt, mpfr_rnd_t Round );
/**
 * Calculates the distance between tow points
 * @param Return | mpfr_t | the computed distance
 * @param PointA | Point | Startpoint
 * @param PointB | Point | Endpoint
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void getDistanceOfPoints( mpfr_t Return, Point* PunktA, Point* PunktB, mpfr_rnd_t Round );
/**
 * Prints a error-message to stderr and quits the programm
 * @param Message | const char* | the message
 */
extern void errorAndOut( const char* Message );
/**
 * Checks if a given string is numeric
 * @param String | const char* | the given input string
 * @param Signed | unsigned short | if the given value can be signed
 * @param Float | unsgined short | if the given value can be a float
 * @return -1 if it's not a number or Base 8|10|16
 */
extern unsigned short validateString( const char* String, unsigned short Signed, unsigned short Float );
/**
 * Returns a substring of given String
 * @param Source | char* | the input string
 * @param From | int | the startposition of the subset
 * @param Length | int | Length of the subset
 * @return | char* | the computed subset
 */
extern char* substring( char* Source, int From, int Length );

extern void cleanUpLib();
#endif
