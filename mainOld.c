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
    mpfr_t EH;
    mpfr_t KH;
    mpfr_t OH;
    mpfr_t SH;
    mpfr_t GH;
    mpfr_t ZH;
    mpfr_t EL;
    mpfr_t HL;
    mpfr_t GL;
    mpfr_t KL;
    mpfr_t ZL;
    mpfr_t IHJ;
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
    mpfr_t* Y;
    mpfr_t* V;
    /* Own */
    mpfr_t* I;
    mpfr_t* J;
} Map;

/*-------------------Vars----------------------*/

mpfr_rnd_t Round;
Map* PtolemMap;
char ErrorBuffer[256];
unsigned short Mirrowing;
/*-------------------Function Declare General -----------------*/
/**
 * Set bits of precusion Numbers
 * @param PrecusionString  | const char* | numeric value for bits of precusion
 */
void setPrecision( const char* PrecusionString );
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
 * @return NewPoint | mpfr_t* | the computed Point
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
 * @return NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
mpfr_t* setPointBySubOnX( mpfr_t* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by move it on x-axis right
 * @return NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
mpfr_t* setPointByAddOnX( mpfr_t* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by mirrow it on the axis
 * @return NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param MirrowPoint | mpfr_t* | coordinate center
 * @param Flag | int | MIRROW_BOTH for mirrow it on both axis, MIRROW_X for mirrow it on x-axis and MIRROW_Y for mirrow it on y-axis
 */
mpfr_t* setPointAndMirrow( mpfr_t* OldPoint, mpfr_t* MirrowPoint, int Flag );
/**
 * Sets a point by rotate it on specific circlecenter
 * @param OldPoint | array of mpfr_t | n = 2 | Point which should be rotated
 * @param CircleCenter | array of mpfr_t | n = 2 | Point which should be the center of rotation
 * @param Degree | mpfr_t | degree for rotation
 * @return NewPoint | mpfr_t* | the computed Point
 */
mpfr_t* setPointAndRotate( mpfr_t* OldPoint, mpfr_t* CircleCenter, mpfr_t Degree );
/**
 * inits a mpfr_t structure by mulitplying something with somthing
 * @param Target | mpfr_t | target structure
 * @param Mul1 | mpfr_t | Factor 1
 * @param Mul2 | mpfr_t | Factor 2
 */
void initAndMultiply( mpfr_t Target, mpfr_t Mul1, mpfr_t Mul2 );
/**
 * inits a mpfr_t structure by mulitplying something with somthing
 * @param Target | mpfr_t | target structure
 * @param Mul1 | mpfr_t | Factor 1
 * @param Mul2 | unsigned int | Factor 2
 */
void initAndMultiplyUi( mpfr_t Something, mpfr_t Mul1, unsigned int Mul2 );
/**
 * allocs and inits a array of mpfr_t
 * @return NewPoint | mpfr_t* |  array of mpfr_t with size 2
 */
mpfr_t* startPoint( void );
/*-------------------Function Declare Platim -----------------*/
/**
 * Computes addtional Points ( like H ) the distances on HZ
 */
void makeHZPointsPlatim();
/**
 * Computes the limits of the parallel circuits
 */
void makeLimitsPlatim();
/**
 * Computes the distances in the plantimetric model
 */
void calacDistancesPlatim();
/*-------------------Function Declare Glob -----------------*/
/**
 * Computes the distances in the glob model
 */
void calacDistancesGlob();
/**
 * Computes addtional Points ( like L ) the distances on LZ
 */
void makeLZPointsGlob();
/**
 * Computes the limits of the parallel circuits
 */
void makeLimitsGlob();
/*-------------------Subroutines-----------------*/
void printCoordinates()
{
    
}

mpfr_t* setPointBySubOnY( mpfr_t* OldPoint, mpfr_t Distance )
{
    mpfr_t* NewPoint;
    NewPoint = startPoint();

    mpfr_set( NewPoint[ 0 ], OldPoint[ 0 ], Round );
    mpfr_sub( NewPoint[ 1 ], OldPoint[ 1 ], Distance, Round );
    return NewPoint;
}

mpfr_t* setPointByAddOnY( mpfr_t* OldPoint, mpfr_t Distance  )
{
    mpfr_t* NewPoint;
    NewPoint = startPoint();

    mpfr_set( NewPoint[ 0 ], OldPoint[ 0 ], Round );
    mpfr_add( NewPoint[ 1 ], OldPoint[ 1 ], Distance, Round );
    return NewPoint;
}

mpfr_t* setPointBySubOnX( mpfr_t* OldPoint, mpfr_t Distance  )
{
    mpfr_t* NewPoint;
    NewPoint = startPoint();

    mpfr_set( NewPoint[ 1 ], OldPoint[ 1 ], Round );
    mpfr_sub( NewPoint[ 0 ], OldPoint[ 0 ], Distance, Round );

    return NewPoint;
}

mpfr_t* setPointByAddOnX( mpfr_t* OldPoint, mpfr_t Distance  )
{
    mpfr_t* NewPoint;
    NewPoint = startPoint();

    mpfr_set( NewPoint[ 1 ], OldPoint[ 1 ], Round );
    mpfr_add( NewPoint[ 0 ], OldPoint[ 0 ], Distance, Round );

    return NewPoint;
}

mpfr_t* setPointAndMirrow( mpfr_t* OldPoint, mpfr_t* MirrowPoint, int Flag )
{
    mpfr_t* NewPoint;
    NewPoint = startPoint();

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

mpfr_t* setPointAndRotate( mpfr_t* OldPoint, mpfr_t* CircleCenter, mpfr_t Degree )
{
    mpfr_t* NewPoint;
    NewPoint = startPoint();

    rotatePoint( NewPoint, OldPoint, CircleCenter, Degree, Round );
    return NewPoint;
}

mpfr_t* startPoint( void )
{
    mpfr_t* NewPoint;
    NewPoint = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == NewPoint)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    mpfr_init( NewPoint[ 0 ] );
    mpfr_init( NewPoint[ 1 ] );
    return NewPoint;
}

void initAndMultiply( mpfr_t Something, mpfr_t Mul1, mpfr_t Mul2 )
{
    mpfr_init( Something );
    mpfr_mul( Something, Mul1, Mul2, Round );
}

void initAndMultiplyUi( mpfr_t Something, mpfr_t Mul1, unsigned int Mul2 )
{
    mpfr_init( Something );
    mpfr_mul_ui( Something, Mul1, Mul2, Round );
}

/*-----------------------------------------------Programm Behavior-------------------------------------*/

void setPrecision( const char* PrecusionString )
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
    PtolemMap = ( Map* ) malloc( sizeof( Map ) * 1 );
    if(NULL == PtolemMap)
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PtolemMap->A = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PtolemMap->A )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
    PtolemMap->Kantenlaenge = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == PtolemMap->Kantenlaenge)
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
    mpfr_init_set_str( PtolemMap->Kantenlaenge[ 0 ], Length, BaseLength, Round );
    /* Kantenlaenge/2 */
    mpfr_init( PtolemMap->Kantenlaenge[ 1 ] );
    mpfr_div_ui( PtolemMap->Kantenlaenge[ 1 ], PtolemMap->Kantenlaenge[ 0 ], 2, Round );
