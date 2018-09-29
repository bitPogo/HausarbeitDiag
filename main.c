/**
 * @autor Matthias Geisler (bitpogo) <geislemx@informatik.hu-berlin.de>
 * @licence GNU General Public License
 */
// man 7 feature_test_macros
// #define _XOPEN_SOURCE 500
#define _GNU_SOURCE
#include "./lib.h"
#include "./types.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>
#include <pthread.h>
#include <string.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#define PRECISION "64"
#define MIRROW_X 0x42
#define MIRROW_Y 0x23
#define MIRROW_BOTH 0x65
#define PARALELLKREISE 24
#define NORMAL 0x100
#define DEGREE 0x101
#define DEGREEX 0x104
#define MINUTES 0x102
#define SECONDS 0x103
#define MAX_THREADS 3
#define THREAD_ERROR 1000
#define THREAD_SLEEP 500
#define FILTER_X_NO 0x0
#define FILTER_X_START 0x3
#define FILTER_X_END 0x4
#define FILTER_X_RANGE 0x7
#define X_MIN_G -16
#define X_MIN_M -25
#define X_MIN_S -0
#define X_MAX_G 63
#define X_MAX_M 0
#define X_MAX_S 0
#define FILTER_Y_NO 0x0
#define FILTER_Y_START 0x3
#define FILTER_Y_END 0x4
#define FILTER_Y_RANGE 0x7
#define Y_MIN_G -90
#define Y_MIN_M 0
#define Y_MIN_S 0
#define Y_MAX_G 90
#define Y_MAX_M 0
#define Y_MAX_S 0
/* Planimetrik */
#define LAENGEN_FAKTOR 18
#define PLAN_LAENGEN_VERHAEHLTNIS 4
/* Pseudohead */
PtMap* Map;
Bool Verbose;
Bool Mirrow;
Bool Org;
Bool UsePlan;
long int Round;
int HardWired[PARALELLKREISE][3];
mpfr_t** Paralellkreise;
unsigned int Sizes[2];
unsigned short Use;
unsigned int UseFactor;
unsigned int GegenFactor;
unsigned int MaxThreads;
unsigned int XFilter;
int FilterX[2][3];
unsigned int YFilter;
int FilterY[2][4];
int MinY;
unsigned int YStartUpper;
double YStartLower;
int MaxY;
unsigned int YEndUpper;
double YEndLower;
unsigned int MemoryBoost;
Writer* DaemonBuffer;
pthread_t WriterDaemon;
pthread_mutex_t SerialWriter = PTHREAD_MUTEX_INITIALIZER;
unsigned short KillDaemon;
/*----------------------------- Utility -----------------------------*/
/**
 * Initilize a point
 * @param InitCooridnates | unsigned short | if x and y should be initilized
 * @return Point
 */
Point* initPoint( unsigned short InitCooridnates );
/**
 * Frees the memory of a given point
 * @param XY | Point
 */
void freePoint( Point* XY );
/**
 * Sets a Point by move it on x-axis right
 * @returns NewPoint | Point* | the computed Point
 * @param OldPoint | Point* | input Point
 * @param Distance | mpfr_t | distance of movement on x-axis
 */
