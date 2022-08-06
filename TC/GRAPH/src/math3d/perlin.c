#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "matrix.h"

#define B  0x100
#define BSTRING "perlin.4"
#define BM 0xff

#define N  0x1000
#define NP 12   /* 2^N */
#define NM 0xfff


#define setup(v,b0,b1,r0,r1)\
        t = v + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;


static int     p[B + B + 2];
static vector3f g3[B + B + 2];          // need g1, g2, g3 cause they are normalized!!!!
static vector2f g2[B + B + 2];
static float    g1[B + B + 2];
static int      start = 1;

float gain(float a, float b) {

   if (a < 0.5) {
      if (a < .001)
         return 0.;

      return 0.5*bias(a + a, 1-b);
   }

   if (a > .999)
      return 1.;

   return 1. - 0.5*bias(2 - (a + a), 1-b);
}


void init_noise(void) {

   int i, j, k;
   FILE *iofile;
   int db = B+B;
   float b;

   start = 0;

   if ((iofile = fopen(BSTRING, "r")) != NULL) {

      k = db + 2;
      for (i=0; i<k; i++)
         fscanf(iofile, "%f %f %f", &g3[i][0], &g3[i][1], &g3[i][2]);
      for (i=0; i<k; i++)
         fscanf(iofile, "%f %f", &g2[i][0], &g2[i][1]);
      for (i=0; i<k; i++)
         fscanf(iofile, "%f", &g1[i]);
      for (i=0; i<k; i++)
         fscanf(iofile, "%d", &p[i]);

      fclose(iofile);
      return;
   }

   b = 1.0/B;

   for (i=0; i<B; i++) {
      p[i] = i;

      g1[i] = g2[i][0] = g3[i][0] = (float)((random() % db) - B);
      g2[i][1] = g3[i][1] = (float)((random() % db) - B);
      g3[i][2] = (float)((random() % db) - B);
      normalize3(g3[i]);
      normalize2(g2[i]);
      g1[i] *= b;
   }

   while (--i) {
      k = p[i];
      p[i] = p[j = random() % B];
      p[j] = k;
   }

   for (i=0, k=B+2, j=B; i < k; i++, j++) {
      p[j] = p[i];
      copyarray3(g3[j], g3[i]);
      copyarray2(g2[j], g2[i]);
      g1[j] = g1[i];
   }

   iofile = fopen(BSTRING, "w");        // save lattice

   k = db + 2;
   for (i=0; i<k; i++)
      fprintf(iofile, "%f %f %f\n", g3[i][0], g3[i][1], g3[i][2]);
   for (i=0; i<k; i++)
      fprintf(iofile, "%f %f\n", g2[i][0], g2[i][1]);
   for (i=0; i<k; i++)
      fprintf(iofile, "%f\n", g1[i]);
   for (i=0; i<k; i++)
      fprintf(iofile, "%d\n", p[i]);

   fclose(iofile);
}


float noise1(float arg) {

   int bx0, bx1;
   float rx0, rx1, sx, t, u, v;

   if (start)
      init_noise();

   setup(arg, bx0,bx1, rx0,rx1);

   sx = s_curve(rx0);

   u = rx0 * g1[ p[ bx0 ] ];
   v = rx1 * g1[ p[ bx1 ] ];

   return lerp(sx, u, v);
}


#define at2(rx,ry, q) ( rx * q[0] + ry * q[1] )

