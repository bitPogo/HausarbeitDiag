/**
 * @autor Matthias Geisler (bitpogo) <geislemx@informatik.hu-berlin.de>
 * @licence GNU General Public License
 */

#include "lib.h"

/*-------------------internals-----------------------------*/
/**
 * Computes the determinate of tow given vectors
 * @param X1 | mpfr_t | x-coordinate of point one
 * @param Y1 | mpfr_t | y-coordinate of point one
 * @param X2 | mpfr_t | x-coordinate of point tow
 * @param Y2 | mpfr_t | y-coordinate of point tow
 * @param Result | mpfr_t | the computed det
 * @param Round | mpfr_rnd_t | which rounding to use
 */
static void det(
        mpfr_t X1,
        mpfr_t Y1,
        mpfr_t X2,
        mpfr_t Y2,
        mpfr_t Result,
        mpfr_rnd_t Round
);
/**
 * @var GradFactor | mpfr_t | factor to transform minutes/seconds in grade
 */
static mpfr_t GradFactor;
/**
 * @var GradFactorIsInit | unsigend short | if GradFactor is set or not
 */

static unsigned short GradFactorIsInit = FALSE;
/**
 * @var Pi | mpfr_t | Pi
 */
static mpfr_t Pi;
/**
 * @var PiIsInit | unsigend short | if pi is set or not
 */
static unsigned short PiIsInit = FALSE;
/*-------------------LibFunctions Definition---------------*/
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
        mpfr_t Grad,
        mpfr_t Minutes,
        mpfr_t Seconds,
        mpfr_rnd_t Round
)
{
    mpfr_t Tmp;

    mpfr_init( Tmp );

    if( FALSE == GradFactorIsInit )
    {
        mpfr_set_ui( Tmp, 1, Round );
        mpfr_div_ui( GradFactor, Tmp, 60, Round );
        GradFactorIsInit = TRUE;
    }

    /* Seconds to Minutes */
    mpfr_mul( Tmp, Seconds, GradFactor, Round );
    /* add Seconds to Current Minutes */
    mpfr_add( Tmp, Minutes, Tmp, Round );
    /* Minutes to Grad */
    mpfr_mul( Tmp, Tmp, GradFactor, Round );
    /* add Minutes to Grad */
    mpfr_mul( Return, Grad, Tmp, Round );
    /*Housekeeping*/
    mpfr_clear( Tmp );
    mpfr_free_cache();
    return;
}

/**
 * Mirrows a point on x-axis
 * @param Return | array of mpfr_t | n = 2 | the mirrowed point
 * @param X | mpfr_t | Zeropoint of X
 * @param Point | array of mpfr_t | n = 2 | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPointOnX( mpfr_t* Return, mpfr_t X, mpfr_t* Point, mpfr_rnd_t Round )
{
    mpfr_t BetragPX, BetragX;
    mpfr_init( BetragPX );
    mpfr_init( BetragX );
    /* New_y = Old_y */
    mpfr_set( Return[ 1 ], Point[ 1 ], Round );

    mpfr_abs( BetragPX, Point[ 0 ], Round );
    mpfr_abs( BetragX, X, Round );

    /* Distance = Midpoint_x - P_x */
    mpfr_sub( Return[ 0 ], BetragX, BetragPX, Round  );
    /* New_x = Midpoint_x + Distance */
    mpfr_add( Return[ 0 ], X, Return[ 0 ], Round  );

    /* Housekeeping */
    mpfr_clear( BetragPX );
    mpfr_clear( BetragX );
    mpfr_free_cache();
}

/**
 * Mirrows a point on y-axis
 * @param Return | array of mpfr_t | n = 2 | the mirrowed point
 * @param Y | mpfr_t | Zeropoint of Y
 * @param Point | array of mpfr_t | n = 2 | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPointOnY( mpfr_t* Return, mpfr_t Y, mpfr_t* Point, mpfr_rnd_t Round )
{
    mpfr_t BetragPY, BetragY;
    mpfr_init( BetragPY );
    mpfr_init( BetragY );
    /* New_x = Old_x */
    mpfr_set( Return[ 0 ], Point[ 0 ], Round );

    mpfr_abs( BetragPY, Point[ 1 ], Round );
    mpfr_abs( BetragY, Y, Round );

    /* Distance = Midpoint_y - P_y */
    mpfr_sub( Return[ 1 ], BetragY, BetragPY, Round  );
    /* New_y = Midpoint_y + Distance */
    mpfr_add( Return[ 1 ], Y, Return[ 1 ], Round  );

    /* Housekeeping */
    mpfr_clear( BetragPY );
    mpfr_clear( BetragY );
    mpfr_free_cache();
}
/**
 * Mirrows a point on x- and y-axis
 * @param Return | array of mpfr_t | n = 2 | the mirrowed point
 * @param X | mpfr_t | Zeropoint of X
 * @param Y | mpfr_t | Zeropoint of Y
 * @param Point | array of mpfr_t | n = 2 | the original point
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void mirrowPoint( mpfr_t* Return, mpfr_t X, mpfr_t Y, mpfr_t* Point, mpfr_rnd_t Round )
{
    mirrowPointOnX( Return, X, Point, Round );
    mirrowPointOnY( Return, Y, Return, Round );
}
/**
 * Computes the determinate of tow given vectors
 * @param X1 | mpfr_t | x-coordinate of point one
 * @param Y1 | mpfr_t | y-coordinate of point one
 * @param X2 | mpfr_t | x-coordinate of point tow
 * @param Y2 | mpfr_t | y-coordinate of point tow
 * @param Result | mpfr_t | the computed det
 * @param Round | mpfr_rnd_t | which rounding to use
 */
/*
 * det = ( X1 * Y2 ) - ( Y1 * X2 )
 */
