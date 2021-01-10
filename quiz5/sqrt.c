// https://www.netlib.org/fdlibm/e_sqrt.c
#include <stdint.h>

/* A union allowing us to convert between a float and a 32-bit integers.*/
typedef union
{
    float value;
    uint32_t v_int;
} ieee_float_shape_type;

/* Set a float from a 32 bit int. */
#define INSERT_WORDS(d, ix0)        \
    do                              \
    {                               \
        ieee_float_shape_type iw_u; \
        iw_u.v_int = (ix0);         \
        (d) = iw_u.value;           \
    } while (0)

/* Get a 32 bit int from a float. */
#define EXTRACT_WORDS(ix0, d)       \
    do                              \
    {                               \
        ieee_float_shape_type ew_u; \
        ew_u.value = (d);           \
        (ix0) = ew_u.v_int;         \
    } while (0)

static const float one = 1.0, tiny = 1.0e-30;

float ieee754_sqrt(float x)
{
    float z;
    int32_t sign = 0x80000000;
    uint32_t r;
    int32_t ix0, s0, q, m, t, i;

    EXTRACT_WORDS(ix0, x);

    /* take care of zero */
    if (ix0 <= 0)
    {
        if ((ix0 & (~sign)) == 0)
            return x; /* sqrt(+-0) = +-0 */
        if (ix0 < 0)
            return (x - x) / (x - x); /* sqrt(-ve) = sNaN */
    }
    /* take care of +INF and NaN */
    if ((ix0 & 0x7f800000) == 0x7f800000)
    {
        /* sqrt(NaN) = NaN, sqrt(+INF) = +INF,*/
        return x;
    }
    /* normalize x */
    m = (ix0 >> 23);
    if (m == 0)
    { /* subnormal x */
        for (i = 0; (ix0 & 0x00800000) == 0; i++)
            ix0 <<= 1;
        m -= i - 1;
    }
    m -= 127; /* unbias exponent */
    ix0 = (ix0 & 0x007fffff) | 0x00800000;
    if (m & 1)
    { /* odd m, double x to make it even */
        ix0 += ix0;
    }
    m >>= 1; /* m = [m/2] */

    /* generate sqrt(x) bit by bit */
    ix0 += ix0;
    q = s0 = 0; /* [q] = sqrt(x) */
    r = 0x01000000; /* r = moving bit from right to left */

    while (r != 0)
    {
        t = s0 + r;
        if (t <= ix0)
        {
            s0 = t + r;
            ix0 -= t;
            q += r;
        }
        ix0 += ix0;
        r >>= 1;
    }

    /* use floating add to find out rounding direction */
    if (ix0 != 0)
    {
        z = one - tiny; /* trigger inexact flag */
        if (z >= one)
        {
            z = one + tiny;
            if (z > one)
                q += 2;
            else
                q += (q & 1);
        }
    }

    ix0 = (q >> 1) + 0x3f000000;
    ix0 += (m << 23);

    INSERT_WORDS(z, ix0);

    return z;
}