#ifdef DEBUG
    printf("\nKantenlänge ist:");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Kantenlaenge[ 0 ], Round );
    printf("\nKantenlänge/2 ist:");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Kantenlaenge[ 1 ], Round );
    fflush( stdout );
#endif

    /* set point A */
    mpfr_init( PtolemMap->A[ 0 ] );
    mpfr_set_str( PtolemMap->A[ 0 ], X, BaseX, Round );
    mpfr_init( PtolemMap->A[ 1 ] );
    mpfr_set_str( PtolemMap->A[ 1 ], Y, BaseY, Round );

    /* C = A_x, A_y-Kantenlaenge/2 */
    PtolemMap->C = setPointBySubOnY( PtolemMap->A, PtolemMap->Kantenlaenge[ 1 ] );

    /* E = A_x+Kantenlaenge/2, A_y */
    PtolemMap->E = setPointByAddOnX( PtolemMap->A, PtolemMap->Kantenlaenge[ 1 ] );

    /* Z = C_x+Kantenlaenge/2, C_y */
    PtolemMap->Z = setPointByAddOnX( PtolemMap->C, PtolemMap->Kantenlaenge[ 1 ] );

    if( TRUE == Mirrowing )
    {
        /* B is mirrowed by E on X of A */
        PtolemMap->B = setPointAndMirrow( PtolemMap->A, PtolemMap->E, MIRROW_X  );
        /* D is mirrowed by Z on X of C */
        PtolemMap->D = setPointAndMirrow( PtolemMap->C, PtolemMap->Z, MIRROW_X  );
    }
    else
    {
        /* B = A_x+Kantenlaenge, A_y */
        PtolemMap->B = setPointByAddOnX( PtolemMap->A, PtolemMap->Kantenlaenge[ 0 ] );
        /* D = B_x, B_y-Kantenlaenge/2 */
        PtolemMap->D = setPointBySubOnY( PtolemMap->B, PtolemMap->Kantenlaenge[ 1 ] );
    }