static void det(
        mpfr_t X1,
        mpfr_t Y1,
        mpfr_t X2,
        mpfr_t Y2,
        mpfr_t Result,
        mpfr_rnd_t Round )
{
    mpfr_t Tmp1, Tmp2;
    mpfr_init( Tmp1 );
    mpfr_init( Tmp2 );

    /* X1 * Y2 */
    mpfr_mul( Tmp1, X1, Y2, Round );
    /* Y1 * X2 */
    mpfr_mul( Tmp2, Y1, X2, Round );
    /* ( X1 * Y2 ) - ( Y1 * X2 ) */
    mpfr_sub( Result, Tmp1, Tmp2, Round );

    /* Housekeeping */
    mpfr_clear( Tmp1 );
    mpfr_clear( Tmp2 );
    mpfr_free_cache ();
}

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
)
{
    int Compare;
    mpfr_t DiffAX, DiffAY, DiffBX, DiffBY, Div, DA, DB, X, Y, Tmp1;

    mpfr_init( DiffAX );
    mpfr_init( DiffAY );
    mpfr_init( DiffBX );
    mpfr_init( DiffBY );
    mpfr_init( Div );
    mpfr_init( DA );
    mpfr_init( DB );
    mpfr_init( X );
    mpfr_init( Y );
    mpfr_init( Tmp1 );
#ifdef DEBUG
    printf( "\nStart line-line-intersection." );
    fflush( stdout );
#endif
    /*
     * DiffAX = PointAEnd_x - PointAStart_x
     * DiffAY = PointAStart_y - PointBStart_y
     */
    /* PointAStart_x - PointAEnd_x */
    mpfr_sub( DiffAX, PointAStart[ 0 ], PointAEnd[ 0 ], Round );
    /* PointAStart_y - PointAEnd_y */
    mpfr_sub( DiffAY, PointAStart[ 1 ], PointAEnd[ 1 ], Round );
#ifdef DEBUG
    printf( "\nDiffAX is:" );
    mpfr_out_str ( stdout, 10, 0, DiffAX, Round );
    printf( "\nDiffAY is:" );
    mpfr_out_str ( stdout, 10, 0, DiffAY, Round );
    fflush( stdout );
#endif
    /*
     * DffBX = PointBEnd_x - PointBStart_x
     * DiffBY = PointBStart_y - PointBStart_y
     */
    /* PointBStart_x - PointBEnd_x */
    mpfr_sub( DiffBX, PointBStart[ 0 ], PointBEnd[ 0 ], Round );
    /* PointBStart_y - PointBEnd_y */
    mpfr_sub( DiffBY, PointBStart[ 1 ], PointBEnd[ 1 ], Round );
#ifdef DEBUG
    printf( "\nDiffBX is:" );
    mpfr_out_str ( stdout, 10, 0, DiffBX, Round );
    printf( "\nDiffBY is:" );
    mpfr_out_str ( stdout, 10, 0, DiffBY, Round );
    fflush( stdout );
#endif
    /* div = det( DiffAX, DiffAY, DiffBX, DiffBY ) */
    det( DiffAX, DiffAY, DiffBX, DiffBY, Div, Round );
#ifdef DEBUG
    printf( "\nDiv is:" );
    mpfr_out_str ( stdout, 10, 0, Div, Round );
    fflush( stdout );
#endif
    Compare = mpfr_cmp_si( Div, 0 );
    if( CMP_EQUAL == Compare )
    {
#ifdef DEBUG
        printf( "\nNo touchpoint found." );
#endif
        /* Housekeeping */
        mpfr_clear( DiffAX );
        mpfr_clear( DiffAY );
        mpfr_clear( DiffBX );
        mpfr_clear( DiffBY );
        mpfr_clear( Div );
        mpfr_clear( DA );
        mpfr_clear( DB );
        mpfr_clear( X );
        mpfr_clear( Y );
        mpfr_clear( Tmp1 );
        mpfr_free_cache ();
        return NO_INTERSECTION;;
    }
    /*         X    Y
     *  D = ( DA , DB )
     *  DA = det( PointAStart, PointAEnd )
     *  DB = det( PointBStart, PointBEnd )
     */
    /* det( PointAStart, PointAEnd ) */
    det( PointAStart[ 0 ], PointAStart[ 1 ], PointAEnd[ 0 ], PointAEnd[ 1 ], DA, Round );
    /* det( PointBStart, PointBEnd ) */
    det( PointBStart[ 0 ], PointBStart[ 1 ], PointBEnd[ 0 ], PointBEnd[ 1 ], DB, Round );
#ifdef DEBUG
    printf( "\nDA is:" );
    mpfr_out_str ( stdout, 10, 0, DA, Round );
    printf( "\nDB is:" );
    mpfr_out_str ( stdout, 10, 0, DB, Round );
    fflush( stdout );
#endif
    /*             X        Y
     * DiffX = ( DiffAX, DiffBX )
     * X = det( D_x, D_y, DiffAX, DiffBX ) / div
     */
    /* det( D_x, D_y, DiffAX, DiffBX ) */
    det( DA, DB, DiffAX, DiffBX, Tmp1, Round );
    /* det( D_x, D_y, DiffAX, DiffBX ) / div */
    mpfr_div( Return[ 0 ], Tmp1, Div, Round );

    /*             X        Y
     * DiffY = ( DiffAY, DiffBY )
     * X = det( D_x, D_y, DiffAY, DiffBY ) / div
     */
    /* det( D_x, D_y, DiffAY, DiffBY ) */
    det( DA, DB, DiffAY, DiffBY, Tmp1, Round );
    /* det( D_x, D_y, DiffAX, DiffBX ) / div */
    mpfr_div( Return[ 1 ], Tmp1, Div, Round );
#ifdef DEBUG
    printf( "\nIntersection-X is:" );
    mpfr_out_str ( stdout, 10, 0, Return[ 0 ], Round );
    fflush( stdout );
#endif

#ifdef DEBUG
    printf( "\nIntersection-Y is:" );
    mpfr_out_str ( stdout, 10, 0, Return[ 1 ], Round );
    fflush( stdout );
#endif

    /* Housekeeping */
    mpfr_clear( DiffAX );
    mpfr_clear( DiffAY );
    mpfr_clear( DiffBX );
    mpfr_clear( DiffBY );
    mpfr_clear( Div );
    mpfr_clear( DA );
    mpfr_clear( DB );
    mpfr_clear( X );
    mpfr_clear( Y );
    mpfr_clear( Tmp1 );
    mpfr_free_cache ();
    return SUCCESS;
}

/**
 * intersectLine computes the touchpoints of to given a circle and a line
 * @param Return | array of mpfr_t | n = 4 | the computed touchpoints
 * @param PointA | array of mpfr_t | n = 2 | startpoint of the given line
 * @param PointB | array of mpfr_t | n = 2 | startpoint of the given line
 * @param Center | array of mpfr_t | n = 2 | circlecenter of the given circle
 * @param Radius | mpfr_t | the radius of the given circle
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return int | SUCCESS or NO_INTERSECTION or ONE_TOUCH
 */
