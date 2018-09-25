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
#define MIRROW_X 0x42
#define MIRROW_Y 0x23
#define MIRROW_BOTH 0x65

typedef struct Map
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
    mpfr_t EZ;
    mpfr_t LZ;
    mpfr_t ZH;
    mpfr_t EH;
    mpfr_t KH;
    mpfr_t OH;
    mpfr_t SH;
    /* Punkte */
    mpfr_t* H;
    mpfr_t* K;
    /* Punkte/ Limitis */
    mpfr_t* G;
    mpfr_t* L;
    mpfr_t* S;
    mpfr_t* O;
    /* Limits */
    mpfr_t* X;
    mpfr_t* R;
    mpfr_t* M;
    mpfr_t* F;
    mpfr_t* P;
    mpfr_t* T;
    mpfr_t* N;
    mpfr_t* U;
} Map;

/*-------------------Vars----------------------*/

mpfr_rnd_t Round;
Map* PytagoMap;
char ErrorBuffer[256];
unsigned short Mirrowing;
/*-------------------Function Declare General -----------------*/
/**
 * Set bits of precusion Numbers
 * @param PrecusionString  | const char* | numeric value for bits of precusion
 */
void setPrecusion( const char* PrecusionString );
/**
 * Set round behavior of the programm
 * @param RoundString | const char* | numeric value for behavior
 * see: https://www.mpfr.org/mpfr-current/mpfr.html Abschnit 4.4
 * 1 = MPFR_RNDN: round to nearest (roundTiesToEven in IEEE 754-2008),
 * 2 = MPFR_RNDZ: round toward zero (roundTowardZero in IEEE 754-2008),
 * 3 = MPFR_RNDU: round toward plus infinity (roundTowardPositive in IEEE 754-2008),
 * 4 = MPFR_RNDD: round toward minus infinity (roundTowardNegative in IEEE 754-2008),
 * 5 = MPFR_RNDA: round away from zero.
 * .*= MPFR_RNDF: faithful rounding. This feature is currently experimental. Specific support for this rounding mode has been added to some functions, such as the basic operations (addition, subtraction, multiplication, square, division, square root) or when explicitly documented. It might also work with other functions, as it is possible that they do not need modification in their code; even though a correct behavior is not guaranteed yet (corrections were done when failures occurred in the test suite, but almost nothing has been checked manually), failures should be regarded as bugs and reported, so that they can be fixed.
 */
void setRound( const char* RoundString );
/**
 * Set the mirrowing behavior of the program
 * @param MirrowString | const char* | 1 for use mirrowing or .* for not
 */
void setMirrowing( const char* MirrowString );
/**
 * Computes the basic workbench( aka paralleologramm )
 * @param X | char* | numeric value of x-coordinate of the startpoint
 * @param Y | char* | numeric value of y-coordinate of the startpoint
 * @param Length | char* | edge length of the paralleologramm
 */
void makeDesk(
        char* X,
        char* Y,
        char* Length
);
/**
 * Sets a Point by move it on y-axis down
 * @returns NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
mpfr_t* setPointBySubOnY( mpfr_t* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by move it on y-axis up
 * @returns NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
mpfr_t* setPointByAddOnY( mpfr_t* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by move it on x-axis left
 * @returns NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
mpfr_t* setPointBySubOnX( mpfr_t* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by move it on x-axis right
 * @returns NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
mpfr_t* setPointByAddOnX( mpfr_t* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by mirrow it on the axis
 * @returns NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param MirrowPoint | mpfr_t* | coordinate center
 * @param Flag | int | MIRROW_BOTH for mirrow it on both axis, MIRROW_X for mirrow it on x-axis and MIRROW_Y for mirrow it on y-axis
 */
mpfr_t* setPointAndMirrow( mpfr_t* OldPoint, mpfr_t* MirrowPoint, int Flag );
/*-------------------Function Declare Platim -----------------*/
/**
 * Computes addtional Points ( like H ) the distances on HZ
 */
void makeHZPointsPlatim();
/**
 * Computes the limits of the parallel circuits
 */
void makeLimitsPlatim();
/*-------------------Function Declare Glob -----------------*/
/**
 * Computes addtional Points ( like L ) the distances on LZ
 */
void makeLZPointsGlob();
/*-------------------Functions-----------------*/
mpfr_t* setPointBySubOnY( mpfr_t* OldPoint, mpfr_t Distance )
{
    mpfr_t* NewPoint;
    NewPoint = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == NewPoint)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    mpfr_init( NewPoint[ 0 ] );
    mpfr_set( NewPoint[ 0 ], OldPoint[ 0 ], Round );

    mpfr_init( NewPoint[ 1 ] );
    mpfr_sub( NewPoint[ 1 ], OldPoint[ 1 ], Distance, Round );
    return NewPoint;
}