#ifdef DEBUG
    printf("\nPoint A ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->A[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->A[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint B ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->B[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->B[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint C ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->C[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->C[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint D ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->D[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->D[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint E ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->E[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->E[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint Z ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Z[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Z[ 1 ], Round );
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

/*-------------------Subroutines Planim-----------------*/

void calacDistancesPlatim()
{
    /* EH = 34E */
    /* HZ = 131 5/12 */
    /* EZ -> 131 5/12 - 34 */
    mpfr_t Tmp, Tmp2;

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
    mpfr_init( PtolemMap->Einheit );
    mpfr_div( PtolemMap->Einheit, PtolemMap->Kantenlaenge[ 1 ], Tmp, Round );

#ifdef DEBUG
    printf("\nEine Einheit beträgt: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Einheit, Round );
    fflush( stdout );
#endif

    /* Calculate Einheiten */
    /*Set HZ = 131 5/12E */
    initAndMultiply( PtolemMap->ZH, PtolemMap->Einheit, Tmp2 );

    /* EH = 34*Einheit */
    initAndMultiplyUi( PtolemMap->EH, PtolemMap->Einheit, 34 );

    /* KH = 79*Einheit */
    initAndMultiplyUi( PtolemMap->KH, PtolemMap->Einheit, 79 );

    /* OH = 52*Einheit siehe seite 121 */
    initAndMultiplyUi( PtolemMap->OH, PtolemMap->Einheit, 52 );

    /* SH = 115*Einheit siehe seite 121 */
    initAndMultiplyUi( PtolemMap->SH, PtolemMap->Einheit, 115 );

#ifdef DEBUG
    printf("\nEH ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->EH, Round );
    fflush( stdout );
    printf("\nKH ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->KH, Round );
    fflush( stdout );
    printf("\nOH ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->OH, Round );
    fflush( stdout );
    printf("\nSH ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->SH, Round );
    fflush( stdout );
    printf("\nZH ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->ZH, Round );
    fflush( stdout );
#endif

    /* Cleanup */
    mpfr_clear( Tmp );
    mpfr_clear( Tmp2 );
    mpfr_free_cache ();
}

void makeHZPointsPlatim()
{
    /* H = E_x, E_y+EH */
    PtolemMap->H = setPointByAddOnY( PtolemMap->E, PtolemMap->EH );

    /* K = H_x, H_y-KH */
    PtolemMap->K = setPointBySubOnY( PtolemMap->H, PtolemMap->KH );

    /* O = H_x, H_y-OH */
    PtolemMap->O = setPointBySubOnY( PtolemMap->H, PtolemMap->OH );

    /* S = H_x, H_y-SH */
    PtolemMap->S = setPointBySubOnY( PtolemMap->H, PtolemMap->SH );

#ifdef DEBUG
    printf("\nPoint H ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->H[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->H[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint K ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->K[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->K[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint O ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->O[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->O[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint S ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->S[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->S[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
}
/* GKL...4 Einheiten bogenmass */
void makeLimitsPlatim()
{
    mpfr_t Diameter, Radians, Degree;
    mpfr_t* Tmp;

    Tmp = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 2 );
    if(NULL == Tmp )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }

    mpfr_init( Diameter );
    mpfr_init( Degree );
    mpfr_init( Radians );
    mpfr_init( Tmp[ 0 ] );
    mpfr_init( Tmp[ 1 ] );


    mpfr_mul_si( Diameter, PtolemMap->KH, 2, Round );
    /* 18 mal : see 119 */
    mpfr_mul_si(Radians, PtolemMap->Einheit, 4, Round);

    mpfr_mul_si(Radians, Radians, 18, Round);
    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of LKG ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nCalculated radians of LKG ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );
    fflush( stdout );
#endif
    /* G is depending on K see S 199 3.ff */
    PtolemMap->G = setPointAndRotate( PtolemMap->K, PtolemMap->H, Degree );
    /* M is depending on Z see S 199 3.ff */
    PtolemMap->M = setPointAndRotate( PtolemMap->Z, PtolemMap->H, Degree );
    /* O is depending on O see S 199 3.ff */
    PtolemMap->X = setPointAndRotate( PtolemMap->O, PtolemMap->H, Degree );
    /* S is depending on R see S 199 3.ff  */
    PtolemMap->R = setPointAndRotate( PtolemMap->S, PtolemMap->H, Degree );

    mpfr_mul_si( Diameter, PtolemMap->OH, 2, Round );
    getRadiansOnCircle( Tmp[ 0 ], Degree, Diameter, Round );
#ifdef DEBUG
    printf("\nCalculated radians of XOP ist: ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    fflush( stdout );
#endif

    mpfr_mul_si( Diameter, PtolemMap->SH, 2, Round );
    getRadiansOnCircle( Tmp[ 0 ], Degree, Diameter, Round );
#ifdef DEBUG
    printf("\nCalculated radians of RST ist: ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    fflush( stdout );
#endif
    /* Set F */
    /* See S. 121 7.ff */
    /* Abstand im Intervall beim Äquator */
    getRadiansOnCircle( Radians, Degree, Diameter, Round );

    /* 18 mal : see 119 */
    mpfr_mul_si(Radians, Radians, 18, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
    mpfr_set_ui( Tmp[ 0 ], 180 , Round );
    mpfr_sub( Degree, Degree, Tmp[ 0 ] , Round );

    PtolemMap->F = setPointAndRotate( PtolemMap->Z, PtolemMap->H, Degree );

    if( TRUE == Mirrowing )
    {
        /* L is mirrow by K of G on X */
        PtolemMap->L = setPointAndMirrow( PtolemMap->G, PtolemMap->K, MIRROW_X );
        /* N is mirrow by Z of M on X */
        PtolemMap->N = setPointAndMirrow( PtolemMap->M, PtolemMap->Z, MIRROW_X );
        /* P is mirrow by O of X on X */
        PtolemMap->P = setPointAndMirrow( PtolemMap->X, PtolemMap->O, MIRROW_X );
        /* T is mirrow by H of R on X */
        PtolemMap->T = setPointAndMirrow( PtolemMap->R, PtolemMap->H, MIRROW_X );
        /* U is mirrow by Z of F on X */
        PtolemMap->U = setPointAndMirrow( PtolemMap->F, PtolemMap->Z, MIRROW_X );
    }
    else
    {
        /* Reset Degree */
        mpfr_mul_si( Diameter, PtolemMap->KH, 2, Round );

        /* 18 mal : see 119 */
        mpfr_mul_si(Radians, PtolemMap->Einheit, 4, Round);
        mpfr_mul_si(Radians, Radians, 18, Round);

        getDegreeOnCircle(Degree, Radians, Diameter, Round);
        /* Flip */
        mpfr_mul_si( Degree, Degree, -1, Round );
        /* L is depending on K see S 199 3.ff */
        PtolemMap->L = setPointAndRotate( PtolemMap->K, PtolemMap->H, Degree );
        /* N is depending on Z see S 199 3.ff */
        PtolemMap->N = setPointAndRotate( PtolemMap->Z, PtolemMap->H, Degree );
        /* P is depending on O see S 199 3.ff */
        PtolemMap->P = setPointAndRotate( PtolemMap->O, PtolemMap->H, Degree );
        /* T is depending on R see S 199 3.ff  */
        PtolemMap->T = setPointAndRotate( PtolemMap->S, PtolemMap->H, Degree );

        /* Set U */
        /* See S. 121 7.ff */
        getRadiansOnCircle( Radians, Degree, Diameter, Round );

        /* 18 mal : see 119 */
        mpfr_mul_si(Radians, Radians, 18, Round);

        getDegreeOnCircle(Degree, Radians, Diameter, Round);
        mpfr_set_ui( Tmp[ 0 ], 180 , Round );
        mpfr_sub( Degree, Degree, Tmp[ 0 ] , Round );

        PtolemMap->U = setPointAndRotate( PtolemMap->Z, PtolemMap->H, Degree );
    }

#ifdef DEBUG
    printf("\nCalculated G ist: (" );
    mpfr_out_str ( stdout, 10, 0, PtolemMap->G[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->G[ 1 ], Round );
    printf(")");
    fflush( stdout );

    /* Gerade von HG Verlängern */
    mpfr_sub( Tmp[ 0 ], PtolemMap->G[ 0 ], PtolemMap->H[ 0 ], Round );
    mpfr_sub( Tmp[ 1 ], PtolemMap->G[ 1 ], PtolemMap->H[ 1 ], Round );
    getEinheitsVector( Tmp, Tmp, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 10, Round );
    mpfr_mul_ui( Tmp[ 1 ], Tmp[ 1 ], 10, Round );
    mpfr_add( Tmp[ 0 ], Tmp[ 0 ], PtolemMap->H[ 0 ], Round );
    mpfr_add( Tmp[ 1 ], Tmp[ 1 ], PtolemMap->H[ 1 ], Round );

    printf("\nCalculated G' ist: (");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated M ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->M[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->M[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated X ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->X[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->X[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated R ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->R[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->R[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated L ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->L[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->L[ 1 ], Round );
    printf(")");
    fflush( stdout );

    /* Gerade von HL Verlängern */
    mpfr_sub( Tmp[ 0 ], PtolemMap->L[ 0 ], PtolemMap->H[ 0 ], Round );
    mpfr_sub( Tmp[ 1 ], PtolemMap->L[ 1 ], PtolemMap->H[ 1 ], Round );
    getEinheitsVector( Tmp, Tmp, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 100, Round );
    mpfr_mul_ui( Tmp[ 1 ], Tmp[ 1 ], 100, Round );
    mpfr_add( Tmp[ 0 ], Tmp[ 0 ], PtolemMap->H[ 0 ], Round );
    mpfr_add( Tmp[ 1 ], Tmp[ 1 ], PtolemMap->H[ 1 ], Round );

    printf("\nCalculated L' ist: (");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated N ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->N[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->N[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated P ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->P[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->P[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated T ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->T[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->T[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated degree of MY/ZN ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );

    printf("\nCalculated F ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->F[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->F[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated U ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->U[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->U[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif

    /* Housekeeping */
    mpfr_clear( Diameter );
    mpfr_clear( Radians );
    mpfr_clear( Degree );
    mpfr_clear( Tmp[ 0 ] );
    mpfr_clear( Tmp[ 1 ] );
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
    setPrecision( ArgVar[ 2 ] );
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
    printf( "\nCalculate Distances" );
#endif
    calacDistancesPlatim();
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
    printf("\n\n");
    fflush( stdout );
#endif
freePoint( PtolemMap->H );
freePoint( PtolemMap->K );
freePoint( PtolemMap->G );
freePoint( PtolemMap->S );
freePoint( PtolemMap->O );
freePoint( PtolemMap->X );
freePoint( PtolemMap->R );
freePoint( PtolemMap->M );
freePoint( PtolemMap->F );
freePoint( PtolemMap->L );
freePoint( PtolemMap->P );
freePoint( PtolemMap->T );
freePoint( PtolemMap->N );
freePoint( PtolemMap->U );
/* --------- Als Globus --------- */
#ifdef DEBUG
    printf("\nStart glob.");
    fflush( stdout );
    printf( "\nCalculate Distances" );
#endif
    calacDistancesGlob();
#ifdef DEBUG
    printf("\nSet LZ Punkte.");
    fflush( stdout );
#endif
    makeLZPointsGlob();
#ifdef DEBUG
    printf("\nSet Limits.");
    fflush( stdout );
#endif
    makeLimitsGlob();
#ifdef DEBUG
    printf("\n\n");
    fflush( stdout );
#endif
    return 0;
}

/*--------------------------Glob Functs -----------------------------------------*/
void calacDistancesGlob()
{
    mpfr_t Tmp, Tmp2;
    mpfr_init( Tmp );
    mpfr_init( Tmp2 );

    /* AC=EZ = 90E */
    /* 1E = EZ/90 */
    mpfr_init( PtolemMap->Einheit );
    mpfr_div_ui( PtolemMap->Einheit, PtolemMap->Kantenlaenge[ 1 ], 90, Round );
#ifdef DEBUG
    printf("\nEine Einheit ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Einheit, Round );
    fflush( stdout );
#endif
    mpfr_init( PtolemMap->EZ );
    mpfr_set( PtolemMap->EZ, PtolemMap->Kantenlaenge[ 1 ], Round );

    /* HL =181 5/6*/
    mpfr_set_ui( Tmp, 5, Round );
    mpfr_div_ui( Tmp, Tmp, 6, Round );
    mpfr_add_ui( Tmp, Tmp, 181, Round );

    initAndMultiply( PtolemMap->HL, Tmp, PtolemMap->Einheit );

    /*
     * ZH = 16 5/12 S.129
     * ZL = HL+ZH
     * ZL = 181 5/6 + 16 5/12
     * EH = EZ-ZH
     * EH = 90-16 5/12
     * EL = LZ-EZ
     */
    mpfr_set_ui( Tmp2, 5, Round );
    mpfr_div_ui( Tmp2, Tmp2, 12, Round );
    mpfr_add_ui( Tmp2, Tmp2, 16, Round );

    /* ZH = 16 5/12 S.129 */
    initAndMultiply( PtolemMap->ZH, Tmp2, PtolemMap->Einheit );

    /* EH = EZ-ZH */
    mpfr_init( PtolemMap->EH );
    mpfr_sub( PtolemMap->EH, PtolemMap->EZ, PtolemMap->ZH, Round );

    /* ZL = HL+ZH */
    mpfr_add( Tmp2, Tmp, Tmp2, Round );
    initAndMultiply( PtolemMap->ZL, Tmp2, PtolemMap->Einheit );

    /* EL = ZL-EZ */
    mpfr_init( PtolemMap->EL );
    mpfr_sub( PtolemMap->EL, PtolemMap->ZL, PtolemMap->EZ, Round );

    /*
     * GH = 23 5/6 S.129
     * GL = LH-HG
     * GL = 181 5/6-23 5/6
     */
    mpfr_set_ui( Tmp2, 5, Round );
    mpfr_div_ui( Tmp2, Tmp2, 6, Round );
    mpfr_add_ui( Tmp2, Tmp2, 23, Round );

    /* GH = 23 5/6 S.129 */
    initAndMultiply( PtolemMap->GH, Tmp2, PtolemMap->Einheit );
    /* GL = LH-HG */
    mpfr_sub( Tmp2, Tmp, Tmp2, Round );
    initAndMultiply( PtolemMap->GL, Tmp2, PtolemMap->Einheit );

    /*
     * KH = 63 S.129
     * KL = LH-KH
     * KL = 181 5/6- 63
     */
    /* KH = 63 S.129 */
    initAndMultiplyUi( PtolemMap->KH, PtolemMap->Einheit, 63 );
    /* KL =LH-KH */
    mpfr_sub_ui( Tmp2, Tmp, 63, Round );
    initAndMultiply( PtolemMap->KL, Tmp2, PtolemMap->Einheit );

#ifdef DEBUG
    printf("\nEZ ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->EZ, Round );
    fflush( stdout );

    printf("\nEH ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->EH, Round );
    fflush( stdout );

    printf("\nEL ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->EL, Round );
    fflush( stdout );

    printf("\nHZ ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->ZH, Round );
    fflush( stdout );

    printf("\nHG ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->GH, Round );
    fflush( stdout );

    printf("\nHK ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->KH, Round );
    fflush( stdout );

    printf("\nHL ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->HL, Round );
    fflush( stdout );

    printf("\nZL ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->ZL, Round );
    fflush( stdout );

    printf("\nGL ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->GL, Round );
    fflush( stdout );

    printf("\nKL ist: ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->KL, Round );
    fflush( stdout );

#endif
    /* Housekeeping */
    mpfr_clear( Tmp );
    mpfr_clear( Tmp2 );
    mpfr_free_cache();
}

void makeLZPointsGlob()
{
    /* H = Z_x, Z_y+ZH */
    PtolemMap->H = setPointByAddOnY( PtolemMap->Z, PtolemMap->ZH );

    /* G = H_x, H_y+GH */
    PtolemMap->G = setPointByAddOnY( PtolemMap->H, PtolemMap->GH );

    /* K = H_x, H_y+KH */
    PtolemMap->K = setPointByAddOnY( PtolemMap->H, PtolemMap->KH );

    /* G = H_x, H_y+LH */
    PtolemMap->L = setPointByAddOnY( PtolemMap->H, PtolemMap->HL );

#ifdef DEBUG
    printf("\nPoint H ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->H[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->H[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint G ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->G[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->G[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint K ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->K[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->K[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint L ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->L[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->L[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
}

void makeLimitsGlob()
{
    mpfr_t* Tmp;
    mpfr_t Diameter, Radians, Degree;

    Tmp = ( mpfr_t* ) malloc( sizeof( mpfr_t ) * 4 );
    if(NULL == Tmp )
    {
        errorAndOut( "Somethings wrong with the memory, jim." );
    }
/*
 * intersectionsThreaded(
            globGridInner,
            TRUE,
            (double)UseFactor/5,
            LAENGEN_FAKTOR*UseFactor,
            NULL
    )
 */
    mpfr_init( Tmp[ 0 ] );
    mpfr_init( Tmp[ 1 ] );
    mpfr_init( Tmp[ 2 ] );
    mpfr_init( Tmp[ 3 ] );
    mpfr_init( Diameter );
    mpfr_init( Degree );
    mpfr_init( Radians );

    /* Fist we calc the touchpoints of the given circles on the parallelogram */
    /* From K to Z */
    /* R/P */
    intersectCircleLine( Tmp, PtolemMap->A, PtolemMap->B, PtolemMap->L, PtolemMap->KL, Round );
    PtolemMap->P = startPoint();
    mpfr_set(PtolemMap->P[ 0 ], Tmp[ 0 ], Round );
    mpfr_set(PtolemMap->P[ 1 ], Tmp[ 1 ], Round );

    PtolemMap->R = startPoint();
    mpfr_set(PtolemMap->R[ 0 ], Tmp[ 2 ], Round );
    mpfr_set(PtolemMap->R[ 1 ], Tmp[ 3 ], Round );
    //-0,019952499 −0,000492205 0,25631605
    //0,009709634 −0,000937595 −0,27686901
    //0,037174313 0,007489 -1,5815551
    /* X */
    intersectCircleLine( Tmp, PtolemMap->A, PtolemMap->C, PtolemMap->L, PtolemMap->GL, Round );
    PtolemMap->X = startPoint();
    mpfr_set(PtolemMap->X[ 0 ], Tmp[ 2 ], Round );
    mpfr_set(PtolemMap->X[ 1 ], Tmp[ 3 ], Round );

    /* M */
    intersectCircleLine( Tmp, PtolemMap->A, PtolemMap->C, PtolemMap->L, PtolemMap->ZL, Round );
    PtolemMap->M = startPoint();
    mpfr_set(PtolemMap->M[ 0 ], Tmp[ 2 ], Round );
    mpfr_set(PtolemMap->M[ 1 ], Tmp[ 3 ], Round );

    /* Now we try to calc the touchpoints on the merdians */
    /* S. 131 */
    mpfr_mul_si( Diameter, PtolemMap->KL, 2, Round );

    /* Y */
    /* 18 mal : see 119 */
    /* Abstand 2 1/4 */
    mpfr_set_ui( Tmp[ 0 ], 1, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 4, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 2, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PtolemMap->Einheit, Tmp[0], Round);
    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of YKV ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nRadians of YKV ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );
    fflush( stdout );
#endif

    PtolemMap->Y = setPointAndRotate( PtolemMap->K, PtolemMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PtolemMap->V = setPointAndRotate( PtolemMap->K, PtolemMap->L, Degree);
    }

    /* T S 133 */

    mpfr_mul_si( Diameter, PtolemMap->GL, 2, Round );
    /* 18 mal : see 119 */
    /* Abstand 4 7/12 */
    mpfr_set_ui( Tmp[ 0 ], 7, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 12, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 4, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PtolemMap->Einheit, Tmp[0], Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of TGU ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nRadians of TGU ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );
    fflush( stdout );
#endif

    PtolemMap->T = setPointAndRotate( PtolemMap->G, PtolemMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PtolemMap->U = setPointAndRotate( PtolemMap->G, PtolemMap->L, Degree);
    }


    /* I S 133 == Äquator*/

    mpfr_mul_si( Diameter, PtolemMap->HL, 2, Round );
    /* 18 mal : see 119 */
    /* Abstand 5 */
    mpfr_set_ui( Tmp[ 0 ], 5, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PtolemMap->Einheit, Tmp[0], Round);
    mpfr_init( PtolemMap->IHJ );
    mpfr_set( PtolemMap->IHJ, Radians, Round );

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of IHJ ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nRadians of IHJ ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );
    fflush( stdout );
#endif

    PtolemMap->I = setPointAndRotate( PtolemMap->H, PtolemMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PtolemMap->J = setPointAndRotate( PtolemMap->H, PtolemMap->L, Degree);
    }

    /* S S 133*/

    mpfr_mul_si( Diameter, PtolemMap->ZL, 2, Round );
    /* 18 mal : see 119 */
    /* S 4 5/6 */
    mpfr_set_ui( Tmp[ 0 ], 5, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 6, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 4, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PtolemMap->Einheit, Tmp[0], Round);
    mpfr_mul_si(Radians, Radians, 1, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of SZF ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nRadians of SZF ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );
    fflush( stdout );
#endif

    PtolemMap->S = setPointAndRotate( PtolemMap->Z, PtolemMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PtolemMap->F = setPointAndRotate( PtolemMap->Z, PtolemMap->L, Degree);
    }

    if( TRUE == Mirrowing )
    {
        /* O */
        PtolemMap->O = setPointAndMirrow( PtolemMap->O, PtolemMap->E, MIRROW_X );
        /* N */
        PtolemMap->N = setPointAndMirrow( PtolemMap->M, PtolemMap->E, MIRROW_X );
        /* V */
        PtolemMap->V = setPointAndMirrow( PtolemMap->Y, PtolemMap->E, MIRROW_X );
        /* U */
        PtolemMap->U = setPointAndMirrow( PtolemMap->T, PtolemMap->E, MIRROW_X );
        /* J */
        PtolemMap->J = setPointAndMirrow( PtolemMap->I, PtolemMap->E, MIRROW_X );
        /* F */
        PtolemMap->F = setPointAndMirrow( PtolemMap->S, PtolemMap->E, MIRROW_X );

    }
    else
    {
        /* O */
        intersectCircleLine(Tmp, PtolemMap->B, PtolemMap->D, PtolemMap->L, PtolemMap->GL, Round);
        PtolemMap->O = startPoint();
        mpfr_set(PtolemMap->O[0], Tmp[2], Round);
        mpfr_set(PtolemMap->O[1], Tmp[3], Round);
        /* N */
        intersectCircleLine(Tmp, PtolemMap->B, PtolemMap->D, PtolemMap->L, PtolemMap->ZL, Round);
        PtolemMap->N = startPoint();
        mpfr_set(PtolemMap->N[0], Tmp[2], Round);
        mpfr_set(PtolemMap->N[1], Tmp[3], Round);
    }
#ifdef DEBUG
    printf("\nPoint P ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->P[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->P[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint R ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->R[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->R[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint X ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->X[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->X[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint O ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->O[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->O[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint M ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->M[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->M[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint N ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->N[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->N[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint Y ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Y[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->Y[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint T ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->T[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->T[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint I ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->I[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->I[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint S ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->S[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->S[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint V ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->V[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->V[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint U ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->U[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->U[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint J ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->J[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->J[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint F ist: (");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->F[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PtolemMap->F[ 1 ], Round );
    printf(")");
    fflush( stdout );
#endif
    /* cleanup */
    mpfr_clear( Tmp[ 0 ] );
    mpfr_clear( Tmp[ 1 ] );
    mpfr_clear( Tmp[ 2 ] );
    mpfr_clear( Tmp[ 3 ] );
    mpfr_clear( Diameter );
    mpfr_clear( Degree );
    mpfr_clear( Radians );
    mpfr_free_cache();
    free( Tmp );
}