extern unsigned short intersectCircleLine(
        mpfr_t* Return,
        mpfr_t* PointA,
        mpfr_t* PointB,
        mpfr_t* Center,
        mpfr_t Radius,
        mpfr_rnd_t Round
)
{
        mpfr_t ABX, ABXExp, ABY, ABYExp, ACX, ACXExp, ACY, ACYExp, A, C, Q, BBy2, PBy2, Found, AbScalingFactor1, AbScalingFactor2, Tmp1, Tmp2;
        int Compare;
#ifdef DEBUG
    printf( "\nStart circle-line-intersection." );
    fflush( stdout );
#endif

        mpfr_init( ABX );
        mpfr_init( ABXExp );
        mpfr_init( ABY );
        mpfr_init( ABYExp );
        mpfr_init( ACX );
        mpfr_init( ACXExp );
        mpfr_init( ACY );
        mpfr_init( ACYExp );
        mpfr_init( A );
        mpfr_init( C );
        mpfr_init( Q );
        mpfr_init( BBy2 );
        mpfr_init( PBy2 );
        mpfr_init( Found );
        mpfr_init( AbScalingFactor1 );
        mpfr_init( AbScalingFactor2 );
        mpfr_init( Tmp1 );
        mpfr_init( Tmp2 );

        /*
         * ABX = B_x - A_x
         * ABY = B_y - A_y
         * ACY = C_x - A_x
         * ACY = C_y - A_y
         */
        /* B_x - A_x */
        mpfr_sub( ABX, PointB[ 0 ], PointA[ 0 ], Round );
        /* B_y - A_y */
        mpfr_sub( ABY, PointB[ 1 ], PointA[ 1 ], Round );
        /* C_x - A_x */
        mpfr_sub( ACX, Center[ 0 ], PointA[ 0 ], Round );
        /* C_y - A_y */
        mpfr_sub( ACY, Center[ 1 ], PointA[ 1 ], Round );
#ifdef DEBUG
        printf( "\nABX is:" );
        mpfr_out_str ( stdout, 10, 0, ABX, Round );
        printf( "\nABY is:" );
        mpfr_out_str ( stdout, 10, 0, ABY, Round );
        printf( "\nACX is:" );
        mpfr_out_str ( stdout, 10, 0, ACX, Round );
        printf( "\nACY is:" );
        mpfr_out_str ( stdout, 10, 0, ACY, Round );
        fflush( stdout );
#endif

        /* A = ABX^2 + ABY^2 */
        /* ABX^2 */
        mpfr_pow_ui( ABXExp, ABX, 2, Round );
        /* ABY^2 */
        mpfr_pow_ui( ABYExp, ABY, 2, Round );
        /* ABX * baX + baY * baY */
        mpfr_add( A, ABXExp, ABYExp, Round );
#ifdef DEBUG
        printf( "\nA is:" );
        mpfr_out_str ( stdout, 10, 0, A, Round );
        fflush( stdout );
#endif

        /* BBy2 = ABX*ACX + ABY*ACY */
        /* ABX*ACX */
        mpfr_mul( Tmp1, ABX, ACX, Round );
        /* ABY*ACY */
        mpfr_mul( Tmp2, ABY, ACY, Round );
        /* ABX*ACX + ABY*ACY */
        mpfr_add( BBy2, Tmp1, Tmp2, Round );
#ifdef DEBUG
        printf( "\nBBy2 is:" );
        mpfr_out_str ( stdout, 10, 0, BBy2, Round );
        fflush( stdout );
#endif
        /* C = ACX^2 + ACY^2 - Radius^2 */
        /* ACX^2 */
        mpfr_pow_ui( ACXExp, ACX, 2, Round );
        /* ACY^2 */
        mpfr_pow_ui( ACYExp, ACY, 2, Round );

        /* Radius^2 */
        mpfr_pow_ui( Tmp1, Radius, 2, Round );
        /* ACX^2 + ACY^2 */
        mpfr_add( Tmp2, ACXExp, ACYExp, Round );
        /* ACX^2 + ACY^2 - Radius^2 */
        mpfr_sub( C, Tmp2, Tmp1, Round );
#ifdef DEBUG
        printf( "\nC is:" );
        mpfr_out_str ( stdout, 10, 0, C, Round );
        fflush( stdout );
#endif

        /* PBy2 = BBy2/a */
        mpfr_div( PBy2, BBy2, A, Round );
#ifdef DEBUG
        printf( "\nPBy2 is:" );
        mpfr_out_str ( stdout, 10, 0, PBy2, Round );
        fflush( stdout );
#endif

        /* Q = C / A */
        mpfr_div( Q, C, A, Round );
#ifdef DEBUG
        printf( "\nQ is:" );
        mpfr_out_str ( stdout, 10, 0, Q, Round );
        fflush( stdout );
#endif
        /* Found = PBy2^2 - Q */
        /* PBy2^2 */
        mpfr_pow_ui( Tmp1, PBy2, 2, Round );
        /* PBy2^2 - Q */
        mpfr_sub( Found, Tmp1, Q, Round );
#ifdef DEBUG
    printf( "\nFound:" );
    mpfr_out_str ( stdout, 10, 0, Found, Round );
    fflush( stdout );
#endif
        Compare = mpfr_cmp_ui( Found, 0 );
        if( CMP_LESS == Compare )
        {
#ifdef DEBUG
            printf( "\nNoting found." );
            fflush( stdout );
#endif
            /* Housekeeping */
            mpfr_clear( ABX );
            mpfr_clear( ABXExp );
            mpfr_clear( ABY );
            mpfr_clear( ABYExp );
            mpfr_clear( ACX );
            mpfr_clear( ACXExp );
            mpfr_clear( ACY );
            mpfr_clear( ACYExp );
            mpfr_clear( A );
            mpfr_clear( C );
            mpfr_clear( Q );
            mpfr_clear( BBy2 );
            mpfr_clear( PBy2 );
            mpfr_clear( Found );
            mpfr_clear( AbScalingFactor1 );
            mpfr_clear( AbScalingFactor2 );
            mpfr_clear( Tmp1 );
            mpfr_clear( Tmp2 );
            mpfr_free_cache ();
            return NO_INTERSECTION;
        }

        /* ... */
        mpfr_sqrt( Tmp2, Found, Round );

        /*
         * AbScalingFactor1 = -1*PBy2 + sqrt( Found )
         * AbScalingFactor1 = -1*PBy2 - sqrt( Found )
         */
        /* -1 * PBy2 */
        mpfr_mul_si( Tmp1, PBy2, -1, Round );
        /* -1 * PBy2 + sqrt( Found ) */
        mpfr_add( AbScalingFactor1, Tmp1, Tmp2, Round );
#ifdef DEBUG
        printf( "\nAbscalingFactor1 is:" );
        mpfr_out_str ( stdout, 10, 0, AbScalingFactor1, Round );
        fflush( stdout );
#endif
        /* -1 * PBy2 - sqrt( Found ) */
        mpfr_sub( AbScalingFactor2, Tmp1, Tmp2, Round );
#ifdef DEBUG
        printf( "\nAbscalingFactor2 is:" );
        mpfr_out_str ( stdout, 10, 0, AbScalingFactor2, Round );
        fflush( stdout );
#endif
        /*
         * P^1_x = PointA_x - ABX * AbScalingFactor1
         * P^1_y = PointA_y - ABY * AbScalingFactor1
         */
        /* ABX * AbScalingFactor1 */
        mpfr_mul( Tmp1, ABX, AbScalingFactor1, Round );
        /* PointA_x - ABX * AbScalingFactor1 */
        mpfr_sub( Return[ 0 ], PointA[ 0 ], Tmp1, Round );
        /* ABY * AbScalingFactor1 */
        mpfr_mul( Tmp1, ABY, AbScalingFactor1, Round );
        /* PointA_x - ABY * AbScalingFactor1 */
        mpfr_sub( Return[ 1 ], PointA[ 1 ], Tmp1, Round );
#ifdef DEBUG
        printf( "\nIntersection-1-X is:" );
        mpfr_out_str ( stdout, 10, 0, Return[ 0 ], Round );
        fflush( stdout );
#endif
#ifdef DEBUG
        printf( "\nIntersection-1-Y is:" );
        mpfr_out_str ( stdout, 10, 0, Return[ 1 ], Round );
        fflush( stdout );
#endif
        if( CMP_EQUAL == Compare )
        {
#ifdef DEBUG
            printf( "\nOne touch found." );
            fflush( stdout );
#endif
            /* Housekeeping */
            mpfr_clear( ABX );
            mpfr_clear( ABXExp );
            mpfr_clear( ABY );
            mpfr_clear( ABYExp );
            mpfr_clear( ACX );
            mpfr_clear( ACXExp );
            mpfr_clear( ACY );
            mpfr_clear( ACYExp );
            mpfr_clear( A );
            mpfr_clear( C );
            mpfr_clear( Q );
            mpfr_clear( BBy2 );
            mpfr_clear( PBy2 );
            mpfr_clear( Found );
            mpfr_clear( AbScalingFactor1 );
            mpfr_clear( AbScalingFactor2 );
            mpfr_clear( Tmp1 );
            mpfr_clear( Tmp2 );
            mpfr_free_cache ();
            return  ONE_TOUCH;
        }
        /*
         * P^2_x = PointA_x - ABX * AbScalingFactor2
         * P^2_y = PointA_y - ABY * AbScalingFactor2
         */
        /* ABX * AbScalingFactor2 */

        mpfr_mul( Tmp1, ABX, AbScalingFactor2, Round );
        /* PointA_x - ABX * AbScalingFactor2 */
        mpfr_sub( Return[ 2 ], PointA[ 0 ], Tmp1, Round );
        /* ABY * AbScalingFactor2 */
        mpfr_mul( Tmp1, ABY, AbScalingFactor2, Round );
        /* PointA_x - ABY * AbScalingFactor2 */
        mpfr_sub( Return[ 3 ], PointA[ 1 ], Tmp1, Round );
#ifdef DEBUG
        printf( "\nIntersection-2-X is:" );
        mpfr_out_str ( stdout, 10, 0, Return[ 2 ], Round );
        fflush( stdout );
#endif
#ifdef DEBUG
        printf( "\nIntersection-2-Y is:" );
        mpfr_out_str ( stdout, 10, 0, Return[ 3 ], Round );
        fflush( stdout );
#endif
        /* Housekeeping */
        mpfr_clear( ABX );
        mpfr_clear( ABXExp );
        mpfr_clear( ABY );
        mpfr_clear( ABYExp );
        mpfr_clear( ACX );
        mpfr_clear( ACXExp );
        mpfr_clear( ACY );
        mpfr_clear( ACYExp );
        mpfr_clear( A );
        mpfr_clear( C );
        mpfr_clear( Q );
        mpfr_clear( BBy2 );
        mpfr_clear( PBy2 );
        mpfr_clear( Found );
        mpfr_clear( AbScalingFactor1 );
        mpfr_clear( AbScalingFactor2 );
        mpfr_clear( Tmp1 );
        mpfr_clear( Tmp2 );
        mpfr_free_cache ();
        return SUCCESS;
}