mpfr_t* setPointByAddOnY( mpfr_t* OldPoint, mpfr_t Distance  )
{
    mpfr_t* NewPoint;
    NewPoint = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == NewPoint)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    mpfr_init( NewPoint[ 0 ] );
    mpfr_set( NewPoint[ 0 ], OldPoint[ 0 ], Round );

    mpfr_init( NewPoint[ 1 ] );
    mpfr_add( NewPoint[ 1 ], OldPoint[ 1 ], Distance, Round );
    return NewPoint;
}

mpfr_t* setPointBySubOnX( mpfr_t* OldPoint, mpfr_t Distance  )
{
    mpfr_t* NewPoint;
    NewPoint = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == NewPoint)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    mpfr_init( NewPoint[ 1 ] );
    mpfr_set( NewPoint[ 1 ], OldPoint[ 1 ], Round );

    mpfr_init( NewPoint[ 0 ] );
    mpfr_sub( NewPoint[ 0 ], OldPoint[ 0 ], Distance, Round );

    return NewPoint;
}

mpfr_t* setPointByAddOnX( mpfr_t* OldPoint, mpfr_t Distance  )
{
    mpfr_t* NewPoint;
    NewPoint = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == NewPoint)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    mpfr_init( NewPoint[ 1 ] );
    mpfr_set( NewPoint[ 1 ], OldPoint[ 1 ], Round );

    mpfr_init( NewPoint[ 0 ] );
    mpfr_add( NewPoint[ 0 ], OldPoint[ 0 ], Distance, Round );

    return NewPoint;
}

mpfr_t* setPointAndMirrow( mpfr_t* OldPoint, mpfr_t* MirrowPoint, int Flag )
{
    mpfr_t* NewPoint;
    NewPoint = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == NewPoint)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    mpfr_init( NewPoint[ 0 ] );
    mpfr_init( NewPoint[ 1 ] );

    if( MIRROW_X == Flag )
    {
        mirrowPointOnX( NewPoint, MirrowPoint[ 0 ], OldPoint, Round );
    }
    else if( MIRROW_Y == Flag )
    {
        mirrowPointOnX( NewPoint, MirrowPoint[ 1 ], OldPoint, Round );
    }
    else
    {
        mirrowPoint( NewPoint, MirrowPoint[ 0 ], MirrowPoint[ 1 ], OldPoint, Round );
    }

    return NewPoint;
}

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

void setMirrowing( const char* MirrowString )
{
    char *EndPointer;

    Mirrowing = strtol(MirrowString, &EndPointer, 10);
    if ('\0' != *EndPointer )
    {
        snprintf(ErrorBuffer, 255, "Der übergebene Rundings-Parameter '%s' is fehlerhaft.", MirrowString);
        errorAndOut(ErrorBuffer);
    }

    printf("%i\n", Mirrowing == TRUE );
}

void freePoint( mpfr_t* Point )
{
    mpfr_clear(Point[0]);
    mpfr_clear(Point[1]);
    mpfr_free_cache();
    free(Point);
}

void makeDesk(
        char* X,
        char* Y,
        char* Length
)
{
    int BaseX, BaseY, BaseLength;

    /* Start Map */
    PytagoMap = ( Map* ) malloc( sizeof( Map ) * 1 );
    if(NULL == PytagoMap)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->A = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->A )
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
    printf("\nKantenlänge ist:");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Kantenlaenge[ 0 ], Round );
    printf("\nKantenlänge/2 ist:");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Kantenlaenge[ 1 ], Round );
    fflush( stdout );
#endif

    /* set point A */
    mpfr_init( PytagoMap->A[ 0 ] );
    mpfr_set_str( PytagoMap->A[ 0 ], X, BaseX, Round );
    mpfr_init( PytagoMap->A[ 1 ] );
    mpfr_set_str( PytagoMap->A[ 1 ], Y, BaseY, Round );

    /* C = A_x, A_y-Kantenlaenge/2 */
    PytagoMap->C = setPointBySubOnY( PytagoMap->A, PytagoMap->Kantenlaenge[ 1 ] );

    /* E = A_x+Kantenlaenge/2, A_y */
    PytagoMap->E = setPointByAddOnX( PytagoMap->A, PytagoMap->Kantenlaenge[ 1 ] );

    /* Z = C_x+Kantenlaenge/2, C_y */
    PytagoMap->Z = setPointByAddOnX( PytagoMap->C, PytagoMap->Kantenlaenge[ 1 ] );

    if( TRUE == Mirrowing )
    {
        /* B is mirrowed by E on X of A */
        PytagoMap->B = setPointAndMirrow( PytagoMap->A, PytagoMap->E, MIRROW_X  );
        /* D is mirrowed by Z on X of C */
        PytagoMap->D = setPointAndMirrow( PytagoMap->C, PytagoMap->Z, MIRROW_X  );
    }
    else
    {
        /* B = A_x+Kantenlaenge, A_y */
        PytagoMap->B = setPointByAddOnX( PytagoMap->A, PytagoMap->Kantenlaenge[ 0 ] );
        /* D = B_x, B_y-Kantenlaenge/2 */
        PytagoMap->D = setPointBySubOnY( PytagoMap->B, PytagoMap->Kantenlaenge[ 1 ] );
    }

