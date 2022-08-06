



/* *************************************************************
   This file handles all the matrix/vector/math operations
************************************************************* */

#if !defined (__MATRIX_H)
#define __MATRIX_H

#include <math.h>

#define CORRECT         0.00001              // accuracy limit
#define CORRECT2        0.01                 // accuracy limit
#define CORRECT3        0.001                // accuracy limit
#define CORRECT4        0.0001               // accuracy limit
#define ICORRECT        0.99999

#define TWOPI2D    6.28

#ifndef PI
#define PI         3.14159265358979323846
#endif

#define TWOPI      6.28318530717958647692
#define HALFPI     1.57079632679489661923

#define LOG2E      1.4426950408889634074
#define LOG2(x)    (LOG2E * log(x))     // nat. log to log 2
#define SQR(x) ((x)*(x))
#define rnd(x) ((x)*(rand() / 32767.0))
#define randomizer(x, y) lerp(rand() / 32767.0, x, y)
#define round(x) ((int)((x) + 0.5))
#define round0(x) (((x)<0) ? (-(int)(-(x))) : ((int)(x)))
#define abs(x) (((x)<0) ? -(x):(x))
#define not_zero(x) (abs(x) > 0.0001)



#ifndef sgn
#define sgn(x)   ( ((x)<0) ? -1 : ((x) ? 1:0) )
#endif


#ifndef powf
#define powf(x, y) pow(x, y)
#endif


#ifdef DOS
double erf(double arg);
#define random rand
#endif


#define s_curve(t) ( SQR(t) * (3. - ((t) + (t))) )

#define lerp(t, a, b) ( (a) + (t) * ((b) - (a)) )
#define smoothstep(t, a, b)  ( (t) <= 0 ? (a) : ((t) >= 1 ? (b) : ((a) + s_curve(t)*((b)-(a))) ) )
#define step(a, x) ((x) >= (a))
#define clamp(x, a, b) ( (x) < (a) ? (a) : ((x) > (b) ? (b) : (x) ) )
#define min(a, b) ( (a) < (b) ? (a) : (b))

#define getgamma(gamma) (1.0/(gamma))
#define gammacorrect(gamma, x) (powf((x), (gamma)))

// ********************************************************************
// init masks

#define MASK_SIN_COS    0x00000001
//#define MASK_SIN        0x00000002
#define MASK_ASIN_ACOS  0x00000004
//#define MASK_ASIN       0x00000008
#define MASK_0_255_0_1  0x00000010
//#define MASK_0_255_0_1  0x00000020
#define MASK_SQRT       0x00000040
#define MASK_BYTE       0x00000080
#define MASK_POW        0x00000100

void init_lut(int fmask = 0xffffffff);

// trig tables

#define MAX_TRIG_SIZE           0x0ffff
#define MAX_TRIG_SIZE_PLUS      0x10000
#define MAX_SIN_COS_TBL_SIZE    0x14000
#define COS_START               0x04000

extern float *cos_table, *sin_table, *asin_table;
extern double trigfactor, atrigfactor;

#define COS(x) (cos_table[((int)((x)*trigfactor)) & MAX_TRIG_SIZE])
#define SIN(x) (sin_table[((int)((x)*trigfactor)) & MAX_TRIG_SIZE])
#define ACOS(x) (HALFPI - asin_table[((int)(((x)+1)*atrigfactor)) & MAX_TRIG_SIZE])
#define ASIN(x) (asin_table[((int)(((x)+1)*atrigfactor)) & MAX_TRIG_SIZE])

// ( 0 .. 255 ) - ( 0.0 .. 1.0 ) conversions

#define MAX_0_1_0_255           0x3ff
#define MAX_0_1_0_255_PLUS      0x400

extern float *table_0_255_0_1;

#define CONVERT_0_255_0_1(x) table_0_255_0_1[x]
#define CONVERT_0_1_0_255(x) ((int)((x)*255))

// sqrt function - x is an element of [0..4]

extern int mantissa_lut[65536];
extern int imantissa_lut[65536];

inline float SQRT(float x) {

    int man;
    register int ex;

    ex    = ((*((int *)&x)) >> 23) + 127;
    man = ((*((int *)&x)) >> 8) & 0x7FFF;

    man = mantissa_lut[man | ((ex & 1)*0x8000)] | ((ex & 0xfffffffe) << 22);

    return *((float *)&man);
}