/**
 * intersectCircles computes the touchpoints of to given circles
 * @param Return | array of mpfr_t | n = 4 | the computed touchpoints
 * @param ACenter | array of mpfr_t | n = 2 | x/y - coordinates of circle A
 * @param BCenter | array of mpfr_t | n = 2 | x/y - coordinates of circle B
 * @param ARadius | mpfr_t | the radius of circle A
 * @param BRadius | mpfr_t | the radius of circle A
 * @param Round | mpfr_rnd_t | which rounding to use
 * @return int | SUCCESS or NO_DISTANCE_OF_CENTERS or NO_INTERSECTION or ONE_TOUCH or INFINTIY
 */
extern unsigned short intersectCircles(
        mpfr_t* Return,
        mpfr_t* ACenter,
        mpfr_t* BCenter,
        mpfr_t ARadius,
        mpfr_t BRadius,
        mpfr_rnd_t Round
)
{

    mpfr_t ABX, ABXExp, ABY, ABYExp, DistanceCenter, X, Y, RadiusAExp, RadiusBExp, EX0, EX1, EY0, EY1, Tmp1;
    int Compare;

#ifdef DEBUG
    printf( "\nStart circle-circle-intersection." );
    fflush( stdout );
#endif

    if(
            CMP_EQUAL == mpfr_cmp( ACenter[ 0 ], BCenter[ 0 ] )
    &&
            CMP_EQUAL == mpfr_cmp( ACenter[ 1 ], BCenter[ 1 ] )
    )
    {
        return INFINTIY;
    }
    mpfr_init( ABX );
    mpfr_init( ABXExp );
    mpfr_init( ABY );
    mpfr_init( ABYExp );
    /*
     * Distance of circle centers
     */
    mpfr_init( DistanceCenter );
    /*
     * Distance on x-scale to center
     */
    mpfr_init( X );
    /*
     * Distance on y-scale to center
     */
    mpfr_init( Y );

    mpfr_init( RadiusAExp );
    mpfr_init( RadiusBExp );
    mpfr_init( EX0 );
    mpfr_init( EX1 );
    mpfr_init( EY0 );
    mpfr_init( EY1 );

    mpfr_init( Tmp1 );

    /* x_B - x_A */
    mpfr_sub( ABX, BCenter[ 0 ], ACenter[ 0 ], Round );
    /* y_B - y_A */
    mpfr_sub( ABY, BCenter[ 1 ], ACenter[ 1 ], Round );

#ifdef DEBUG
    printf( "\nABX is:" );
    mpfr_out_str ( stdout, 10, 0, ABX, Round );
    printf( "\nABY is:" );
    mpfr_out_str ( stdout, 10, 0, ABY, Round );
    fflush( stdout );
#endif

    /*
     * Distance = sqrt( ABX^2 + ABY^2 )
     */
    /* ABX^2 */
    mpfr_pow_ui  ( ABXExp, ABX, 2, Round );

    /* ABY^2 */
    mpfr_pow_ui  ( ABYExp, ABY, 2, Round );

    /* ABX^2 + ABY^2 */
    mpfr_add( DistanceCenter, ABXExp, ABYExp, Round );

    /* ... */
    mpfr_sqrt( DistanceCenter, DistanceCenter, Round );
#ifdef DEBUG
    printf( "\nDistance to center is:" );
    mpfr_out_str ( stdout, 10, 0, DistanceCenter, Round );
    fflush( stdout );
#endif
    /* 0 ?= Distance -> same point */
    Compare = mpfr_cmp_si( DistanceCenter, 0 );
    if( CMP_EQUAL == Compare )
    {
#ifdef DEBUG
        printf( "\nno intersection found." );
        fflush( stdout );
#endif
        /* Housekeeping */
        mpfr_clear( ABX );
        mpfr_clear( ABXExp );
        mpfr_clear( ABY );
        mpfr_clear( ABYExp );
        mpfr_clear( DistanceCenter );
        mpfr_clear( X );
        mpfr_clear( Y );
        mpfr_clear( RadiusAExp );
        mpfr_clear( RadiusBExp );
        mpfr_clear( EX0 );
        mpfr_clear( EX1 );
        mpfr_clear( EY0 );
        mpfr_clear( EY1 );
        mpfr_clear( Tmp1 );
        mpfr_free_cache ();
        return NO_DISTANCE_OF_CENTERS;
    }

    /*
     * x = (a*a + c*c - b*b) / (2*c);
     */
    /* Distance^2 */
    mpfr_pow_ui( Tmp1, DistanceCenter, 2, Round );
    /* ARadius^2 */
    mpfr_pow_ui( RadiusAExp, ARadius, 2, Round );
    /* BRadius^2 */
    mpfr_pow_ui( RadiusBExp, BRadius, 2, Round );
    /* ARadius^2 + Distance^2 */
    mpfr_add( Tmp1, RadiusAExp, DistanceCenter, Round );
    /* ARadius^2 + Distance^2- BRadius^2 */
    mpfr_sub( Tmp1, Tmp1, RadiusBExp, Round );
    /* 2 * Distance */
    mpfr_mul_ui( X, DistanceCenter, 2, Round );
    /* ( ARadius^2 + Distance^2- BRadius^2)/(2*Distance) */
    mpfr_div( X, Tmp1, X, Round );
#ifdef DEBUG
    printf( "\nDistance X is:" );
    mpfr_out_str ( stdout, 10, 0, X, Round );
    fflush( stdout );
#endif
    /*
     *  y = a^2 - x^2
     */
    /* X^2 */
    mpfr_pow_ui( Tmp1, X, 2, Round );
    /* ARadius^2 - X^2 */
    mpfr_sub( Y, ARadius, Tmp1, Round );
    Compare = mpfr_cmp_si( Y, 0 );
    if( CMP_LESS == Compare )
    {
        /* Housekeeping */
        mpfr_clear( ABX );
        mpfr_clear( ABXExp );
        mpfr_clear( ABY );
        mpfr_clear( ABYExp );
        mpfr_clear( DistanceCenter );
        mpfr_clear( X );
        mpfr_clear( Y );
        mpfr_clear( RadiusAExp );
        mpfr_clear( RadiusBExp );
        mpfr_clear( EX0 );
        mpfr_clear( EX1 );
        mpfr_clear( EY0 );
        mpfr_clear( EY1 );
        mpfr_clear( Tmp1 );
        mpfr_free_cache ();
        return NO_INTERSECTION;
    }

    if( CMP_EQUAL != Compare )
    {
        mpfr_sqrt( Y, Y, Round );
    }
#ifdef DEBUG
    printf( "\nDistance Y is:" );
    mpfr_out_str ( stdout, 10, 0, Y, Round );
    fflush( stdout );
#endif

    /*
     * Q^1_x = A_x + X * EX0
     * Q^1_y = A_y + X * EX1
     */

    /* ABX / Distance */
    mpfr_div( EX0, ABX, DistanceCenter, Round );
    /* ABY / Distance */
    mpfr_div( EX1, ABY, DistanceCenter, Round );

    /* x * EX0 */
    mpfr_mul( Tmp1, X, EX0, Round );
    /* A_x + x * EX0 */
    mpfr_add( Return[ 0 ], ACenter[ 0 ], Tmp1, Round );
    /* x * EX1 */
    mpfr_mul( Tmp1, X, EX1, Round );
    /* A_y + x * EX1 */
    mpfr_add( Return[ 1 ], ACenter[ 1 ], Tmp1, Round );
    if( CMP_EQUAL == Compare )
    {
#ifdef DEBUG
        printf( "\nFound only one intersection." );
        fflush( stdout );
#endif

#ifdef DEBUG
        printf( "\nIntersection-1-X is:" );
        mpfr_out_str ( stdout, 10, 0, Return[ 0 ], Round );
        fflush( stdout );
#endif

#ifdef DEBUG
        printf( "\nIntersection-1-Y is:" );
        mpfr_out_str ( stdout, 10, 0, Return[ 1 ], Round );
        fflush( stdout );
#endif
        /* Housekeeping */
        mpfr_clear( ABX );
        mpfr_clear( ABXExp );
        mpfr_clear( ABY );
        mpfr_clear( ABYExp );
        mpfr_clear( DistanceCenter );
        mpfr_clear( X );
        mpfr_clear( Y );
        mpfr_clear( RadiusAExp );
        mpfr_clear( RadiusBExp );
        mpfr_clear( EX0 );
        mpfr_clear( EX1 );
        mpfr_clear( EY0 );
        mpfr_clear( EY1 );
        mpfr_clear( Tmp1 );
        mpfr_free_cache ();
        return ONE_TOUCH;
    }

    /*
     * Q^2_x = Q^1_x + y * EY0
     * Q^2_y = Q^1_y - y * EY1
     * Q^1_x = Q^1_x + y * ey0
     * Q^1_y = Q^1_y + y * ey1
     */
    /* EY0 = EX1 */
    mpfr_set( EY0, EX1, Round );
    /* EY1 = EX0 */
    mpfr_set( EY1, EX0, Round );

    /* y * EY0 */
    mpfr_mul( Tmp1, Y, EY0, Round );
    /* Q^2_x + y * EY0 */
    mpfr_add( Return[ 2 ], Return[ 0 ], Tmp1, Round );

    /* Q^1_x += y * EY0 */
    mpfr_add( Return[ 0 ], Return[ 0 ], Tmp1, Round );

    /* y * EY1 */
    mpfr_mul( Tmp1, Y, EY1, Round );
    /* Q^2_x - y * EY1 */
    mpfr_add( Return[ 3 ], Return[ 1 ], Tmp1, Round );

    /* Q^1_y -= y * EY1 */
    mpfr_sub( Return[ 1 ], Return[ 1 ], Tmp1, Round );
#ifdef DEBUG
    printf( "\nIntersection-1-X is:" );
    mpfr_out_str ( stdout, 10, 0, Return[ 0 ], Round );
    fflush( stdout );
#endif
#ifdef DEBUG
    printf( "\nIntersection-1-Y is:" );
    mpfr_out_str ( stdout, 10, 0, Return[ 1 ], Round );
    fflush( stdout );
#endif
#ifdef DEBUG
    printf( "\nIntersection-2-X is:" );
    mpfr_out_str ( stdout, 10, 0, Return[ 2 ], Round );
    fflush( stdout );
#endif
#ifdef DEBUG
    printf( "\nIntersection-2-Y is:" );
    mpfr_out_str ( stdout, 10, 0, Return[ 3 ], Round );
    fflush( stdout );
#endif
    /* Housekeeping */
    mpfr_clear( ABX );
    mpfr_clear( ABY );
    mpfr_clear( DistanceCenter );
    mpfr_clear( X );
    mpfr_clear( Y );
    mpfr_clear( RadiusAExp );
    mpfr_clear( RadiusBExp );
    mpfr_clear( EX0 );
    mpfr_clear( EX1 );
    mpfr_clear( EY0 );
    mpfr_clear( EY1 );
    mpfr_clear( Tmp1 );
    mpfr_free_cache ();
    return SUCCESS;
}
/**
 * Caculate the grade by given radians and
 * @param Return | mpfr_t | the computed degree
 * @param Radians | mpfr_t | the given radians
 * @param Diameter | mpfr_t | the given diameter
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void getDegreeOnCircle( mpfr_t Return, mpfr_t Radians, mpfr_t Diameter, mpfr_rnd_t Round )
{
    //mpfr_const_pi
    /* b = alpha/360°⋅pi⋅d
     * (b*360°/pi)/d = alpha
     * d = 2*r */
    mpfr_t Tmp;

    mpfr_init( Tmp );

    if( FALSE == PiIsInit )
    {
        mpfr_init( Pi );
        mpfr_const_pi( Pi, Round );
        PiIsInit = TRUE;
    }
    /* b*360° */
    mpfr_mul_ui( Tmp, Radians, 360, Round );
    /* (b*360°/pi) */
    mpfr_div( Tmp, Tmp, Pi, Round );
    /* (b*360°/pi)/d */
    mpfr_div( Return, Tmp, Diameter, Round );
    mpfr_clear( Tmp );
    mpfr_free_cache ();
}
/**
 * Converts degrees to rad
 * @param Return | mpfr_t | the computed radians
 * @param Degree | mpfr_t | the given degrees
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void degree2Rad (mpfr_t Return, mpfr_t Degree, mpfr_rnd_t Round )
{
    /*
     * degrees = radians × 180° / π
     * degrees/180° × π = radians
     */
    if( FALSE == PiIsInit )
    {
        mpfr_init( Pi );
        mpfr_const_pi( Pi, Round );
        PiIsInit = TRUE;
    }
    /*degrees/180°*/
    mpfr_div_ui( Return, Degree, 180, Round );
    /*degrees/180° × π*/
    mpfr_mul( Return, Return, Pi, Round );
}
/**
 * Rotates a point at a given Point
 * @param Return | array of mpfr_t | n = 2 | the computed rotated point
 * @param Point | array of mpfr_t | n = 2 | Point which should be rotated
 * @param Center | array of mpfr_t | n = 2 | Point which should be the center of rotation
 * @param Degree | mpfr_t | degree for rotation
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void rotatePoint( mpfr_t* Return, mpfr_t* Point, mpfr_t* Center, mpfr_t Degree, mpfr_rnd_t Round )
{
    /* x' = x1 + cos(\theta) * (x - x1) - sin(\theta) * (y - y1)
     * y' = y1 + sin(\theta) * (x - x1) + cos(\theta) * (y - y1)
    */
    mpfr_t Sin, Cos, X, Y, Tmp1, Tmp2;
    mpfr_init( Sin );
    mpfr_init( Cos );
    mpfr_init( X );
    mpfr_init( Y );
    mpfr_init( Tmp1 );
    mpfr_init( Tmp2 );
    /* We have to calculate \theta in rad */
    degree2Rad( Tmp1, Degree, Round );
    /* sin(\theta) | cos(\theta) */
    mpfr_sin_cos( Sin, Cos, Tmp1, Round );
    /* (x - x_1) */
    mpfr_sub( X, Point[ 0 ], Center[ 0 ], Round  );
    /* (y - y_1) */
    mpfr_sub( Y, Point[ 1 ], Center[ 1 ], Round  );

    /* x' = x1 + cos(\theta) * (x - x1) - sin(\theta) * (y - y1) */
    /* cos(\theta) * (x - x1) */
    mpfr_mul( Tmp1, X, Cos, Round );
    /* sin(\theta) * (y - y1) */
    mpfr_mul( Tmp2, Y, Sin, Round );
    /* cos(\theta) * (x - x1) - sin(\theta) * (y - y1) */
    mpfr_sub( Tmp1, Tmp1, Tmp2, Round );
    /* x1 + cos(\theta) * (x - x1) - sin(\theta) * (y - y1) */
    mpfr_add( Return[ 0 ], Center[ 0 ], Tmp1, Round );

    /* y' = y1 + sin(\theta) * (x - x1) + cos(\theta) * (y - y1) */
    /* sin(\theta) * (x - x1) */
    mpfr_mul( Tmp1, X, Sin, Round );
    /* cos(\theta) * (y - y1) */
    mpfr_mul( Tmp2, Y, Cos, Round );
    /* sin(\theta) * (x - x1) + cos(\theta) * (y - y1) */
    mpfr_add( Tmp1, Tmp1, Tmp2, Round );
    /* y1 + sin(\theta) * (x - x1) + cos(\theta) * (y - y1) */
    mpfr_add( Return[ 1 ], Center[ 1 ], Tmp1, Round );
    /*Housekeeping*/
    mpfr_clear( Sin );
    mpfr_clear( Cos );
    mpfr_clear( X );
    mpfr_clear( Y );
    mpfr_clear( Tmp1 );
    mpfr_clear( Tmp2 );
    mpfr_free_cache();
}

