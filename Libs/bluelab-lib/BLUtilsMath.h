#ifndef BL_UTILS_MATH_H
#define BL_UTILS_MATH_H

#include <math.h>

#include <vector>
using namespace std;

#include <BLTypes.h>

#include "IPlug_include_in_plug_hdr.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif

#ifndef TWO_PI
#define TWO_PI 6.28318530717958647692
#endif

#ifndef MIN
#define MIN(__x__, __y__) ((__x__ < __y__) ? __x__ : __y__)
#endif

#ifndef MAX
#define MAX(__x__, __y__) ((__x__ > __y__) ? __x__ : __y__)
#endif

#define BL_EPS 1e-15
#define BL_EPS3 1e-3
#define BL_EPS6 1e-6
#define BL_EPS8 1e-8
#define BL_EPS10 1e-10

#define BL_INF 1e15
#define BL_INF8 1e8
#define BL_INFI 1e9

inline double
bl_round(double x)
{ return round(x); }

inline float
bl_round(float x)
{ return roundf(x); }

class BLUtilsMath
{
 public:
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE Round(FLOAT_TYPE val, int precision);
    
    template <typename FLOAT_TYPE>
    static void Round(FLOAT_TYPE *buf, int nFrames, int precision);
    
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE DomainAtan2(FLOAT_TYPE x, FLOAT_TYPE y);
    
    static int NextPowerOfTwo(int value);

    template <typename FLOAT_TYPE>
        static int SecondOrderEqSolve(FLOAT_TYPE a, FLOAT_TYPE b,
                                      FLOAT_TYPE c, FLOAT_TYPE res[2]);

    template <typename FLOAT_TYPE>
    static FLOAT_TYPE FindNearestHarmonic(FLOAT_TYPE value, FLOAT_TYPE refValue);
    
    // GCD
    // (same as hcf or pgcd in French)
    //
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE gcd(FLOAT_TYPE a, FLOAT_TYPE b);
    
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE gcd(const vector<FLOAT_TYPE> &arr);

    // See: http://paulbourke.net/miscellaneous/correlate/
    template <typename FLOAT_TYPE>
    static void CrossCorrelation2D(const vector<WDL_TypedBuf<FLOAT_TYPE> > &image,
                                   const vector<WDL_TypedBuf<FLOAT_TYPE> > &mask,
                                   vector<WDL_TypedBuf<FLOAT_TYPE> > *corr);

    template <typename FLOAT_TYPE>
    static void Reshape(WDL_TypedBuf<FLOAT_TYPE> *ioBuf,
                        int inWidth, int inHeight,
                        int outWidth, int outHeight);
    
    template <typename FLOAT_TYPE>
    static void Transpose(WDL_TypedBuf<FLOAT_TYPE> *ioBuf,
                          int width, int height);
    
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE ComputeVariance(const WDL_TypedBuf<FLOAT_TYPE> &data);
    
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE ComputeVariance(const vector<FLOAT_TYPE> &data);
    
    // Try a fix
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE ComputeVariance2(const vector<FLOAT_TYPE> &data);

    // Lagrange: https://en.wikipedia.org/wiki/Lagrange_polynomial
    // Note: we must not have two identical x
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE LagrangeInterp4(FLOAT_TYPE x,
                                      FLOAT_TYPE p0[2], FLOAT_TYPE p1[2],
                                      FLOAT_TYPE p2[2], FLOAT_TYPE p3[2]);

    // See: https://numpy.org/doc/stable/reference/generated/numpy.convolve.html
#define CONVO_MODE_FULL  0
#define CONVO_MODE_SAME  1
#define CONVO_MODE_VALID 2
    static void Convolve(const WDL_TypedBuf<BL_FLOAT> &a,
                         const WDL_TypedBuf<BL_FLOAT> &b,
                         WDL_TypedBuf<BL_FLOAT> *result,
                         int convoMode = CONVO_MODE_FULL);
    
    static bool SegSegIntersect(BL_FLOAT seg0[2][2], BL_FLOAT seg1[2][2]);

    // Schlick sigmoid, see:
    //
    // https://dept-info.labri.u-bordeaux.fr/~schlick/DOC/gem2.ps.gz
    //
    // a included in [0, 1]
    // a = 0.5 -> gives a line
    template <typename FLOAT_TYPE>
    static FLOAT_TYPE ApplySigmoid(FLOAT_TYPE t, FLOAT_TYPE a);
};

#endif