inline float ISQRT(float x) {

   int man;
   register int ex;

   ex    = ((*((int *)&x)) >> 23) - 127;
   man = imantissa_lut[(((*((int *)&x)) >> 8) & 0x7FFF) | ((ex & 1)*0x8000)];

   if (man)
      man |= ((253 - ex) & 0xfffffffe) << 22;
   else
      man  = ((254 - ex) & 0xfffffffe) << 22;

   return *((float *)&man);
}


extern unsigned char    *pbyte_lut;
#define BYTE_LUT(x)     pbyte_lut[x]

#define MAX_POW_X 100
#define MAX_POW_N 100
extern float *ppow_lut[MAX_POW_N];

#define POW(x,y) ppow_lut[((int)(y))-1][(int)((x)*100)]

// ********************************************************************

typedef float vector2f[2];
typedef vector2f *pvector2f;
typedef float vector3f[3];
typedef vector3f *pvector3f;
typedef float vector4f[4];
typedef vector4f *pvector4f;
typedef unsigned char vector3uc[3];
typedef vector3uc *pvector3uc;
typedef unsigned char vector4uc[4];
typedef vector4uc *pvector4uc;



#define dotproduct3(x, y) (x[0]*y[0] + x[1]*y[1] + x[2]*y[2])
#define dotproduct4(x, y) (x[0]*y[0] + x[1]*y[1] + x[2]*y[2] + x[3]*y[3])

#define copyarray2(x, y) {x[0] = y[0]; x[1] = y[1];}
#define copyarray3(x, y) {copyarray2(x, y); x[2] = y[2];}
#define copyarray4(x, y) {copyarray3(x, y); x[3] = y[3];}

#define addarray2(x, y) {x[0] += y[0]; x[1] += y[1];}
#define addarray3(x, y) {x[0] += y[0]; x[1] += y[1]; x[2] += y[2];}
#define addarray4(x, y) {x[0] += y[0]; x[1] += y[1]; x[2] += y[2]; x[3] += y[3];}

#define subarray2(x, y) {x[0] -= y[0]; x[1] -= y[1]; }
#define subarray3(x, y) {x[0] -= y[0]; x[1] -= y[1]; x[2] -= y[2];}
#define subeqarray3(a, b, c) { a[0] = b[0] - c[0]; a[1] = b[1] - c[1]; a[2] = b[2] - c[2]; }

#define multarray3(x, y) {x[0] *= y[0]; x[1] *= y[1]; x[2] *= y[2];}

#define divarray3(x, y) {x[0] /= y[0]; x[1] /= y[1]; x[2] /= y[2];}

#define saddarray2(x,y)  {x[0] += y; x[1] += y;}
#define saddarray3(x,y)  {x[0] += y; x[1] += y; x[2] += y;}

#define ssubarray3(x,y)  {x[0] -= y; x[1] -= y; x[2] -= y;}

#define smultarray3(x,y) {x[0] *= y; x[1] *= y; x[2] *= y;}
#define smultarray4(x,y) {x[0] *= y; x[1] *= y; x[2] *= y; x[3] *= y;}

#define sdivarray3(x,y)  {x[0] /= y; x[1] /= y; x[2] /= y;}
#define sdivarray4(x,y)  {x[0] /= y; x[1] /= y; x[2] /= y; x[3] /= y;}

#define nsame3(x, y) (x[0]!=y[0] || x[1]!=y[1] || x[2]!=y[2])

#define copymx4x4(x, y) memcpy(x, y, 64);
#define init_mx(x) {memset(x, 0, 64); x[0][0] = x[1][1] = x[2][2] = x[3][3] = 1.0;}

#define print_v3(x) printf("%f %f %f\n", x[0], x[1], x[2])
#define print_v4(x) printf("%f %f %f %f\n", x[0], x[1], x[2], x[3])
#define print_mx(i, x) for (i=0; i<4; i++) printf("%f %f %f %f\n", x[i][0], x[i][1], x[i][2], x[i][3])


#define magnitude2(x) sqrt(x[0]*x[0] + x[1]*x[1])
#define magnitude3(x) sqrt(dotproduct3(x, x))
#define magnitude4(x) sqrt(dotproduct4(x, x))