/**
 * Computtes the einheitsvektor
 * @param Return | array of mpfr_t | n = 2 | the computed einheitsvektor
 * @param Point | array of mpfr_t | n = 2 | Point which from which should be derrived
 * @param Round | mpfr_rnd_t | which rounding to use
 */
extern void getEinheitsVector( mpfr_t* Return, mpfr_t* Point, mpfr_rnd_t Round )
{
    mpfr_t Tmp1, Tmp2;
    mpfr_init( Tmp1 );
    mpfr_init( Tmp2 );

    /* |v|=√x^2+y^2 */
    mpfr_pow_ui( Tmp1, Point[ 0 ], 2, Round );
    mpfr_pow_ui( Tmp2, Point[ 1 ], 2, Round );
    mpfr_add( Tmp1, Tmp1, Tmp2, Round );
    mpfr_sqrt( Tmp1, Tmp1, Round );

    mpfr_set_ui( Tmp2, 1, Round );
    mpfr_div( Tmp1, Tmp2, Tmp1, Round );
    mpfr_mul( Return[ 0 ], Point[ 0 ], Tmp1, Round );
    mpfr_mul( Return[ 1 ], Point[ 1 ], Tmp1, Round );

    mpfr_clear( Tmp1 );
    mpfr_clear( Tmp2 );
    mpfr_free_cache();
}
/**
 * Prints a error-message to stderr and quits the programm
 * @param Message | const char* | the message
 */
