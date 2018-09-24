/**
 * @autor Matthias Geisler (bitpogo) <geislemx@informatik.hu-berlin.de>
 * @licence GNU General Public License
 */
#include "./lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>
#include <string.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifdef LIBTEST
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
    }
#endif
typedef struct Planimetik
{
    /* Parallelogramm Punkte */
    mpfr_t* A;
    mpfr_t* B;
    mpfr_t* C;
    mpfr_t* D;
    mpfr_t* E;
    mpfr_t* Z;
    mpfr_t* Kantenlaenge;
    /* Meridan Punkte */
    mpfr_t Einheit;
    /* Strecken */
    mpfr_t ZH;
    mpfr_t EH;
    mpfr_t KH;
    mpfr_t OH;
    mpfr_t SH;
    /* Punkte */
    mpfr_t* H;
    mpfr_t* O;
    mpfr_t* K;
    mpfr_t* S;
    /* Limits */
    mpfr_t* X;
    mpfr_t* G;
    mpfr_t* R;
    mpfr_t* M;
    mpfr_t* P;
    mpfr_t* L;
    mpfr_t* T;
    mpfr_t* N;
} Planimetik;

/*-------------------Vars----------------------*/

mpfr_rnd_t Round;
Planimetik* PytagoMap;
char ErrorBuffer[256];

/*-------------------Functions-----------------*/
void setPrecusion( const char* PrecusionString )
{
    char* EndPointer;
    long int Precusion;

    Precusion = strtol( PrecusionString, &EndPointer, 10);

    if( '\0' != *EndPointer || 0 >= Precusion )
    {
        snprintf( ErrorBuffer, 255, "Der übergebene Precusion Parameter '%s' is fehlerhaft.", PrecusionString );
        errorAndOut( ErrorBuffer );
    }
    printf( "%lu", Precusion );
    /* Set precusion */
    mpfr_set_default_prec( (mpfr_prec_t) Precusion );
}

/*
* see: https://www.mpfr.org/mpfr-current/mpfr.html Abschnit 4.4
* MPFR_RNDN: round to nearest (roundTiesToEven in IEEE 754-2008),
* MPFR_RNDZ: round toward zero (roundTowardZero in IEEE 754-2008),
* MPFR_RNDU: round toward plus infinity (roundTowardPositive in IEEE 754-2008),
* MPFR_RNDD: round toward minus infinity (roundTowardNegative in IEEE 754-2008),
* MPFR_RNDA: round away from zero.
* MPFR_RNDF: faithful rounding. This feature is currently experimental. Specific support for this rounding mode has been added to some functions, such as the basic operations (addition, subtraction, multiplication, square, division, square root) or when explicitly documented. It might also work with other functions, as it is possible that they do not need modification in their code; even though a correct behavior is not guaranteed yet (corrections were done when failures occurred in the test suite, but almost nothing has been checked manually), failures should be regarded as bugs and reported, so that they can be fixed.
*/
void setRound( const char* RoundString )
{
    char* EndPointer;
    long int Rounding;
    printf( "%s\n", RoundString );
    Rounding = strtol( RoundString, &EndPointer, 10);
    if( '\0' != *EndPointer || 0 >= Rounding )
    {
        snprintf( ErrorBuffer, 255, "Der übergebene Rundings-Parameter '%s' is fehlerhaft.", RoundString );
        errorAndOut( ErrorBuffer );
    }

    if( 1 == Rounding )
    {
        Round = MPFR_RNDN;
    }
    else if( 2 == Rounding )
    {
        Round = MPFR_RNDZ;
    }
    else if( 3 == Rounding )
    {
        Round = MPFR_RNDU;
    }
    else if( 4 == Rounding )
    {
        Round = MPFR_RNDA;
    }
    else if( 5 == Rounding )
    {
        Round = MPFR_RNDF;
    }
    else
    {
        Round = MPFR_RNDZ;
    }
}