#ifdef DEBUG
    printf("\nPoint A ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->A[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->A[ 1 ], Round );
    printf(")");
    fflush( stdout );
    printf("\nPoint B ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->B[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->B[ 1 ], Round );
    printf(")");
    fflush( stdout );
    printf("\nPoint C ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->C[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->C[ 1 ], Round );
    printf(")");
    fflush( stdout );
    printf("\nPoint D ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->D[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->D[ 1 ], Round );
    printf(")");
    fflush( stdout );
    printf("\nPoint E ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->E[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->E[ 1 ], Round );
    printf(")");
    fflush( stdout );
    printf("\nPoint Z ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Z[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Z[ 1 ], Round );
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
    printf("\nPoint H ist: (");
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
    printf("\nPoint KH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->KH, Round );
    fflush( stdout );
#endif
    /* K = H_x, H_y-KH */
    mpfr_init( PytagoMap->K[ 0 ] );
    mpfr_set( PytagoMap->K[ 0 ], PytagoMap->H[ 0 ], Round );

    mpfr_init( PytagoMap->K[ 1 ] );
    mpfr_sub( PytagoMap->K[ 1 ], PytagoMap->H[ 1 ], PytagoMap->KH, Round );
#ifdef DEBUG
    printf("\nPoint K ist: (");
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
    printf("\nPoint OH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->OH, Round );
    fflush( stdout );
#endif
    /* O = H_x, H_y-OH */
    mpfr_init( PytagoMap->O[ 0 ] );
    mpfr_set( PytagoMap->O[ 0 ], PytagoMap->H[ 0 ], Round );

    mpfr_init( PytagoMap->O[ 1 ] );
    mpfr_sub( PytagoMap->O[ 1 ], PytagoMap->H[ 1 ], PytagoMap->OH, Round );
#ifdef DEBUG
    printf("\nPoint O ist: (");
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
    printf("\nPoint SH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->SH, Round );
    fflush( stdout );
#endif
    /* S = H_x, H_y-SH */
    mpfr_init( PytagoMap->S[ 0 ] );
    mpfr_set( PytagoMap->S[ 0 ], PytagoMap->H[ 0 ], Round );

    mpfr_init( PytagoMap->S[ 1 ] );
    mpfr_sub( PytagoMap->S[ 1 ], PytagoMap->H[ 1 ], PytagoMap->SH, Round );
#ifdef DEBUG
    printf("\nPoint S ist: (");
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
    PytagoMap->F = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if( NULL == PytagoMap->F )
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
    mpfr_init( PytagoMap->F[ 0 ] );
    mpfr_init( PytagoMap->F[ 1 ] );


    mpfr_init( PytagoMap->L[ 0 ] );
    mpfr_init( PytagoMap->L[ 1 ] );
    mpfr_init( PytagoMap->P[ 0 ] );
    mpfr_init( PytagoMap->P[ 1 ] );

    mpfr_init( Tmp[ 0 ] );
    mpfr_init( Tmp[ 1 ] );
    mpfr_init( Tmp[ 2 ] );
    mpfr_init( Tmp[ 3 ] );

    mpfr_mul_si( Diameter, PytagoMap->KH, 2, Round );

    /* 18 mal : see 119 */
    mpfr_mul_si(Radians, PytagoMap->Einheit, 4, Round);
    mpfr_mul_si(Radians, Radians, 18, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of LKG ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif
    rotatePoint( PytagoMap->G, PytagoMap->K, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated G ist: (" );
    mpfr_out_str ( stdout, 10, 0, PytagoMap->G[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->G[ 1 ], Round );
    printf(")");
    fflush( stdout );

    /* Gerade von HG Verlängern */
    mpfr_sub( Tmp[ 0 ], PytagoMap->G[ 0 ], PytagoMap->H[ 0 ], Round );
    mpfr_sub( Tmp[ 1 ], PytagoMap->G[ 1 ], PytagoMap->H[ 1 ], Round );
    getEinheitsVector( Tmp, Tmp, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 10, Round );
    mpfr_mul_ui( Tmp[ 1 ], Tmp[ 1 ], 10, Round );
    mpfr_add( Tmp[ 0 ], Tmp[ 0 ], PytagoMap->H[ 0 ], Round );
    mpfr_add( Tmp[ 1 ], Tmp[ 1 ], PytagoMap->H[ 1 ], Round );

    printf("\nCalculated G' ist: (");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set M */
    rotatePoint( PytagoMap->M, PytagoMap->Z, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated M ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set X */
    rotatePoint( PytagoMap->X, PytagoMap->O, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated X ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->X[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->X[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set S */
    rotatePoint( PytagoMap->R, PytagoMap->S, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated R ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->R[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->R[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set F */
    /* Abstand im Intervall beim Äquator */
    getRadiansOnCircle( Radians, Degree, Diameter, Round );

    /* 18 mal : see 119 */
    mpfr_mul_si(Radians, Radians, 18, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
    mpfr_set_ui( Tmp[ 0 ], 180 , Round );
    mpfr_sub( Degree, Degree, Tmp[ 0 ] , Round );

#ifdef DEBUG
    printf("\nCalculated degree of MY/ZN ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif
    rotatePoint( PytagoMap->F, PytagoMap->Z, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated F ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->F[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->F[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Set L */
    /*

    mpfr_mul_si( Radians, PytagoMap->Einheit, 4, Round );
    mpfr_mul_si( Degree, Degree, -1, Round );
    rotatePoint( PytagoMap->L, PytagoMap->K, PytagoMap->H, Degree, Round );
#ifdef DEBUG
    printf("\nCalculated L ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif*/
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
    printf("\nSet mirrowing to ");
    fflush( stdout );
#endif
    setMirrowing( ArgVar[ 1 ] );
#ifdef DEBUG
    printf("\nSet precusion to ");
    fflush( stdout );
#endif
    setPrecusion( ArgVar[ 2 ] );
#ifdef DEBUG
    printf("\nSet round to ");
    fflush( stdout );
#endif
    setRound( ArgVar[ 3 ] );
#ifdef DEBUG
    printf("\nSet parallelogramm.");
    fflush( stdout );
#endif
    makeDesk( ArgVar[ 4 ], ArgVar[ 5 ], ArgVar[ 6 ] );
#ifdef DEBUG
printf("\n\nStart planimetik.");
    fflush( stdout );
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
    printf("\n\n");
    fflush( stdout );
#endif
freePoint( PytagoMap->H );
freePoint( PytagoMap->K );
freePoint( PytagoMap->G );
freePoint( PytagoMap->S );
freePoint( PytagoMap->O );
freePoint( PytagoMap->X );
freePoint( PytagoMap->R );
freePoint( PytagoMap->M );
freePoint( PytagoMap->F );
        /* ...
        mpfr_t* L
        mpfr_t* P;
        mpfr_t* T;
        mpfr_t* N;
        mpfr_t* U; */
/* --------- Als Globus --------- */
#ifdef DEBUG
    printf("\nStart glob.");
    fflush( stdout );
    printf("\nSet LZ Punkte.");
    fflush( stdout );
#endif
    makeLZPointsGlob();
#ifdef DEBUG
    printf("\n\n");
    fflush( stdout );
#endif

    return 0;
}


/*--------------------------Glob Functs -----------------------------------------*/
void makeLZPointsGlob()
{
    mpfr_t Tmp;

    mpfr_init( Tmp );

    PytagoMap->G = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->G )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->H = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->H)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->K = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->K)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PytagoMap->L = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PytagoMap->L)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    /* AC=EZ = 90E */
    /* 1E = EZ/90 */
    mpfr_init( PytagoMap->Einheit );
    mpfr_div_ui( PytagoMap->Einheit, PytagoMap->Kantenlaenge[ 1 ], 90, Round );
#ifdef DEBUG
    printf("\nEine Einheit ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Einheit, Round );
    fflush( stdout );
#endif
    mpfr_init( PytagoMap->EZ );
    mpfr_set( PytagoMap->EZ, PytagoMap->Kantenlaenge[ 1 ], Round );
#ifdef DEBUG
    printf("\nEZ ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->EZ, Round );
    fflush( stdout );
#endif

    /* ZH = 16 5/12 */
    mpfr_set_ui( Tmp, 5, Round );
    mpfr_div_ui( Tmp, Tmp, 12, Round );
    mpfr_add_ui( Tmp, Tmp, 16, Round );

    mpfr_mul( PytagoMap->ZH, Tmp, PytagoMap->Einheit, Round );
#ifdef DEBUG
    printf("\nHZ ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->ZH, Round );
    fflush( stdout );
#endif


    /* Housekeeping */
    mpfr_clear( Tmp );
    mpfr_free_cache();
}