#define fmagnitude3(x) SQRT(dotproduct3(x, x))

inline float distance3(float *x, float *y) {
        vector3f d;
        d[0] = x[0] - y[0];
        d[1] = x[1] - y[1];
        d[2] = x[2] - y[2];
        return (float)magnitude3(d);
}

int    similar3(float *x, float *y);

float  normalize2(float x[]);
float  normalize3(float x[]);
float  normalize4(float x[]);


inline float fnormalize3(float *x) {

   register float mag, imag;

   mag = fmagnitude3(x);

   if (mag) {
      imag = 1.0/mag;
      smultarray3(x, imag);
   }

   return mag;
}


void   swap4(float mx1[], float mx2[]);
void   xproduct(float *a, float *x, float *y);
#define xproduct2(x, y) (x[0]*y[1] - x[1]*y[0])
void   transpose(float r[][4], float s[][4]);
void   transpose(float r[][4]);

void   matmatmult(float y[][4], float x[][4]);
void   matvecmult(float y[][4], float x[]);
void   vecmatmult(float y[][4], float x[]);

void   matmatmulto(float y[][4], float x[][4]);
void   matvecmultv(float y[][4], float x[]);
void   vecmatmultv(float y[][4], float x[]);
void   matvecmulto(float y[][4], float x[]);
void   vecmatmulto(float y[][4], float x[]);

int    inversemx(float a[][4],float b[][4]);
int    inversemx2(float mx[][4], float mxinverse[][4]);
void   inversemxrt(float mx[][4], float mxinverse[][4]);

// ********************************************************************
//                                              orientation


#define ANGLE2RAD  0.01745329251994329577
#define rad2deg(x) ((x)*57.29577951)    // times 180.0/PI
#define deg2rad(x) ((x)*ANGLE2RAD)      // times 180.0/PI

void calc_quat(float mx[][4], float current[]);
void calc_euler(float mx[][4], float current[]);
void euler2quat(float angle[], float quat[]);
void quatquatmult(float y[], float x[]);

void decompose_xyz(vector4f mx[4], float *roll, float *pitch, float *yaw);
void decompose_zyx(vector4f mx[4], float *roll, float *pitch, float *yaw);
void rotate_mx_x(vector4f mx[4], float x);
void rotate_mx_y(vector4f mx[4], float x);
void rotate_mx_z(vector4f mx[4], float x);
void rotate_vector(float *start, float *axis, float rad, float *sol);

// ********************************************************************
//                                                      splines

#define M_CR    0
#define M_B     1

void spline(float s, float in[][4], float out[], int i, int stype);
void spline(float s, vector4f *in, float *out, int stype);
void splinet(float s, float Q[][4], float *out, int stype);

void spnormal(float s, float t, float in[][4], float out[], int ptr[], int stype);
void spnormal(float s, float t, vector4f **in, float *out, int stype);
void spnormal(float s, float t, float Q[][4][4], float *out, int stype);

void spnormal_base(vector4f **in, float out[], int stype);

void sppatch(float s, float t, float **in, float out[], int stype);
void sppatch(float s, float t, vector4f **in, float *out, int stype);
void sppatch(float s, float t, float Q[][4][4], float *out, int stype);

// ********************************************************************
//                                                      geometric

#define cone_area(radius, length) (PI*(radius)*sqrt(SQR(radius) + SQR(length)))
float cylinder_area(float radius, float length);
#define sphere_area(radius) (4*PI*SQR(radius))

void make_box(vector4f *in, vector4f *out);

// ********************************************************************
//                                                      collision stuff

#define X          0
#define Y          1
#define Z          2

#define CYL_TOP         0
#define CYL_BOTTOM      1
#define CYL_SIDE        3

#define dist_pt2plane(pt, plane) (dotproduct3(pt, plane) + plane[3])

#define query_point_point_intersect(pt1, pt2) (abs(pt1[0]-pt2[0])<CORRECT && abs(pt1[1]-pt2[1])<CORRECT && abs(pt1[2]-pt2[2])<CORRECT)
int query_point_sphere_intersect(float *pt, float *center, float r);
int query_point_cpoly_intersect(float *pt, float *normal, int count, vector4f *vertex);
int query_point_poly_intersect(float *pt, float *normal, int count, vector4f *vertex);
int query_point_plane_intersect(float *pt, float *n);
int query_point_rect_intersect(float *pt, vector4f *n);

