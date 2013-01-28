/*              chbevl.c
 *  Evaluate Chebyshev series
 * SYNOPSIS:
 * int N;
 * double x, y, coef[N], chebevl();
 * y = chbevl( x, coef, N );
 * DESCRIPTION:
 * Evaluates the series
 *        N-1
 *         - '
 *  y  =   >   coef[i] T (x/2)
 *         -            i
 *        i=0
 * of Chebyshev polynomials Ti at argument x/2.
 * Coefficients are stored in reverse order, i.e. the zero
 * order term is last in the array.  Note N is the number of
 * coefficients, not the order.
 * If coefficients are for the interval a to b, x must
 * have been transformed to x -> 2(2x - b - a)/(b-a) before
 * entering the routine.  This maps x from (a, b) to (-1, 1),
 * over which the Chebyshev polynomials are defined.
 * If the coefficients are for the inverted interval, in
 * which (a, b) is mapped to (1/b, 1/a), the transformation
 * required is x -> 2(2ab/x - b - a)/(b-a).  If b is infinity,
 * this becomes x -> 4a/x - 1.
 * SPEED:
 * Taking advantage of the recurrence properties of the
 * Chebyshev polynomials, the routine requires one more
 * addition per loop than evaluating a nested polynomial of
 * the same degree.
 */
/*              chbevl.c  */

/*
Cephes Math Library Release 2.0:  April, 1987
Copyright 1985, 1987 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

double chbevl(double x, const double array[], const int n )
{
  double const * p = array;
  double b0 = *p++;
  double b1 = 0.0;
  double b2=b1;
  int i = n - 1;
  do
    {
    b2 = b1;
    b1 = b0;
    b0 = x * b1  -  b2  + *p++;
    }
  while( --i );
  return( 0.5*(b0-b2) );
}


/* i0.c */

/*
 * Modified Bessel function of order zero
 * SYNOPSIS:
 * double x, y, i0();
 * y = i0( x );
 * DESCRIPTION:
 * Returns modified Bessel function of order zero of the
 * argument.
 * The function is defined as i0(x) = j0( ix ).
 * The range is partitioned into the two intervals [0,8] and
 * (8, infinity).  Chebyshev polynomial expansions are employed
 * in each interval.
 * ACCURACY:
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       0,30         6000       8.2e-17     1.9e-17
 *    IEEE      0,30        30000       5.8e-16     1.4e-16
 */

/*              i0e.c
 *  Modified Bessel function of order zero,
 *  exponentially scaled
 * SYNOPSIS:
 * double x, y, i0e();
 * y = i0e( x );
 * DESCRIPTION:
 * Returns exponentially scaled modified Bessel function
 * of order zero of the argument.
 * The function is defined as i0e(x) = exp(-|x|) j0( ix ).
 * ACCURACY:
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,30        30000       5.4e-16     1.2e-16
 * See i0().
 */

/*              i0.c    */
/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1987, 2000 by Stephen L. Moshier
*/

#include <math.h>
#include "cephes.h"

/* Chebyshev coefficients for exp(-x) I0(x)
 * in the interval [0,8].
 * lim(x->0){ exp(-x) I0(x) } = 1.
 */


static const double A[] =
{
  -4.41534164647933937950E-18,
  3.33079451882223809783E-17,
  -2.43127984654795469359E-16,
  1.71539128555513303061E-15,
  -1.16853328779934516808E-14,
  7.67618549860493561688E-14,
  -4.85644678311192946090E-13,
  2.95505266312963983461E-12,
  -1.72682629144155570723E-11,
  9.67580903537323691224E-11,
  -5.18979560163526290666E-10,
  2.65982372468238665035E-9,
  -1.30002500998624804212E-8,
  6.04699502254191894932E-8,
  -2.67079385394061173391E-7,
  1.11738753912010371815E-6,
  -4.41673835845875056359E-6,
  1.64484480707288970893E-5,
  -5.75419501008210370398E-5,
  1.88502885095841655729E-4,
  -5.76375574538582365885E-4,
  1.63947561694133579842E-3,
  -4.32430999505057594430E-3,
  1.05464603945949983183E-2,
  -2.37374148058994688156E-2,
  4.93052842396707084878E-2,
  -9.49010970480476444210E-2,
  1.71620901522208775349E-1,
  -3.04682672343198398683E-1,
  6.76795274409476084995E-1
};

/* Chebyshev coefficients for exp(-x) sqrt(x) I0(x)
 * in the inverted interval [8,infinity].
 * lim(x->inf){ exp(-x) sqrt(x) I0(x) } = 1/sqrt(2pi).
 */

static const double B[] =
{
  -7.23318048787475395456E-18,
  -4.83050448594418207126E-18,
  4.46562142029675999901E-17,
  3.46122286769746109310E-17,
  -2.82762398051658348494E-16,
  -3.42548561967721913462E-16,
  1.77256013305652638360E-15,
  3.81168066935262242075E-15,
  -9.55484669882830764870E-15,
  -4.15056934728722208663E-14,
  1.54008621752140982691E-14,
  3.85277838274214270114E-13,
  7.18012445138366623367E-13,
  -1.79417853150680611778E-12,
  -1.32158118404477131188E-11,
  -3.14991652796324136454E-11,
  1.18891471078464383424E-11,
  4.94060238822496958910E-10,
  3.39623202570838634515E-9,
  2.26666899049817806459E-8,
  2.04891858946906374183E-7,
  2.89137052083475648297E-6,
  6.88975834691682398426E-5,
  3.36911647825569408990E-3,
  8.04490411014108831608E-1
};

double i0(double x)
{
  if( x < 0 )
    {
    x = -x;
    }
  if( x <= 8.0 )
    {
    const double y = (x*0.5) - 2.0;
    return( exp(x) * chbevl( y, A, 30 ) );
    }
  return(  exp(x) * chbevl( 32.0/x - 2.0, B, 25 ) / sqrt(x) );
}

double i0e( double x )
{
  if( x < 0 )
    {
    x = -x;
    }
  if( x <= 8.0 )
    {
    const double y = (x*0.5) - 2.0;
    return( chbevl( y, A, 30 ) );
    }
  return(  chbevl( 32.0/x - 2.0, B, 25 ) / sqrt(x) );
}