extern void errorAndOut( const char* Message )
{
    fprintf( stderr, "%s\n", Message);
    exit( 0 );
}
/**
 * Checks if a given char is a diget or not
 * @param Char | char | the given diget
 * @param IsHex | TRUE or FALSE | if hexdigets are allowed
 * @return | TRUE or FALSE
 */
extern unsigned short isDigit( char Char, unsigned  short IsHex )
{
    if( 48 > Char || 57 < Char )
    {
        if( TRUE == IsHex )
        {
            if(
                'a' == Char
            ||
                'A' == Char
            ||
                'b' == Char
            ||
                'B' == Char
            ||
                'c' == Char
            ||
                'C' == Char
            ||
                'd' == Char
            ||
                'D' == Char
            ||
                'e' == Char
            ||
                'E' == Char
            ||
                'f' == Char
            ||
                'F' == Char
            )
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return TRUE;
    }
}
/**
 * Checks if a given string is nummeric or not
 * Also hexstrings with prefix x|X or ocatalstrings with prefix o|O are allowed
 * @param String | const char* | the given input string
 * @return -1 if it's not a number or Base 8|10|16
 */
extern short validateString( const char* String )
{
    int Index, Start;
    short Base;
    unsigned short IsHex;
    unsigned short PercusionPoint;

    Base = 10;
    Start = 0;
    IsHex = FALSE;
    PercusionPoint = FALSE;

    if( 0 == strlen( String ) )
    {
        return -1;
    }

    if( 'X' == String[ 0 ] || 'x' == String[ 0 ] )
    {
        Start = 1;
        Base = 16;
        IsHex = TRUE;
    }
    else if( 'O' == String[ 0 ] || 'o' == String[ 0 ] )
    {
        Start = 1;
        Base = 8;
    }

    for( Index = Start; Index < strlen( String ); Index++ )
    {
        if( FALSE == isDigit( String[ Index ], IsHex ) )
        {
            if( '.' == String[ Index ] || ',' == String[ Index ] )
            {
                if( FALSE == PercusionPoint )
                {
                    PercusionPoint = TRUE;
                }
                else
                {
                    return -1;
                }
            }
        }
    }

    return Base;
}

/**
 * Returns a substring of given String
 * @param Source | char* | the input string
 * @param From | int | the startposition of the subset
 * @param Length | int | Length of the subset
 * @return | char* | the computed subset
 */
extern char* substring( char* Source, int From, int Length )
{
    char* Return;
    int Index1, Index2;

    if( NULL == Source )
    {
        return NULL;
    }

    if(-1 == Length)
    {
        Length = strlen( Source );
    }

    if( 0 >= Length )
    {
        return NULL;
    }

    if( From >= Length )
    {
        return NULL;
    }


    if(0 == From && strlen( Source ) == Length)
    {

        Return = (char *) malloc( ( strlen( Source )+1 )/sizeof(char) );
        if( NULL == Return )
        {
            errorAndOut( "Somethings wrong with the memory, jim." );
        }

        strcpy( Return , Source );
        return Return;
    }

    Return = (char *) malloc( ( ( Length-From ) + 1 )/sizeof(char) );
    if( NULL == Return )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    for( Index1=From, Index2=0; Length>Index2; Index1++, Index2++)
    {
        Return[ Index2 ] = Source[ Index1 ];
    }

    Return[ Index2 ] = '\0';
    return Return;
}
/*
int main()
{
    mpfr_t a, b;
    mpfr_t* Intersection;
    mpfr_t* ACenter;
    mpfr_t* BCenter;
    mpfr_t* CCenter;
    mpfr_t* DCenter;

    mpfr_set_default_prec ( 200 );
    Intersection = malloc( sizeof(mpfr_t) * 4);
    ACenter = malloc( sizeof(mpfr_t) * 2);
    BCenter = malloc( sizeof(mpfr_t) * 2);

    mpfr_init_set_si( ACenter[ 0 ], 10, MPFR_RNDD );
    mpfr_init_set_si( ACenter[ 1 ], 10, MPFR_RNDD );
    mpfr_init_set_si( BCenter[ 0 ], 11, MPFR_RNDD );
    mpfr_init_set_si( BCenter[ 1 ], 10, MPFR_RNDD );

    mpfr_init( Intersection[ 0 ] );
    mpfr_init( Intersection[ 1 ] );
    mpfr_init( Intersection[ 2 ] );
    mpfr_init( Intersection[ 3 ] );
    mpfr_set_nan( Intersection[ 0 ] );
    mpfr_set_nan( Intersection[ 1 ] );
    mpfr_set_nan( Intersection[ 2 ] );
    mpfr_set_nan( Intersection[ 3 ] );

    mpfr_init_set_ui( a, 10, MPFR_RNDD );
    mpfr_init_set_ui( b, 10, MPFR_RNDD );

    intersectCircles(
            Intersection,
            ACenter,
            BCenter,
            a,
            b,
            MPFR_RNDD
    );
    printf("\n");

    mpfr_clear( Intersection[ 0 ] );
    mpfr_clear( Intersection[ 1 ] );
    mpfr_clear( Intersection[ 2 ] );
    mpfr_clear( Intersection[ 3 ] );
    mpfr_clear( ACenter[ 0 ] );
    mpfr_clear( ACenter[ 1 ] );
    mpfr_clear( BCenter[ 0 ] );
    mpfr_clear( BCenter[ 1 ] );
    mpfr_clear( a );
    mpfr_free_cache ();
    free( Intersection );
    free( ACenter );
    free( BCenter );

    Intersection = malloc( sizeof(mpfr_t) * 4);
    ACenter = malloc( sizeof(mpfr_t) * 2);
    BCenter = malloc( sizeof(mpfr_t) * 2);
    CCenter = malloc( sizeof(mpfr_t) * 2);

    mpfr_init( Intersection[ 0 ] );
    mpfr_init( Intersection[ 1 ] );
    mpfr_init( Intersection[ 2 ] );
    mpfr_init( Intersection[ 3 ] );
    mpfr_set_nan( Intersection[ 0 ] );
    mpfr_set_nan( Intersection[ 1 ] );
    mpfr_set_nan( Intersection[ 2 ] );
    mpfr_set_nan( Intersection[ 3 ] );

    mpfr_init_set_si( ACenter[ 0 ], -3, MPFR_RNDD );
    mpfr_init_set_si( ACenter[ 1 ], -3, MPFR_RNDD );
    mpfr_init_set_si( BCenter[ 0 ], -3, MPFR_RNDD );
    mpfr_init_set_si( BCenter[ 1 ], 3, MPFR_RNDD );
    mpfr_init_set_si( CCenter[ 0 ], 0, MPFR_RNDD );
    mpfr_init_set_si( CCenter[ 1 ], 0, MPFR_RNDD );
    mpfr_init_set_ui( a, 5, MPFR_RNDD );

    intersectCircleLine(
            Intersection,
            ACenter,
            BCenter,
            CCenter,
            a,
            MPFR_RNDD
    );

    printf("\n");
    DCenter = malloc( sizeof(mpfr_t) * 2);
    mpfr_set_nan( Intersection[ 0 ] );
    mpfr_set_nan( Intersection[ 1 ] );
    mpfr_set_nan( Intersection[ 2 ] );
    mpfr_set_nan( Intersection[ 3 ] );
    mpfr_set_si( ACenter[ 0 ], 0, MPFR_RNDD );
    mpfr_set_si( ACenter[ 1 ], 0, MPFR_RNDD );
    mpfr_set_si( BCenter[ 0 ], 10, MPFR_RNDD );
    mpfr_set_si( BCenter[ 1 ], 10, MPFR_RNDD );
    mpfr_set_si( CCenter[ 0 ], 0, MPFR_RNDD );
    mpfr_set_si( CCenter[ 1 ], 10, MPFR_RNDD );
    mpfr_init_set_si( DCenter[ 0 ], 10, MPFR_RNDD );
    mpfr_init_set_si( DCenter[ 1 ], 0, MPFR_RNDD );
    intersectLines(
            Intersection,
            ACenter,
            BCenter,
            CCenter,
            DCenter,
            MPFR_RNDD
    );
    printf("\n");
#ifdef DEBUG
    printf( "\nX is:" );
    mpfr_out_str ( stdout, 10, 0, BCenter[ 0 ], MPFR_RNDD );
    printf( "\nY is:" );
    mpfr_out_str ( stdout, 10, 0, BCenter[ 1 ], MPFR_RNDD );
#endif
    printf("\n");
    mirrowPointOnX( Intersection, BCenter, MPFR_RNDD );
#ifdef DEBUG
    printf( "\nX is:" );
    mpfr_out_str ( stdout, 10, 0, Intersection[ 0 ], MPFR_RNDD );
    printf( "\nY is:" );
    mpfr_out_str ( stdout, 10, 0, Intersection[ 1 ], MPFR_RNDD );
#endif
    printf("\n");
    mirrowPointOnY( Intersection, Intersection, MPFR_RNDD );
#ifdef DEBUG
    printf( "\nX is:" );
    mpfr_out_str ( stdout, 10, 0, Intersection[ 0 ], MPFR_RNDD );
    printf( "\nY is:" );
    mpfr_out_str ( stdout, 10, 0, Intersection[ 1 ], MPFR_RNDD );
#endif
    printf("\n");
    mirrowPoint( Intersection, Intersection, MPFR_RNDD );
#ifdef DEBUG
    printf( "\nX is:" );
    mpfr_out_str ( stdout, 10, 0, Intersection[ 0 ], MPFR_RNDD );

    printf( "\nY is:" );
    mpfr_out_str ( stdout, 10, 0, Intersection[ 1 ], MPFR_RNDD );
#endif
    printf("\n");
    return 0;
}*/