int query_poly_plane_intersect(vector4f *pt, int len, float *n);

int query_sphere_sphere_intersect(float *c1, float r1, float *c2, float r2);
int query_sphere_plane_intersect(float *c, float r, float *n);
int query_sphere_rect_intersect(float *c, float r, vector4f *n);

int query_plane_plane_intersect(float *n1, float *n2);
// #define query_plane_plane_intersect(n1, n2) (abs(n1[0]-n2[0]) < CORRECT && abs(n1[1]-n2[1]) < CORRECT && abs(n1[2]-n2[2]) < CORRECT)
int query_plane_rect_intersect(float *n, vector4f *p);

int query_rect_rect_intersect(vector4f *n1, vector4f *p1, vector4f *n2, vector4f *p2);

int line_rect_intersect(float *pt, float *v, vector4f *p, vector4f *n, float *flag);
int line_plane_intersect(float *plane, float *pt, float *vector, float *t);
int line_sphere_intersect(float radius, float *sphere, float *pt, float *vector, float *t);
int line_ellipse_intersect(float *scale, float *pt, float *vector, float *t);
int line_ellipse_intersecti(float *iscale, float *pt, float *vector, float *t);
int line_poly_intersect(float *pt, float *vector, float *plane, vector4f *vertex, int count, float *point);
int line_cpoly_intersect(float *pt, float *vector, float *plane, vector4f *vertex, int count, float *point);
int line2d_line2d_intersect(float *v1, float *v2, float *pt);

void reflect(float normal[], float vector[]);

int sphere_sphere_reflectn3vf(float *c1, float *c2, float *normal);
int sphere_plane_reflectn3vf(float *n, float *normal);
int sphere_rect_reflectn3vf(float *c, vector4f *p, vector4f *n, float *normal);

int rect_rect_reflectn3vf(vector4f *bouncep, vector4f *surfacep, vector4f *surfacen, float *normal);

int plane_rect_reflectn3vf(float *n, vector4f *surfacep, vector4f *surfacen, float *normal);

int line_cylinder_intersect(vector4f pt, vector4f v, float length, float radius, float *t, int *side);
int line_cone_intersect(vector4f pt, vector4f v, float length, float *radius, float *t, int *side);


// ********************************************************************
//                                                      fractal

typedef struct {
        float octaves, H, lacunarity;
        float *freq;
} fractal_params;


#define bias(a, b) (pow((a), log(b) / (-0.693147181)))

void  init_noise();
void init_freq(fractal_params *params);
float gain(float a, float b);
float noise1(float arg);
float noise2(float vec[2]);
float noise3(float vec[3]);
float noise(float vec[], int len);
float turbulence(float *v, float freq);

int bound_fBm(fractal_params *params, float offset, float *rmin, float *rmax);
float fBm(float *point, fractal_params *params);

int bound_fBm2(fractal_params *params, float offset, float *rmin, float *rmax);
float fBm2(float *point, fractal_params *params);

int bound_HeteroTerrain(fractal_params *params, float offset, float *rmin, float *rmax);
float Hetero_Terrain( float *point, fractal_params *params, float *offset);

int bound_HybridMultifractal(fractal_params *params, float offset, float *rmin, float *rmax);
float HybridMultifractal(float *point, fractal_params *params, float *offset, float *gain);

int bound_RidgedMultifractal(fractal_params *params, float offset, float *rmin, float *rmax);
float RidgedMultifractal(float *point, fractal_params *params, float offset, float *gain);

int bound_RidgedMultifractal2(fractal_params *params, float offset, float *rmin, float *rmax);
float RidgedMultifractal2(float *point, fractal_params *params, float offset, float gain);


//              ASM

#ifdef LINUX

#define F2I(x, y) __asm__( "flds %1\n\t" "fistpl %0\n\t"  : "=m" (*&x) : "m" (*&y))
#define WAIT() __asm__( "fwait\n\t" : : )

#endif

#ifdef DOS

#define F2I(x,y) _asm {fld y }; _asm {fistp x }
#define WAIT() _asm { fwait }

#endif

#endif
