

#if !defined __SHADERS_H
#define __SHADERS_H


// -------------------------- MATERIAL -----------------------------------

int marble(void *x);
int wood(void *x);
int wood_panel (void *x);
int wave(void *x);
int checker(void *x);

void moon_init();
int moon(void *x);
void planet2_init();
int planet2(void *x);
void planet_init();
int planet(void *x);
void cloud_init();
int cloud(void *x);
void cloud2_init();
int cloud2(void *x);
void warp_init();
int warp(void *x);
void stars_init();
int stars(void *x);
void terrain_init();
int terrain(void *x);
void terrain2_init();
int terrain2(void *x);
void terrain3_init();
int terrain3(void *x);
void cave_floor_init();
int cave_floor(void *x);
void cave_ceiling_init();
int cave_ceiling(void *x);

int energy_beam(void *x);

int planetclouds (void *x);
int contour (void *x);
int carpet (void *x);


// -------------------------- ATMOSPHERE -----------------------------------


int fog_mus(void *x);
int fog(void *x);
int fog2(void *x);


// -------------------------- SBFAST -----------------------------------

int default_alt(void *x);
int x_alt(void *x);
int mnb_alt(void *x);
int barfland_alt(void *x);


#endif
