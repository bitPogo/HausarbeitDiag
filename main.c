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
Planimetik* Hilfskonstrukt;
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

    Hilfskonstrukt = ( Planimetik* ) malloc( sizeof( Planimetik ) * 1 );
    if(NULL == Hilfskonstrukt)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->A = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->A )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->B = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->B)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->C = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->C)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->D = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->D)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->E = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->E)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->Z = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->Z)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    Hilfskonstrukt->Kantenlaenge = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Hilfskonstrukt->Kantenlaenge)
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
    mpfr_init_set_str( Hilfskonstrukt->Kantenlaenge[ 0 ], Length, BaseLength, Round );
    /* Kantenlaenge/2 */
    mpfr_init( Hilfskonstrukt->Kantenlaenge[ 1 ] );
    mpfr_div_ui( Hilfskonstrukt->Kantenlaenge[ 1 ], Hilfskonstrukt->Kantenlaenge[ 0 ], 2, Round );
#ifdef DEBUG
    printf("\nKantenlänge is:");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->Kantenlaenge[ 0 ], Round );
    fflush( stdout );
#endif

    /* set point A */
    mpfr_init( Hilfskonstrukt->A[ 0 ] );
    mpfr_set_str( Hilfskonstrukt->A[ 0 ], X, BaseX, Round );
    mpfr_init( Hilfskonstrukt->A[ 1 ] );
    mpfr_set_str( Hilfskonstrukt->A[ 1 ], Y, BaseX, Round );
