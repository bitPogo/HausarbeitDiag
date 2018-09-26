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
    mpfr_t HL;
    mpfr_t GL;
    mpfr_t KL;
    mpfr_t ZL;
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
    mpfr_init( PytagoMap->Einheit );
    mpfr_div( PytagoMap->Einheit, PytagoMap->Kantenlaenge[ 1 ], Tmp, Round );

#ifdef DEBUG
    printf("\nEine Einheit beträgt: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Einheit, Round );
    fflush( stdout );
#endif

    /* Calculate Einheiten */
    /*Set HZ = 131 5/12E */
    initAndMultiply( PytagoMap->ZH, PytagoMap->Einheit, Tmp2 );

    /* EH = 34*Einheit */
    initAndMultiplyUi( PytagoMap->EH, PytagoMap->Einheit, 34 );

    /* KH = 79*Einheit */
    initAndMultiplyUi( PytagoMap->KH, PytagoMap->Einheit, 79 );

    /* OH = 52*Einheit siehe seite 121 */
    initAndMultiplyUi( PytagoMap->OH, PytagoMap->Einheit, 52 );

    /* SH = 115*Einheit siehe seite 121 */
    initAndMultiplyUi( PytagoMap->SH, PytagoMap->Einheit, 115 );

#ifdef DEBUG
    printf("\nEH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->EH, Round );
    fflush( stdout );
    printf("\nKH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->KH, Round );
    fflush( stdout );
    printf("\nOH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->OH, Round );
    fflush( stdout );
    printf("\nSH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->SH, Round );
    fflush( stdout );
    printf("\nZH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->ZH, Round );
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
    PytagoMap->H = setPointByAddOnY( PytagoMap->E, PytagoMap->EH );

    /* K = H_x, H_y-KH */
    PytagoMap->K = setPointBySubOnY( PytagoMap->H, PytagoMap->KH );

    /* O = H_x, H_y-OH */
    PytagoMap->O = setPointBySubOnY( PytagoMap->H, PytagoMap->OH );

    /* S = H_x, H_y-SH */
    PytagoMap->S = setPointBySubOnY( PytagoMap->H, PytagoMap->SH );

#ifdef DEBUG
    printf("\nPoint H ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->H[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->H[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint K ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->K[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->K[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint O ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->O[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->O[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint S ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->S[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->S[ 1 ], Round );
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
    /* G is depending on K see S 199 3.ff */
    PytagoMap->G = setPointAndRotate( PytagoMap->K, PytagoMap->H, Degree );
    /* M is depending on Z see S 199 3.ff */
    PytagoMap->M = setPointAndRotate( PytagoMap->Z, PytagoMap->H, Degree );
    /* O is depending on O see S 199 3.ff */
    PytagoMap->X = setPointAndRotate( PytagoMap->O, PytagoMap->H, Degree );
    /* S is depending on R see S 199 3.ff  */
    PytagoMap->R = setPointAndRotate( PytagoMap->S, PytagoMap->H, Degree );

    /* Set F */
    /* See S. 121 7.ff */
    /* Abstand im Intervall beim Äquator */
    getRadiansOnCircle( Radians, Degree, Diameter, Round );

    /* 18 mal : see 119 */
    mpfr_mul_si(Radians, Radians, 18, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
    mpfr_set_ui( Tmp[ 0 ], 180 , Round );
    mpfr_sub( Degree, Degree, Tmp[ 0 ] , Round );

    PytagoMap->F = setPointAndRotate( PytagoMap->Z, PytagoMap->H, Degree );

    if( TRUE == Mirrowing )
    {
        /* L is mirrow by K of G on X */
        PytagoMap->L = setPointAndMirrow( PytagoMap->G, PytagoMap->K, MIRROW_X );
        /* N is mirrow by Z of M on X */
        PytagoMap->N = setPointAndMirrow( PytagoMap->M, PytagoMap->Z, MIRROW_X );
        /* P is mirrow by O of X on X */
        PytagoMap->P = setPointAndMirrow( PytagoMap->X, PytagoMap->O, MIRROW_X );
        /* T is mirrow by H of R on X */
        PytagoMap->T = setPointAndMirrow( PytagoMap->R, PytagoMap->H, MIRROW_X );
        /* U is mirrow by Z of F on X */
        PytagoMap->U = setPointAndMirrow( PytagoMap->F, PytagoMap->Z, MIRROW_X );
    }
    else
    {
        /* Reset Degree */
        mpfr_mul_si( Diameter, PytagoMap->KH, 2, Round );

        /* 18 mal : see 119 */
        mpfr_mul_si(Radians, PytagoMap->Einheit, 4, Round);
        mpfr_mul_si(Radians, Radians, 18, Round);

        getDegreeOnCircle(Degree, Radians, Diameter, Round);
        /* Flip */
        mpfr_mul_si( Degree, Degree, -1, Round );
        /* L is depending on K see S 199 3.ff */
        PytagoMap->L = setPointAndRotate( PytagoMap->K, PytagoMap->H, Degree );
        /* N is depending on Z see S 199 3.ff */
        PytagoMap->N = setPointAndRotate( PytagoMap->Z, PytagoMap->H, Degree );
        /* P is depending on O see S 199 3.ff */
        PytagoMap->P = setPointAndRotate( PytagoMap->O, PytagoMap->H, Degree );
        /* T is depending on R see S 199 3.ff  */
        PytagoMap->T = setPointAndRotate( PytagoMap->S, PytagoMap->H, Degree );

        /* Set U */
        /* See S. 121 7.ff */
        getRadiansOnCircle( Radians, Degree, Diameter, Round );

        /* 18 mal : see 119 */
        mpfr_mul_si(Radians, Radians, 18, Round);

        getDegreeOnCircle(Degree, Radians, Diameter, Round);
        mpfr_set_ui( Tmp[ 0 ], 180 , Round );
        mpfr_sub( Degree, Degree, Tmp[ 0 ] , Round );

        PytagoMap->U = setPointAndRotate( PytagoMap->Z, PytagoMap->H, Degree );
    }

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

    printf("\nCalculated M ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated X ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->X[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->X[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated R ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->R[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->R[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated L ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 1 ], Round );
    printf(")");
    fflush( stdout );

    /* Gerade von HL Verlängern */
    mpfr_sub( Tmp[ 0 ], PytagoMap->L[ 0 ], PytagoMap->H[ 0 ], Round );
    mpfr_sub( Tmp[ 1 ], PytagoMap->L[ 1 ], PytagoMap->H[ 1 ], Round );
    getEinheitsVector( Tmp, Tmp, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 100, Round );
    mpfr_mul_ui( Tmp[ 1 ], Tmp[ 1 ], 100, Round );
    mpfr_add( Tmp[ 0 ], Tmp[ 0 ], PytagoMap->H[ 0 ], Round );
    mpfr_add( Tmp[ 1 ], Tmp[ 1 ], PytagoMap->H[ 1 ], Round );

    printf("\nCalculated L' ist: (");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, Tmp[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated N ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->N[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->N[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated P ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->P[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->P[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated T ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->T[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->T[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated degree of MY/ZN ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );

    printf("\nCalculated F ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->F[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->F[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nCalculated U ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->U[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->U[ 1 ], Round );
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
freePoint( PytagoMap->H );
freePoint( PytagoMap->K );
freePoint( PytagoMap->G );
freePoint( PytagoMap->S );
freePoint( PytagoMap->O );
freePoint( PytagoMap->X );
freePoint( PytagoMap->R );
freePoint( PytagoMap->M );
freePoint( PytagoMap->F );
freePoint( PytagoMap->L );
freePoint( PytagoMap->P );
freePoint( PytagoMap->T );
freePoint( PytagoMap->N );
freePoint( PytagoMap->U );
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
    mpfr_init( PytagoMap->Einheit );
    mpfr_div_ui( PytagoMap->Einheit, PytagoMap->Kantenlaenge[ 1 ], 90, Round );
#ifdef DEBUG
    printf("\nEine Einheit ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Einheit, Round );
    fflush( stdout );
#endif
    mpfr_init( PytagoMap->EZ );
    mpfr_set( PytagoMap->EZ, PytagoMap->Kantenlaenge[ 1 ], Round );

    /* HL =181 5/6*/
    mpfr_set_ui( Tmp, 5, Round );
    mpfr_div_ui( Tmp, Tmp, 6, Round );
    mpfr_add_ui( Tmp, Tmp, 181, Round );

    initAndMultiply( PytagoMap->HL, Tmp, PytagoMap->Einheit );

    /*
     * ZH = 16 5/12 S.129
     * ZL = HL+ZH
     * ZL = 181 5/6 + 16 5/12
     * EH = EZ-ZH
     * EH = 90-16 5/12
     */
    mpfr_set_ui( Tmp2, 5, Round );
    mpfr_div_ui( Tmp2, Tmp2, 12, Round );
    mpfr_add_ui( Tmp2, Tmp2, 16, Round );

    /* ZH = 16 5/12 S.129 */
    initAndMultiply( PytagoMap->ZH, Tmp2, PytagoMap->Einheit );

    /* EH = EZ-ZH */
    mpfr_init( PytagoMap->EH );
    mpfr_sub( PytagoMap->EH, PytagoMap->EZ, PytagoMap->ZH, Round );

    /* ZL = HL+ZH */
    mpfr_add( Tmp2, Tmp, Tmp2, Round );
    initAndMultiply( PytagoMap->ZL, Tmp2, PytagoMap->Einheit );

    /*
     * GH = 23 5/6 S.129
     * GL = LH-HG
     * GL = 181 5/6-23 5/6
     */
    mpfr_set_ui( Tmp2, 5, Round );
    mpfr_div_ui( Tmp2, Tmp2, 6, Round );
    mpfr_add_ui( Tmp2, Tmp2, 23, Round );

    /* GH = 23 5/6 S.129 */
    initAndMultiply( PytagoMap->GH, Tmp2, PytagoMap->Einheit );
    /* GL = LH-HG */
    mpfr_sub( Tmp2, Tmp, Tmp2, Round );
    initAndMultiply( PytagoMap->GL, Tmp2, PytagoMap->Einheit );

    /*
     * KH = 63 S.129
     * KL = LH-KH
     * KL = 181 5/6- 63
     */
    /* KH = 63 S.129 */
    initAndMultiplyUi( PytagoMap->KH, PytagoMap->Einheit, 63 );
    /* KL =LH-KH */
    mpfr_sub_ui( Tmp2, Tmp, 63, Round );
    initAndMultiply( PytagoMap->KL, Tmp2, PytagoMap->Einheit );

#ifdef DEBUG
    printf("\nEZ ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->EZ, Round );
    fflush( stdout );

    printf("\nEH ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->EH, Round );
    fflush( stdout );

    printf("\nHZ ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->ZH, Round );
    fflush( stdout );

    printf("\nHG ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->GH, Round );
    fflush( stdout );

    printf("\nHK ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->KH, Round );
    fflush( stdout );

    printf("\nHL ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->HL, Round );
    fflush( stdout );

    printf("\nZL ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->ZL, Round );
    fflush( stdout );

    printf("\nGL ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->GL, Round );
    fflush( stdout );

    printf("\nKL ist: ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->KL, Round );
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
    PytagoMap->H = setPointByAddOnY( PytagoMap->Z, PytagoMap->ZH );

    /* G = H_x, H_y+GH */
    PytagoMap->G = setPointByAddOnY( PytagoMap->H, PytagoMap->GH );

    /* K = H_x, H_y+KH */
    PytagoMap->K = setPointByAddOnY( PytagoMap->H, PytagoMap->KH );

    /* G = H_x, H_y+LH */
    PytagoMap->L = setPointByAddOnY( PytagoMap->H, PytagoMap->HL );

#ifdef DEBUG
    printf("\nPoint H ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->H[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->H[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint G ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->G[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->G[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint K ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->K[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->K[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint L ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->L[ 1 ], Round );
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
    intersectCircleLine( Tmp, PytagoMap->A, PytagoMap->B, PytagoMap->L, PytagoMap->KL, Round );
    PytagoMap->P = startPoint();
    mpfr_set(PytagoMap->P[ 0 ], Tmp[ 0 ], Round );
    mpfr_set(PytagoMap->P[ 1 ], Tmp[ 1 ], Round );

    PytagoMap->R = startPoint();
    mpfr_set(PytagoMap->R[ 0 ], Tmp[ 2 ], Round );
    mpfr_set(PytagoMap->R[ 1 ], Tmp[ 3 ], Round );

    /* X */
    intersectCircleLine( Tmp, PytagoMap->A, PytagoMap->C, PytagoMap->L, PytagoMap->GL, Round );
    PytagoMap->X = startPoint();
    mpfr_set(PytagoMap->X[ 0 ], Tmp[ 2 ], Round );
    mpfr_set(PytagoMap->X[ 1 ], Tmp[ 3 ], Round );


    /* M */
    intersectCircleLine( Tmp, PytagoMap->A, PytagoMap->C, PytagoMap->L, PytagoMap->ZL, Round );
    PytagoMap->M = startPoint();
    mpfr_set(PytagoMap->M[ 0 ], Tmp[ 2 ], Round );
    mpfr_set(PytagoMap->M[ 1 ], Tmp[ 3 ], Round );

    /* Now we try to calc the touchpoints on the merdians */
    /* S. 131 */
    mpfr_mul_si( Diameter, PytagoMap->KL, 2, Round );

    /* Y */
    /* 18 mal : see 119 */
    /* Abstand 2 1/4 */
    mpfr_set_ui( Tmp[ 0 ], 1, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 4, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 2, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PytagoMap->Einheit, Tmp[0], Round);
    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of PKR ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif

    PytagoMap->Y = setPointAndRotate( PytagoMap->K, PytagoMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PytagoMap->V = setPointAndRotate( PytagoMap->K, PytagoMap->L, Degree);
    }

    /* T S 133 */

    mpfr_mul_si( Diameter, PytagoMap->GL, 2, Round );
    /* 18 mal : see 119 */
    /* Abstand 4 7/12 */
    mpfr_set_ui( Tmp[ 0 ], 7, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 12, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 4, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PytagoMap->Einheit, Tmp[0], Round);
    mpfr_mul_si(Radians, Radians, 1, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of TGU ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif

    PytagoMap->T = setPointAndRotate( PytagoMap->G, PytagoMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PytagoMap->U = setPointAndRotate( PytagoMap->G, PytagoMap->L, Degree);
    }


    /* I S 133 == Äquator*/

    mpfr_mul_si( Diameter, PytagoMap->HL, 2, Round );
    /* 18 mal : see 119 */
    /* Abstand 5 */
    mpfr_set_ui( Tmp[ 0 ], 5, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PytagoMap->Einheit, Tmp[0], Round);
    mpfr_mul_si(Radians, Radians, 1, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of IHJ ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif

    PytagoMap->I = setPointAndRotate( PytagoMap->H, PytagoMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PytagoMap->J = setPointAndRotate( PytagoMap->H, PytagoMap->L, Degree);
    }

    /* S S 133*/

    mpfr_mul_si( Diameter, PytagoMap->ZL, 2, Round );
    /* 18 mal : see 119 */
    /* S 4 5/6 */
    mpfr_set_ui( Tmp[ 0 ], 5, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 6, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 4, Round );
    mpfr_mul_ui( Tmp[ 0 ], Tmp[ 0 ], 18, Round );


    mpfr_mul(Radians, PytagoMap->Einheit, Tmp[0], Round);
    mpfr_mul_si(Radians, Radians, 1, Round);

    getDegreeOnCircle(Degree, Radians, Diameter, Round);
#ifdef DEBUG
    printf("\nCalculated degree of SZF ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    fflush( stdout );
#endif

    PytagoMap->S = setPointAndRotate( PytagoMap->Z, PytagoMap->L, Degree);
    if ( TRUE != Mirrowing )
    {
        mpfr_mul_si(Degree, Degree, -1, Round);
        PytagoMap->F = setPointAndRotate( PytagoMap->Z, PytagoMap->L, Degree);
    }

    if( TRUE == Mirrowing )
    {
        /* O */
        PytagoMap->O = setPointAndMirrow( PytagoMap->O, PytagoMap->E, MIRROW_X );
        /* N */
        PytagoMap->N = setPointAndMirrow( PytagoMap->M, PytagoMap->E, MIRROW_X );
        /* V */
        PytagoMap->V = setPointAndMirrow( PytagoMap->Y, PytagoMap->E, MIRROW_X );
        /* U */
        PytagoMap->U = setPointAndMirrow( PytagoMap->T, PytagoMap->E, MIRROW_X );
        /* J */
        PytagoMap->J = setPointAndMirrow( PytagoMap->I, PytagoMap->E, MIRROW_X );
        /* F */
        PytagoMap->F = setPointAndMirrow( PytagoMap->S, PytagoMap->E, MIRROW_X );

    }
    else
    {
        /* O */
        intersectCircleLine(Tmp, PytagoMap->B, PytagoMap->D, PytagoMap->L, PytagoMap->GL, Round);
        PytagoMap->O = startPoint();
        mpfr_set(PytagoMap->O[0], Tmp[2], Round);
        mpfr_set(PytagoMap->O[1], Tmp[3], Round);
        /* N */
        intersectCircleLine(Tmp, PytagoMap->B, PytagoMap->D, PytagoMap->L, PytagoMap->ZL, Round);
        PytagoMap->N = startPoint();
        mpfr_set(PytagoMap->N[0], Tmp[2], Round);
        mpfr_set(PytagoMap->N[1], Tmp[3], Round);
    }
#ifdef DEBUG
    printf("\nPoint P ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->P[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->P[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint R ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->R[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->R[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint X ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->X[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->X[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint O ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->O[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->O[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint M ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->M[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint N ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->N[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->N[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint Y ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Y[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->Y[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint T ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->T[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->T[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint I ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->I[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->I[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint S ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->S[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->S[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint V ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->V[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->V[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint U ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->U[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->U[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint J ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->J[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->J[ 1 ], Round );
    printf(")");
    fflush( stdout );

    printf("\nPoint F ist: (");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->F[ 0 ], Round );
    printf(", ");
    mpfr_out_str ( stdout, 10, 0, PytagoMap->F[ 1 ], Round );
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