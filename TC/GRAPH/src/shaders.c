
#include "shader.h"
#include "sbfast.h"

/***************************************************************************
****************************************************************************/
float grey_marble_color(float value) {

   if (value < 0.4)
      return 1 - 0.571429*value;
   if (value < 0.8)
      return 1.125 - 0.75 * value;

   return 1.7866336 - 1.4851485 * value;
}


/***************************************************************************
****************************************************************************/
float sub_marble_color(float value) {

   if (0.8 <= value)
      return -value + 1.2;

   return -0.5*value + 0.8;
}


/***************************************************************************
****************************************************************************/
float main_marble_color(float value) {

   if (value < 0.3)
      return 1.0;
   if (value < 0.7)
      return -1.0*value + 1.3;
   if (value < 0.9)
      return -0.75*value + 0.315;

   return -0.015*value + 0.315;
}


/***************************************************************************
* marble - calculate marble color for given point.
*
* Input : float point[3]
*
* Output: float color[3]
*
* Fc call: turbulence, marble_color;
****************************************************************************/
int marble (void *x) {

//   float  size = 2;             // high freq
   float  isize = 0.5;
   float  ans;
   int    freq = 13;
   vector4f point;
   float temp;

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   point[0] = data->in[0] * isize;
   point[1] = data->in[1] * isize;
   point[2] = data->in[2] * isize;
   point[3] = 1.0;

//   out[0] = main_marble_color(SIN((point[0]+turbulence(point, size)) * freq * PI));
//   out[1] = sub_marble_color(SIN((point[0]+turbulence(point, size)) * freq * PI));

   temp = (point[0]+turbulence(point, 300)) * freq * PI;
   ans = grey_marble_color(SIN(temp));

   smultarray3(data->out, ans);

   return 1;
}


/***************************************************************************
****************************************************************************/
int wood (void *x) {

   float  size = 2;             // high freq
   float  isize = 0.5;
   float  ans;
   int    freq = 13;
   float  point[4];
   float  temp;

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   point[0] = data->in[0] * isize;
   point[1] = data->in[1] * isize;
   point[2] = data->in[2] * isize;
   point[3] = 1;

//   out[0] = main_marble_color(SIN((point[0]+turbulence(point, size)) * freq * PI));
//   out[1] = sub_marble_color(SIN((point[0]+turbulence(point, size)) * freq * PI));

   temp = (point[0]+turbulence(point, size)) * freq * PI;
   ans = main_marble_color(SIN(temp));

   data->out[0] *= ans*1.0;
   data->out[1] *= ans*0.6;
   data->out[2] *= ans*0.30196;

   return 1;
}


/***************************************************************************
****************************************************************************/
int wood_panel (void *x) {

   float  size = 2;             // high freq
   float  isize = 0.5;
   float  ans;
   int    freq = 13;
   float  point[4];
   float  temp;

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   point[0] = data->in[0] + 0.005 - round0(data->in[0]/4.0)*4.0;         // 4x1x8
   point[1] = data->in[1] + 0.005 - round0(data->in[1]);
   point[2] = data->in[2] + 0.005 - round0(data->in[2]/8.0)*8.0;

   if (point[0] < 0)
      point[0] = 4.0 + point[0];

   if (point[1] < 0)
      point[1] = 1.0 + point[1];

   if (point[2] < 0)
      point[2] = 8.0 + point[2];

   if ((point[0] > 0.01 && point[0] < 0.2) ||
       (point[1] > 0.01 && point[1] < 0.2) ||
       (point[2] > 0.01 && point[2] < 0.2)) {
      smultarray3(data->out, 0.1);

      return 1;
   }

   smultarray3(point, isize);

   temp = (point[0]+turbulence(point, size)) * freq * PI;
   ans = main_marble_color(SIN(temp));

   data->out[0] *= ans*1.0;
   data->out[1] *= ans*0.6;
   data->out[2] *= ans*0.30196;

   return 1;
}


/***************************************************************************
****************************************************************************/
int wave(void *x) {

   float value;

   shaderparamtype *data;

   data = (shaderparamtype *)x;
   value = turbulence(data->in, 5.745);
   saddarray3(data->normal, value);
   normalize3(data->normal);

   return 1;
}


/***************************************************************************
****************************************************************************/
int checker (void *x) {

   int value;
   shaderparamtype *data;

   data = (shaderparamtype *)x;

/*
   value = abs((int)data->in[0]);
   value += abs((int)data->in[0]);
   value += abs((int)data->in[0]);
*/

   value = abs((int)data->in[0]) + abs((int)data->in[1]) + abs((int)data->in[2]);

   if (value & 0x01)
      data->out[0] = data->out[1] = data->out[2] = 0;

   return 1;
}


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7