void makePlanimetik(
        char* X,
        char* Y,
        char* Length
)
{
    int BaseX, BaseY, BaseLength;

    PytagoMap = ( Planimetik* ) malloc( sizeof( Planimetik ) * 1 );
    if(NULL == PytagoMap)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->A = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->A )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->B = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->B)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->C = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->C)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->D = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->D)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->E = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->E)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->Z = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->Z)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->Kantenlaenge = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->Kantenlaenge)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    BaseX = validateString( X );
    if( -1 == BaseX )
    {
        snprintf( ErrorBuffer, 255, "Der übergebene X-Parameter '%s' is fehlerhaft.", X );
        errorAndOut( ErrorBuffer );
    }

    if( 10 != BaseX )
    {
        X = substring( X, 1, -1 );
    }

    BaseY = validateString( Y );
    if( -1 == BaseY )
    {
        snprintf( ErrorBuffer, 255, "Der übergebene Y-Parameter '%s' is fehlerhaft.", Y );
        errorAndOut( ErrorBuffer );
    }
    if( 10 != BaseY )
    {
        Y = substring( Y, 1, -1 );
    }

    BaseLength = validateString( Length );
    if( -1 == BaseLength )
    {
        snprintf( ErrorBuffer, 255, "Der übergebene Kantenlaenge-Parameter '%s' is fehlerhaft.", Length );
        errorAndOut( ErrorBuffer );
    }

    if( 10 != BaseLength )
    {
        Length = substring( Length, 1, -1 );
    }

    /* set Kantenlaenge */
    mpfr_init_set_str( PytagoMap->Kantenlaenge[ 0 ], Length, BaseLength, Round );
    /* Kantenlaenge/2 */
    mpfr_init( PytagoMap->Kantenlaenge[ 1 ] );
    mpfr_div_ui( PytagoMap->Kantenlaenge[ 1 ], PytagoMap->Kantenlaenge[ 0 ], 2, Round );
#ifdef DEBUG
    printf("\nKantenlänge is:");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Kantenlaenge[ 0 ], Round );
    printf("\nKantenlänge/2 is:");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Kantenlaenge[ 1 ], Round );
    fflush( stdout );
#endif

    /* set point A */
    mpfr_init( PytagoMap->A[ 0 ] );
    mpfr_set_str( PytagoMap->A[ 0 ], X, BaseX, Round );
    mpfr_init( PytagoMap->A[ 1 ] );
    mpfr_set_str( PytagoMap->A[ 1 ], Y, BaseX, Round );
