/**
 * @autor Matthias Geisler (bitpogo) <geislemx@informatik.hu-berlin.de>
 * @licence GNU General Public License
 */
#include <gmp.h>
#include <mpfr.h>

#ifndef HAUSARBEIT_TYPES_H
#define HAUSARBEIT_TYPES_H
#define WRITER_DONE 2
#define WRITER_READY 0
#define WRITER_RUN 1
/** Simple **/
typedef struct OutputBuffer
{
    Point** Value;
    long Start;
    long End;
    unsigned int OutIndex;
} OutputBuffer;

typedef struct Writer
{
    OutputBuffer Buffer;
    unsigned short Status;
} Writer;

typedef struct Interaction
{
    pthread_t Thread;
    unsigned int Index;
} Interaction;

typedef struct ThreadHelper
{
    unsigned int BreitenIndex;
    OutputBuffer Buffer;
    unsigned int Offset;
    double Factor;
    Point* Center;
} ThreadHelper;

typedef struct Bool
{
    unsigned int V: 1;
} Bool;
/** Planimetrik */
typedef struct Planimetrik
{
    mpfr_t Einheit;
    Point* H; // Zentrum der Breitengradkreise
    /* Redundanzpunkte auf dem Lot H~CD */
    Point* O;
    Point* K;
    Point* S;
    /* Distanzen auf dem Lot von H bis CD */
    mpfr_t HE;
    mpfr_t HO;
    mpfr_t HK;
    mpfr_t HS;
    mpfr_t HZ;
    /* Endpunkte der Breitenkreise */
    Point* X;
    Point* G;
    Point* R;
    Point* M;
    Point* F;
    Point* P;
    Point* L;
    Point* T;
    Point* N;
    Point* U;
    /* Special Laengen */
    Point** Laengen;
} Planimetrik;
/** Globus */
typedef struct Globus
{
    mpfr_t Einheit;
    Point* L; // Zentrum der Breitengradkreise
    /* Redundanzpunkte auf dem Lot H~CD */
    Point* K;
    Point* G;
    Point* H;
    /* Distanzen auf dem Lot von H bis CD */
    mpfr_t LK;
    mpfr_t HK;
    mpfr_t LG;
    mpfr_t HG;
    mpfr_t LH;
    mpfr_t LZ;
    mpfr_t HZ;
    /* Endpunkte der Breitenkreise */
    Point* P;
    Point* X;
    Point* M;
    Point* Y;
    Point* T;
    Point* S;
    Point* R;
    Point* O;
    Point* N;
    Point* V;
    Point* U;
    Point* F;
    /* Custom */
    Point* I;
    Point* J;
} Globus;
/** Common */
typedef struct PtMap
{
    /* Grundtypen */
    mpfr_t* Kantenlaenge;
    /* Reisbrettfläche */
    Point* A;
    Point* B;
    Point* C;
    Point* D;
    Point* E;
    Point* Z;
    Point** BreitenPunkte;
    mpfr_t* Breiten;
    mpfr_t* AequatorDistanze;
    Point* ToMove;
    Point** Laengen;
    union
    {
        Planimetrik* Plan;
        Globus* Glob;
    } BaseMap;

} PtMap;
#endif //HAUSARBEIT_TYPES_H