fractal_params moon_params = { OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void moon_init () {

   init_freq(&moon_params);
}


/***************************************************************************
****************************************************************************/
int moon (void *x) {

   float high[3] = {0,0.7,0.8};
   float basehi[3] = {0.1,0.3,0.2};

   float low[3]  = {0.1,0.4,0.3};
   float baselow[3] = {0.1,0.4, 0.6};
   float value;

   float threshold = 0.0;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0] + data->in[0];
   p[1] = data->in[1] + data->in[1];
   p[2] = data->in[2] + data->in[2];

   value = fBm(p, &moon_params);

   if ( value > threshold) {
      data->out[0] *= (basehi[0] + high[0]*value);
      data->out[1] *= (basehi[1] + high[1]*value);
      data->out[2] *= (basehi[2] + high[2]*value);
      return 1;
   }

   data->out[0] *= (baselow[0] + low[0]*value);
   data->out[1] *= (baselow[1] + low[1]*value);
   data->out[2] *= (baselow[2] + low[2]*value);

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params planet2_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void planet2_init () {

   init_freq(&planet2_params);
}


/***************************************************************************
****************************************************************************/
int planet2 (void *x) {

   float high[3] = {0,1.254,0.2};
   float basehi[3] = {0.429,0.373,0.135};

   float high2[3] = {0.196,0.765,0};

   float low[3]  = {0.1,0.4,0.3};
   float baselow[3] = {0.1,0.4, 0.6};

   float value;
   float temp;

   float threshold = -0.134544;
   float threshold2 = 0.134544;
   float threshold3 = 0.764352;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0]*2.9987983;
   p[1] = data->in[1]*2.9987983;
   p[2] = data->in[2]*2.9987983;

   value = fBm(p, &planet2_params);
   temp  = data->in[1] + 0.3*value;

   if (abs(temp) > 0.7) {
      value = 0.25*(3+value);
      smultarray3(data->out, value);
   }

   else
      if ( value > threshold) {
         if (value < threshold2) {
            multarray3(data->out, basehi);
         }

         else if (value < threshold3) {
            data->out[0] *= (basehi[0] + high[0]*value);
            data->out[1] *= (basehi[1] + high[1]*value);
            data->out[2] *= (basehi[2] + high[2]*value);
         }

         else {
            data->out[0] *= (basehi[0] + high2[0]*value);
            data->out[1] *= (basehi[1] + high2[1]*value);
            data->out[2] *= (basehi[2] + high2[2]*value);
         }

      }

      else {
         data->out[0] *= (baselow[0] + low[0]*value);
         data->out[1] *= (baselow[1] + low[1]*value);
         data->out[2] *= (baselow[2] + low[2]*value);
      }

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params planet_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void planet_init () {

   init_freq(&planet_params);
}


/***************************************************************************
****************************************************************************/
int planet (void *x) {

   float high[3] = {0,1.254,0.2};
   float basehi[3] = {0.429,0.373,0.135};

   float high2[3] = {0.196,0.765,0};

   float low[3]  = {0.1,0.4,0.3};
   float baselow[3] = {0.1,0.4, 0.6};

   float value;

   float threshold = 0;
   float threshold2 = 0.134544;
   float threshold3 = 0.764352;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0]*9.9987983;
   p[1] = data->in[1]*9.9987983;
   p[2] = data->in[2]*9.9987983;

   value = fBm(p, &planet_params);

   if ( value > threshold) {
      if (value < threshold2) {
         multarray3(data->out, basehi);
      }

      else if (value < threshold3) {
         data->out[0] *= (basehi[0] + high[0]*value);
         data->out[1] *= (basehi[1] + high[1]*value);
         data->out[2] *= (basehi[2] + high[2]*value);
      }

      else {
         data->out[0] *= (basehi[0] + high2[0]*value);
         data->out[1] *= (basehi[1] + high2[1]*value);
         data->out[2] *= (basehi[2] + high2[2]*value);
      }

   }

   else {
      data->out[0] *= (baselow[0] + low[0]*value);
      data->out[1] *= (baselow[1] + low[1]*value);
      data->out[2] *= (baselow[2] + low[2]*value);
   }

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params cloud_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void cloud_init () {

   init_freq(&cloud_params);
}


/***************************************************************************
****************************************************************************/
int cloud (void *x) {

   float value;

   float high     = 0.4;
   float basehigh = 0.175;
   float low      = 0.175;
   float baselow  = 0.15;

   float threshold = 0.0;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0]*0.09472388;
   p[1] = data->in[1]*0.09472388;
   p[2] = data->in[2]*0.09472388;

   value = fBm(p, &cloud_params);

   if (value > threshold)
      value = basehigh + high*value;
   else
      value = baselow + low*value;

   smultarray3(data->out, value);

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params cloud2_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void cloud2_init () {

   init_freq(&cloud2_params);
}


/***************************************************************************
        Note: planetclouds() seems to be better visually than cloud2()
****************************************************************************/
int cloud2 (void *x) {

   float value;

   float high     = 0.4;
   float basehigh = 0.475;
   float low      = 0.275;
   float baselow  = 0.15;

   float threshold = 0.1;

   float p[3];
   shaderparamtype *data;

   marble(x);

   data = (shaderparamtype *)x;

   copyarray3(p, data->in);

   value = fBm(p, &cloud2_params);

   if (value > threshold)
      value = basehigh + high*value;
   else
      value = baselow + low*value;

   smultarray3(data->out, value);

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params warp_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void warp_init () {

   init_freq(&warp_params);
}


/***************************************************************************
****************************************************************************/
int warp (void *x) {

   float value;

   float threshold = 0.6;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   copyarray3(p, data->in);

   value = fBm(p, &warp_params);

   if (value < threshold) {
      data->out[0] = data->out[1] = data->out[2] = 0;
      return 1;
   }

   if ((p[2]=abs(data->in[2])) > 60)
      value *= 60/p[2];

   smultarray3(data->out, value);

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params stars_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void stars_init () {

   init_freq(&stars_params);
}


/***************************************************************************
****************************************************************************/
int stars (void *x) {

   float value;
   float threshold = 0.6;
   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = abs(data->in[0]);
   p[1] = data->in[1];
   p[2] = data->in[2];

   value = fBm(p, &stars_params);

   if (value < threshold) {
      data->out[0] = data->out[1] = data->out[2] = 0;
      return 1;
   }

   p[0] += value;
   p[1] -= value;
   p[2] += value;

   value = fBm(p, &stars_params);

   if (value < threshold) {
      data->out[0] = data->out[1] = data->out[2] = 0;
      return 1;
   }

   value = abs(value);

   if (value > 1)
      value -= (int)value;

   if ((p[2]=-data->pt[2]) > 100)
      value *=100/p[2];
   else
      value += 0.5*(1-value);

   smultarray3(data->out, value);

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params terrain_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void terrain_init () {

   init_freq(&terrain_params);
}


/***************************************************************************
****************************************************************************/
int terrain (void *x) {

   float value;

   vector3f p, p2;
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0]*0.0002;
   p[1] = data->in[1]*0.0002;
   p[2] = abs(data->in[2])*0.0005;                      // remove negative to get bluemoon.rgb

   p[2] *= 2000 * (1 + 0.732743*fBm(p, &terrain_params));

   if (p[2] < 100) {            // water
      p[2] *= 0.01;

      value = fBm(p, &terrain_params);
      value = abs(value);

      data->out[0] *= 0.1;
      data->out[1] *= 0.3 - 0.2*value;
      data->out[2] *= 0.6 - 0.25*value;
      return 0;
   }

   copyarray3(p2, data->in);

   smultarray3(data->in, 0.0005);

   marble(data);

   copyarray3(data->in, p2);

   if (p[2] > 1600) {           // snow
      p[2] *= 0.0005;

      value = fBm(p, &terrain_params);
      value = abs(value);

      value = 0.5 * (1+ value);
      smultarray3(data->out, value);

      return 1;
   }

   if (p[2] > 1000) {           // red rock
      p[2] *= 0.000625;

      value = fBm(p, &terrain_params);
      value = abs(value);

      data->out[0] *= 0.5*(0.6+value);
      data->out[1] *= 0.1*(0.6+value);
      data->out[2] *= 0.1;
      return 1;
   }

   if (p[2] > 200) {            // vegetation
      p[2] *= 0.001;

      value = fBm(p, &terrain_params);
      value = abs(value);

      data->out[0] *= 0.5*(1-value);
      data->out[1] *= 0.5*(1+value);
      data->out[2] *= 0.1;
      return 1;
   }

   p[2] *= 0.005;               // sand

   value = fBm(p, &terrain_params);
   value = abs(value);

   value = 0.3 * (1+value);

   data->out[0] *= value;
   data->out[1] *= value;
   data->out[2] = 0;

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params terrain2_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void terrain2_init () {

   init_freq(&terrain2_params);
}


/***************************************************************************
****************************************************************************/
int terrain2 (void *x) {

   float value;

   float p[3], p2[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0]*0.002;
   p[1] = data->in[1]*0.002;
   p[2] = data->in[2]*0.006666667;                      // remove negative to get bluemoon.rgb

   p[2] *= 150 * (1 + 0.732743*fBm(p, &terrain2_params));

   if (p[2] < -0.09) {            // water deep
      p[2] *= 0.006666667;

      value = fBm(p, &terrain2_params);
      value = abs(value);

      data->out[0] *= 0.02;
      data->out[1] *= 0.14 - 0.09*value;
      data->out[2] *= 0.28 - 0.12*value;

      return 1;
   }

   if (p[2] < 0.01) {            // water
      p[2] *= 0.006666667;

      value = fBm(p, &terrain2_params);
      value = abs(value);

      data->out[0] *= 0.1;
      data->out[1] *= 0.3 - 0.2*value;
      data->out[2] *= 0.6 - 0.25*value;

      copyarray3(p2, data->in);

      smultarray3(data->in, 3.5473);
      wave(data);

      copyarray3(data->in, p2);

      return 1;
   }

   copyarray3(p2, data->in);

   data->in[0] *= 0.0005;
   data->in[1] *= 0.0005;
   data->in[2] *= 0.001;

   marble(data);

   copyarray3(data->in, p2);

   if (p[2] > 110) {           // snow
      p[2] *= 0.006666667;

      value = fBm(p, &terrain2_params);
      value = 0.5 * (1+abs(value));

      smultarray3(data->out, value);

      return 1;
   }

   if (p[2] > 70) {           // red rock
      p[2] *= 0.009090909;

      value = fBm(p, &terrain2_params);
      value = abs(value);

      data->out[2] *= 0.1 * (1+ value);

      value = 0.08 * (1+ value);

      data->out[0] *= value;
      data->out[1] *= value;

      return 1;
   }

   if (p[2] > 10) {            // vegetation
      p[2] *= 0.014285714;

      value = fBm(p, &terrain2_params);
      value = abs(value);

      data->out[0] *= 0.5*(1-value);
      data->out[1] *= 0.5*(1+value);
      data->out[2] *= 0.1;
      return 1;
   }

   p[2] *= 0.1;               // sand

   value = fBm(p, &terrain2_params);
   value = abs(value);
   value = 0.3 * (1+ value);

   data->out[0] *= value;
   data->out[1] *= value;
   data->out[2] = 0;

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params terrain3_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void terrain3_init () {

   init_freq(&terrain3_params);
}


/***************************************************************************
****************************************************************************/
int terrain3 (void *x) {

   float value;

   float p[3], p2[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;

   p[0] = data->in[0]*0.002;
   p[1] = data->in[1]*0.002;
   p[2] = data->in[2]*0.0066667;                      // remove negative to get bluemoon.rgb

   p[2] *= 150 * (1 + 0.732743*fBm(p, &terrain3_params));

   if (p[2] < -0.09) {            // water deep
      p[2] *= 0.0066667;

      value = fBm(p, &terrain3_params);
      value = abs(value);

      data->out[0] *= 0.02;
      data->out[1] *= 0.14 - 0.09*value;
      data->out[2] *= 0.28 - 0.12*value;

      return 1;
   }

   if (p[2] < 0.01) {            // water
      p[2] *= 0.0066667;

      value = fBm(p, &terrain3_params);
      value = abs(value);

      data->out[0] *= 0.1;
      data->out[1] *= 0.3 - 0.2*value;
      data->out[2] *= 0.6 - 0.25*value;

      return 1;
   }

   copyarray3(p2, data->in);

   data->in[0] *= 0.0005;
   data->in[1] *= 0.0005;
   data->in[2] *= 0.001;

   marble(data);

   copyarray3(data->in, p2);

   if (p[2] > 100) {           // snow
      p[2] *= 0.0066667;

      value = fBm(p, &terrain3_params);
      value = 0.5 * (1+ abs(value));

      smultarray3(data->out, value);

      return 1;
   }

   if (p[2] > 80) {
      p[2] *= 0.01;

      value = fBm(p, &terrain3_params);
      value = abs(value);

      data->out[0] *= 0.73 + 0.27*value;
      data->out[1] *= 0.75 + 0.25*value;
      data->out[2] *= 0.69 + 0.31*value;
      return 1;
   }

   if (p[2] > 60) {
      p[2] *= 0.0125;

      value = fBm(p, &terrain3_params);
      value = abs(value);

      data->out[0] *= 0.79 - 0.06*value;
      data->out[1] *= 0.75;
      data->out[2] *= 0.59 + 0.10*value;
      return 1;
   }

   if (p[2] > 40) {
      p[2] *= 0.0166667;

      value = fBm(p, &terrain3_params);
      value = abs(value);

      data->out[0] *= 0.70 + 0.09*value;
      data->out[1] *= 0.61 + 0.14*value;
      data->out[2] *= 0.36 + 0.23*value;
      return 1;
   }

   if (p[2] > 20) {
      p[2] *= 0.025;

      value = fBm(p, &terrain3_params);
      value = abs(value);

      data->out[0] *= 0.34 + 0.36*value;
      data->out[1] *= 0.65 - 0.04*value;
      data->out[2] *= 0.04 + 0.32*value;
      return 1;
   }

   p[2] *= 0.05;

   value = fBm(p, &terrain3_params);
   value = abs(value);

   data->out[0] *= 0.29 + 0.06*value;
   data->out[1] *= 0.36 + 0.29*value;
   data->out[2] = 0.04*value;

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params cave_floor_params = {OCTAVES, HH, LACUNARITY, NULL};
static float terrain_shades[3][3] = {   1, 0, 0,
                                        0.628, 0, 0.08,
                                        1, 0, 0.236};


/***************************************************************************
****************************************************************************/
void cave_floor_init () {

   init_freq(&cave_floor_params);
}


/***************************************************************************
****************************************************************************/
int cave_floor (void *x) {

   float value;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;
                        // calc general color
   p[0] = data->in[0]*0.002;
   p[1] = data->in[1]*0.002;
   p[2] = data->in[2]*0.006666667;                      // remove negative to get bluemoon.rgb

   p[2] *= 1 + 0.732743*fBm(p, &cave_floor_params);

   value = fBm(p, &cave_floor_params);
   value = abs(value);
                                // red bands
   switch (((int)(value*6.0)) % 6)      {
      case 0:
         data->out[0] *= value*terrain_shades[0][0];
         data->out[1] *= value*terrain_shades[0][1];
         data->out[2] *= value*terrain_shades[0][2];
         return 1;

      case 2:
         data->out[0] *= value*terrain_shades[1][0];
         data->out[1] *= value*terrain_shades[1][1];
         data->out[2] *= value*terrain_shades[1][2];
         return 1;

      default:
         data->out[0] *= value*terrain_shades[2][0];
         data->out[1] *= value*terrain_shades[2][1];
         data->out[2] *= value*terrain_shades[2][2];
         return 1;
   }

}


#undef HH
#undef LACUNARITY
#undef OCTAVES


#define HH              0.632
#define LACUNARITY      2
#define OCTAVES         7


fractal_params cave_ceiling_params = {OCTAVES, HH, LACUNARITY, NULL};

/***************************************************************************
****************************************************************************/
void cave_ceiling_init () {

   init_freq(&cave_ceiling_params);
}


/***************************************************************************
****************************************************************************/
int cave_ceiling (void *x) {

   float value;

   float p[3];
   shaderparamtype *data;

   data = (shaderparamtype *)x;
                        // calc general color
   p[0] = data->in[0]*0.002;
   p[1] = data->in[1]*0.002;
   p[2] = data->in[2]*0.006666667;                      // remove negative to get bluemoon.rgb

   p[2] *= 1 + 0.732743*fBm(p, &cave_ceiling_params);

   value = fBm(p, &cave_ceiling_params);
   value = abs(value);

   switch (((int)(value*6.0)) % 6)      {
      case 0:
         value *= 0.5;
         data->out[0] *= value;
         data->out[1] *= value;
         data->out[2] *= value*1.2;
         break;

      case 2:
         value *= 0.5;
         data->out[0] *= value*1.1;
         data->out[1] *= value;
         data->out[2] *= value*1.2;
         break;

      default:
         value *= 0.5;
         smultarray3(data->out, value);
         break;
   }

   return 1;
}


#undef HH
#undef LACUNARITY
#undef OCTAVES


/***************************************************************************
****************************************************************************/
int planetclouds (void *x) {

   float omega = 0.7;
   float lambda = 2;
   int octaves = 9;

   vector3f PP, temp;           /* Point after distortion */
   float o, a, b;               /* Loop control for fractal sum */

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   copyarray3(PP, data->in);

   /* Add in "distortion" vector */
   a = noise3(PP);
   saddarray3(PP, a);

        /* Second cirrus: replace DNoise with vector fBm */

   /* Compute VLfBm */
   o = 1;  a = 0;
   for (; octaves > 0  &&  o >= CORRECT;  octaves--) {
      b = noise3(PP);
      temp[0] = PP[0] + b;
      temp[1] = PP[1] + b;
      temp[2] = PP[2] + b;

      a += o * noise3(temp);

      smultarray3(PP, lambda);
      o *= omega;
   }

   if (a < 0)
      a = 0;
   else
      a += 0.3;

   if (a > 1)
      a = 1;

   smultarray3(data->out, a);
   return 1;
}


/***************************************************************************
****************************************************************************/
int contour (void *x) {

   float omega = 0.7;
   float lambda = 2;
   int octaves = 9;

  vector3f PP;                  /* Point after distortion */
  float o, a, b;                /* Loop control for fractal sum */

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   copyarray3(PP, data->in);

   /* Add in "distortion" vector */
   a = noise3(PP);
   saddarray3(PP, a);
   b = noise3(PP);

        /* Second cirrus: replace DNoise with vector fBm */

   /* Compute VLfBm */
   o = 1;  a = 0;
   for (; octaves > 0  &&  o >= CORRECT;  octaves--) {
      a += o*noise1(b+noise1(b));
      b *= lambda;
      o *= omega;
   }

   if (a < 0)
      a = 0;
   else
      a += 0.3;

   if (a > 1)
      a = 1;

   smultarray3(data->out, a);
   return 1;
}


/*-------------------------------------------------------------------------
 *      carpet - creates a carpeted surface with variable nap and scuffing.
 *      Makes a reasonable attempt at anti-aliasing.
 *
 *      Kd, Ka - the usual meaning
 *      scuff - the "amount of scuffing": footprints, vacuum cleaner marks,
 *         etc. (the relative frequency of intensity variation)
 *      nap - the "shagginess" of the carpet (size of the speckles)
 *-------------------------------------------------------------------------*/
int carpet (void *x) {

   shaderparamtype *data;
   float    scuff = 1.0, nap = 1.0;
   vector3f PP;
   float    pixelsize, twice, scale, iscale, weight, turb = 0, napfreq,
            speckle, mottling;
   float    temp;
   vector3f tempv;
   float    horizon = 7.85, ihorizon = 0.12738854;

   data = (shaderparamtype *)x;

        /* large-scale mottling */
   if (scuff < CORRECT)
      mottling = 1.0;
   else {

        /* get pixel size */
      temp = scuff * 0.5;
      PP[0] = data->in[0]*temp; /* frequency adjustment (S-shaped curve) */
      PP[1] = data->in[1]*temp;
      PP[2] = data->in[2]*temp;

      pixelsize = temp/abs(data->pt[2]);
      twice = pixelsize + pixelsize;

        // create some turbulence  - similar to turbulence(), but need variables and goes backwards...
      copyarray3(tempv, PP);
      for (scale = iscale = 1; scale > twice; scale *= 0.5, iscale += iscale) {
         temp = noise3(tempv);
         turb += abs(temp) * scale;
         addarray3(tempv, tempv);
      }

        /* gradual fade out of highest frequency component near limit */
      if (scale > pixelsize) {
         weight = (scale/pixelsize) - 1;
         weight = clamp(weight, 0, 1);

         temp = noise3(tempv);
         turb += abs(temp) * scale * weight;
      }

        /* scuffing will be between 0.5 and 1 */

      turb = 0.5*(turb + 1.0);
      mottling = min(turb, 1);
   }

   pixelsize = abs(data->pt[2]);        // distance factor

        /* small-scale speckling */
   if (pixelsize > horizon + horizon)
      speckle = 0;
   else if (nap < CORRECT)
      speckle = 1;
   else {
        /* get high-frequency speckles */
      napfreq = 84.456/nap;     /* scale up speckle frequency */

      tempv[0] = data->in[0]*napfreq;
      tempv[1] = data->in[1]*napfreq;
      tempv[2] = data->in[2]*napfreq;

      speckle = noise3(tempv);
      speckle = abs(speckle);

        /* two-level speckles */
      speckle = (speckle <= 0.25) ? 0.2 : 1;

        /* smooth out speckles as they get smaller */
      if (speckle<1-CORRECT) {
         temp = pixelsize/napfreq;
         temp += temp;
         speckle = smoothstep(temp, speckle, 1);
      }

   }

   speckle = sqrt(speckle);

   temp = pixelsize - horizon;

   if (temp <= 0)
      temp = speckle;
   else if (speckle <= mottling) {
      temp *= ihorizon;
      temp = smoothstep(temp, speckle, mottling);
   }

   else {
      temp = 1-temp*ihorizon;
      temp = smoothstep(temp, mottling, speckle);
   }

   smultarray3(data->out, temp);

   return 1;
}


// -------------------------- ATMOSPHERE ---------------------------


/***************************************************************************
        note: this is an alpha blending shader - should be the last
                shader called...
****************************************************************************/
int fog_mus(void *x) {

   float ray_dir[3];
   float t_e;                           // distance to object (i think)
   float alpha[3];
   float beta;
   float bsqrt;
   float b = 1;                         // fallout w/ altitude
   float a = 0.5;                       // average fog density
   float near_val, far_val;
   float origin[3] = {0, 0, -100};
   float rc_square;
   float tau[3];

   shaderparamtype *data;

   data = (shaderparamtype *)x;

                // compute distance from fog origin to rays closest approach
   alpha[0] = -origin[0];
   alpha[1] = -origin[1];
   alpha[2] = -origin[2];

   copyarray3(ray_dir, data->pt);

   t_e = normalize3(ray_dir);

   beta = dotproduct3(ray_dir, alpha);    // calc fallout w/ altitude
   beta = abs(beta);

   bsqrt = sqrt(b);
   near_val = erf(bsqrt*beta);
   far_val = erf(bsqrt*(beta+t_e));

                // comput dist from fog origin to rays closest approach

   rc_square = dotproduct3(alpha, alpha) - SQR(beta);

   tau[0] = a/bsqrt*exp(-b*rc_square)*(far_val-near_val);
   tau[1] = tau[0]*2.25;
   tau[2] = tau[0]*21.0;

   tau[0] = exp(-tau[0]);
   tau[1] = exp(-tau[1]);
   tau[2] = exp(-tau[2]);

   data->out[0] = data->out[0]*tau[0] + (1-tau[0]);     // alpha blend color with white
   data->out[1] = data->out[1]*tau[1] + (1-tau[1]);     // alpha blend color with white
   data->out[2] = data->out[2]*tau[2] + (1-tau[2]);     // alpha blend color with white

   return 1;
}


/***************************************************************************
        note: this is an alpha blending shader - should be the last
                shader called...
                based on height
****************************************************************************/
int fog(void *x) {

   float b = 0.007;                      // fallout w/ altitude
   float a = 0.0004;                     // average fog density
   float tau, taui;
   float distance;
   float height;

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   height = abs(data->pt[1]);

   distance = magnitude3(data->pt);

   tau = a*distance*exp(-b*height);
   if (tau > 1.0)
      tau = 1.0;

   taui = 1.0 - tau;

   saddarray3(data->add, tau);

   data->out[0] = data->out[0]*taui;     // alpha blend color with white
   data->out[1] = data->out[1]*taui;     // alpha blend color with white
   data->out[2] = data->out[2]*taui;     // alpha blend color with white

   return 1;
}


/***************************************************************************
        note: this is an alpha blending shader - should be the last
                shader called...

        based on distance
****************************************************************************/
int fog2(void *x) {

   float a = 0.0004;                     // average fog density
   float tau, taui;
   float distance;

   shaderparamtype *data;

   data = (shaderparamtype *)x;

   distance = magnitude3(data->pt);

   tau = exp(-a*distance);

   if (tau > 1.0)
      tau = 1.0;

   taui = 1.0 - tau;

   saddarray3(data->add, taui);

   data->out[0] = data->out[0]*tau;     // alpha blend color with white
   data->out[1] = data->out[1]*tau;     // alpha blend color with white
   data->out[2] = data->out[2]*tau;     // alpha blend color with white

   return 1;
}


/***************************************************************************
****************************************************************************/
int energy_beam(void *x) {

   float temp, temp2;
   int i, j;
   shaderparamtype *data;
   float value = 0;
   float y;

   data = (shaderparamtype *)x;

   if (data->in[0] < -56 || data->in[0] > 56 || data->in[1] < -256 || data->in[1] > 256) {
      data->out[0] = data->out[1] = data->out[2] = 0;
      return 0;
   }

   y = data->in[1] + 256;

   if (y < 1) {
      if (data->in[0] >= -CORRECT && data->in[0] <= CORRECT)
         return 1;

      temp2 = 1 - 0.3*abs(data->in[0]);

      if (temp2 > 0 && temp2 <= 1)
         return 1;

      data->out[0] = data->out[1] = data->out[2] = 0;
      return 0;
   }

   temp = 31.41632*data->frame;
   j = (int)noise1(temp);

   i = (int)(54.4563*(noise1(y*0.011904762+temp)-j));

   if (data->in[0] >= i-CORRECT && data->in[0] <= i+CORRECT)
      return 1;

   temp2 = i - data->in[0];
   temp2 = 1 - 0.3*abs(temp2);

   if (temp2 > 0 && temp2 <= 1)
      value += temp2;

   temp *= 0.125;
   i = (int)(54.4563*(noise1(y*0.010416667+temp)-j));

   if (data->in[0] >= i-CORRECT && data->in[0] <= i+CORRECT)
      return 1;

   temp2 = i - data->in[0];
   temp2 = 1 - 0.3*abs(temp2);

   if (temp2 > 0 && temp2 <= 1)
      value += temp2;

   temp *= 0.25;
   i = (int)(54.4563*(noise1(y*0.0078125+temp)-j));

   if (data->in[0] >= i-0.5-CORRECT && data->in[0] <= i+0.5+CORRECT)
      return 1;

   temp2 = i - data->in[0];
   temp2 = 1 - 0.25*abs(temp2);

   if (temp2 > 0 && temp2 <= 1)
      value += temp2;

   temp *= 0.25;
   i = (int)(54.4563*(noise1(y*0.00390625+temp)-j));

   if (data->in[0] >= i-1-CORRECT && data->in[0] <= i+1+CORRECT)
      return 1;

   temp2 = i - data->in[0];
   temp2 = 1 - 0.2*abs(temp2);

   if (temp2 > 0 && temp2 <= 1)
      value += temp2;

   temp *= 0.25;
   if (value > CORRECT) {
      if (value < 1) {
         value *= HALFPI;
//         smultarray3(data->out, COS(value));
         smultarray3(data->out, SIN(value));
      }

      return 1;
   }

   data->out[0] = data->out[1] = data->out[2] = 0;
   return 0;
}


// -------------------------- SBFAST ---------------------------


/* *************************************************************
  calculate level of detail, based on distance from eye
                run, chase, sbfast pics

        pt[0] - distance
        in - pos;
        octaves - octave_scalar
        out[0] - height

#define SBSCALE 0.0063596645
#define SCALESB 157.241

************************************************************* */
int default_alt(void *x) {

   shaderparamtype *data = (shaderparamtype *)x;
   float octaves;

//      octaves truncated from 0-3

   octaves = data->params.octaves;

   if (data->pt[0] < 1)
      data->params.octaves = -data->params.octaves + 0.5;
   else
      data->params.octaves = -(data->params.octaves + LOG2(data->pt[0]))+0.5;

                                                // use any fBm function
   data->out[0] = RidgedMultifractal2(data->in, &data->params, 1.0, 2.0) -1;  // altitude - mountains

   data->params.octaves = octaves;
   return 1;
}


/* *************************************************************
  calculate level of detail, based on distance from eye
                run, chase, sbfast pics

        pt[0] - distance
        in - pos;
        octaves - octave_scalar
        out[0] - height

#define SBSCALE 0.0063596645
#define SCALESB 157.241
        same shader as default_alt, but no restriction on octaves
************************************************************* */
int x_alt(void *x) {

   shaderparamtype *data = (shaderparamtype *)x;
   float octaves;
   float dist;

   octaves = data->params.octaves;

   dist = data->pt[0];
   octaves = -(data->params.octaves + LOG2(dist))+0.5;

   if (octaves < 0)
      octaves = 0;
                                                // use any fBm function
   data->out[0] = RidgedMultifractal2(data->in, &data->params, 1.0, 2.0) -1;  // altitude - mountains

   data->params.octaves = octaves;
   return 1;
}


int bound_mnb_alt(void *x) {

   float octaves;

   shaderparamtype *data = (shaderparamtype *)x;

   octaves = data->params.octaves;

   data->params.octaves = -data->params.octaves + 0.5;

   bound_RidgedMultifractal2(&data->params, 1.0, &data->min, &data->max);
   data->min -= 1.0;
   data->max -= 1.0;

   data->params.octaves = octaves;
   return 1;
}


/* *************************************************************
  calculate level of detail, based on distance from eye
                "dip" for road

#define SBSCALE 0.0063596645
#define SCALESB 157.241

************************************************************* */
int mnb_alt(void *x) {

   shaderparamtype *data = (shaderparamtype *)x;
   float octaves;
   float dist;

   float road;
   float edge;

   road = 0.31798322;           // sbscale * 50
   edge = abs(data->in[0]);

   if (edge < road) {           // road 50+ units wide
      data->out[0] = 0;
      return 1;
   }

   // octaves truncated to range 0..3
   dist = ((data->pt[0] < 1) ? 1 : data->pt[0]);

   octaves = data->params.octaves;

   data->params.octaves = -(data->params.octaves + LOG2(dist))+0.5;

   if (data->params.octaves < 0)
      data->params.octaves = 0;

   if (edge < road+road)
      dist = edge/road - 1.0;
   else
      dist = 1;
                                                // use any fBm function
   data->out[0] = dist*(RidgedMultifractal2(data->in, &data->params, 1.0, 2.0)-1);  // altitude - mountains

   data->params.octaves = octaves;
   return 1;
}


/* *************************************************************

        suggested params:

                H = 0.2765
                lacunarity = 2.0
                octaves = 6

************************************************************* */
int barfland_alt(void *x) {

   shaderparamtype *data = (shaderparamtype *)x;
   vector3f p;
   float octaves;
   sbfast *ob;

   octaves = data->params.octaves;

   ob = (sbfast *)data->ob;
   data->params.octaves = ob->query_octaves();

   p[0] = data->in[1] * 0.002;
   p[1] = data->in[0] * 0.002;

   p[2] = 0;
                                        // use any fBm function
   p[2] = fBm(p, &data->params);        // altitude - mountains

   data->out[0] = 20 + abs(p[2]) * p[2] * (300 - (data->in[0] + data->in[1] + 1000.0)*0.099009901);

   data->params.octaves = octaves;
   return 1;
}