float noise2(float vec[2]) {

   int bx0, bx1, by0, by1, b00, b10, b01, b11;
   float rx0, rx1, ry0, ry1;
   float *q;
   float sx, sy, a, b, t, u, v;
   register int i, j;

   if (start)
      init_noise();

   setup(vec[0], bx0,bx1, rx0,rx1);
   setup(vec[1], by0,by1, ry0,ry1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   sx = s_curve(rx0);           // removing this gives about
   sy = s_curve(ry0);           //      6.25% increase in speed

   q = g2[ b00 ] ; u = at2(rx0,ry0, q);
   q = g2[ b10 ] ; v = at2(rx1,ry0, q);
   a = lerp(sx, u, v);

   q = g2[ b01 ] ; u = at2(rx0,ry1, q);
   q = g2[ b11 ] ; v = at2(rx1,ry1, q);
   b = lerp(sx, u, v);

   return lerp(sy, a, b);
}


float noise3(float vec[3]) {

   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   float rx0, rx1, ry0, ry1, rz0, rz1, sy, sz, a, b, c, d, t, u, v;
   float *q;
   register int i, j;

   if (start)
      init_noise();

   setup(vec[0], bx0,bx1, rx0,rx1);
   setup(vec[1], by0,by1, ry0,ry1);
   setup(vec[2], bz0,bz1, rz0,rz1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

   q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
   q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
   a = lerp(t, u, v);

   q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
   q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
   b = lerp(t, u, v);

   c = lerp(sy, a, b);

   q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
   q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
   a = lerp(t, u, v);

   q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
   q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
   b = lerp(t, u, v);

   d = lerp(sy, a, b);

   return lerp(sz, c, d);
}


float noise(float vec[], int len) {

   switch (len) {
      case 3:
         return noise3(vec);
      case 2:
         return noise2(vec);
      case 1:
         return noise1(vec[0]);
      default:
         return 0;
   }

}

// freq = high frequency, low freq defaulted to 1
float turbulence(float *v, float freq) {

   float t, ifreq, x;
   vector3f vec;


   ifreq = 1.0/freq;

   vec[0] = v[0] * freq;
   vec[1] = v[1] * freq;
   vec[2] = v[2] * freq;

   for (t=0; ifreq<=1. ; ifreq += ifreq) {
      x = noise3(vec);
      t += abs(x) * ifreq;
      smultarray3(vec, 0.5);
   }

   return t;
}


/*
        bound : max(freq[0], freq[(int)octaves+1])
*/
void init_freq(fractal_params *params) {

   int i;
   float frequency;
   int oct;

   if (params->freq != NULL)
      delete [] params->freq;

   oct = (int)params->octaves + 1;
   params->freq = new float[oct];

   frequency = 1.0;
   for (i=0; i<oct; i++) { // compute weight for each frequency
      params->freq[i] = pow( frequency, -params->H );
      frequency *= params->lacunarity;
   }

}


int bound_fBm(fractal_params *params, float offset, float *rmin, float *rmax) {

   int i, index;

   index = ((int)params->octaves) + 1;
   *rmax = 0;

   for (i=0; i<index; i++)
      *rmax += params->freq[i];

   *rmin = -*rmax;

   return 1;
}


/*
 * Procedural fBm evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *    ``H''  is the fractal increment parameter
 *    ``lacunarity''  is the gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm

   *NOTE: this is the same function as Ken Musgrave's "multifractal() add" function
        w/ offset = 0
 */
float fBm( float *point, fractal_params *params ) {

   float             value, remainder;
   int               i;
   float             p[3];
   int               j = (int)params->octaves;

   copyarray3(p, point);

   value = 0.0;            /* initialize vars to proper values */

   /* inner loop of spectral construction */
   for (i=0; i<=j; i++) {
      value += noise3( p ) * params->freq[i];
      smultarray3(p, params->lacunarity);
   } /* for */

   remainder = params->octaves - j;
   if ( remainder > CORRECT )      /* add in ``octaves''  remainder */
         /* ``i''  and spatial freq. are preset in loop above */
      value += remainder * noise3( p ) * params->freq[i];

   return value;

} /* fBm() */




int bound_fBm2(fractal_params *params, float offset, float *rmin, float *rmax) {

   return bound_fBm(params, offset, rmin, rmax);
}


/*
 * Procedural fBm evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *    ``H''  is the fractal increment parameter
 *    ``lacunarity''  is the gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm


        bound:  +/ ((int)octaves+1) * max (freq[0], freq[(int)octaves + 1])

 */
float fBm2(float *point, fractal_params *params) {

   float             value, remainder;
   int               i;
   float             p[3];
   int               j = (int)params->octaves;

   copyarray3(p, point);

   value = 0.0;            /* initialize vars to proper values */

   /* inner loop of spectral construction */
   for (i=0; i<=j; i++) {
      value += noise2( p ) * params->freq[i];
      smultarray3(p, params->lacunarity);
   } /* for */

   remainder = params->octaves - j;
   if ( remainder > CORRECT )      /* add in ``octaves''  remainder */
         /* ``i''  and spatial freq. are preset in loop above */
      value += remainder * noise2( p ) * params->freq[i];

   return value;

} /* fBm() */


/*

        bound[0]:       offset +/- 1
        bound[i]:       bound[i-1] + ((offset +/-1 ) * freq[i] * bound[i-1]

*/
int bound_HeteroTerrain(fractal_params *params, float offset, float *rmin, float *rmax) {

   int            i;
   float          temp;
   int            j = (int)params->octaves+1;

   temp = abs(offset);
   *rmax = temp + 1;

   /* spectral construction inner loop, where the fractal is built */
   for (i=1; i<=j; i++)
      *rmax += (temp + 1)*params->freq[i]**rmax;        // obtain displaced noise value

   *rmin = -*rmax;

   return 1;
}


/*
 * Heterogeneous procedural terrain function: stats by altitude method.
 * Evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *       ``H''  determines the fractal dimension of the roughest areas
 *       ``lacunarity''  is the gap between successive frequencies
 *       ``octaves''  is the number of frequencies in the fBm
 *       ``offset''  raises the terrain from `sea level'

   *NOTE: this is the same function as Ken Musgrave's "multifractal() mult" function
        w/ offset = 0
 */
float Hetero_Terrain( float *point, fractal_params *params, float offset) {

   float          value, increment, remainder;
   int            i;
   float          p[3];
   int            j = (int)params->octaves;

   copyarray3(p, point);

   /* first unscaled octave of function; later octaves are scaled */
   value = offset + noise3( p );

   /* spectral construction inner loop, where the fractal is built */
   for (i=1; i<=j; i++) {
      smultarray3(p, params->lacunarity);       /* raise spatial frequency */
      increment = noise3( p ) + offset; /* obtain displaced noise value */
      increment *= params->freq[i];             /* scale amplitude appropriately for this frequency */
      increment *= value;               /* scale increment by current `altitude' of function */
      value += increment;               /* add increment to ``value''  */
   } /* for */

   /* take care of remainder in ``octaves''  */
   remainder = params->octaves - j;
   if ( remainder > CORRECT ) {
      /* ``i''  and spatial freq. are preset in loop above */
      /* note that the main loop code is made shorter here */
      /* you may want to that loop more like this */
      /* raise spatial frequency */
      smultarray3(p, params->lacunarity);       /* raise spatial frequency */
      increment = (noise3( p ) + offset) * params->freq[i];
      value += remainder * increment * value;
   }

   return value;

} /* Hetero_Terrain() */


/*

        bound:  (offset+/-1)*max(freq[0], freq[octaves+1])*(octaves + 1)

*/
int bound_HybridMultifractal(fractal_params *params, float offset, float *rmin, float *rmax) {

   int   index;
   int   i;
   float fmax, fmin;

   index = ((int)params->octaves) + 1;

   *rmax = 1;
   *rmin = 1;

   fmax = offset + 1;
   fmin = offset - 1;

   for (i=1; i<index; i++)
      *rmin += params->freq[i];

   *rmax = *rmin * fmax;
   *rmin *= fmin;

   return 1;
}


/* Hybrid additive/multiplicative multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           0.25
 *      offset:      0.7

        bound:  (offset+/-1)*max(freq[0], freq[octaves+1])*(octaves + 1)

 */
float HybridMultifractal(float *point, fractal_params *params, float offset, float gain) {

   float          result, signal, weight, remainder;
   int            i;
   float          p[3];
   int            j = (int)params->octaves;

   copyarray3(p, point);

   /* get first octave of function */
   result = ( noise3( p ) + offset ) * params->freq[0];
   weight = gain * result;

   /* spectral construction inner loop, where the fractal is built */
   for (i=1; i<=j; i++) {
      /* prevent divergence */
      if (weight <= 0.0)
         return result;
      if ( weight > 1.0 )
         weight = 1.0;

      /* increase frequency */
      smultarray3(p, params->lacunarity);

      /* get next higher frequency */
      signal = ( noise3( p ) + offset ) * params->freq[i];

      /* add it in, weighted by previous freq's local value */
      result += weight * signal;

      /* update the (monotonically decreasing) weighting value */
      /* (this is why H must specify a high fractal dimension) */
      weight *= gain * signal;
   } /* for */

   /* take care of remainder in ``octaves''  */
   remainder = params->octaves - j;
   if (remainder > CORRECT) {
      if (weight <= 0.0)
         return result;
      if ( weight > 1.0 )
         weight = 1.0;

      /* ``i''  and spatial freq. are preset in loop above */
      smultarray3(p, params->lacunarity);

      signal = ( noise3( p ) + offset ) * params->freq[i];
      result += remainder * weight * signal;
   }

   return result;

} /* HybridMultifractal() */


/* Ridged multifractal terrain model.

        bound: SQR(offset-1/0) *max(freq[0], freq[octaves+1]) *((int)octaves+1)

*/
int bound_RidgedMultifractal(fractal_params *params, float offset, float *rmin, float *rmax) {

   int   index;
   float temp;
   int   i;
   float fmax, fmin;

   index = ((int)params->octaves) + 1;

   fmax = SQR(offset);
   fmin = offset-1;
   fmin = SQR(fmin);

   if (fmin > fmax) {
      temp = fmin;
      fmin = fmax;
      fmax = temp;
   }

   *rmin = 1;

   for (i=1; i<index; i++)
      *rmin += params->freq[i];

   *rmax = *rmin * fmax;
   *rmin *= fmin;

   return 1;
}


/* Ridged multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           1.0 | 0.9
 *      offset:      1.0 | 1
 *      gain:        2.0 | 2
*/
float RidgedMultifractal(float *point, fractal_params *params, float offset, float gain) {

   float          result, signal, weight;
   int            i;
   float          p[3];
   float          remainder;
   int            j = (int)params->octaves;

   copyarray3(p, point);

   /* get first octave */
   /* get absolute value of signal (this creates the ridges) */
   /* invert and translate (note that "offset" should be ~= 1.0) */
   signal = noise3(p);
   signal = offset - abs(signal);

   signal *= signal;                    /* square the signal, to increase "sharpness" of ridges */

   result = signal;                     /* assign initial values */

   for( i=1; i<=j; i++ ) {
      weight = signal * gain;           /* weight successive contributions by previous signal */

      if (weight <= 0.0)
         return result;

      if ( weight > 1.0 )
         weight = 1.0;

      smultarray3(p, params->lacunarity);       /* increase the frequency */

      signal = noise3(p);
      signal = offset - abs(signal);
      signal *= signal;

      signal *= weight;                 /* weight the contribution */
      result += signal * params->freq[i];
   }

   /* take care of remainder in ``octaves''  */
   remainder = params->octaves - j;
   if ( remainder > CORRECT ) {

      weight = signal * gain;

      if (weight <= 0.0)
         return result;

      if ( weight > 1.0 )
         weight = 1.0;

      /* ``i''  and spatial freq. are preset in loop above */
      smultarray3(p, params->lacunarity);

      signal = noise3(p);
      signal = offset - abs(signal);
      signal *= signal;                 /* weight the contribution */
      signal *= weight;
      result += remainder*signal * params->freq[i];
   }

   return result;

} /* RidgedMultifractal() */


int bound_RidgedMultifractal2(fractal_params *params, float offset, float *rmin, float *rmax) {

   return bound_RidgedMultifractal(params, offset, rmin, rmax);
}


/* Ridged multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           1.0 | 0.9
 *      offset:      1.0 | 1
 *      gain:        2.0 | 2
 */
float RidgedMultifractal2(float *point, fractal_params *params, float offset, float gain) {

   float          result, signal, weight;
   int            i;
   float          p[3];
   float          remainder;
   int            j = (int)params->octaves;

   copyarray2(p, point);

   /* get first octave */
   /* get absolute value of signal (this creates the ridges) */
   /* invert and translate (note that "offset" should be ~= 1.0) */
   signal = noise2(p);
   signal = offset - abs(signal);

   /* square the signal, to increase "sharpness" of ridges */
   signal *= signal;

   /* assign initial values */
   result = signal;

   for( i=1; i<=j; i++ ) {

      /* weight successive contributions by previous signal */
      weight = signal * gain;

      if (weight <= 0.0)
         return result;

      if ( weight > 1.0 )
         weight = 1.0;

      /* increase the frequency */
      p[0] *= params->lacunarity;
      p[1] *= params->lacunarity;

      signal = noise2(p);
      signal = offset - abs(signal);

      /* sqr signal and weight the contribution */
      signal *= signal * weight;
      result += signal * params->freq[i];
   }

   // take care of remainder in ``octaves'' : remainder adds ~ 13.5% to rendering time

   remainder = params->octaves - j;
   if ( remainder > CORRECT ) {

      weight = signal * gain;

      if (weight <= 0.0)
         return result;

      if ( weight > 1.0 )
         weight = 1.0;

      // ``i''  and spatial freq. are preset in loop above
      p[0] *= params->lacunarity;
      p[1] *= params->lacunarity;

      signal = noise2(p);
      signal = offset - abs(signal);
      // sqr signal and weight the contribution
      result += SQR(signal) * weight * params->freq[i] * remainder;
   }

   return result;

} /* RidgedMultifractal2() */

