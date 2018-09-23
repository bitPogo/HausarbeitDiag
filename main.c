/**
 * @autor Matthias Geisler (bitpogo) <geislemx@informatik.hu-berlin.de>
 * @licence GNU General Public License
 */
#include "./lib.h"

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
