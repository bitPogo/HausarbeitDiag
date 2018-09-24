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

/*-------------------LibFunctions Declaration---------------*/
/**
 * Transform grade, minutes and seconds to grades
 * @param Return | mpfr_t | the computed Grad
 * @param Grad | mpfr_t | the start grades
 * @param Minutes | mpfr_t | the start minutes
 * @param Seconds | mpfr_t | the start seconds
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void toGrad(
        mpfr_t Return,
        mpfr_t Grade,
        mpfr_t Minutes,
        mpfr_t Seconds,
        mpfr_rnd_t Round
);
/**
 * Mirrows a point on x-axis
 * @param Return | array of mpfr_t | n = 2 | the mirrowed point
 * @param Point | array of mpfr_t | n = 2 | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPointOnX( mpfr_t* Return, mpfr_t* Point, mpfr_rnd_t Round );
/**
 * Mirrows a point on y-axis
 * @param Return | array of mpfr_t | n = 2 | the mirrowed point
 * @param Point | array of mpfr_t | n = 2 | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPointOnY( mpfr_t* Return, mpfr_t* Point, mpfr_rnd_t Round );
/**
 * Mirrows a point on x- and y-axis
 * @param Return | array of mpfr_t | n = 2 | the mirrowed point
 * @param Point | array of mpfr_t | n = 2 | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPoint( mpfr_t* Return, mpfr_t* Point, mpfr_rnd_t Round );
/**
 * intersectLine computes the touchpoints of to given a line and a second line
 * @param Return | array of mpfr_t | n = 2 | the computed touchpoints
 * @param PointAStart | array of mpfr_t | n = 2 | the startpoint of the first line
 * @param PointAEnd | array of mpfr_t | n = 2 | the endpoint of the first line
 * @param PointBStart | array of mpfr_t | n = 2 | the startpoint of the second line
 * @param PointBEnd | array of mpfr_t | n = 2 | the endpoint of the second line
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return int | SUCCESS or NO_INTERSECTION
 */
extern unsigned short intersectLines(
        mpfr_t* Return,
        mpfr_t* PointAStart,
        mpfr_t* PointAEnd,
        mpfr_t* PointBStart,
        mpfr_t* PointBEnd,
        mpfr_rnd_t Round
);

/**
 * intersectLine computes the touchpoints of to given a circle and a line
 * @param Return | array of mpfr_t | n = 4 | the computed touchpoints
 * @param PointA | array of mpfr_t | n = 2 | startpoint of the given line
 * @param PointB | array of mpfr_t | n = 2 | startpoint of the given line
 * @param Center | array of mpfr_t | n = 2 | circlecenter of the given circle
 * @param Radius | mpfr_t | the radius of the given circle
 * @param Round | mpfr_rnd_t | SUCCESS or NO_INTERSECTION or ONE_TOUCH
 * @return
 */
extern unsigned short intersectCircleLine(
        mpfr_t* Return,
        mpfr_t* PointA,
        mpfr_t* PointB,
        mpfr_t* Center,
        mpfr_t Radius,
        mpfr_rnd_t Round
);

/**
 * intersectCircles computes the touchpoints of to given circles
 * @param Return | array of mpfr_t | n = 4 | the computed touchpoints
 * @param ACenter | array of mpfr_t | n = 2 | x/y - coordinates of circle A
 * @param BCenter | array of mpfr_t | n = 2 | x/y - coordinates of circle B
 * @param ARadius | mpfr_t | the radius of circle A
 * @param BRadius | mpfr_t | the radius of circle A
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return | SUCCESS or NO_DISTANCE_OF_CENTERS or NO_INTERSECTION or ONE_TOUCH or INFINTIY
 */
extern unsigned short intersectCircles(
        mpfr_t* Return,
        mpfr_t* ACenter,
        mpfr_t* BCenter,
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
 * Prints a error-message to stderr and quits the programm
 * @param Message | const char* | the message
 */
extern void errorAndOut( const char* Message );
/**
 * Checks if a given string is nummeric or not
 * Also hexstrings with prefix x|X or ocatalstrings with prefix o|O are allowed
 * @param String | const char* | the given input string
 * @return -1 if it's not a number or Base 8|10|16
 */
extern short validateString( const char* String );
/**
 * Returns a substring of given String
 * @param Source | char* | the input string
 * @param From | int | the startposition of the subset
 * @param Length | int | Length of the subset
 * @return | char* | the computed subset
 */
extern char* substring( char* Source, int From, int Length );
#endif