#ifdef DEBUG
    printf("\nPoint A is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->A[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->A[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    mpfr_set_str( PytagoMap->A[ 1 ], Y, BaseY, Round );

    /* C = A_x, A_y-Kantenlaenge/2 */
    mpfr_init( PytagoMap->C[ 0 ] );
    mpfr_set( PytagoMap->C[ 0 ], PytagoMap->A[ 0 ], Round );

    mpfr_init( PytagoMap->C[ 1 ] );
    /*  A_x, A_y-Kantenlaenge/2 */
    mpfr_sub( PytagoMap->C[ 1 ], PytagoMap->A[ 1 ], PytagoMap->Kantenlaenge[ 1 ], Round );

#ifdef DEBUG
    printf("\nPoint C is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->C[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->C[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* E = A_x+Kantenlaenge/2, A_y */
    mpfr_init( PytagoMap->E[ 0 ] );
    mpfr_add( PytagoMap->E[ 0 ], PytagoMap->A[ 0 ], PytagoMap->Kantenlaenge[ 1 ], Round );

    mpfr_init( PytagoMap->E[ 1 ] );
    mpfr_set( PytagoMap->E[ 1 ], PytagoMap->A[ 1 ], Round );

#ifdef DEBUG
    printf("\nPoint E is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->E[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->E[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Z = C_x+Kantenlaenge/2, C_y */
    mpfr_init( PytagoMap->Z[ 0 ] );
    mpfr_add( PytagoMap->Z[ 0 ], PytagoMap->C[ 0 ], PytagoMap->Kantenlaenge[ 1 ], Round );

    mpfr_init( PytagoMap->Z[ 1 ] );
    mpfr_set( PytagoMap->Z[ 1 ], PytagoMap->C[ 1 ], Round );
#ifdef DEBUG
    printf("\nPoint Z is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Z[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Z[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* set point B */
    mpfr_init( PytagoMap->B[ 0 ] );
    mpfr_init( PytagoMap->B[ 1 ] );
#ifdef MIRROW
    mirrowPointOnX( PytagoMap->B, PytagoMap->E[0], PytagoMap->A, Round );
#else
    /* B = A_x+Kantenlaenge, A_y */
    mpfr_add( PytagoMap->B[ 0 ], PytagoMap->Kantenlaenge[ 0 ], PytagoMap->A[ 0 ], Round );
    mpfr_set( PytagoMap->B[ 1 ], PytagoMap->A[ 1 ], Round );
#endif
#ifdef DEBUG
    printf("\nPoint B is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->B[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->B[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set D */
    mpfr_init( PytagoMap->D[ 0 ] );
    mpfr_init( PytagoMap->D[ 1 ] );
#ifdef MIRROW
    mpfr_init( PytagoMap->D[ 0 ] );
    mpfr_init( PytagoMap->D[ 1 ] );
    mirrowPointOnX( PytagoMap->D, PytagoMap->E[0], PytagoMap->C, Round );
#else
    /* D = B_x, B_y-Kantenlaenge/2 */
    mpfr_set( PytagoMap->D[ 0 ], PytagoMap->B[ 0 ], Round );
    /*  A_x, A_y-Kantenlaenge/2 */
    mpfr_sub( PytagoMap->D[ 1 ], PytagoMap->B[ 1 ], PytagoMap->Kantenlaenge[ 1 ], Round );
#endif
#ifdef DEBUG
    printf("\nPoint D is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->D[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->D[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    if( 10 != BaseX )
    {
        free(X);
    }
    if( 10 != BaseY )
    {
        free(Y);
    }
    if( 10 != BaseLength )
    {
        free(Length);
    }
}

void makeHZPointsPlatim()
{
    /* EH = 34E */
    /* HZ = 131 5/12 */
    /* EZ -> 131 5/12 - 34 */
    mpfr_t Tmp, Tmp2;

    PytagoMap->H = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->H )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    PytagoMap->K = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->K )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    PytagoMap->O = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->O )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    PytagoMap->S = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->S )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    /* HZ = 131 5/12 */
    mpfr_init( Tmp2 );
    mpfr_init_set_ui( Tmp2, 5, Round );
    mpfr_div_ui( Tmp2, Tmp2, 12, Round );

    mpfr_init( Tmp );
    mpfr_set_ui( Tmp, 131, Round );
    mpfr_add( Tmp, Tmp, Tmp2, Round );
    mpfr_set( Tmp2, Tmp, Round );

    mpfr_sub_ui( Tmp, Tmp, 34, Round );
    /* 1E =  (Kantenlänge/2)/(131 5/12 - 34) */
    mpfr_init( PytagoMap->Einheit );
    mpfr_div( PytagoMap->Einheit, PytagoMap->Kantenlaenge[ 1 ], Tmp, Round );
#ifdef DEBUG
    printf("\nEine Einheit ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Einheit, Round );
    fflush( stdout );
#endif

    /*Set HZ = 131 5/12E */
    mpfr_init( PytagoMap->ZH );
    mpfr_mul( PytagoMap->ZH, PytagoMap->Einheit, Tmp2, Round );
#ifdef DEBUG
    printf("\nZH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->ZH, Round );
    fflush( stdout );
#endif

    /* EH = 34*Einheit */
    mpfr_init( PytagoMap->EH );
    mpfr_mul_ui( PytagoMap->EH, PytagoMap->Einheit, 34, Round );
#ifdef DEBUG
    printf("\nEH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->EH, Round );
    fflush( stdout );
#endif
    /* H = E_x, E_y+EH */
    mpfr_init( PytagoMap->H[ 0 ] );
    mpfr_set( PytagoMap->H[ 0 ], PytagoMap->E[ 0 ], Round );

    mpfr_init( PytagoMap->H[ 1 ] );
    mpfr_add( PytagoMap->H[ 1 ], PytagoMap->E[ 1 ], PytagoMap->EH, Round );
#ifdef DEBUG
    printf("\nPoint H is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->H[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->H[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* KH = 79*Einheit */
    mpfr_init( PytagoMap->KH );
    mpfr_mul_ui( PytagoMap->KH, PytagoMap->Einheit, 79, Round );
#ifdef DEBUG
    printf("\nPoint KH is: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->KH, Round );
    fflush( stdout );
#endif
    /* K = H_x, H_y-KH */
    mpfr_init( PytagoMap->K[ 0 ] );
    mpfr_set( PytagoMap->K[ 0 ], PytagoMap->H[ 0 ], Round );

    mpfr_init( PytagoMap->K[ 1 ] );
    mpfr_sub( PytagoMap->K[ 1 ], PytagoMap->H[ 1 ], PytagoMap->KH, Round );
#ifdef DEBUG
    printf("\nPoint K is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->K[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->K[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* OH = 52*Einheit siehe seite 121 */
    mpfr_init( PytagoMap->OH );
    mpfr_mul_ui( PytagoMap->OH, PytagoMap->Einheit, 52, Round );
#ifdef DEBUG
    printf("\nPoint OH is: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->OH, Round );
    fflush( stdout );
#endif
    /* O = H_x, H_y-OH */
    mpfr_init( PytagoMap->O[ 0 ] );
    mpfr_set( PytagoMap->O[ 0 ], PytagoMap->H[ 0 ], Round );

    mpfr_init( PytagoMap->O[ 1 ] );
    mpfr_sub( PytagoMap->O[ 1 ], PytagoMap->H[ 1 ], PytagoMap->OH, Round );
#ifdef DEBUG
    printf("\nPoint O is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->O[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->O[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* SH = 115*Einheit siehe seite 121 */
    mpfr_init( PytagoMap->SH );
    mpfr_mul_ui( PytagoMap->SH, PytagoMap->Einheit, 115, Round );
#ifdef DEBUG
    printf("\nPoint SH is: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->SH, Round );
    fflush( stdout );
#endif
    /* S = H_x, H_y-SH */
    mpfr_init( PytagoMap->S[ 0 ] );
    mpfr_set( PytagoMap->S[ 0 ], PytagoMap->H[ 0 ], Round );

    mpfr_init( PytagoMap->S[ 1 ] );
    mpfr_sub( PytagoMap->S[ 1 ], PytagoMap->H[ 1 ], PytagoMap->SH, Round );
#ifdef DEBUG
    printf("\nPoint S is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->S[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->S[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    mpfr_clear( Tmp );
    mpfr_clear( Tmp2 );
    mpfr_free_cache ();
}
/* GKL...4 Einheiten bogenmass */
void makeLimitsPlatim()
{
    mpfr_t Diameter, Radians, Degree;
    mpfr_t* Tmp;
    int Counter;

    mpfr_init( Diameter );
    mpfr_init( Degree );
    mpfr_init( Radians );

    PytagoMap->G = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->G )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->X = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->X )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->M = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->M )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->R = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->R )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }


    PytagoMap->L = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->L )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    PytagoMap->P = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->P )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    Tmp = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 4 );
    if( NULL == Tmp )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    mpfr_init( PytagoMap->G[ 0 ] );
    mpfr_init( PytagoMap->G[ 1 ] );
    mpfr_init( PytagoMap->X[ 0 ] );
    mpfr_init( PytagoMap->X[ 1 ] );
    mpfr_init( PytagoMap->M[ 0 ] );
    mpfr_init( PytagoMap->M[ 1 ] );
    mpfr_init( PytagoMap->R[ 0 ] );
    mpfr_init( PytagoMap->R[ 1 ] );


    mpfr_init( PytagoMap->L[ 0 ] );
    mpfr_init( PytagoMap->L[ 1 ] );
    mpfr_init( PytagoMap->P[ 0 ] );
    mpfr_init( PytagoMap->P[ 1 ] );

    mpfr_init( Tmp[ 0 ] );
    mpfr_init( Tmp[ 1 ] );
    mpfr_init( Tmp[ 2 ] );
    mpfr_init( Tmp[ 3 ] );

    mpfr_mul_si( Diameter, PytagoMap->KH, 2, Round );
    for( Counter = 1; Counter <= 18; Counter++ )
    {
        /* 18 mal : see 119 */
        mpfr_mul_si(Radians, PytagoMap->Einheit, 4, Round);
        mpfr_mul_si(Radians, Radians, -Counter, Round);

        getDegreeOnCircle(Degree, Radians, Diameter, Round);
        mpfr_add_si(Degree, Degree, 180, Round);
#ifdef DEBUG
        printf("\nCalculated %i degree of LKG is: ", Counter);
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        fflush( stdout );
#endif
        rotatePoint( PytagoMap->G, PytagoMap->K, PytagoMap->H, Degree, Round );
#ifdef DEBUG
        printf("\nCalculated %i G is: (", Counter );
        mpfr_out_str ( stdout, 10, 0, PytagoMap->G[ 0 ], Round );
        printf(", ");
        mpfr_out_str ( stdout, 10, 0, PytagoMap->G[ 1 ], Round );
        printf(")");
        fflush( stdout );
#endif
    }

    rotatePoint( PytagoMap->M, PytagoMap->Z, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated M is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Gerade von HG Verlängern */
    mpfr_mul_si( Tmp[ 0 ], Degree, 1000, Round );
    mpfr_add( Tmp[ 0 ], Tmp[ 0 ], PytagoMap->G[ 0 ], Round );

    mpfr_mul_si( Tmp[ 1 ], Degree, -1000, Round );
    mpfr_add( Tmp[ 1 ], Tmp[ 1 ], PytagoMap->G[ 1 ], Round );
#ifdef DEBUG
    printf("\nCalculated G' is: (");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    intersectCircleLine( Tmp, PytagoMap->H, Tmp, PytagoMap->H, PytagoMap->ZH, Round );
#ifdef DEBUG
    printf("\nCalculated M is: (");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set L */
    mpfr_mul_si( Radians, PytagoMap->Einheit, 4, Round );
    mpfr_mul_si( Degree, Degree, -1, Round );
    mpfr_add_si( Degree, Degree, 0, Round );
#ifdef DEBUG
    printf("\nCalculated degree of G is: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif
    rotatePoint( PytagoMap->L, PytagoMap->K, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated L is: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* Housekeeping */
    mpfr_clear( Diameter );
    mpfr_clear( Radians );
    mpfr_clear( Degree );
    mpfr_clear( Tmp[ 0 ] );
    mpfr_clear( Tmp[ 1 ] );
    mpfr_clear( Tmp[ 2 ] );
    mpfr_clear( Tmp[ 3 ] );
    mpfr_free_cache ();
    free( Tmp );
}

int main( int ArgCount, char* ArgVar[] )
{
    /*if( 7 != ArgCount )
    {
        snprintf( ErrorBuffer, 255, "Die übergebenen Parameter waren nicht ausreichend - es müssen exact 7 Parameter sein, allerding wurden %i übergeben.", ArgCount );
        errorAndOut( ErrorBuffer );
    }*/
#ifdef DEBUG
    printf("\nSet precusion.");
    fflush( stdout );
#endif
    setPrecusion( ArgVar[ 1 ] );
#ifdef DEBUG
    printf("\nSet round.");
    fflush( stdout );
#endif
    setRound( ArgVar[ 2 ] );
#ifdef DEBUG
    printf("\nSet parallelogramm.");
    fflush( stdout );
#endif
    makePlanimetik( ArgVar[ 3 ], ArgVar[ 4 ], ArgVar[ 5 ] );
#ifdef DEBUG
    printf("\nSet HZ Punkte.");
    fflush( stdout );
#endif
    makeHZPointsPlatim();
#ifdef DEBUG
    printf("\nSet Limits.");
    fflush( stdout );
#endif
    makeLimitsPlatim();
#ifdef DEBUG
    printf("\n");
    fflush( stdout );
#endif
    return 0;
}