Point* setPointByAddOnX( Point* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by move it on y-axis right
 * @returns NewPoint | Point* | the computed Point
 * @param OldPoint | Point* | input Point
 * @param Distance | mpfr_t | distance of movement on y-axis
 */
Point* setPointByAddOnY( Point* OldPoint, mpfr_t Distance );
/**
 * Sets a Point by move it on y-axis down
 * @returns NewPoint | Point* | the computed Point
 * @param OldPoint | Point* | input Point
 * @param Distance | mpfr_t | distance of movement on x-axis
 */
Point* setPointBySubOnY( Point* OldPoint, mpfr_t Distance );
/**
 * inits a mpfr_t structure by multiplying something with something
 * @param Target | mpfr_t | target structure
 * @param Mul1 | mpfr_t | Factor 1
 * @param Mul2 | mpfr_t | Factor 2
 */
void initAndMultiply( mpfr_t Target, mpfr_t Mul1, mpfr_t Mul2 );
/**
 * inits a mpfr_t structure by multiplying something with something
 * @param Target | mpfr_t | target structure
 * @param Mul1 | mpfr_t | Factor 1
 * @param Mul2 | unsigned integer | Factor 2
 */
void initAndMultiplyUI( mpfr_t Target, mpfr_t Mul1, unsigned int Mul2 );
/**
 * Sets a Point by mirrow it on the axis
 * @return NewPoint | mpfr_t* | the computed Point
 * @param OldPoint | mpfr_t* | input Point
 * @param MirrowPoint | mpfr_t* | coordinate center
 * @param Flag | int | MIRROW_BOTH for mirrow it on both axis, MIRROW_X for mirrow it on x-axis and MIRROW_Y for mirrow it on y-axis
 */
Point* setPointByMirrow( Point* OldPoint, Point* MirrowPoint, int Flag );

Point* setPointAndRotate( Point* OldPoint, Point* CircleCenter, mpfr_t Degree );
/**
 * Converts new representation of a point to the old one
 * @deprecated
 * @param P | Point | ...
 * @return array of mpfr_t
 */
mpfr_t* libConvert( Point* P );
/**
 * Converts old representation of a point to the new one
 * @deprecated
 * @param P | array of mpfr_t | ...
 * @return Point
 */
Point* mainConvert( mpfr_t* P );
/*----------------------------- Behavior -----------------------------*/
/**
 * Set bits of precusion Numbers
 * @param PrecusionString  | const char* | numeric value for bits of precusion
 */
void setPrecision( const char* PrecisionString );
/*----------------------------- Functionality -----------------------------*/
/**
 * Computes the basic workbench( aka paralleologramm )
 * @param X | const char* | X position of the startpoint alias A of the paralleologramm
 * @param Y | const char* | Y position of the startpoint alias A of the paralleologramm
 * @param Grunddistanze | char* | edge length of the paralleologramm
 */
void makeDesk( const char* X, const char* Y, const char* Grunddistanze );
void setParalellkreise();
void* makeThreadedBreiten( void* Einheit );
void printDesk();
void outputPoints( OutputBuffer* Buffer );
void printPoints( OutputBuffer* Points);
void filterX();
/*----------------------------- Planimetrik --------------------------------*/
void startPlanimetrik();
void planimetrikGetMetrum();
void planimetrikDistances();
void planimetrikBreitenPunkte();
void planimetrikLaengen();
void planimetrikSpecialLaenge();
void planimetrikPrintBreitenPunkte();
void planimetrikPrintLimits();
void planimetrikPrintDistances();
void planimetrikPrint();
void planimetrikHouskeeping();
/*------------------------------Globus---------------------------------------*/
unsigned int globFilterYFindStart( Point** Buffer );
unsigned int globFilterYFindEnd( Point** Buffer );
/*----------------------------- Utility -----------------------------*/
Point* initPoint( unsigned short InitCooridnates )
{
    Point* NewPoint = (Point*) malloc( sizeof(Point)*1);
    if(NULL == NewPoint)
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    if( TRUE == InitCooridnates )
    {
        mpfr_init( NewPoint->X );
        mpfr_init( NewPoint->Y );
    }

    return NewPoint;
}

void freePoint( Point* XY )
{
    mpfr_clear(XY->X);
    mpfr_clear(XY->Y);
    mpfr_free_cache();
    free( XY );
}

Point* setPointByAddOnX( Point* OldPoint, mpfr_t Distance  )
{
    Point* NewPoint = initPoint( TRUE );

    mpfr_set( NewPoint->Y, OldPoint->Y, Round );
    mpfr_add( NewPoint->X, OldPoint->X, Distance, Round );

    return NewPoint;
}

Point* setPointByAddOnY( Point* OldPoint, mpfr_t Distance  )
{
    Point* NewPoint = initPoint( TRUE );

    mpfr_set( NewPoint->X, OldPoint->X, Round );
    mpfr_add( NewPoint->Y, OldPoint->Y, Distance, Round );

    return NewPoint;
}

Point* setPointBySubOnY( Point* OldPoint, mpfr_t Distance  )
{
    Point* NewPoint = initPoint( TRUE );

    mpfr_set( NewPoint->X, OldPoint->X, Round );
    mpfr_sub( NewPoint->Y, OldPoint->Y, Distance, Round );

    return NewPoint;
}

void initAndMultiply( mpfr_t Target, mpfr_t Mul1, mpfr_t Mul2 )
{
    mpfr_init( Target );
    mpfr_mul( Target, Mul1, Mul2, Round);
}

void initAndMultiplyUI( mpfr_t Target, mpfr_t Mul1, unsigned int Mul2 )
{
    mpfr_init( Target );
    mpfr_mul_ui( Target, Mul1, Mul2, Round);
}

Point* setPointAndRotate( Point* OldPoint, Point* CircleCenter, mpfr_t Degree )
{
    Point* NewPoint = initPoint( TRUE );

    rotatePoint( NewPoint, OldPoint, CircleCenter, Degree, Round );
    return NewPoint;
}

mpfr_t* libConvert( Point* P )
{
    mpfr_t* New = (mpfr_t*)malloc( sizeof(mpfr_t)<<1 );
    mpfr_init_set( New[ 0 ], P->X, Round );
    mpfr_init_set( New[ 1 ], P->Y, Round );
    return New;
}

Point* mainConvert( mpfr_t * P )
{
    Point* New = initPoint( FALSE );
    mpfr_init_set( New->X, P[ 0 ], Round );
    mpfr_init_set( New->Y, P[ 1 ], Round );
    return New;
}

Point* setPointByMirrow( Point* OldPoint, Point* MirrowPoint, int Flag )
{
    Point* NewPoint = initPoint( TRUE );

   if( MIRROW_X == Flag )
    {
        mirrowPointOnX( NewPoint, MirrowPoint->X, OldPoint, Round );
    }
    else if( MIRROW_Y == Flag )
    {
        mirrowPointOnY( NewPoint, MirrowPoint->Y, OldPoint, Round );
    }
    else
    {
        mirrowPoint( NewPoint, MirrowPoint, OldPoint, Round );
    }
    return NewPoint;
}

/*----------------------------- Behavior -----------------------------*/
void setPrecision( const char* PrecisionString )
{
    char* EndPointer;
    long int Precision;
    char ErrorBuffer[256];

    Precision = strtol( PrecisionString, &EndPointer, 10);

    if( '\0' != *EndPointer || 0 >= Precision )
    {
        snprintf( ErrorBuffer, 255, "Der übergebene Precusion Parameter '%s' is fehlerhaft.", PrecisionString );
        errorAndOut( ErrorBuffer );
    }
    /* Set precusion */
    mpfr_set_default_prec( (mpfr_prec_t) Precision );
}

/*----------------------------- Functionality -----------------------------*/
/*
 * Note:
 * Die Punkte A, E, Z sind ausreichend fuer die gesamte restliche Berchnung
 */
void makeDesk( const char* X, const char* Y, const char* Grunddistanze )
{
    Map->Kantenlaenge = ( mpfr_t* ) malloc( sizeof( mpfr_t )<<1 );
    if( NULL == Map->Kantenlaenge )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }
    mpfr_init_set_str( Map->Kantenlaenge[ 0 ], Grunddistanze, 10, Round );
    /* Kantenlaenge/2 */
    mpfr_init( Map->Kantenlaenge[ 1 ] );
    mpfr_div_ui( Map->Kantenlaenge[ 1 ], Map->Kantenlaenge[ 0 ], 2, Round );
    if( TRUE == Verbose.V )
    {
        printf("\nKantenlaenge betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->Kantenlaenge[ 0 ], Round );
        printf("\nKantenlaenge/2 betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->Kantenlaenge[ 1 ], Round );
        fflush( stdout );
    }

    Map->A = initPoint( TRUE );
    mpfr_set_str( Map->A->X, X, 10, Round );
    mpfr_set_str( Map->A->Y, Y, 10, Round );
    Map->E = setPointByAddOnX( Map->A, Map->Kantenlaenge[ 1 ] );
    Map->Z = setPointByAddOnY( Map->E, Map->Kantenlaenge[ 1 ] );

    if( TRUE == Verbose.V )
    {
        Map->B = setPointByAddOnX( Map->A, Map->Kantenlaenge[ 0 ] );
        Map->C = setPointByAddOnY( Map->A, Map->Kantenlaenge[ 1 ] );
        Map->D = setPointByAddOnX( Map->C, Map->Kantenlaenge[ 0 ] );
        printf("\nA ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->A->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->A->Y, Round );
        printf("\nB ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->B->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->B->Y, Round );
        printf("\nC ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->C->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->C->Y, Round );
        printf("\nD ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->D->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->D->Y, Round );
        printf("\nE ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->E->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->E->Y, Round );
        printf("\nZ ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->Z->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->Z->Y, Round );
    }
    else
    {
        freePoint(Map->A);
    }
}

void printDesk()
{
    printf("\n\tdesk: {");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tA: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->A->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->A->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tB: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->B->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->B->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tC: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->C->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->C->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tD: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->D->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->D->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tE: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->E->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->E->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tZ: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->Z->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->Z->Y, Round);
        printf("\n\t\t},");
    }
    printf("\n\t},");
}

void makeBreitenPunkte( unsigned int Index, const char* Label )
{
    Map->BreitenPunkte[ Index ] = setPointByAddOnY( Map->ToMove, Map->Breiten[Index] );
    if( TRUE == Verbose.V )
    {
        printf("\n%s ist vorortet bei:", Label );
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BreitenPunkte[ Index ]->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BreitenPunkte[ Index ]->Y, Round );
        fflush( stdout );
    }
}

void makeBreitenDistance( mpfr_t* Einheit, unsigned int Index, const char* Label, mpfr_t** Paralellkreise )
{
    mpfr_init( Map->Breiten[ Index ] );
    toDegree(
            Map->Breiten[ Index ],
            Paralellkreise[ Index ][ 0 ],
            Paralellkreise[ Index ][ 1 ],
            Paralellkreise[ Index ][ 2 ],
            Round
    );

    mpfr_mul( Map->Breiten[ Index ],  Map->Breiten[ Index ], *Einheit, Round );
    mpfr_sub( Map->Breiten[ Index ], *Map->AequatorDistanze, Map->Breiten[ Index ], Round );

    if( TRUE == Verbose.V )
    {
        printf("\nBerechnete Distanz von %s: ", Label );
        mpfr_out_str(stdout, 10, 0,  Map->Breiten[ Index ], Round);
    };
}

void makeBreiten( mpfr_t* Einheit )
{
    unsigned int Index;
    char Label[250];

    Map->Breiten = (mpfr_t*) malloc(Sizes[0]*sizeof(mpfr_t));
    if( NULL == Map->Breiten )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    if( TRUE == Verbose.V  || FALSE == UsePlan.V )
    {
        Map->BreitenPunkte = (Point**) malloc(Sizes[0]*sizeof(Point*));
        if( NULL == Map->BreitenPunkte )
        {
            errorAndOut( "\nSomethings wrong with the memory, Jim." );
        }
    }

    for( Index = 0; Index < Sizes[0]; Index++ )
    {
        snprintf( Label, 249, "Parallelkreis%u", Index+1 );
        makeBreitenDistance( Einheit, Index, Label, Paralellkreise );
        if( TRUE == Verbose.V || FALSE == UsePlan.V )
        {
            makeBreitenPunkte( Index, Label );
        }
    }
}

void* makeThreadedBreiten( void* Einheit )
{
    setParalellkreise();
    makeBreiten( (mpfr_t*) Einheit );
    return NULL;
}

void printBreiten()
{
    unsigned int Index;
    printf("\n\tbreite: [");
    if( TRUE == Verbose.V )
    {
        for( Index = 0; Index<Sizes[0]; Index++)
        {
            printf("\n\t\t{\n\t\t\tX: ");
            mpfr_out_str(stdout, 10, 0, Map->BreitenPunkte[ Index ]->X, Round);
            printf(",\n\t\t\tY: ");
            mpfr_out_str(stdout, 10, 0, Map->BreitenPunkte[ Index ]->Y, Round);
            printf("\n\t\t},");
        }
    }
    printf("\n\t],");
}

void printBreitenDistances()
{
    unsigned int Index;
    printf("\n\tbreiteD: [");
    if( TRUE == Verbose.V )
    {
        for( Index = 0; Index<Sizes[0]; Index++)
        {
            printf("\n\t\t");
            mpfr_out_str(stdout, 10, 0, Map->Breiten[ Index ], Round);
            printf(",");
        }
    }
    printf("\n\t],");
}

void outputPoints( OutputBuffer* Buffer )
{
    unsigned int Index;

    printPoints( Buffer );
    for( Index = 0; Index<Sizes[1]; Index++)
    {
        freePoint( Buffer->Value[ Index ] );
    }
}

void printPoints( OutputBuffer* Buffer )
{
    unsigned int Index;
    unsigned int Start, End;

    if( 0 > Buffer->Start )
    {
        Start = 0;
    }
    else
    {
        Start = Buffer->Start;
    }

    if( 0 > Buffer->End )
    {
        End = Sizes[1];
    }
    else
    {
        End = Buffer->End;
    }

    printf("\n\t\t[");
    for( Index = Start; Index<End; Index++)
    {
        printf("\n\t\t\t{\n\t\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Buffer->Value[ Index ]->X, Round);
        printf(",\n\t\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Buffer->Value[ Index ]->Y, Round);
        printf("\n\t\t\t},");
    }
    printf("\n\t\t],");
}

void printStart()
{
    printf("\nJSON>>>{");
}

void printPayloadStart()
{
    printf("\n\tpayload: [");
}

void printEnd()
{
    printf("\n}<<<JSON\n");
}

void printPayloadEnd()
{
    printf("\n\t],");
}

void houseKeeping()
{
    unsigned int Index;

    Map->AequatorDistanze = NULL;
    Map->ToMove = NULL;

    for( Index = 0; Index<Sizes[0]; Index++)
    {
        mpfr_clear(Map->Breiten[Index]);

        if( TRUE == Verbose.V || FALSE == UsePlan.V )
        {
            freePoint(Map->BreitenPunkte[Index]);
        }
        mpfr_free_cache();
    }

    free( Map->Breiten );

    if( TRUE == Verbose.V  || FALSE == UsePlan.V )
    {
        free(Map->BreitenPunkte);
    }

    if( TRUE == UsePlan.V )
    {
        for( Index = 0; Index<Sizes[1]; Index++)
        {
            freePoint(Map->Laengen[Index]);
        }

        free(Map->Laengen);
    }
}

void cleanUp()
{
    unsigned Index;

    mpfr_clear( Map->Kantenlaenge[ 0 ] );
    mpfr_clear( Map->Kantenlaenge[ 1 ] );
    free( Map->Kantenlaenge );
    freePoint( Map->E );
    freePoint( Map->Z );
    if( TRUE == Verbose.V )
    {
        freePoint( Map->A );
        freePoint( Map->B );
        freePoint( Map->C );
        freePoint( Map->D );
    }
    for( Index = 0; Index < Sizes[0]; Index++ )
    {
        mpfr_clear( Paralellkreise[Index][0] );
        mpfr_clear( Paralellkreise[Index][1] );
        mpfr_clear( Paralellkreise[Index][2] );
        free( Paralellkreise[Index] );
    }
    mpfr_free_cache();
    free( Paralellkreise );
    free( Map );
    cleanUpLib();
}

void filterY(
        Point** Container,
        Point* ToMove,
        Point* Center,
        mpfr_t* Radians,
        mpfr_t* Diameter,
        unsigned int Index
)
{
    mpfr_t InBetween, Tmp, Degree;

    mpfr_init( InBetween );
    mpfr_init( Tmp );
    mpfr_init( Degree );

    if( Index < YStartUpper || Index > YEndUpper )
    {
        mpfr_set_nan( Container[ Index ]->X );
        mpfr_set_nan( Container[ Index ]->Y );
    }
    else if( Index == YStartUpper && 0 != YStartLower )
    {
        freePoint( Container[ Index ] );

        mpfr_set_si( InBetween, FilterY[0][3], Round );
        mpfr_div_si( InBetween, InBetween, GegenFactor, Round );

        mpfr_mul( Tmp, *Radians, InBetween, Round );
        getDegreeOnCircle( Degree, Tmp, *Diameter, Round );

        Container[ Index ] = setPointAndRotate( ToMove, Center, Degree );
    }
    else if( Index == YEndUpper && 0 != YEndLower )
    {
        mpfr_set_si( InBetween, FilterY[1][3], Round );
        mpfr_div_si( InBetween, InBetween, GegenFactor, Round );

        mpfr_mul( Tmp, *Radians, InBetween, Round );
        getDegreeOnCircle( Degree, Tmp, *Diameter, Round );

        Container[ Index+1 ] = setPointAndRotate( ToMove, Center, Degree );
    }

    mpfr_clear( InBetween );
    mpfr_clear( Tmp );
    mpfr_clear( Degree );
    mpfr_free_cache();
}

unsigned int computeLaengenAtBreite(
        Point** Container,
        Point* ToMove,
        Point* Center,
        mpfr_t* Radians,
        mpfr_t* Diameter,
        unsigned int Index,
        unsigned int Factor,
        unsigned int Offset
)
{
    mpfr_t Tmp, Degree;

    mpfr_init( Tmp );
    mpfr_init( Degree );

    mpfr_mul_si( Tmp, *Radians, Factor, Round );
    getDegreeOnCircle( Degree, Tmp, *Diameter, Round );

    Container[ Index ] = setPointAndRotate( ToMove, Center, Degree );

    if( 0 != Factor )
    {
        if( TRUE == Mirrow.V )
        {
            Container[ Factor+Offset ] = setPointByMirrow( Container[Index], ToMove, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si( Degree, Degree, -1, Round);
            Container[ Factor+Offset ] = setPointAndRotate( ToMove, Center, Degree );
        }

        filterY(
                Container,
                ToMove,
                Center,
                Radians,
                Diameter,
                Factor+Offset
        );
    }

    filterY(
            Container,
            ToMove,
            Center,
            Radians,
            Diameter,
            Index
    );

    mpfr_clear( Tmp );
    mpfr_clear( Degree );
    mpfr_free_cache();

    Factor--;
    return  Factor;
}

void* outputDaemon()
{
    OutputBuffer* Buffer;
    unsigned int Index;

    while( TRUE )
    {
        if( WRITER_DONE == DaemonBuffer[ Sizes[ 0 ]-1 ].Status )
        {
            break;
        }

        for( Index = 0; Index < Sizes[ 0 ]; Index++ )
        {
            if( WRITER_RUN == DaemonBuffer[ Index ].Status )
            {
                break;
            }
        }

        if( Sizes[ 0 ] <= Index )
        {
            if( TRUE == KillDaemon )
            {
                break;
            }
            pthread_yield();
            continue;
        }

        Buffer = &DaemonBuffer[ Index ].Buffer;
        outputPoints( Buffer );
        free( Buffer->Value );
        DaemonBuffer[ Index ].Status = WRITER_DONE;
    }
    return NULL;
}

void outputHelper( OutputBuffer* Buffer )
{
    unsigned int Index;

    if( 0 == ( ( Buffer->OutIndex+1 )%( MaxThreads*MemoryBoost ) ) && 0 != Buffer->OutIndex )
    {
        while( WRITER_DONE != DaemonBuffer[ Buffer->OutIndex-1 ].Status )
        {
            pthread_yield();
        }
    }

    DaemonBuffer[ Buffer->OutIndex ].Buffer.Value = (Point**)malloc( Sizes[1] * sizeof( Point* ) );
    if( NULL == DaemonBuffer[ Buffer->OutIndex ].Buffer.Value )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    for( Index = 0; Index<Sizes[1]; Index++)
    {
        DaemonBuffer[ Buffer->OutIndex ].Buffer.OutIndex = Buffer->OutIndex;
        DaemonBuffer[ Buffer->OutIndex ].Buffer.Value[ Index ] = Buffer->Value[ Index ];
    }

    DaemonBuffer[ Buffer->OutIndex ].Buffer.Start = Buffer->Start;
    DaemonBuffer[ Buffer->OutIndex ].Buffer.End = Buffer->End;
    DaemonBuffer[ Buffer->OutIndex ].Status = WRITER_RUN;
}

void intersectionsThreaded(
        void* (* Hook)( void* ),
        unsigned short UseFilter,
        double Factor,
        unsigned int Offset,
        Point* Center
)
{
    unsigned int ThreadIndex, BreitenIndex;
    Interaction IntersectionsThread[ MaxThreads ];
    ThreadHelper Helpers[ MaxThreads ];
    long Start, End;

    Start = -1;
    End = -1;

    DaemonBuffer = ( Writer* )malloc( Sizes[ 0 ]* sizeof( Writer ) );
    if( NULL == DaemonBuffer )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    for( BreitenIndex = 0; BreitenIndex < Sizes[ 0 ]; BreitenIndex++ )
    {
        DaemonBuffer[ BreitenIndex ].Status = WRITER_READY;
    }

    for( ThreadIndex = 0; ThreadIndex < MaxThreads; ThreadIndex++ )
    {
        Helpers[ ThreadIndex ].Buffer.Value = (Point**)malloc( Sizes[1] * sizeof( Point* ) );
        if( NULL == Helpers[ ThreadIndex ].Buffer.Value )
        {
            errorAndOut( "\nSomethings wrong with the memory, Jim." );
        }

        Helpers[ ThreadIndex ].Buffer.Start = -1;
        Helpers[ ThreadIndex ].Buffer.End = -1;
        Helpers[ ThreadIndex ].Factor = Factor;
        Helpers[ ThreadIndex ].Offset = Offset;
        Helpers[ ThreadIndex ].Center = Center;
    }

    pthread_create(
            &WriterDaemon,
            NULL,
            outputDaemon,
            NULL
    );

    BreitenIndex = 0;

    ThreadIndex = 0;
    while( BreitenIndex < Sizes[0] )
    {
        if( BreitenIndex < MaxThreads )
        {
            Helpers[ ThreadIndex ].BreitenIndex = BreitenIndex;
            IntersectionsThread[ ThreadIndex ].Index = BreitenIndex;
            pthread_create(
                    &IntersectionsThread[ ThreadIndex ].Thread,
                    NULL,
                    Hook,
                    &Helpers[ ThreadIndex ]
            );

            ThreadIndex++;
            BreitenIndex++;
            continue;
        }

        if( Sizes[ 0 ] <= BreitenIndex+MaxThreads )
        {
            break;
        }

        for( ThreadIndex = 0; ThreadIndex < MaxThreads; ThreadIndex++ )
        {
            pthread_join( IntersectionsThread[ ThreadIndex ].Thread, NULL );

            if( TRUE == UseFilter )
            {
                if( -1 == Start )
                {
                    Start = globFilterYFindStart( Helpers[ThreadIndex].Buffer.Value );
                }

                Helpers[ThreadIndex].Buffer.Start = Start;

                if( -1 == End )
                {
                    End = Sizes[1]-globFilterYFindEnd( Helpers[ThreadIndex].Buffer.Value );
                }

                Helpers[ThreadIndex].Buffer.End = End;
            }

            Helpers[ ThreadIndex ].Buffer.OutIndex = IntersectionsThread[ ThreadIndex ].Index;
            outputHelper( &Helpers[ ThreadIndex ].Buffer );

            Helpers[ ThreadIndex ].BreitenIndex = BreitenIndex;
            IntersectionsThread[ ThreadIndex ].Index = BreitenIndex;
            pthread_create(
                    &IntersectionsThread[ ThreadIndex ].Thread,
                    NULL,
                    Hook,
                    &Helpers[ ThreadIndex ]
            );

            BreitenIndex++;
        }
    }

    for( ThreadIndex = 0; ThreadIndex < MaxThreads; ThreadIndex++ )
    {
        pthread_join( IntersectionsThread[ ThreadIndex ].Thread, NULL );
        if( TRUE == UseFilter )
        {
            if( -1 == Start )
            {
                Start = globFilterYFindStart( Helpers[ThreadIndex].Buffer.Value );
            }

            Helpers[ThreadIndex].Buffer.Start = Start;

            if( -1 == End )
            {
                End = Sizes[1]-globFilterYFindEnd( Helpers[ThreadIndex].Buffer.Value );
            }

            Helpers[ThreadIndex].Buffer.End = End;
        }
        Helpers[ ThreadIndex ].Buffer.OutIndex = IntersectionsThread[ ThreadIndex ].Index;
        outputHelper( &Helpers[ ThreadIndex ].Buffer );
    }

    KillDaemon = TRUE;
    pthread_join( WriterDaemon, NULL );

    if( 0 != ( Sizes[ 0 ]%MaxThreads ) )
    {
        for(
                ThreadIndex = 0, BreitenIndex = Sizes[ 0 ]-( Sizes[ 0 ]%MaxThreads );
                BreitenIndex < Sizes[ 0 ];
                BreitenIndex++, ThreadIndex++ )
        {
            Helpers[ ThreadIndex ].BreitenIndex = BreitenIndex;
            Helpers[ ThreadIndex ].Buffer.OutIndex = BreitenIndex;
            IntersectionsThread[ ThreadIndex ].Index = BreitenIndex;
            Hook( &Helpers[ ThreadIndex ] );
            if( TRUE == UseFilter )
            {
                if( -1 == Start )
                {
                    Start = globFilterYFindStart( Helpers[ThreadIndex].Buffer.Value );
                }

                Helpers[ThreadIndex].Buffer.Start = Start;

                if( -1 == End )
                {
                    End = Sizes[1]-globFilterYFindEnd( Helpers[ThreadIndex].Buffer.Value );
                }

                Helpers[ThreadIndex].Buffer.End = End;
            }
            outputPoints( &Helpers[ ThreadIndex ].Buffer );
        }
    }

    free( DaemonBuffer );

    /*Housekeeping*/
    for( ThreadIndex = 0; ThreadIndex < MaxThreads; ThreadIndex++ )
    {
        free( Helpers[ ThreadIndex ].Buffer.Value );
    }
}

/*----------------------------- Planimetrik -----------------------------*/
void startPlanimetrik()
{
    Map->BaseMap.Plan = (Planimetrik*) malloc( 1*sizeof(Planimetrik) );
    if( NULL == Map->BaseMap.Plan )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }
}

/*
 * 34E = HE
 * 131\frac5{12}E = HZ
 * Bekannte EZ
 * => EZ entspricht (131\frac5{12}-34)
 * => \frac12 Kantenlaenge/(131\frac5{12}-34)
 */
void planimetrikGetMetrum()
{
    mpfr_t Tmp;
    mpfr_init( Tmp );
    mpfr_init( Map->BaseMap.Plan->Einheit );
    mpfr_set_ui( Tmp, 5, Round );
    mpfr_div_ui( Tmp, Tmp, 12, Round );
    mpfr_add_ui( Tmp, Tmp, 131, Round );
    mpfr_sub_ui( Tmp, Tmp, 34, Round );
    mpfr_div( Map->BaseMap.Plan->Einheit, Map->Kantenlaenge[ 1 ], Tmp, Round );

    if( TRUE == Verbose.V )
    {
        printf("\nEine Einheit betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->Einheit, Round );
    }

    mpfr_clear( Tmp );
    mpfr_free_cache ();
}

/*
 * HZ = 131 5/12E
 * HE = 34E
 * HK = 79E
 * HO = 79E-27E = 52E
 * HS = 79E+36E = 115E
 * Note: Allein die Strecke HZ reicht fuer die restliche Berechnung
 */
void planimetrikDistances()
{
    mpfr_t Tmp[2];

    mpfr_init_set_ui( Tmp[0], 131, Round );
    mpfr_init_set_ui( Tmp[1], 5, Round );
    mpfr_div_ui( Tmp[1], Tmp[1], 12, Round );
    mpfr_add( Tmp[0], Tmp[0], Tmp[1], Round );
    initAndMultiply( Map->BaseMap.Plan->HZ, Map->BaseMap.Plan->Einheit, Tmp[0] );

    initAndMultiplyUI( Map->BaseMap.Plan->HS, Map->BaseMap.Plan->Einheit, 115 );
    Map->AequatorDistanze = &Map->BaseMap.Plan->HS;
    initAndMultiplyUI( Map->BaseMap.Plan->HK, Map->BaseMap.Plan->Einheit, 79 );

    if( TRUE == Verbose.V )
    {
        initAndMultiplyUI( Map->BaseMap.Plan->HE, Map->BaseMap.Plan->Einheit, 34 );
        initAndMultiplyUI( Map->BaseMap.Plan->HO, Map->BaseMap.Plan->Einheit, 52 );

        printf("\nHZ betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HZ, Round );
        printf("\nHE betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HE, Round );
        printf("\nHK betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HK, Round );
        printf("\nHO betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HO, Round );
        printf("\nHS betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HS, Round );
        fflush( stdout );
    }

    mpfr_clear( Tmp[0] );
    mpfr_clear( Tmp[1] );
    mpfr_free_cache ();
}
/*
 * Note: Der Text denkt es von E nicht von Z!
 */
void planimetrikBreitenPunkte()
{
    /* H = Z_x, Z_y+HZ */
    Map->BaseMap.Plan->H = setPointBySubOnY( Map->Z, Map->BaseMap.Plan->HZ );
    /* S = H_x, H_y-SH */
    Map->BaseMap.Plan->S = setPointByAddOnY( Map->BaseMap.Plan->H, Map->BaseMap.Plan->HS );

    Map->ToMove = Map->BaseMap.Plan->H;

    if( TRUE == Verbose.V )
    {
        /* K = H_x, H_y-KH */
        Map->BaseMap.Plan->K = setPointByAddOnY( Map->BaseMap.Plan->H, Map->BaseMap.Plan->HK );

        /* O = H_x, H_y-OH */
        Map->BaseMap.Plan->O = setPointByAddOnY( Map->BaseMap.Plan->H, Map->BaseMap.Plan->HO );

        printf("\nH ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->H->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->H->Y, Round );
        printf("\nK ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->K->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->K->Y, Round );
        printf("\nO ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->O->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->O->Y, Round );
        printf("\nS ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->S->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->S->Y, Round );
    }
}

void planimetrikCleanUpLaengen()
{
    Point** Laengen;
    Point** LaengenSpeacial;
    int Index, InnerIndex;

    if( FILTER_Y_NO == YFilter )
    {
        return;
    }

    Laengen = (Point**)malloc(Sizes[1]*sizeof(Point*));
    if( NULL == Laengen )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    LaengenSpeacial = (Point**)malloc(Sizes[1]*sizeof(Point*));
    if( NULL == LaengenSpeacial )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    InnerIndex = 0;
    for( Index = 0; Index < Sizes[1]; Index++ )
    {
        if( 0 != mpfr_nan_p( Map->Laengen[ Index ]->X ) && 0 != mpfr_nan_p( Map->Laengen[ Index ]->Y ) )
        {
            freePoint( Map->Laengen[ Index ] );
            freePoint( Map->BaseMap.Plan->Laengen[ Index ] );
        }
        else
        {
            Laengen[ InnerIndex ] = Map->Laengen[ Index ];
            LaengenSpeacial[ InnerIndex ] = Map->BaseMap.Plan->Laengen[ Index ];
            InnerIndex++;
        }
    }

    Sizes[1] = InnerIndex;
    free( Map->Laengen );
    free( Map->BaseMap.Plan->Laengen );

    Map->Laengen = (Point**)malloc(Sizes[1]*sizeof(Point*));
    if( NULL == Map->Laengen )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    for( Index = 0; Index<Sizes[1]; Index++ )
    {
        Map->Laengen[Index] = Laengen[Index];
    }

    Map->BaseMap.Plan->Laengen = (Point**)malloc(Sizes[1]*sizeof(Point*));
    if( NULL == Map->BaseMap.Plan->Laengen )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    for( Index = 0; Index<Sizes[1]; Index++ )
    {
        Map->BaseMap.Plan->Laengen[Index] = LaengenSpeacial[Index];
    }
    free( Laengen );
    free( LaengenSpeacial );
}

void planimetrikLaengenVerbose( mpfr_t* Diameter )
{
    mpfr_t Radians, Tmp, Degree;

    mpfr_init( Degree );
    mpfr_init( Radians );
    mpfr_init( Tmp );

    /* 18 mal : see 119 */
    mpfr_mul_ui( Radians, Map->BaseMap.Plan->Einheit, PLAN_LAENGEN_VERHAEHLTNIS, Round );
    mpfr_mul_ui( Tmp, Radians, LAENGEN_FAKTOR, Round );
    getDegreeOnCircle( Degree, Tmp, *Diameter, Round );

    /* M is depending on Z see S 199 3.ff */
    Map->BaseMap.Plan->M = setPointAndRotate( Map->Z, Map->BaseMap.Plan->H, Degree );
    /* G is depending on K see S 199 3.ff */
    Map->BaseMap.Plan->G = setPointAndRotate( Map->BaseMap.Plan->K, Map->BaseMap.Plan->H, Degree );
    /* O is depending on O see S 199 3.ff */
    Map->BaseMap.Plan->X = setPointAndRotate( Map->BaseMap.Plan->O, Map->BaseMap.Plan->H, Degree );
    /* S is depending on R see S 199 3.ff  */
    Map->BaseMap.Plan->R = setPointAndRotate( Map->BaseMap.Plan->S, Map->BaseMap.Plan->H, Degree );

    printf("\nBerechnete Grad von LKG ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nBerechneter Radiant von LKG ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );

    printf("\nM ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->M->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->M->Y, Round );
    printf("\nG ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->G->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->G->Y, Round );
    printf("\nX ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->X->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->X->Y, Round );
    printf("\nR ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->R->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->R->Y, Round );

    if( TRUE == Mirrow.V )
    {
        /* N is mirrow by Z of M on X */
        Map->BaseMap.Plan->N = setPointByMirrow( Map->BaseMap.Plan->M, Map->BaseMap.Plan->K, MIRROW_X );
        /* L is mirrow by K of G on X */
        Map->BaseMap.Plan->L = setPointByMirrow( Map->BaseMap.Plan->G, Map->BaseMap.Plan->K, MIRROW_X );
        /* P is mirrow by O of X on X */
        Map->BaseMap.Plan->P = setPointByMirrow( Map->BaseMap.Plan->X, Map->BaseMap.Plan->O, MIRROW_X );
        /* T is mirrow by H of R on X */
        Map->BaseMap.Plan->T = setPointByMirrow( Map->BaseMap.Plan->R, Map->BaseMap.Plan->H, MIRROW_X );
    }
    else
    {
        /* Flip */
        mpfr_mul_si( Degree, Degree, -1, Round );
        /* N is depending on Z see S 199 3.ff */
        Map->BaseMap.Plan->N = setPointAndRotate( Map->Z, Map->BaseMap.Plan->H, Degree );
        /* L is depending on K see S 199 3.ff */
        Map->BaseMap.Plan->L = setPointAndRotate( Map->BaseMap.Plan->K, Map->BaseMap.Plan->H, Degree );
        /* P is depending on O see S 199 3.ff */
        Map->BaseMap.Plan->P = setPointAndRotate( Map->BaseMap.Plan->O, Map->BaseMap.Plan->H, Degree );
        /* T is depending on R see S 199 3.ff  */
        Map->BaseMap.Plan->T = setPointAndRotate( Map->BaseMap.Plan->S, Map->BaseMap.Plan->H, Degree );
    }

    printf("\nBerechnete Grad von GKL ist: ");
    mpfr_out_str ( stdout, 10, 0, Degree, Round );
    printf("\nBerechneter Radiant von GKL ist: ");
    mpfr_out_str ( stdout, 10, 0, Radians, Round );

    printf("\nN ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->N->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->N->Y, Round );
    printf("\nL ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->L->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->L->Y, Round );
    printf("\nX ist vorortet bei:");
    printf("\n\tP:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->O->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->O->Y, Round );
    printf("\nT ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->T->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->T->Y, Round );
    fflush( stdout );

    /* Housekeeping */
    mpfr_clear( Degree );
    mpfr_clear( Radians );
    mpfr_clear( Tmp );
    mpfr_free_cache();
}

void planimetrikLaengen()
{
    unsigned int Index, Offset, Factor;

    mpfr_t Diameter, Radians, Tmp;

    mpfr_init( Diameter );
    mpfr_init( Radians );
    mpfr_init( Tmp );

	mpfr_set_ui( Tmp,PLAN_LAENGEN_VERHAEHLTNIS , Round );
	mpfr_div_ui( Tmp, Tmp, UseFactor, Round );

    mpfr_mul_si( Diameter, Map->BaseMap.Plan->HK, 2, Round );
    /* 18 mal : see 119 */
    mpfr_mul( Radians, Map->BaseMap.Plan->Einheit, Tmp, Round );

    Sizes[1] = ( LAENGEN_FAKTOR<<1 ) * UseFactor;
    Sizes[1] += 1;//+Aequator

	Map->Laengen = (Point**)malloc(Sizes[1]*sizeof(Point*));
	if( NULL == Map->Laengen )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

	Offset = LAENGEN_FAKTOR*UseFactor;
    Factor = Offset;

    for( Index = 0; Index<=Offset; Index++ )
    {
        Factor = computeLaengenAtBreite(
            Map->Laengen,
            Map->BaseMap.Plan->S,
            Map->BaseMap.Plan->H,
            &Radians,
            &Diameter,
            Index,
            Factor,
            Offset
        );
    }

    if( TRUE == Verbose.V )
    {
		planimetrikLaengenVerbose( &Diameter );
    }

    /* Housekeeping */
    mpfr_clear( Diameter );
    mpfr_clear( Radians );
    mpfr_clear( Tmp );
    mpfr_free_cache();
}

void planimetrikSpecialLaengeVerbose( mpfr_t* Diameter )
{
    mpfr_t Radians, Tmp, Degree;

    mpfr_init( Degree );
    mpfr_init( Radians );
    mpfr_init( Tmp );

    mpfr_mul_ui( Radians, Map->BaseMap.Plan->Einheit, PLAN_LAENGEN_VERHAEHLTNIS, Round );
    mpfr_mul_ui( Tmp, Radians, LAENGEN_FAKTOR, Round );
    getDegreeOnCircle( Degree, Tmp, *Diameter, Round );

    /* Set F */
    /* See S. 121 7.ff */
    /* Abstand im Intervall beim Äquator */
    Map->BaseMap.Plan->F = setPointAndRotate( Map->Z, Map->BaseMap.Plan->H, Degree );
    if( TRUE == Mirrow.V )
    {
        /* U is mirrow by Z of F on X */
        Map->BaseMap.Plan->U = setPointByMirrow(Map->BaseMap.Plan->F, Map->Z, MIRROW_X);
    }
    else
    {
        /* Flip */
        mpfr_mul_si( Degree, Degree, -1, Round );
        Map->BaseMap.Plan->U = setPointAndRotate( Map->Z, Map->BaseMap.Plan->H, Degree );
    }

    printf("\nF ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->F->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->F->Y, Round );
    printf("\nU ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->U->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->U->Y, Round );
    fflush( stdout );

    /* Housekeeping */
    mpfr_clear( Degree );
    mpfr_clear( Radians );
    mpfr_clear( Tmp );
    mpfr_free_cache();
}

void planimetrikSpecialLaenge()
{
    unsigned int Index, Offset, Factor;
    mpfr_t Degree, Radians, Diameter, Tmp;

    mpfr_init( Degree );
    mpfr_init( Radians );
    mpfr_init( Diameter );
    mpfr_init( Tmp );

	mpfr_set_ui( Tmp, PLAN_LAENGEN_VERHAEHLTNIS, Round );
	mpfr_div_ui( Tmp, Tmp, UseFactor, Round );

    mpfr_mul_si( Diameter, Map->BaseMap.Plan->HS, 2, Round );
    /* 18 mal : see 119 */
    mpfr_mul(Radians, Map->BaseMap.Plan->Einheit, Tmp, Round);

    Map->BaseMap.Plan->Laengen = (Point**)malloc(Sizes[1]*sizeof(Point*));
    if( NULL == Map->Laengen )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

	Offset = LAENGEN_FAKTOR*UseFactor;
    Factor = Offset;

    for( Index = 0; Index<=Offset; Index++ )
    {
        Factor = computeLaengenAtBreite(
                Map->BaseMap.Plan->Laengen,
                Map->Z,
                Map->BaseMap.Plan->H,
                &Radians,
                &Diameter,
                Index,
                Factor,
                Offset
        );
    }

    if( TRUE == Verbose.V )
    {
	    planimetrikSpecialLaengeVerbose( &Diameter );
    }

    mpfr_clear( Diameter );
    mpfr_clear( Degree );
    mpfr_clear( Radians );
    mpfr_clear( Tmp );
    mpfr_free_cache();
}

void* planimetrikIntersectionsInnerLoop( void* Helper )
{
    mpfr_t Tmp[4];
    mpfr_t Intermedian[2];
    unsigned int LaengenIndex, Total;
    short Type;

    Total = 0;
    mpfr_init(Tmp[0]);
    mpfr_init(Tmp[1]);
    mpfr_init(Tmp[2]);
    mpfr_init(Tmp[3]);
    mpfr_init(Intermedian[0]);
    mpfr_init(Intermedian[1]);

    for (LaengenIndex = 0; LaengenIndex < Sizes[1]; LaengenIndex++)
    {
        if( 0 >= mpfr_cmp( *Map->AequatorDistanze, Map->Breiten[( (ThreadHelper* ) Helper )->BreitenIndex ] ) )
        {
            continue;
        }

        Type = intersectCircleLine(
                Tmp,
                ( (ThreadHelper* ) Helper )->Center,
                Map->Laengen[ LaengenIndex ],
                ( (ThreadHelper* ) Helper )->Center,
                Map->Breiten[ ( (ThreadHelper* ) Helper )->BreitenIndex ],
                Round
        );

        if( ONE_TOUCH == Type )
        {
            mpfr_set(Intermedian[0], Tmp[1], Round);
            mpfr_set(Intermedian[1], Tmp[2], Round);
        }
        else
        {
            mpfr_set(Intermedian[0], Tmp[2], Round);
            mpfr_set(Intermedian[1], Tmp[3], Round);
        }

        ( (ThreadHelper* ) Helper )->Buffer.Value[ Total ] = mainConvert( Intermedian );
        Total++;
    }

    for (LaengenIndex = 0; LaengenIndex < Sizes[1]; LaengenIndex++)
    {
        if( 0 < mpfr_cmp( *Map->AequatorDistanze, Map->Breiten[ ( (ThreadHelper* ) Helper )->BreitenIndex ] ) )
        {
            continue;
        }

        Type = intersectCircleLine(
                Tmp,
                Map->Laengen[ LaengenIndex ],
                Map->BaseMap.Plan->Laengen[ LaengenIndex ],
                ( (ThreadHelper* ) Helper )->Center,
                Map->Breiten[ ( (ThreadHelper* ) Helper )->BreitenIndex ],
                Round
        );

        if( ONE_TOUCH == Type )
        {
            mpfr_set(Intermedian[0], Tmp[1], Round);
            mpfr_set(Intermedian[1], Tmp[2], Round);
        }
        else
        {
            mpfr_set(Intermedian[0], Tmp[2], Round);
            mpfr_set(Intermedian[1], Tmp[3], Round);
        }

        ( (ThreadHelper* ) Helper )->Buffer.Value[ Total ] = mainConvert( Intermedian );
        Total++;
    }
    mpfr_clear(Tmp[0]);
    mpfr_clear(Tmp[1]);
    mpfr_clear(Tmp[2]);
    mpfr_clear(Tmp[3]);
    mpfr_clear(Intermedian[0]);
    mpfr_clear(Intermedian[1]);
    return NULL;
}

void planimetrikIntersections()
{
    ThreadHelper Helper;

    Helper.Center = Map->BaseMap.Plan->H;
    Helper.Buffer.Value = (Point**)malloc(Sizes[1] * sizeof( Point* ));
    if( NULL == Helper.Buffer.Value )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    Helper.Buffer.Start = -1;
    Helper.Buffer.End = -1;

    for( Helper.BreitenIndex = 0; Helper.BreitenIndex < Sizes[0]; Helper.BreitenIndex++ )
    {
        planimetrikIntersectionsInnerLoop( &Helper );
        outputPoints( &Helper.Buffer );
    }

    /*Housekeeping*/
    free( Helper.Buffer.Value );
}

void planimetrikThreadedIntersections()
{
    intersectionsThreaded(
            &planimetrikIntersectionsInnerLoop,
            FALSE,
            0.0,
            0,
            Map->BaseMap.Plan->H
    );
}

void planimetrikPrintBreitenPunkte()
{
    printf("\n\tlot: { ");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tH: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->H->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->H->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tK: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->K->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->K->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tO: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->O->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->O->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tS: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->S->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->S->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tstart: 'H'");
    }
    printf("\n\t},");
}

void planimetrikPrintLimits()
{
    printf("\n\tlimits: {");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tM: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->M->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->M->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tG: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->G->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->G->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tX: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->X->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->X->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tR: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->R->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->R->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tN: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->N->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->N->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tL: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->L->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->L->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tP: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->P->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->P->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tT: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->T->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->T->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tF: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->F->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->F->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tU: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->U->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->U->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tlimes: [ 'M', 'N' ],");
    }
    printf("\n\t},");
}

void planimetrikPrintDistances()
{
    printf("\n\tdistances: {");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tHZ:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HZ, Round );
        printf(",\n\t\tHE:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HE, Round );
        printf(",\n\t\tHK:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HK, Round );
        printf(",\n\t\tHO:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HO, Round );
        printf(",\n\t\tHS:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Plan->HS, Round );
    }
    printf("\n\t},");
}

void planimetrikPrintLaengen()
{
    unsigned int Index;

    printf("\n\tlaenge: [");
    if( TRUE == Verbose.V )
    {
        for( Index = 0; Index<Sizes[1]; Index++)
        {
            printf("\n\t\t{\n\t\t\tX: ");
            mpfr_out_str(stdout, 10, 0, Map->Laengen[ Index ]->X, Round);
            printf(",\n\t\t\tY: ");
            mpfr_out_str(stdout, 10, 0, Map->Laengen[ Index ]->Y, Round);
            printf("\n\t\t},");
        }

        for( Index = 0; Index<Sizes[1]; Index++)
        {
            printf("\n\t\t{\n\t\t\tX: ");
            mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->Laengen[ Index ]->X, Round);
            printf(",\n\t\t\tY: ");
            mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->Laengen[ Index ]->Y, Round);
            printf("\n\t\t},");
        }
    }
    printf("\n\t],");
}

void planimetrikPrint()
{
    printDesk();
    printf( "\n\teinheit: " );
    mpfr_out_str(stdout, 10, 0, Map->BaseMap.Plan->Einheit, Round);
    printf( "," );
    planimetrikPrintDistances();
    planimetrikPrintBreitenPunkte();
    planimetrikPrintDistances();
    planimetrikPrintLimits();
    planimetrikPrintLaengen();
    printBreiten();
    printBreitenDistances();
}

void planimetrikHouskeeping()
{
    unsigned int Index;

    houseKeeping();

    mpfr_clear(Map->BaseMap.Plan->HZ);
    mpfr_clear(Map->BaseMap.Plan->HS);
    mpfr_clear(Map->BaseMap.Plan->HK);
    mpfr_clear( Map->BaseMap.Plan->Einheit );

    freePoint(Map->BaseMap.Plan->H);
    freePoint(Map->BaseMap.Plan->S);

    for( Index = 0; Index<Sizes[1]; Index++)
    {
        freePoint( Map->BaseMap.Plan->Laengen[Index] );
    }

    free(Map->BaseMap.Plan->Laengen);

    if( TRUE == Verbose.V )
    {
        mpfr_clear(Map->BaseMap.Plan->HE);
        mpfr_clear(Map->BaseMap.Plan->HO);
        /* Redundanzpunkte auf dem Lot H~CD */
        freePoint(Map->BaseMap.Plan->O);
        freePoint(Map->BaseMap.Plan->K);
        freePoint(Map->BaseMap.Plan->X);
        freePoint(Map->BaseMap.Plan->G);
        freePoint(Map->BaseMap.Plan->R);
        freePoint(Map->BaseMap.Plan->M);
        freePoint(Map->BaseMap.Plan->F);
        freePoint(Map->BaseMap.Plan->P);
        freePoint(Map->BaseMap.Plan->L);
        freePoint(Map->BaseMap.Plan->T);
        freePoint(Map->BaseMap.Plan->N);
        freePoint(Map->BaseMap.Plan->U);
    }

    free(Map->BaseMap.Plan);
}

void* planimetrikThreadedLaenge()
{
    pthread_yield();
    planimetrikLaengen();
    planimetrikSpecialLaenge();
    planimetrikCleanUpLaengen();
    return NULL;
}

void* planimetrikThreaded()
{
    pthread_t Breiten, Laengen;
    startPlanimetrik();
    planimetrikGetMetrum();
    planimetrikDistances();

    planimetrikBreitenPunkte();
    pthread_create( &Breiten, NULL, makeThreadedBreiten, &Map->BaseMap.Plan->Einheit );
    pthread_create( &Laengen, NULL, planimetrikThreadedLaenge, NULL );
    pthread_join(Breiten, NULL);
    pthread_join(Laengen, NULL);
    printStart();
    planimetrikPrint();

    if( 0 != mpfr_nan_p( Map->Laengen[0]->X ) )
    {
        errorAndOut( "\nLaufzeitfehler: Die Laengengrade konnten nicht berechnet werden.\n" );
    }

    // Adjust MaxThread to avoid Mem iusses
    if( MaxThreads > Sizes[ 0 ] )
    {
        MaxThreads = Sizes[ 0 ];
    }

    printPayloadStart();
    planimetrikThreadedIntersections();
    printPayloadEnd();
    printEnd();
    planimetrikHouskeeping();

    return NULL;
}
/*----------------------------- Globus -----------------------------*/
void startGlob()
{
	Map->BaseMap.Glob = (Globus*) malloc( 1*sizeof(Globus) );
	if( NULL == Map->BaseMap.Plan )
	{
		errorAndOut( "\nSomethings wrong with the memory, Jim." );
	}
}
/*
 * AC=EZ = 90E
 * 1E = EZ/90
 * */
void globGetMetrum()
{
    mpfr_init( Map->BaseMap.Glob->Einheit );
    mpfr_div_ui( Map->BaseMap.Glob->Einheit, Map->Kantenlaenge[ 1 ], 90, Round );
    if( TRUE == Verbose.V )
    {
        printf("\nEine Einheit betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->Einheit, Round );
    }
}

/*
 * HL =181 5/6
 * ZH = 16 5/12 S.129
 * ZL = HL+ZH
 * ZL = 181 5/6 + 16 5/12
 * EH = EZ-ZH
 * EH = 90-16 5/12
 * EL = LZ-EZ
 * HG = 23 5/6
 * HK = 63
 */
void globDistances()
{
    mpfr_t Tmp[2];
    mpfr_init( Tmp[ 0 ] );
    mpfr_init( Tmp[ 1 ] );

    /* HL =181 5/6*/
    mpfr_set_ui( Tmp[ 0 ], 5, Round );
    mpfr_div_ui( Tmp[ 0 ], Tmp[ 0 ], 6, Round );
    mpfr_add_ui( Tmp[ 0 ], Tmp[ 0 ], 181, Round );

    initAndMultiply( Map->BaseMap.Glob->LH, Tmp[ 0 ], Map->BaseMap.Glob->Einheit );
    Map->AequatorDistanze = &Map->BaseMap.Glob->LH;

    mpfr_set_ui( Tmp[ 1 ], 5, Round );
    mpfr_div_ui( Tmp[ 1 ], Tmp[ 1 ], 12, Round );
    mpfr_add_ui( Tmp[ 1 ], Tmp[ 1 ], 16, Round );
    mpfr_init_set( Map->BaseMap.Glob->HZ, Tmp[ 1 ], Round );
    //initAndMultiply( Map->BaseMap.Glob->HZ, Tmp[ 1 ], Map->BaseMap.Glob->Einheit );

    mpfr_add( Tmp[ 1 ], Tmp[ 0 ], Tmp[ 1 ], Round );
    initAndMultiply( Map->BaseMap.Glob->LZ, Tmp[ 1 ], Map->BaseMap.Glob->Einheit );

    if( TRUE == Verbose.V )
    {
        mpfr_set_ui( Tmp[ 1 ], 5, Round );
        mpfr_div_ui( Tmp[ 1 ], Tmp[ 1 ], 6, Round );
        mpfr_add_ui( Tmp[ 1 ], Tmp[ 1 ], 23, Round );

        mpfr_init_set( Map->BaseMap.Glob->HG, Tmp[ 1 ], Round );
    //  initAndMultiply( Map->BaseMap.Glob->HG, Tmp[ 1 ], Map->BaseMap.Glob->Einheit );
        mpfr_sub( Tmp[ 1 ], Tmp[ 0 ], Tmp[ 1 ], Round );
        initAndMultiply( Map->BaseMap.Glob->LG, Tmp[ 1 ], Map->BaseMap.Glob->Einheit );

        mpfr_set_ui( Tmp[ 1 ], 63, Round );
        //initAndMultiply( Map->BaseMap.Glob->HK, Tmp[ 1 ], Map->BaseMap.Glob->Einheit );
        mpfr_init_set( Map->BaseMap.Glob->HK, Tmp[ 1 ], Round );
        mpfr_sub( Tmp[ 1 ], Tmp[ 0 ], Tmp[ 1 ], Round );
        initAndMultiply( Map->BaseMap.Glob->LK, Tmp[ 1 ], Map->BaseMap.Glob->Einheit );

        printf("\nLH betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LH, Round );
        printf("\nLZ betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LZ, Round );
        printf("\nHZ betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->HZ, Round );
        printf("\nLG betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LG, Round );
        printf("\nHG betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->HG, Round );
        printf("\nLK betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LK, Round );
        printf("\nHK betraegt:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->HK, Round );
    }

    /* Housekeeping */
    mpfr_clear( Tmp[ 0 ] );
    mpfr_clear( Tmp[ 1 ] );
    mpfr_free_cache();
}

/*
 * H = Z_x, Z_y-LZ
 * G = L_x, L_y+LG
 * K = L_x, L_y+KK
 * H = L_x, L_y+LH
 */
void globBreitenPunkte()
{
    mpfr_t Tmp;

    mpfr_init( Tmp );

    Map->BaseMap.Glob->L = setPointBySubOnY( Map->Z, Map->BaseMap.Glob->LZ );
    Map->ToMove = Map->BaseMap.Glob->L;

    if( TRUE == Verbose.V )
    {
        Map->BaseMap.Glob->H = setPointByAddOnY( Map->BaseMap.Glob->L, Map->BaseMap.Glob->LH );
        mpfr_mul( Tmp, Map->BaseMap.Glob->Einheit, Map->BaseMap.Glob->HK, Round );
        Map->BaseMap.Glob->K = setPointBySubOnY( Map->BaseMap.Glob->H, Tmp );
        mpfr_mul( Tmp, Map->BaseMap.Glob->Einheit, Map->BaseMap.Glob->HG, Round );
        Map->BaseMap.Glob->G = setPointBySubOnY( Map->BaseMap.Glob->H, Tmp );

        printf("\nL ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->L->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->L->Y, Round );
        printf("\nK ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->K->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->K->Y, Round );
        printf("\nG ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->G->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->G->Y, Round );
        printf("\nH ist vorortet bei:");
        printf("\n\tX:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->H->X, Round );
        printf("\n\tY:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->H->Y, Round );
    }

    mpfr_clear( Tmp );
    mpfr_free_cache();
}

void globLimits()
{
    mpfr_t Tmp[4];
    mpfr_t Diameter, Radians, Degree;

    if( FALSE == Verbose.V )
    {
        return;
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
    intersectCircleLine( Tmp, Map->A, Map->B, Map->BaseMap.Glob->L, Map->BaseMap.Glob->LK, Round );
    Map->BaseMap.Glob->P = initPoint( FALSE );
    mpfr_init_set( Map->BaseMap.Glob->P->X, Tmp[ 0 ], Round );
    mpfr_init_set( Map->BaseMap.Glob->P->Y, Tmp[ 1 ], Round );

    intersectCircleLine( Tmp, Map->A, Map->C, Map->BaseMap.Glob->L, Map->BaseMap.Glob->LG, Round );
    Map->BaseMap.Glob->X = initPoint( FALSE );
    mpfr_init_set( Map->BaseMap.Glob->X->X, Tmp[ 2 ], Round );
    mpfr_init_set( Map->BaseMap.Glob->X->Y, Tmp[ 3 ], Round );

    intersectCircleLine( Tmp, Map->A, Map->C, Map->BaseMap.Glob->L, Map->BaseMap.Glob->LZ, Round );
    Map->BaseMap.Glob->M = initPoint( FALSE );
    mpfr_init_set( Map->BaseMap.Glob->M->X, Tmp[ 2 ], Round );
    mpfr_init_set( Map->BaseMap.Glob->M->Y, Tmp[ 3 ], Round );

    if( TRUE == Mirrow.V )
    {
        Map->BaseMap.Glob->R = setPointByMirrow( Map->BaseMap.Glob->P, Map->BaseMap.Glob->K, MIRROW_X );

        Map->BaseMap.Glob->O = setPointByMirrow( Map->BaseMap.Glob->X, Map->BaseMap.Glob->G, MIRROW_X );

        Map->BaseMap.Glob->N = setPointByMirrow( Map->BaseMap.Glob->M, Map->Z, MIRROW_X );

    }
    else
    {
        intersectCircleLine( Tmp, Map->A, Map->B, Map->BaseMap.Glob->L, Map->BaseMap.Glob->LK, Round );
        Map->BaseMap.Glob->R = initPoint( FALSE );
        mpfr_init_set( Map->BaseMap.Glob->R->X, Tmp[ 2 ], Round );
        mpfr_init_set( Map->BaseMap.Glob->R->Y, Tmp[ 3 ], Round );

        intersectCircleLine( Tmp, Map->B, Map->D, Map->BaseMap.Glob->L, Map->BaseMap.Glob->LG, Round );
        Map->BaseMap.Glob->O = initPoint( FALSE );
        mpfr_init_set( Map->BaseMap.Glob->O->X, Tmp[ 2 ], Round );
        mpfr_init_set( Map->BaseMap.Glob->O->Y, Tmp[ 3 ], Round );

        intersectCircleLine( Tmp, Map->B, Map->D, Map->BaseMap.Glob->L, Map->BaseMap.Glob->LZ, Round );
        Map->BaseMap.Glob->N = initPoint( FALSE );
        mpfr_init_set( Map->BaseMap.Glob->N->X, Tmp[ 2 ], Round );
        mpfr_init_set( Map->BaseMap.Glob->N->Y, Tmp[ 3 ], Round );
    }

    printf("\nP ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->P->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->P->Y, Round );

    printf("\nR ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->R->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->R->Y, Round );

    printf("\nX ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->X->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->X->Y, Round );

    printf("\nO ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->O->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->O->Y, Round );

    printf("\nM ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->M->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->M->Y, Round );

    printf("\nN ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->N->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->N->Y, Round );

    /* Housekeeping */
    mpfr_clear( Tmp[ 0 ] );
    mpfr_clear( Tmp[ 1 ] );
    mpfr_clear( Tmp[ 2 ] );
    mpfr_clear( Tmp[ 3 ] );
    mpfr_clear( Diameter );
    mpfr_clear( Degree );
    mpfr_clear( Radians );
}

void globLaengen()
{
    mpfr_t Diameter, Radians, Degree, Tmp;

    if( FALSE == Verbose.V )
    {
        return;
    }

    mpfr_init( Tmp );
    mpfr_init( Diameter );
    mpfr_init( Degree );
    mpfr_init( Radians );

    /* Y */
    /* 18 mal : see 119 */
    /* Abstand 2 1/4 */
    if( TRUE == Org.V )
    {
        mpfr_set_ui( Tmp, 1, Round);
        mpfr_div_ui( Tmp, Tmp, 4, Round);
        mpfr_add_ui( Tmp, Tmp, 2, Round);

        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LK, 2, Round );
        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->Y = setPointAndRotate( Map->BaseMap.Glob->K, Map->BaseMap.Glob->L, Degree );

        printf("\nBerechnete Grad von VKY ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von VKY ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );

        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->V = setPointByMirrow( Map->BaseMap.Glob->Y, Map->BaseMap.Glob->K, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->V = setPointAndRotate( Map->BaseMap.Glob->K, Map->BaseMap.Glob->L, Degree );
        }

        mpfr_set_ui( Tmp, 7, Round);
        mpfr_div_ui( Tmp, Tmp, 12, Round);
        mpfr_add_ui( Tmp, Tmp, 4, Round);
        /* 18 mal : see 119 */
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LG, 2, Round );

        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->T = setPointAndRotate( Map->BaseMap.Glob->G, Map->BaseMap.Glob->L, Degree );
        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->U = setPointByMirrow( Map->BaseMap.Glob->T, Map->BaseMap.Glob->G, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->U = setPointAndRotate( Map->BaseMap.Glob->G, Map->BaseMap.Glob->L, Degree );
        }

        printf("\nBerechnete Grad von TGU ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von TGU ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );

        mpfr_set_ui( Tmp, 5, Round);
        /* 18 mal : see 119 */
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LH, 2, Round );
        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->I = setPointAndRotate( Map->BaseMap.Glob->H, Map->BaseMap.Glob->L, Degree );
        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->J = setPointByMirrow( Map->BaseMap.Glob->I, Map->BaseMap.Glob->H, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->J = setPointAndRotate( Map->BaseMap.Glob->H, Map->BaseMap.Glob->L, Degree );
        }

        printf("\nBerechnete Grad von IHJ ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von IHJ ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );

        mpfr_set_ui( Tmp, 5, Round);
        mpfr_div_ui( Tmp, Tmp, 6, Round);
        mpfr_add_ui( Tmp, Tmp, 4, Round);
        /* 18 mal : see 119 */
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LZ, 2, Round );

        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->S = setPointAndRotate( Map->Z, Map->BaseMap.Glob->L, Degree );
        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->F = setPointByMirrow( Map->BaseMap.Glob->F, Map->Z, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->F = setPointAndRotate( Map->Z, Map->BaseMap.Glob->L, Degree );
        }

        printf("\nBerechnete Grad von SZF ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von SZF ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );
    }
    else
    {
        degree2Rad( Tmp,  Map->BaseMap.Glob->HK, Round );
        mpfr_cos( Tmp, Tmp, Round );
        mpfr_mul_ui( Tmp, Tmp, 5, Round );
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LK, 2, Round );

        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->Y = setPointAndRotate( Map->BaseMap.Glob->K, Map->BaseMap.Glob->L, Degree );

        printf("\nBerechnete Grad von VKY ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von VKY ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );

        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->V = setPointByMirrow( Map->BaseMap.Glob->Y, Map->BaseMap.Glob->K, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->V = setPointAndRotate( Map->BaseMap.Glob->K, Map->BaseMap.Glob->L, Degree );
        }

        degree2Rad( Tmp,  Map->BaseMap.Glob->HG, Round );
        mpfr_cos( Tmp, Tmp, Round );
        mpfr_mul_ui( Tmp, Tmp, 5, Round );
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LG, 2, Round );

        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->T = setPointAndRotate( Map->BaseMap.Glob->G, Map->BaseMap.Glob->L, Degree );
        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->U = setPointByMirrow( Map->BaseMap.Glob->T, Map->BaseMap.Glob->G, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->U = setPointAndRotate( Map->BaseMap.Glob->G, Map->BaseMap.Glob->L, Degree );
        }

        printf("\nBerechnete Grad von TGU ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von TGU ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );

        mpfr_set_ui( Tmp, 5, Round );
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LH, 2, Round );

        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->I = setPointAndRotate( Map->BaseMap.Glob->H, Map->BaseMap.Glob->L, Degree );
        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->J = setPointByMirrow( Map->BaseMap.Glob->I, Map->BaseMap.Glob->H, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->J = setPointAndRotate( Map->BaseMap.Glob->H, Map->BaseMap.Glob->L, Degree );
        }

        printf("\nBerechnete Grad von IHJ ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von IHJ ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );

        degree2Rad( Tmp,  Map->BaseMap.Glob->HZ, Round );
        mpfr_cos( Tmp, Tmp, Round );
        mpfr_mul_ui( Tmp, Tmp, 5, Round );
        mpfr_mul_si( Diameter, Map->BaseMap.Glob->LZ, 2, Round );

        mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
        mpfr_mul_ui( Radians, Radians, LAENGEN_FAKTOR, Round );
        getDegreeOnCircle( Degree, Radians, Diameter, Round );

        Map->BaseMap.Glob->S = setPointAndRotate( Map->Z, Map->BaseMap.Glob->L, Degree );
        if( TRUE == Mirrow.V )
        {
            Map->BaseMap.Glob->F = setPointByMirrow( Map->BaseMap.Glob->S, Map->Z, MIRROW_X );
        }
        else
        {
            /* Flip */
            mpfr_mul_si(Degree, Degree, -1, Round);
            Map->BaseMap.Glob->F = setPointAndRotate( Map->Z, Map->BaseMap.Glob->L, Degree );
        }

        printf("\nBerechnete Grad von SZF ist: ");
        mpfr_out_str ( stdout, 10, 0, Degree, Round );
        printf("\nBerechneter Radiant von SZF ist: ");
        mpfr_out_str ( stdout, 10, 0, Radians, Round );
    }

    printf("\nY ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->Y->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->Y->Y, Round );

    printf("\nV ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->V->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->V->Y, Round );

    printf("\nT ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->T->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->T->Y, Round );

    printf("\nU ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->U->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->U->Y, Round );

    printf("\nI ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->I->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->I->Y, Round );

    printf("\nJ ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->J->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->J->Y, Round );

    printf("\nS ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->S->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->S->Y, Round );

    printf("\nF ist vorortet bei:");
    printf("\n\tX:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->F->X, Round );
    printf("\n\tY:");
    mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->F->Y, Round );

    /* Housekeeping */
    mpfr_clear( Tmp );
    mpfr_clear( Diameter );
    mpfr_clear( Degree );
    mpfr_clear( Radians );

}

unsigned int globFilterYFindStart( Point** Buffer )
{
    unsigned int Index;
    for( Index = 0; Index < Sizes[1]; Index++ )
    {
        if( 0 == mpfr_nan_p( Buffer[ Index ]->X ) && 0 == mpfr_nan_p( Buffer[ Index ]->Y ) )
        {
            return Index;
        }
    }

    return Sizes[1];
}

unsigned int globFilterYFindEnd( Point** Buffer )
{
    unsigned int Index, Minimizer;
    for( Index = Sizes[1]-1, Minimizer=0; Index>=0; Minimizer++, Index-- )
    {
        if( 0 == mpfr_nan_p( Buffer[ Index ]->X ) && 0 == mpfr_nan_p( Buffer[ Index ]->Y ) )
        {
            return Minimizer;
        }
    }

    return Sizes[1];
}

void* globGridInner( void* Helper )
{
    unsigned int LaengenIndex, Factor2;
    mpfr_t Diameter, Radians, Degree, Tmp, Tmp2;

    mpfr_init( Tmp );
    mpfr_init( Tmp2 );
    mpfr_init( Diameter );
    mpfr_init( Degree );
    mpfr_init( Radians );

    mpfr_abs( Tmp2, Paralellkreise[
            ( ( ThreadHelper *) Helper) ->BreitenIndex
            ][ 2 ], Round );
    mpfr_div_ui( Tmp2, Tmp2, 60*60, Round );
    mpfr_abs( Tmp, Paralellkreise[
            ( ( ThreadHelper *) Helper )->BreitenIndex
            ][ 1 ], Round );
    mpfr_div_ui( Tmp, Tmp, 60, Round );
    mpfr_add( Tmp2, Tmp, Tmp2, Round );
    mpfr_abs( Tmp, Paralellkreise[
            ( ( ThreadHelper *) Helper )->BreitenIndex
            ][ 0 ], Round );
    mpfr_add( Tmp, Tmp, Tmp2, Round );

    degree2Rad( Tmp, Tmp, Round );
    mpfr_cos( Tmp, Tmp, Round );
    mpfr_div_d( Tmp, Tmp, ( ( ThreadHelper *) Helper )->Factor, Round );

    mpfr_mul_si( Diameter, Map->Breiten[
            ( ( ThreadHelper *) Helper) ->BreitenIndex
            ], 2, Round );

    mpfr_mul( Radians, Map->BaseMap.Glob->Einheit, Tmp, Round );
    Factor2 = ( ( ThreadHelper *) Helper)->Offset;
    for( LaengenIndex = 0; LaengenIndex<=( ( ThreadHelper *) Helper )->Offset; LaengenIndex++ )
    {
        Factor2 = computeLaengenAtBreite(
                ( ( ThreadHelper *) Helper)->Buffer.Value,
                Map->BreitenPunkte[ ( ( ThreadHelper *) Helper )->BreitenIndex ],
                Map->BaseMap.Glob->L,
                &Radians,
                &Diameter,
                LaengenIndex,
                Factor2,
                ( ( ThreadHelper *) Helper )->Offset
        );
    }

    mpfr_clear( Tmp );
    mpfr_clear( Tmp2 );
    mpfr_clear( Diameter );
    mpfr_clear( Degree );
    mpfr_clear( Radians );
    mpfr_free_cache();
    return NULL;
}

void globGrid()
{
    unsigned int BreitenIndex;
    ThreadHelper Helper;

    Helper.Buffer.Start = -1;
    Helper.Buffer.End = -1;

    Sizes[1] = ( LAENGEN_FAKTOR<<1 ) * UseFactor;
    Sizes[1] += 1;//+Aequator

    Helper.Buffer.Value = (Point**)malloc(Sizes[1] * sizeof( Point* ));
    if( NULL == Helper.Buffer.Value )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    Helper.Factor = (double)UseFactor/5;
    Helper.Offset = LAENGEN_FAKTOR*UseFactor;

    for( BreitenIndex = 0; BreitenIndex < Sizes[0]; BreitenIndex++ )
    {
        Helper.BreitenIndex = BreitenIndex;
        globGridInner( &Helper );
        if( -1 == Helper.Buffer.Start )
        {
            Helper.Buffer.Start = globFilterYFindStart( Helper.Buffer.Value );
        }

        if( -1 == Helper.Buffer.End )
        {
            Helper.Buffer.End = Sizes[1]-globFilterYFindEnd( Helper.Buffer.Value );
        }

        outputPoints( &Helper.Buffer );
    }

    free( Helper.Buffer.Value );
}

void globGridThreaded()
{
    Sizes[1] = ( LAENGEN_FAKTOR<<1 ) * UseFactor;
    Sizes[1] += 1;//+Aequator
    intersectionsThreaded(
            &globGridInner,
            TRUE,
            (double)UseFactor/5,
            LAENGEN_FAKTOR*UseFactor,
            NULL
    );
}

void globPrintBreitenPunkte()
{
    printf("\n\tlot: { ");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tL: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->L->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->L->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tK: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->K->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->K->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tG: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->G->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->G->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tH: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->H->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->H->Y, Round);
        printf("\n\t\t},");
        printf("\n\t\tstart: 'L'");
    }
    printf("\n\t},");
}

void globPrintDistances()
{
    printf("\n\tdistances: {");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tLZ:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LZ, Round );
        printf(",\n\t\tLH:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LH, Round );
        printf(",\n\t\tLG:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LG, Round );
        printf(",\n\t\tLK:");
        mpfr_out_str ( stdout, 10, 0, Map->BaseMap.Glob->LK, Round );
    }
    printf("\n\t},");
}

void globPrintLimits()
{
    printf("\n\tlimits: {");
    if( TRUE == Verbose.V )
    {
        printf("\n\t\tP: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->P->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->P->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tR: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->R->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->R->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tY: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->Y->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->Y->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tV: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->V->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->V->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tX: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->X->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->X->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tO: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->O->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->O->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tT: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->T->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->T->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tU: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->U->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->U->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tI: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->I->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->I->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tJ: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->J->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->J->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tM: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->M->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->M->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tN: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->N->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->N->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tS: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->S->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->S->Y, Round);
        printf("\n\t\t},");

        printf("\n\t\tF: {\n\t\t\tX: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->F->X, Round);
        printf(",\n\t\t\tY: ");
        mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->F->Y, Round);
        printf("\n\t\t},");
    }
    printf("\n\t},");
}

void globPrint()
{
    printDesk();
    printf( "\n\teinheit: " );
    mpfr_out_str(stdout, 10, 0, Map->BaseMap.Glob->Einheit, Round);
    printf( "," );
    printBreitenDistances();
    globPrintDistances();
    printBreiten();
    globPrintBreitenPunkte();
    globPrintLimits();
}

void globHouskeeping()
{
    houseKeeping();

    mpfr_clear( Map->BaseMap.Glob->Einheit );
    freePoint( Map->BaseMap.Glob->L );
    /* Redundanzpunkte auf dem Lot H~CD */
    if( TRUE == Verbose.V )
    {
        freePoint( Map->BaseMap.Glob->K );
        freePoint( Map->BaseMap.Glob->G );
        freePoint( Map->BaseMap.Glob->H );
        freePoint( Map->BaseMap.Glob->P );
        freePoint( Map->BaseMap.Glob->X );
        freePoint( Map->BaseMap.Glob->M );
        freePoint( Map->BaseMap.Glob->Y );
        freePoint( Map->BaseMap.Glob->T );
        freePoint( Map->BaseMap.Glob->S );
        freePoint( Map->BaseMap.Glob->R );
        freePoint( Map->BaseMap.Glob->O );
        freePoint( Map->BaseMap.Glob->N );
        freePoint( Map->BaseMap.Glob->V );
        freePoint( Map->BaseMap.Glob->U );
        freePoint( Map->BaseMap.Glob->F );
        freePoint( Map->BaseMap.Glob->I );
        freePoint( Map->BaseMap.Glob->J );
        mpfr_clear( Map->BaseMap.Glob->LK );
        mpfr_clear( Map->BaseMap.Glob->HK );
        mpfr_clear( Map->BaseMap.Glob->LG );
        mpfr_clear( Map->BaseMap.Glob->HG );
    }
    mpfr_clear( Map->BaseMap.Glob->HZ );
    mpfr_clear( Map->BaseMap.Glob->LH );
    mpfr_clear( Map->BaseMap.Glob->LZ );
    mpfr_free_cache();
    free( Map->BaseMap.Glob );
}
/*----------------------------- MAIN -----------------------------*/

int getAddionalCoordinateCount( int HardWired[PARALELLKREISE][3] )
{
    unsigned int Index;
    int Additional = 0;

    if( DEGREEX == Use )
    {
        for( Index = 0; Index < PARALELLKREISE; Index++ )
        {
            if( 0 != HardWired[Index][2] || 0 != HardWired[Index][1] )
            {
                Additional++;
            }
        }
    }

    return Additional;
}

void setBaseData()
{
    HardWired[0][0] = 0;
    HardWired[0][1] = 0;
    HardWired[0][2] = 0;

    HardWired[1][0] = 4;
    HardWired[1][1] = 15;
    HardWired[1][2] = 0;

    HardWired[2][0] = 8;
    HardWired[2][1] = 25;
    HardWired[2][2] = 0;

    HardWired[3][0] = 12;
    HardWired[3][1] = 30;
    HardWired[3][2] =  0;

    HardWired[4][0] = 16;
    HardWired[4][1] = 25;
    HardWired[4][2] = 0;

    HardWired[5][0] = 20;
    HardWired[5][1] = 15;
    HardWired[5][2] = 0;

    HardWired[6][0] = 23;
    HardWired[6][1] = 50;
    HardWired[6][2] = 0;

    HardWired[7][0] = 27;
    HardWired[7][1] = 10;
    HardWired[7][2] = 0;

    HardWired[8][0] = 30;
    HardWired[8][1] = 20;
    HardWired[8][2] = 0;

    HardWired[9][0] = 33;
    HardWired[9][1] = 20;
    HardWired[9][2] = 0;

    HardWired[10][0] = 36;
    HardWired[10][1] = 0;
    HardWired[10][2] = 0;

    HardWired[11][0] = 38;
    HardWired[11][1] = 35;
    HardWired[11][2] = 0;

    HardWired[12][0] = 40;
    HardWired[12][1] = 55;
    HardWired[12][2] = 0;

    HardWired[13][0] = 43;
    HardWired[13][1] = 5;
    HardWired[13][2] = 0;

    HardWired[14][0] = 45;
    HardWired[14][1] = 0;
    HardWired[14][2] = 0;

    HardWired[15][0] = 48;
    HardWired[15][1] = 30;
    HardWired[15][2] = 0;

    HardWired[16][0] = 51;
    HardWired[16][1] = 30;
    HardWired[16][2] = 0;

    HardWired[17][0] = 54;
    HardWired[17][1] = 0;
    HardWired[17][2] = 0;

    HardWired[18][0] = 56;
    HardWired[18][1] = 0;
    HardWired[18][2] = 0;

    HardWired[19][0] = 58;
    HardWired[19][1] = 0;
    HardWired[19][2] = 0;

    HardWired[20][0] = 61;
    HardWired[20][1] = 0;
    HardWired[20][2] = 0;

    HardWired[21][0] = 63;
    HardWired[21][1] = 0;
    HardWired[21][2] = 0;

    HardWired[22][0] = -8;
    HardWired[22][1] = -25;
    HardWired[22][2] = 0;

    HardWired[23][0] = -16;
    HardWired[23][1] = -25;
    HardWired[23][2] = 0;
}

void setParalellkreise()
{
    int Internal, Border, Additional, Index, InnerIndex, OuterIndex;
    int Part[3];

    Border = 63;
	if( DEGREE == Use || DEGREEX == Use)
	{
        Internal = 80;// add 16°25'
        Sizes[0] = Internal;
	}
	else if( MINUTES == Use )
	{
        Sizes[0] = Internal = 78*60+26;// add 16°25'
	}
	else if( SECONDS == Use )
	{
        Sizes[0] = Internal = 78*60*60+26*60;// add 16°25'
	}
	else
	{
        Sizes[0] = PARALELLKREISE;
	}

	if( DEGREEX == Use )
    {
        Sizes[0] += getAddionalCoordinateCount( HardWired );
    }

    if( DEGREE == Use )
    {
        Sizes[0]++;//+Begrenzung
    }

    Paralellkreise = (mpfr_t**)malloc(sizeof(mpfr_t*)*Sizes[0]);
    if( NULL == Paralellkreise )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    if (NORMAL == Use )
    {
        InnerIndex = 0;
        for( Index = PARALELLKREISE-1; 0 <= Index; Index--)
        {
            if( 0 > HardWired[Index][0] )
            {
                Paralellkreise[InnerIndex] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                if (NULL == Paralellkreise[InnerIndex])
                {
                    errorAndOut("\nSomethings wrong with the memory, Jim.");
                }

                mpfr_init_set_si(Paralellkreise[InnerIndex][0], HardWired[Index][0], Round);
                mpfr_init_set_si(Paralellkreise[InnerIndex][1], HardWired[Index][1], Round);
                mpfr_init_set_si(Paralellkreise[InnerIndex][2], HardWired[Index][2], Round);
                InnerIndex++;
            }
            else
            {
                break;
            }
        }

        for( Index = 0; PARALELLKREISE > Index; Index++)
        {
            if( 0 <= HardWired[Index][0] )
            {
                Paralellkreise[InnerIndex] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                if (NULL == Paralellkreise[InnerIndex])
                {
                    errorAndOut("\nSomethings wrong with the memory, Jim.");
                }

                mpfr_init_set_si(Paralellkreise[InnerIndex][0], HardWired[Index][0], Round);
                mpfr_init_set_si(Paralellkreise[InnerIndex][1], HardWired[Index][1], Round);
                mpfr_init_set_si(Paralellkreise[InnerIndex][2], HardWired[Index][2], Round);
                InnerIndex++;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        Part[0] = 0;
        Part[1] = 0;
        Part[2] = 0;
        Additional = 0;
        OuterIndex = 0;

        Paralellkreise[ 0 ] = (mpfr_t *) malloc( sizeof(mpfr_t) * 3);
        if (NULL == Paralellkreise[0])
        {
            errorAndOut("\nSomethings wrong with the memory, Jim.");
        }

        mpfr_set_zero( Paralellkreise[0][0], 1 );// Grad
        mpfr_set_zero( Paralellkreise[0][1], 1 );// Minuten
        mpfr_set_zero( Paralellkreise[0][2], 1 );// Sekunden

        for( Index = 1; Sizes[0] > Index; Index++ )
        {
            if( Part[0] >= Border )
            {
                Additional = Index;
                break;
            }

            Paralellkreise[Index] = (mpfr_t *) malloc( sizeof(mpfr_t) * 3 );
            if (NULL == Paralellkreise[Index])
            {
                errorAndOut("\nSomethings wrong with the memory, Jim.");
            }

            if( DEGREE == Use || DEGREEX == Use )
            {
                Part[0]++;
            }
            else if( MINUTES == Use )
            {
                Part[1]++;
                if( 60 == Part[1] )
                {
                    Part[1] = 0;
                    Part[0]++;
                }
            }
            else
            {
                Part[2]++;
                if( 60 == Part[2] )
                {
                    Part[2] = 0;
                    Part[1]++;
                    if( 60 == Part[1] )
                    {
                        Part[1] = 0;
                        Part[0]++;
                    }
                }
            }

            mpfr_init_set_si(Paralellkreise[Index][0], Part[0], Round );// Grad
            mpfr_init_set_si(Paralellkreise[Index][1], Part[1], Round );// Minuten
            mpfr_init_set_si(Paralellkreise[Index][2], Part[2], Round );// Sekunden

            if( DEGREEX == Use )
            {
                for( InnerIndex = 0; InnerIndex < PARALELLKREISE; InnerIndex++ )
                {
                    if( HardWired[InnerIndex][0] == Part[0] )
                    {
                        if( 0 != HardWired[InnerIndex][1] )
                        {
                            Index++;

                            Paralellkreise[Index] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                            if (NULL == Paralellkreise[Index])
                            {
                                errorAndOut("\nSomethings wrong with the memory, Jim.");
                            }

                            mpfr_init_set_si(Paralellkreise[Index][0], HardWired[InnerIndex][0], Round );// Grad
                            mpfr_init_set_si(Paralellkreise[Index][1], HardWired[InnerIndex][1], Round );// Minuten
                            mpfr_init_set_si(Paralellkreise[Index][2], HardWired[InnerIndex][2], Round );// Sekunden
                        }
                    }

                    if( HardWired[InnerIndex][0] > Part[0] )
                    {
                        break;
                    }
                }
            }
            OuterIndex++;
        }

        Additional--;

        InnerIndex = Sizes[0]-1;
        for( Index = Additional; Index >= 0; Index-- )
        {
            Paralellkreise[InnerIndex] = Paralellkreise[Index];
            InnerIndex--;
        }

        if( MINUTES == Use || SECONDS == Use )
        {
            Part[0] = -16;
            Part[1] = -25;
            Part[2] = 0;
        }
        else
        {
            Part[0] = -16;
            Part[1] = 0;
            Part[2] = 0;
        }

        InnerIndex = 0;
        free( Paralellkreise[0] );
        for( Index = 0; Index < Sizes[0]-Additional; Index++ )
        {
            Paralellkreise[Index] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
            if (NULL == Paralellkreise[Index])
            {
                errorAndOut("\nSomethings wrong with the memory, Jim.");
            }

            if( 0 == Index && DEGREE == Use )
            {
                mpfr_init_set_si(Paralellkreise[Index][0], HardWired[PARALELLKREISE-1][0], Round );// Grad
                mpfr_init_set_si(Paralellkreise[Index][1], HardWired[PARALELLKREISE-1][1], Round );// Minuten
                mpfr_init_set_si(Paralellkreise[Index][2], HardWired[PARALELLKREISE-1][2], Round );// Sekunden
                Index++;

                Paralellkreise[Index] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                if (NULL == Paralellkreise[Index])
                {
                    errorAndOut("\nSomethings wrong with the memory, Jim.");
                }
            }

            if( DEGREEX == Use )
            {
                for( InnerIndex = PARALELLKREISE-1; InnerIndex>=0; InnerIndex-- )
                {
                    if( HardWired[InnerIndex][0] == Part[0] )
                    {
                        if( 0 != HardWired[InnerIndex][1] )
                        {
                            mpfr_init_set_si(Paralellkreise[Index][0], HardWired[InnerIndex][0], Round );// Grad
                            mpfr_init_set_si(Paralellkreise[Index][1], HardWired[InnerIndex][1], Round );// Minuten
                            mpfr_init_set_si(Paralellkreise[Index][2], HardWired[InnerIndex][2], Round );// Sekunden

                            Index++;
                            Paralellkreise[Index] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                            if (NULL == Paralellkreise[Index])
                            {
                                errorAndOut("\nSomethings wrong with the memory, Jim.");
                            }
                        }
                    }

                    if( HardWired[InnerIndex][0] > Part[0] )
                    {
                        break;
                    }
                }
            }

            mpfr_init_set_si(Paralellkreise[Index][0], Part[0], Round );// Grad
            mpfr_init_set_si(Paralellkreise[Index][1], Part[1], Round );// Minuten
            mpfr_init_set_si(Paralellkreise[Index][2], Part[2], Round );// Sekunden

            if( DEGREE == Use || DEGREEX == Use )
            {
                Part[0]++;
            }
            else if( MINUTES == Use )
            {
                Part[1]++;
                if( 1 == Part[1] )
                {
                    Part[1] = -59;
                    Part[0]++;
                }
            }
            else
            {
                Part[2]++;
                if( 1 == Part[2] )
                {
                    Part[2] = -59;
                    Part[1]++;
                    if( 1 == Part[1] )
                    {
                        Part[1] = -59;
                        Part[0]++;
                    }
                }
            }
        }
    }

	if( TRUE == Verbose.V )
	{		
		for( Index = 0; Index < Sizes[ 0 ]; Index++ )
		{
			printf("\nDistanze des Paralellkreises%d betraegt: ", Index+1 );
            fflush( stdout );
			mpfr_out_str ( stdout, 10, 0, Paralellkreise[Index][0], Round );
			printf(" Grad ");
			mpfr_out_str ( stdout, 10, 0, Paralellkreise[Index][1], Round );
			printf(" Minuten und ");
			mpfr_out_str ( stdout, 10, 0, Paralellkreise[Index][2], Round );
			printf(" Sekunden");
			fflush( stdout );
		}
	}

	filterX();
}

void filterX()
{
    mpfr_t Start[3];
    mpfr_t End[3];
    mpfr_t** Tmp;
    unsigned short Started = TRUE;
    unsigned short Ended = FALSE;
    unsigned int Index, InnerIndex;

    if( FILTER_X_NO == XFilter )
    {
        return;
    }

    mpfr_init( Start[0] );
    mpfr_init( Start[1] );
    mpfr_init( Start[2] );

    mpfr_init( End[0] );
    mpfr_init( End[1] );
    mpfr_init( End[2] );

    if( 0 != ( XFilter&FILTER_X_START ) )
    {
        Started = FALSE;
        mpfr_set_si( Start[0], FilterX[0][0], Round );
        mpfr_set_si( Start[1], FilterX[0][1], Round );
        mpfr_set_si( Start[2], FilterX[0][2], Round );
    }
    else
    {
        mpfr_set_si( Start[0], HardWired[0][0], Round );
        mpfr_set_si( Start[1], HardWired[0][1], Round );
        mpfr_set_si( Start[2], HardWired[0][2], Round );
    }

    if( 0 !=  ( XFilter&FILTER_X_END ) )
    {
        mpfr_set_si( End[0], FilterX[1][0], Round );
        mpfr_set_si( End[1], FilterX[1][1], Round );
        mpfr_set_si( End[2], FilterX[1][2], Round );
    }
    else
    {
        mpfr_set_si( End[0], HardWired[PARALELLKREISE-1][0], Round );
        mpfr_set_si( End[1], HardWired[PARALELLKREISE-1][1], Round );
        mpfr_set_si( End[2], HardWired[PARALELLKREISE-1][2], Round );
    }

    if( FILTER_X_RANGE == XFilter )
    {
        if(
            FilterX[0][0] == FilterX[1][0]
        &&
            FilterX[0][1] == FilterX[1][1]
        &&
            FilterX[0][2] == FilterX[1][2]
        )
        {
            free(Paralellkreise);
            Sizes[0] = 1;
            Paralellkreise = (mpfr_t **)malloc(1*sizeof(mpfr_t*));
            if (NULL == Paralellkreise)
            {
                errorAndOut("\nSomethings wrong with the memory, Jim.");
            }

            Paralellkreise[0] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
            if (NULL == Paralellkreise[0])
            {
                errorAndOut("\nSomethings wrong with the memory, Jim.");
            }
            mpfr_init_set( Paralellkreise[0][0], Start[0], Round );
            mpfr_init_set( Paralellkreise[0][1], Start[1], Round );
            mpfr_init_set( Paralellkreise[0][2], Start[2], Round );
            printf("\nDistanze des gefilterten Paralellkreises1 betraegt: " );
            mpfr_out_str ( stdout, 10, 0, Paralellkreise[0][0], Round );
            fflush( stdout );
            printf(" Grad ");
            mpfr_out_str ( stdout, 10, 0, Paralellkreise[0][1], Round );
            printf(" Minuten und ");
            mpfr_out_str ( stdout, 10, 0, Paralellkreise[0][2], Round );
            printf(" Sekunden");
            fflush( stdout );
            return;
        }
    }

    Tmp = (mpfr_t**)malloc(sizeof(mpfr_t*)*Sizes[0]);
    if( NULL == Tmp )
    {
        errorAndOut( "\nSomethings wrong with the memory, Jim." );
    }

    InnerIndex = 0;
    for( Index = 0; Index < Sizes[0]; Index++ )
    {
        if( FALSE == Started )
        {
            if(
                0 == mpfr_cmp( Start[0], Paralellkreise[Index][0] )
            &&
                0 == mpfr_cmp( Start[1], Paralellkreise[Index][1] )
            &&
                0 == mpfr_cmp( Start[2], Paralellkreise[Index][2] )
            )
            {
                Started = TRUE;
                Tmp[InnerIndex] = Paralellkreise[Index];
                InnerIndex++;
            }
            else if(
                (
                    0 == mpfr_cmp( Start[0], Paralellkreise[Index][0] )
                &&
                    0 == mpfr_cmp( Start[1], Paralellkreise[Index][1] )
                &&
                    0 > mpfr_cmp( Start[2], Paralellkreise[Index][2] )
                )
            ||
                (
                    0 == mpfr_cmp( Start[0], Paralellkreise[Index][0] )
                &&
                    0 > mpfr_cmp( Start[1], Paralellkreise[Index][1] )
                )
            ||
                (
                    0 > mpfr_cmp( Start[0], Paralellkreise[Index][0] )
                )
            )
            {
                Started = TRUE;
                Tmp[InnerIndex] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                if (NULL == Tmp[InnerIndex])
                {
                    errorAndOut("\nSomethings wrong with the memory, Jim.");
                }
                mpfr_init_set( Tmp[InnerIndex][0], Start[0], Round );
                mpfr_init_set( Tmp[InnerIndex][1], Start[1], Round );
                mpfr_init_set( Tmp[InnerIndex][2], Start[2], Round );
                InnerIndex++;
                Started = TRUE;
                Tmp[InnerIndex] = Paralellkreise[Index];
                InnerIndex++;
            }
            else
            {
                mpfr_clear(Paralellkreise[Index][0]);
                mpfr_clear(Paralellkreise[Index][1]);
                mpfr_clear(Paralellkreise[Index][2]);
                mpfr_free_cache();
                free(Paralellkreise[Index]);
            }
        }
        else
        {
            if( FALSE == Ended )
            {
                if(
                    0 == mpfr_cmp( End[0], Paralellkreise[Index][0] )
                &&
                    0 == mpfr_cmp( End[1], Paralellkreise[Index][1] )
                &&
                    0 == mpfr_cmp( End[2], Paralellkreise[Index][2] )
                )
                {
                    Ended = TRUE;
                    Tmp[InnerIndex] = Paralellkreise[Index];
                    InnerIndex++;
                }
                else if(
                    (
                        0 == mpfr_cmp( End[0], Paralellkreise[Index][0] )
                    &&
                        0 == mpfr_cmp( End[1], Paralellkreise[Index][1] )
                    &&
                        0 > mpfr_cmp( End[2], Paralellkreise[Index][2] )
                    )
                ||
                    (
                        0 == mpfr_cmp( End[0], Paralellkreise[Index][0] )
                    &&
                        0 > mpfr_cmp( End[1], Paralellkreise[Index][1] )
                    )
                ||
                    (
                        0 > mpfr_cmp( End[0], Paralellkreise[Index][0] )
                    )
                )
                {
                    Ended = TRUE;
                    Tmp[InnerIndex] = (mpfr_t *) malloc(sizeof(mpfr_t) * 3);
                    if (NULL == Tmp[InnerIndex])
                    {
                        errorAndOut("\nSomethings wrong with the memory, Jim.");
                    }
                    mpfr_init_set( Tmp[InnerIndex][0], End[0], Round );
                    mpfr_init_set( Tmp[InnerIndex][1], End[1], Round );
                    mpfr_init_set( Tmp[InnerIndex][2], End[2], Round );
                    InnerIndex++;
                    mpfr_clear(Paralellkreise[Index][0]);
                    mpfr_clear(Paralellkreise[Index][1]);
                    mpfr_clear(Paralellkreise[Index][2]);
                    mpfr_free_cache();
                    free(Paralellkreise[Index]);
                }
                else
                {
                    Tmp[InnerIndex] = Paralellkreise[Index];
                    InnerIndex++;
                }

            }
            else
            {
                mpfr_clear(Paralellkreise[Index][0]);
                mpfr_clear(Paralellkreise[Index][1]);
                mpfr_clear(Paralellkreise[Index][2]);
                mpfr_free_cache();
                free(Paralellkreise[Index]);
            }
        }
    }

    free( Paralellkreise );
    Sizes[0] = InnerIndex;

    Paralellkreise = (mpfr_t **)malloc(Sizes[0]*sizeof(mpfr_t*));
    if (NULL == Paralellkreise)
    {
        errorAndOut("\nSomethings wrong with the memory, Jim.");
    }

    for( Index = 0; Index<Sizes[0]; Index++ )
    {
        Paralellkreise[ Index ] = Tmp[Index];
    }
    free( Tmp );

    if( TRUE == Verbose.V )
    {
        for( Index = 0; Index < Sizes[ 0 ]; Index++ )
        {
            printf("\nDistanze des gefilterten Paralellkreises%d betraegt: ", Index+1 );
            mpfr_out_str ( stdout, 10, 0, Paralellkreise[Index][0], Round );
            fflush( stdout );
            printf(" Grad ");
            mpfr_out_str ( stdout, 10, 0, Paralellkreise[Index][1], Round );
            printf(" Minuten und ");
            mpfr_out_str ( stdout, 10, 0, Paralellkreise[Index][2], Round );
            printf(" Sekunden");
            fflush( stdout );
        }
    }

    /* Housekeeping */
    mpfr_clear( Start[0] );
    mpfr_clear( Start[1] );
    mpfr_clear( Start[2] );

    mpfr_clear( End[0] );
    mpfr_clear( End[1] );
    mpfr_clear( End[2] );
    mpfr_free_cache();
}

unsigned short validateFilter( char* FilterString, int Filter[3] )
{
    unsigned short IsSigned;
    char Parts[3][3];//2+\0
    char* Part;
    char* End;
    int Index, OuterIndex;

    Parts[0][0] = '0';
    Parts[0][1] = '\0';
    Parts[1][0] = '0';
    Parts[1][1] = '\0';
    Parts[2][0] = '0';
    Parts[2][1] = '\0';
    IsSigned = FALSE;
    Index = 0;

    if( '-' == FilterString[Index] )
    {
        IsSigned=TRUE;
    }

    OuterIndex = 0;
    while( TRUE )
    {

        if( 3 == OuterIndex || '\0' == FilterString[Index] || Index >= strlen( FilterString ) )
        {
            break;
        }

        if( '-' == FilterString[Index] || '+' == FilterString[Index] )
        {
            Index++;
        }

        Part = substring( FilterString, Index, 2 );
        if( FALSE == isDigit( Part[ 0 ], FALSE ) )
        {
            return FALSE;
        }
        Index++;

        if(FALSE == isDigit( Part[ 1 ], FALSE ) )
        {
            if( '\0' == FilterString[Index] )
            {
                Part[ 1 ] = '\0';
                strcpy( Parts[ OuterIndex ], Part );
                break;
            }

            if( ',' != Part[ 1 ] && '\0' != Part[ 1 ] )
            {
                return FALSE;
            }
            else
            {
                Part[ 1 ] = '\0';
            }
        }
        else
        {
            Index++;
            if( '\0' != FilterString[ Index ] )
            {
                if (',' != FilterString[Index])
                {
                    return FALSE;
                }
                Index++;
            }
        }

        strcpy( Parts[ OuterIndex ], Part );
        free(Part);
        OuterIndex++;
    }

    Filter[0] = (int) strtol( Parts[0], &End, 10 );
    Filter[1] = (int) strtol( Parts[1], &End, 10 );
    Filter[2] = (int) strtol( Parts[2], &End, 10 );
    if( TRUE == IsSigned )
    {
        Filter[0] *= -1;
        Filter[1] *= -1;
        Filter[2] *= -1;
    }
    return TRUE;
}

int main( int ArgC, char* Arguments[] )
{
    int Index, X, Y, Precision;
    Bool UseThreads;
    char* ThreadEnd;
    pthread_t PlanimetrikThread;

    X = 0;
    Y = 0;
    Precision = 0;

    UsePlan.V = TRUE;
    UseThreads.V = FALSE;
    Verbose.V = FALSE;
    Mirrow.V = FALSE;
    Round = MPFR_RNDZ;
    Use = NORMAL;
    MaxThreads = MAX_THREADS;
    Org.V = FALSE;
    MemoryBoost = 1;
    KillDaemon = FALSE;

    XFilter = FILTER_X_NO;
    YFilter = FILTER_Y_NO;

    FilterY[0][0] = Y_MIN_G;
    FilterY[0][1] = Y_MIN_M;
    FilterY[0][2] = Y_MIN_S;

    FilterY[1][0] = Y_MAX_G;
    FilterY[1][1] = Y_MAX_M;
    FilterY[1][2] = Y_MAX_S;

    setBaseData();

    if(
        1 == ArgC
    ||
        (
            2 < ArgC &&
            (
                ( 0 == strcmp( "--addx", Arguments[ ArgC-2 ] ) || 0 == strcmp( "-ax", Arguments[ ArgC-2 ] ) )
            ||
                ( 0 == strcmp( "--addy", Arguments[ ArgC-2 ] ) || 0 == strcmp( "-ay", Arguments[ ArgC-2 ] ) )
            ||
                ( 0 == strcmp( "--precision", Arguments[ ArgC-2 ] ) || 0 == strcmp( "-p", Arguments[ ArgC-2 ] ) )
            ||
                ( 0 == strcmp( "---max-threads", Arguments[ ArgC-2 ] ) || 0 == strcmp( "-mt", Arguments[ ArgC-2 ] ) )
            ||
                ( 0 == strcmp( "--filter-x-start", Arguments[ ArgC-2 ] ) || 0 == strcmp( "-fsx", Arguments[ ArgC-2 ] ) )
            ||
                ( 0 == strcmp( "--filter-x-end", Arguments[ ArgC-2 ] ) || 0 == strcmp( "-fxe", Arguments[ ArgC-2 ] ) )
            )
        )
    )
    {
        errorAndOut( "\nEs wurde keine valide Kantenlaenge gegeben." );
    }

    if( FALSE == validateString( Arguments[ ArgC-1 ], FALSE, TRUE ) )
    {
        errorAndOut( "\nDie gebene Kantenlaenge ist kein valider Wert." );
    }

    for( Index = 0; Index < ArgC; Index++ )
    {
        if( 0 == strcmp( "--globus", Arguments[ Index ] ) || 0 == strcmp( "-g", Arguments[ Index ] ) )
        {
            UsePlan.V = FALSE;
        }

        if( 0 == strcmp( "--verbose", Arguments[ Index ] ) || 0 == strcmp( "-v", Arguments[ Index ] ) )
        {
            Verbose.V = TRUE;
        }

        if( 0 == strcmp( "--addx", Arguments[ Index ] ) || 0 == strcmp( "-ax", Arguments[ Index ] ) )
        {
            if( FALSE == validateString( Arguments[ Index+1 ], TRUE, TRUE ) )
            {
                errorAndOut( "\nDer Parameter --addx ist fehlerhaft." );
            }
            X = Index+1;
        }

        if( 0 == strcmp( "--addy", Arguments[ Index ] ) || 0 == strcmp( "-ay", Arguments[ Index ] ) )
        {
            if( FALSE == validateString( Arguments[ Index+1 ], TRUE, TRUE ) )
            {
                errorAndOut( "\nDer Parameter --addy ist fehlerhaft." );
            }
            Y = Index+1;
        }

        if( 0 == strcmp( "--precision", Arguments[ Index ] ) || 0 == strcmp( "-p", Arguments[ Index ] ) )
        {
            if( FALSE == validateString( Arguments[ Index+1 ], FALSE, FALSE ) )
            {
                errorAndOut( "\nDer Parameter --precision ist fehlerhaft." );
            }
            Precision = Index+1;
        }

        if( 0 == strcmp( "--useRNDN", Arguments[ Index ] ) || 0 == strcmp( "-n", Arguments[ Index ] ) )
        {
            Round = MPFR_RNDN;
        }

        if( 0 == strcmp( "--useRNDZ", Arguments[ Index ] ) || 0 == strcmp( "-z", Arguments[ Index ] ) )
        {
            Round = MPFR_RNDZ;
        }

        if( 0 == strcmp( "--useRNDU", Arguments[ Index ] ) || 0 == strcmp( "-u", Arguments[ Index ] ) )
        {
            Round = MPFR_RNDU;
        }

        if( 0 == strcmp( "--useRNDA", Arguments[ Index ] ) || 0 == strcmp( "-a", Arguments[ Index ] ) )
        {
            Round = MPFR_RNDA;
        }

        if( 0 == strcmp( "--useRNDF", Arguments[ Index ] ) || 0 == strcmp( "-f", Arguments[ Index ] ) )
        {
            Round = MPFR_RNDF;
        }

        if( 0 == strcmp( "--mirrowing", Arguments[ Index ] ) || 0 == strcmp( "-m", Arguments[ Index ] ) )
        {
            Mirrow.V = TRUE;
        }

        if( 0 == strcmp( "--degree", Arguments[ Index ] ) || 0 == strcmp( "-d", Arguments[ Index ] ) )
        {
            Use = DEGREE;
        }

        if( 0 == strcmp( "--degree-ex", Arguments[ Index ] ) || 0 == strcmp( "-dx", Arguments[ Index ] ) )
        {
            Use = DEGREEX;
        }

        if( 0 == strcmp( "--minutes", Arguments[ Index ] ) || 0 == strcmp( "-m", Arguments[ Index ] ) )
        {
            Use = MINUTES;
        }

        if( 0 == strcmp( "--seconds", Arguments[ Index ] ) || 0 == strcmp( "-s", Arguments[ Index ] ) )
        {
            Use = SECONDS;
        }

        if( 0 == strcmp( "--threads", Arguments[ Index ] ) || 0 == strcmp( "-t", Arguments[ Index ] ) )
        {
            UseThreads.V = TRUE;
        }

        if( 0 == strcmp( "--max-threads", Arguments[ Index ] ) || 0 == strcmp( "-mt", Arguments[ Index ] ) )
        {
            if( FALSE == validateString( Arguments[ Index+1 ], FALSE, FALSE ) )
            {
                errorAndOut( "\nDer Parameter --max-threads ist fehlerhaft." );
            }
            MaxThreads = (unsigned int) strtol( Arguments[ Index+1 ], &ThreadEnd, 10 );
            if( THREAD_ERROR <= MaxThreads )
            {
                errorAndOut( "\nDer Parameter --max-threads ist zu groß." );
            }

            if( 2 >= MaxThreads )
            {
                errorAndOut( "\nDer Parameter --max-threads ist zu klein." );
            }
        }

        if( 0 == strcmp( "--memory-boost", Arguments[ Index ] ) || 0 == strcmp( "-mb", Arguments[ Index ] ) )
        {
            if( FALSE == validateString( Arguments[ Index+1 ], FALSE, FALSE ) )
            {
                errorAndOut( "\nDer Parameter --memory-boost ist fehlerhaft." );
            }
            MemoryBoost = (unsigned int) strtol( Arguments[ Index+1 ], &ThreadEnd, 10 );
            if( THREAD_ERROR <= MaxThreads )
            {
                errorAndOut( "\nDer Parameter --memory-boost ist zu groß." );
            }

            if( 0 >= MaxThreads )
            {
                errorAndOut( "\nDer Parameter --memory-boost ist zu klein." );
            }
        }

        if( 0 == strcmp( "--orignial", Arguments[ Index ] ) || 0 == strcmp( "-o", Arguments[ Index ] ) )
        {
            Org.V = TRUE;
        }

        if(
            (
                0 == strcmp( "--filter-x-start", Arguments[ Index ] )
            ||
                0 == strcmp( "-fxs", Arguments[ Index ] )
            )
        &&
            0 == ( XFilter&FILTER_X_START )
        )
        {
            XFilter += FILTER_X_START;
            if( FALSE == validateFilter( Arguments[ Index+1 ], FilterX[0] ) )
            {
                errorAndOut( "\nDer Parameter --filter-x-start ist fehlerhaft." );
            }

            if(
                X_MAX_G < FilterX[0][0]
            ||
                (
                        X_MAX_G == FilterX[0][0]
                    &&
                    (
                            X_MAX_M < FilterX[0][1]
                        ||
                        (
                            X_MAX_M == FilterX[0][1]
                        &&
                            X_MAX_S < FilterX[0][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-x-start ist zu groß." );
            }

            if(
                X_MIN_G > FilterX[0][0]
            ||
                (
                       X_MIN_G == FilterX[0][0]
                    &&
                    (
                            X_MIN_M > FilterX[0][1]
                        ||
                        (
                            X_MIN_M == FilterX[0][1]
                        &&
                            X_MIN_S > FilterX[0][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-x-start ist zu klein." );
            }

        }

        if(
            (
                0 == strcmp( "--filter-x-end", Arguments[ Index ] )
            ||
                0 == strcmp( "-fxe", Arguments[ Index ] )
            )
        &&
            0 == ( XFilter&FILTER_X_END )
        )
        {
            XFilter += FILTER_X_END;
            if( FALSE == validateFilter( Arguments[ Index+1 ], FilterX[1] ) )
            {
                errorAndOut( "\nDer Parameter --filter-x-end ist fehlerhaft." );
            }

            if(
                X_MAX_G < FilterX[1][0]
            ||
                (
                    X_MAX_G == FilterX[1][0]
                &&
                    (
                        X_MAX_M < FilterX[1][1]
                    ||
                        (
                            X_MAX_M == FilterX[1][1]
                        &&
                            X_MAX_S < FilterX[1][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-x-end ist zu groß." );
            }

            if(
                X_MIN_G > FilterX[1][0]
            ||
                (
                    X_MIN_G == FilterX[1][0]
                &&
                    (
                        X_MIN_M > FilterX[1][1]
                    ||
                        (
                            X_MIN_M == FilterX[1][1]
                        &&
                            X_MIN_S > FilterX[1][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-x-end ist zu klein." );
            }
        }

        if(
            (
                0 == strcmp( "--filter-y-start", Arguments[ Index ] )
            ||
                0 == strcmp( "-fys", Arguments[ Index ] )
                )
        &&
            0 == ( YFilter&FILTER_Y_START )
        )
        {
            YFilter += FILTER_Y_START;
            if( FALSE == validateFilter( Arguments[ Index+1 ], FilterY[0] ) )
            {
                errorAndOut( "\nDer Parameter --filter-y-start ist fehlerhaft." );
            }

            if(
                Y_MAX_G < FilterY[0][0]
            ||
                (
                    Y_MAX_G == FilterY[0][0]
                &&
                    (
                        Y_MAX_M < FilterY[0][1]
                    ||
                        (
                            Y_MAX_M == FilterY[0][1]
                        &&
                            Y_MAX_S < FilterY[0][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-y-start ist zu gross." );
            }

            if(
                Y_MIN_G > FilterY[0][0]
            ||
                (
                    Y_MIN_G == FilterY[0][0]
                &&
                    (
                        Y_MIN_M > FilterY[0][1]
                    ||
                        (
                            Y_MIN_M == FilterY[0][1]
                        &&
                            Y_MIN_S > FilterY[0][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-y-start ist zu klein." );
            }

        }

        if(
            (
                0 == strcmp( "--filter-y-end", Arguments[ Index ] )
            ||
                0 == strcmp( "-fye", Arguments[ Index ] )
            )
        &&
            0 == ( YFilter&FILTER_Y_END )
        )
        {
            YFilter += FILTER_Y_END;
            if( FALSE == validateFilter( Arguments[ Index+1 ], FilterY[1] ) )
            {
                errorAndOut( "\nDer Parameter --filter-y-end ist fehlerhaft." );
            }

            if(
                Y_MAX_G < FilterY[1][0]
            ||
                (
                    Y_MAX_G == FilterY[1][0]
                &&
                    (
                        Y_MAX_M < FilterY[1][1]
                    ||
                        (
                            Y_MAX_M == FilterY[1][1]
                        &&
                            Y_MAX_S < FilterY[1][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-y-end ist zu gross." );
            }

            if(
                Y_MIN_G > FilterY[1][0]
            ||
                (
                    Y_MIN_G == FilterY[1][0]
                &&
                    (
                        Y_MIN_M > FilterY[1][1]
                    ||
                        (
                            Y_MIN_M == FilterY[1][1]
                        &&
                            Y_MIN_S > FilterY[1][2]
                        )
                    )
                )
            )
            {
                errorAndOut( "\nDer Parameter --filter-y-end ist zu klein." );
            }
        }
    }

    if( FILTER_X_RANGE == XFilter )
    {
        if(
            FilterX[0][0] > FilterX[1][0]
        ||
            (
                FilterX[0][0] == FilterX[1][0]
            &&
                (
                    FilterX[0][1] > FilterX[1][1]
                ||
                    (
                        FilterX[0][1] == FilterX[1][1]
                    &&
                        FilterX[0][2] > FilterX[1][2]
                    )
                )
            )
        )
        {
            errorAndOut( "\nDas gegebene Intervall der Breitegrade ist fehlerhaft." );
        }
    }

    if( FILTER_Y_RANGE == YFilter )
    {
        if(
            FilterY[0][0] > FilterY[1][0]
        ||
            (
                FilterY[0][0] == FilterY[1][0]
            &&
                (
                    FilterY[0][1] > FilterY[1][1]
                ||
                    (
                        FilterY[0][1] == FilterY[1][1]
                    &&
                        FilterY[0][2] > FilterY[1][2]
                    )
                )
            )
        )
        {
            errorAndOut( "\nDas gegebene Intervall der Laengengrade ist fehlerhaft." );
        }
    }

    if( 2 == MaxThreads )
    {
        UseThreads.V = FALSE;
    }

    FilterY[0][0] += 90;
    FilterY[0][1] = ( FilterY[0][1]+60 )%60;
    FilterY[0][2] = ( FilterY[0][2]+60 )%60;

    FilterY[1][0] += 90;
    FilterY[1][1] = ( FilterY[1][1]+60 )%60;
    FilterY[1][2] = ( FilterY[1][2]+60 )%60;

	Map = (PtMap*) malloc( sizeof(PtMap) );
	if( NULL == Map )
	{
		errorAndOut( "\nSomethings wrong with the memory, Jim." );
	}

    if( DEGREE == Use || DEGREEX == Use)
    {
        UseFactor = 5;
        GegenFactor = 60*60;
        YStartUpper = FilterY[0][0];
        if( 0 != FilterY[0][1] || 0 != FilterY[0][2] )
        {
            YStartLower = 1;
        }
        else
        {
            YStartLower = 0;
        }

        YEndUpper = FilterY[1][0];
        if( 0 != FilterY[1][1] || 0 != FilterY[1][2] )
        {
            YEndLower = 1;
        }
        else
        {
            YEndLower = 0;
        }
    }
    else if( MINUTES == Use )
    {
        UseFactor = 5*60;
        GegenFactor = 60;
        YStartUpper = ( FilterY[0][0] )*60+FilterY[0][1];
        if( 0 != FilterY[0][2] )
        {
            YStartLower = 1;
        }
        else
        {
            YStartLower = 0;
        }

        YEndUpper = ( FilterY[1][0] )*60+FilterY[1][1];
        if( 0 != FilterY[1][2] )
        {
            YEndLower = 1;
        }
        else
        {
            YEndLower = 0;
        }
    }
    else if( SECONDS == Use )
    {
        UseFactor = 5*60*60;
        GegenFactor = 1;
        YStartLower = 0;
        YStartUpper = ( FilterY[0][0] )*60*60+FilterY[0][1]*60+FilterY[0][2];
        YEndLower = 0;
        YEndUpper = ( FilterY[1][0] )*60*60+FilterY[1][1]*60+FilterY[1][2];
    }
    else
    {
        UseFactor = 1;
        GegenFactor = 5*60*60;

        if( 0 != FilterY[0][0]%5 || 0 != FilterY[0][1] || 0 != FilterY[0][2] )
        {
            YStartLower = 1;
        }
        else
        {
            YStartLower = 0;
        }
        YStartUpper = (int)(( FilterY[0][0] )/5);

        if( 0 != FilterY[1][0]%5 || 0 != FilterY[1][1] || 0 != FilterY[1][2] )
        {
            YEndLower = 1;
        }
        else
        {
            YEndLower = 0;
        }
        YEndUpper = (int)(( FilterY[1][0])/5);
    }

    FilterY[0][3] = ( FilterY[0][0]%90 )*60*60+FilterY[0][1]*60+FilterY[0][2];
    if( 90 < FilterY[0][3] )
    {
        FilterY[0][3] *= -1;
    }
    FilterY[1][3] = ( FilterY[1][0]%90 )*60*60+FilterY[1][1]*60+FilterY[1][2];
    if( 90 < FilterY[1][3] )
    {
        FilterY[1][3] *= -1;
    }

    if( 0 != Precision )
    {
        setPrecision( Arguments[ Precision ] );
    }
    else
    {
        setPrecision( PRECISION );
    }

    if( 0 != X && 0 != Y )
    {
        makeDesk( Arguments[ X ], Arguments[ Y ], Arguments[ ArgC-1 ] );
    }
    else if( 0 != X )
    {
        makeDesk( Arguments[ X ], "0", Arguments[ ArgC-1 ] );
    }
    else if( 0 != Y )
    {
        makeDesk( "0", Arguments[ Y ], Arguments[ ArgC-1 ] );
    }
    else
    {
        makeDesk( "0", "0", Arguments[ ArgC-1 ] );
    }

    if( FALSE == UseThreads.V)
    {
        setParalellkreise();

        if (TRUE == UsePlan.V)
        {
            startPlanimetrik();
            planimetrikGetMetrum();
            planimetrikDistances();
            planimetrikBreitenPunkte();
            planimetrikLaengen();
            planimetrikSpecialLaenge();
            planimetrikCleanUpLaengen();
            makeBreiten( &Map->BaseMap.Plan->Einheit );
            printStart();
            planimetrikPrint();
            printPayloadStart();
            planimetrikIntersections();
            printPayloadEnd();
            printEnd();
            planimetrikHouskeeping();
        }
        else
        {
            startGlob();
            globGetMetrum();
            globDistances();
            globBreitenPunkte();
            globLimits();
            globLaengen();
            makeBreiten( &Map->BaseMap.Glob->Einheit );
            printStart();
            globPrint();
            printPayloadStart();
            globGrid();
            printPayloadEnd();
            printEnd();
            globHouskeeping();
        }
    }
    else
    {
        if (TRUE == UsePlan.V)
        {
            pthread_create( &PlanimetrikThread, NULL, planimetrikThreaded, NULL );
        }
        else
        {
            setParalellkreise();
            startGlob();
            globGetMetrum();
            globDistances();
            globBreitenPunkte();
            globLimits();
            globLaengen();
            makeBreiten( &Map->BaseMap.Glob->Einheit );
            printStart();
            globPrint();
            printPayloadStart();
            globGridThreaded();
            printPayloadEnd();
            printEnd();
            globHouskeeping();
        }

        if (TRUE == UsePlan.V)
        {
            pthread_join( PlanimetrikThread, NULL );
        }
    }

    cleanUp(); 
}