#ifdef DEBUG
    printf("\nPoint A is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->A[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->A[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    mpfr_set_str( Hilfskonstrukt->A[ 1 ], Y, BaseY, Round );

    /* set point B */
    /* B = A_x+Kantenlaenge, A_y */
    mpfr_init( Hilfskonstrukt->B[ 0 ] );
    mpfr_add( Hilfskonstrukt->B[ 0 ], Hilfskonstrukt->Kantenlaenge[ 0 ], Hilfskonstrukt->A[ 0 ], Round );
    mpfr_init( Hilfskonstrukt->B[ 1 ] );
    mpfr_set( Hilfskonstrukt->B[ 1 ], Hilfskonstrukt->A[ 1 ], Round );
#ifdef DEBUG
    printf("\nPoint B is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->B[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->B[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* C = A_x, A_y-Kantenlaenge/2 */
    mpfr_init( Hilfskonstrukt->C[ 0 ] );
    mpfr_set( Hilfskonstrukt->C[ 0 ], Hilfskonstrukt->A[ 0 ], Round );

    mpfr_init( Hilfskonstrukt->C[ 1 ] );
    /*  A_x, A_y-Kantenlaenge/2 */
    mpfr_sub( Hilfskonstrukt->C[ 1 ], Hilfskonstrukt->A[ 1 ], Hilfskonstrukt->Kantenlaenge[ 1 ], Round );

#ifdef DEBUG
    printf("\nPoint C is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->C[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->C[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* D = B_x, B_y-Kantenlaenge/2 */
    mpfr_init( Hilfskonstrukt->D[ 0 ] );
    mpfr_set( Hilfskonstrukt->D[ 0 ], Hilfskonstrukt->B[ 0 ], Round );

    mpfr_init( Hilfskonstrukt->D[ 1 ] );
    /*  A_x, A_y-Kantenlaenge/2 */
    mpfr_sub( Hilfskonstrukt->D[ 1 ], Hilfskonstrukt->B[ 1 ], Hilfskonstrukt->Kantenlaenge[ 1 ], Round );

#ifdef DEBUG
    printf("\nPoint D is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->D[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->D[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* E = A_x-Kantenlaenge/2, A_y */
    mpfr_init( Hilfskonstrukt->E[ 0 ] );
    mpfr_sub( Hilfskonstrukt->E[ 0 ], Hilfskonstrukt->A[ 0 ], Hilfskonstrukt->Kantenlaenge[ 1 ], Round );

    mpfr_init( Hilfskonstrukt->E[ 1 ] );
    mpfr_set( Hilfskonstrukt->E[ 1 ], Hilfskonstrukt->A[ 1 ], Round );

#ifdef DEBUG
    printf("\nPoint E is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->E[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->E[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Z = C_x-Kantenlaenge/2, C_y */
    mpfr_init( Hilfskonstrukt->Z[ 0 ] );
    mpfr_sub( Hilfskonstrukt->Z[ 0 ], Hilfskonstrukt->C[ 0 ], Hilfskonstrukt->Kantenlaenge[ 1 ], Round );

    mpfr_init( Hilfskonstrukt->Z[ 1 ] );
    mpfr_set( Hilfskonstrukt->Z[ 1 ], Hilfskonstrukt->C[ 1 ], Round );

#ifdef DEBUG
    printf("\nPoint Z is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->Z[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->Z[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
}

void makeHZPointsPlatim()
{
    /* EH = 34E */
    /* HZ = 131 5/12 */
    /* EZ -> 131 5/12 - 34 */
    mpfr_t Tmp, Tmp2;

    Hilfskonstrukt->H = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == Hilfskonstrukt->H )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    Hilfskonstrukt->K = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == Hilfskonstrukt->K )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    Hilfskonstrukt->O = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == Hilfskonstrukt->O )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    Hilfskonstrukt->S = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == Hilfskonstrukt->S )
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
    mpfr_sub_ui( Tmp, Tmp, 34, Round );
    /* 1E =  (Kantenlänge/2)/(131 5/12 - 34) */
    mpfr_init( Hilfskonstrukt->Einheit );
    mpfr_div( Hilfskonstrukt->Einheit, Hilfskonstrukt->Kantenlaenge[ 1 ], Tmp, Round );
#ifdef DEBUG
    printf("\nEine Einheit ist: ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->Einheit, Round );
    fflush( stdout );
#endif
    /* EH = 34*Einheit */
    mpfr_init( Hilfskonstrukt->EH );
    mpfr_mul_ui( Hilfskonstrukt->EH, Hilfskonstrukt->Einheit, 34, Round );
#ifdef DEBUG
    printf("\nEH ist: ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->EH, Round );
    fflush( stdout );
#endif
    /* H = E_x, E_y+EH */
    mpfr_init( Hilfskonstrukt->H[ 0 ] );
    mpfr_set( Hilfskonstrukt->H[ 0 ], Hilfskonstrukt->E[ 0 ], Round );

    mpfr_init( Hilfskonstrukt->H[ 1 ] );
    mpfr_add( Hilfskonstrukt->H[ 1 ], Hilfskonstrukt->E[ 1 ], Hilfskonstrukt->EH, Round );
#ifdef DEBUG
    printf("\nPoint H is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->H[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->H[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* KH = 79*Einheit */
    mpfr_init( Hilfskonstrukt->KH );
    mpfr_mul_ui( Hilfskonstrukt->KH, Hilfskonstrukt->Einheit, 79, Round );
#ifdef DEBUG
    printf("\nPoint KH is: ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->KH, Round );
    fflush( stdout );
#endif
    /* K = H_x, H_y-KH */
    mpfr_init( Hilfskonstrukt->K[ 0 ] );
    mpfr_set( Hilfskonstrukt->K[ 0 ], Hilfskonstrukt->H[ 0 ], Round );

    mpfr_init( Hilfskonstrukt->K[ 1 ] );
    mpfr_sub( Hilfskonstrukt->K[ 1 ], Hilfskonstrukt->H[ 1 ], Hilfskonstrukt->KH, Round );
#ifdef DEBUG
    printf("\nPoint K is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->K[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->K[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* OH = 52*Einheit siehe seite 121 */
    mpfr_init( Hilfskonstrukt->OH );
    mpfr_mul_ui( Hilfskonstrukt->OH, Hilfskonstrukt->Einheit, 52, Round );
#ifdef DEBUG
    printf("\nPoint OH is: ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->OH, Round );
    fflush( stdout );
#endif
    /* O = H_x, H_y-OH */
    mpfr_init( Hilfskonstrukt->O[ 0 ] );
    mpfr_set( Hilfskonstrukt->O[ 0 ], Hilfskonstrukt->H[ 0 ], Round );

    mpfr_init( Hilfskonstrukt->O[ 1 ] );
    mpfr_sub( Hilfskonstrukt->O[ 1 ], Hilfskonstrukt->H[ 1 ], Hilfskonstrukt->OH, Round );
#ifdef DEBUG
    printf("\nPoint K is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->O[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->O[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* SH = 115*Einheit siehe seite 121 */
    mpfr_init( Hilfskonstrukt->SH );
    mpfr_mul_ui( Hilfskonstrukt->SH, Hilfskonstrukt->Einheit, 115, Round );
#ifdef DEBUG
    printf("\nPoint SH is: ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->SH, Round );
    fflush( stdout );
#endif
    /* S = H_x, H_y-SH */
    mpfr_init( Hilfskonstrukt->S[ 0 ] );
    mpfr_set( Hilfskonstrukt->S[ 0 ], Hilfskonstrukt->H[ 0 ], Round );

    mpfr_init( Hilfskonstrukt->S[ 1 ] );
    mpfr_sub( Hilfskonstrukt->S[ 1 ], Hilfskonstrukt->H[ 1 ], Hilfskonstrukt->SH, Round );
#ifdef DEBUG
    printf("\nPoint S is: (");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->S[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Hilfskonstrukt->S[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
}

makeLimitsPlatim()
{

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
    printf("\n");
    fflush( stdout );
#endif
    return 0;
}