#include <iostream>
#include <graphics.h>
#include <math.h>


using namespace std;

//int graphdriver;
//int graphmode;

unsigned int resx;
unsigned int resy;
float x, y, z;
char pal[256][3];
char r;
char g;
char b;

void genpal()
{
    pal[0][0] = 0;
    pal[0][1] = 0;
    pal[0][2] = 0;
    pal[1][0] = 0;
    pal[1][1] = 0;
    pal[1][2] = 170;
    pal[2][0] = 0;
    pal[2][1] = 170;
    pal[2][2] = 0;
    pal[3][0] = 0;
    pal[3][1] = 170;
    pal[3][2] = 170;
    pal[4][0] = 170;
    pal[4][1] = 0;
    pal[4][2] = 0;
    pal[5][0] = 170;
    pal[5][1] = 0;
    pal[5][2] = 170;
    pal[6][0] = 170;
    pal[6][1] = 85;
    pal[6][2] = 0;
    pal[7][0] = 170;
    pal[7][1] = 170;
    pal[7][2] = 170;
    pal[8][0] = 85;
    pal[8][1] = 85;
    pal[8][2] = 85;
    pal[9][0] = 85;
    pal[9][1] = 85;
    pal[9][2] = 255;
    pal[10][0] = 85;
    pal[10][1] = 255;
    pal[10][2] = 85;
    pal[11][0] = 85;
    pal[11][1] = 255;
    pal[11][2] = 255;
    pal[12][0] = 255;
    pal[12][1] = 85;
    pal[12][2] = 85;
    pal[13][0] = 255;
    pal[13][1] = 85;
    pal[13][2] = 255;
    pal[14][0] = 255;
    pal[14][1] = 255;
    pal[14][2] = 85;
    pal[15][0] = 255;
    pal[15][1] = 255;
    pal[15][2] = 255;
    pal[16][0] = 0;
    pal[16][1] = 0;
    pal[16][2] = 0;
    pal[17][0] = 16;
    pal[17][1] = 16;
    pal[17][2] = 16;
    pal[18][0] = 32;
    pal[18][1] = 32;
    pal[18][2] = 32;
    pal[19][0] = 53;
    pal[19][1] = 53;
    pal[19][2] = 53;
    pal[20][0] = 69;
    pal[20][1] = 69;
    pal[20][2] = 69;
    pal[21][0] = 85;
    pal[21][1] = 85;
    pal[21][2] = 85;
    pal[22][0] = 101;
    pal[22][1] = 101;
    pal[22][2] = 101;
    pal[23][0] = 117;
    pal[23][1] = 117;
    pal[23][2] = 117;
    pal[24][0] = 138;
    pal[24][1] = 138;
    pal[24][2] = 138;
    pal[25][0] = 154;
    pal[25][1] = 154;
    pal[25][2] = 154;
    pal[26][0] = 170;
    pal[26][1] = 170;
    pal[26][2] = 170;
    pal[27][0] = 186;
    pal[27][1] = 186;
    pal[27][2] = 186;
    pal[28][0] = 202;
    pal[28][1] = 202;
    pal[28][2] = 202;
    pal[29][0] = 223;
    pal[29][1] = 223;
    pal[29][2] = 223;
    pal[30][0] = 239;
    pal[30][1] = 239;
    pal[30][2] = 239;
    pal[31][0] = 255;
    pal[31][1] = 255;
    pal[31][2] = 255;
    pal[32][0] = 0;
    pal[32][1] = 0;
    pal[32][2] = 255;
    pal[33][0] = 65;
    pal[33][1] = 0;
    pal[33][2] = 255;
    pal[34][0] = 130;
    pal[34][1] = 0;
    pal[34][2] = 255;
    pal[35][0] = 190;
    pal[35][1] = 0;
    pal[35][2] = 255;
    pal[36][0] = 255;
    pal[36][1] = 0;
    pal[36][2] = 255;
    pal[37][0] = 255;
    pal[37][1] = 0;
    pal[37][2] = 190;
    pal[38][0] = 255;
    pal[38][1] = 0;
    pal[38][2] = 130;
    pal[39][0] = 255;
    pal[39][1] = 0;
    pal[39][2] = 65;
    pal[40][0] = 255;
    pal[40][1] = 0;
    pal[40][2] = 0;
    pal[41][0] = 255;
    pal[41][1] = 65;
    pal[41][2] = 0;
    pal[42][0] = 255;
    pal[42][1] = 130;
    pal[42][2] = 0;
    pal[43][0] = 255;
    pal[43][1] = 190;
    pal[43][2] = 0;
    pal[44][0] = 255;
    pal[44][1] = 255;
    pal[44][2] = 0;
    pal[45][0] = 190;
    pal[45][1] = 255;
    pal[45][2] = 0;
    pal[46][0] = 130;
    pal[46][1] = 255;
    pal[46][2] = 0;
    pal[47][0] = 65;
    pal[47][1] = 255;
    pal[47][2] = 0;
    pal[48][0] = 0;
    pal[48][1] = 255;
    pal[48][2] = 0;
    pal[49][0] = 0;
    pal[49][1] = 255;
    pal[49][2] = 65;
    pal[50][0] = 0;
    pal[50][1] = 255;
    pal[50][2] = 130;
    pal[51][0] = 0;
    pal[51][1] = 255;
    pal[51][2] = 190;
    pal[52][0] = 0;
    pal[52][1] = 255;
    pal[52][2] = 255;
    pal[53][0] = 0;
    pal[53][1] = 190;
    pal[53][2] = 255;
    pal[54][0] = 0;
    pal[54][1] = 130;
    pal[54][2] = 255;
    pal[55][0] = 0;
    pal[55][1] = 65;
    pal[55][2] = 255;
    pal[56][0] = 130;
    pal[56][1] = 130;
    pal[56][2] = 255;
    pal[57][0] = 158;
    pal[57][1] = 130;
    pal[57][2] = 255;
    pal[58][0] = 190;
    pal[58][1] = 130;
    pal[58][2] = 255;
    pal[59][0] = 223;
    pal[59][1] = 130;
    pal[59][2] = 255;
    pal[60][0] = 255;
    pal[60][1] = 130;
    pal[60][2] = 255;
    pal[61][0] = 255;
    pal[61][1] = 130;
    pal[61][2] = 223;
    pal[62][0] = 255;
    pal[62][1] = 130;
    pal[62][2] = 190;
    pal[63][0] = 255;
    pal[63][1] = 130;
    pal[63][2] = 158;
    pal[64][0] = 255;
    pal[64][1] = 130;
    pal[64][2] = 130;
    pal[65][0] = 255;
    pal[65][1] = 158;
    pal[65][2] = 130;
    pal[66][0] = 255;
    pal[66][1] = 190;
    pal[66][2] = 130;
    pal[67][0] = 255;
    pal[67][1] = 223;
    pal[67][2] = 130;
    pal[68][0] = 255;
    pal[68][1] = 255;
    pal[68][2] = 130;
    pal[69][0] = 223;
    pal[69][1] = 255;
    pal[69][2] = 130;
    pal[70][0] = 190;
    pal[70][1] = 255;
    pal[70][2] = 130;
    pal[71][0] = 158;
    pal[71][1] = 255;
    pal[71][2] = 130;
    pal[72][0] = 130;
    pal[72][1] = 255;
    pal[72][2] = 130;
    pal[73][0] = 130;
    pal[73][1] = 255;
    pal[73][2] = 158;
    pal[74][0] = 130;
    pal[74][1] = 255;
    pal[74][2] = 190;
    pal[75][0] = 130;
    pal[75][1] = 255;
    pal[75][2] = 223;
    pal[76][0] = 130;
    pal[76][1] = 255;
    pal[76][2] = 255;
    pal[77][0] = 130;
    pal[77][1] = 223;
    pal[77][2] = 255;
    pal[78][0] = 130;
    pal[78][1] = 190;
    pal[78][2] = 255;
    pal[79][0] = 130;
    pal[79][1] = 158;
    pal[79][2] = 255;
    pal[80][0] = 186;
    pal[80][1] = 186;
    pal[80][2] = 255;
    pal[81][0] = 202;
    pal[81][1] = 186;
    pal[81][2] = 255;
    pal[82][0] = 223;
    pal[82][1] = 186;
    pal[82][2] = 255;
    pal[83][0] = 239;
    pal[83][1] = 186;
    pal[83][2] = 255;
    pal[84][0] = 255;
    pal[84][1] = 186;
    pal[84][2] = 255;
    pal[85][0] = 255;
    pal[85][1] = 186;
    pal[85][2] = 239;
    pal[86][0] = 255;
    pal[86][1] = 186;
    pal[86][2] = 223;
    pal[87][0] = 255;
    pal[87][1] = 186;
    pal[87][2] = 202;
    pal[88][0] = 255;
    pal[88][1] = 186;
    pal[88][2] = 186;
    pal[89][0] = 255;
    pal[89][1] = 202;
    pal[89][2] = 186;
    pal[90][0] = 255;
    pal[90][1] = 223;
    pal[90][2] = 186;
    pal[91][0] = 255;
    pal[91][1] = 239;
    pal[91][2] = 186;
    pal[92][0] = 255;
    pal[92][1] = 255;
    pal[92][2] = 186;
    pal[93][0] = 239;
    pal[93][1] = 255;
    pal[93][2] = 186;
    pal[94][0] = 223;
    pal[94][1] = 255;
    pal[94][2] = 186;
    pal[95][0] = 202;
    pal[95][1] = 255;
    pal[95][2] = 186;
    pal[96][0] = 186;
    pal[96][1] = 255;
    pal[96][2] = 186;
    pal[97][0] = 186;
    pal[97][1] = 255;
    pal[97][2] = 202;
    pal[98][0] = 186;
    pal[98][1] = 255;
    pal[98][2] = 223;
    pal[99][0] = 186;
    pal[99][1] = 255;
    pal[99][2] = 239;
    pal[100][0] = 186;
    pal[100][1] = 255;
    pal[100][2] = 255;
    pal[101][0] = 186;
    pal[101][1] = 239;
    pal[101][2] = 255;
    pal[102][0] = 186;
    pal[102][1] = 223;
    pal[102][2] = 255;
    pal[103][0] = 186;
    pal[103][1] = 202;
    pal[103][2] = 255;
    pal[104][0] = 0;
    pal[104][1] = 0;
    pal[104][2] = 113;
    pal[105][0] = 28;
    pal[105][1] = 0;
    pal[105][2] = 113;
    pal[106][0] = 57;
    pal[106][1] = 0;
    pal[106][2] = 113;
    pal[107][0] = 85;
    pal[107][1] = 0;
    pal[107][2] = 113;
    pal[108][0] = 113;
    pal[108][1] = 0;
    pal[108][2] = 113;
    pal[109][0] = 113;
    pal[109][1] = 0;
    pal[109][2] = 85;
    pal[110][0] = 113;
    pal[110][1] = 0;
    pal[110][2] = 57;
    pal[111][0] = 113;
    pal[111][1] = 0;
    pal[111][2] = 28;
    pal[112][0] = 113;
    pal[112][1] = 0;
    pal[112][2] = 0;
    pal[113][0] = 113;
    pal[113][1] = 28;
    pal[113][2] = 0;
    pal[114][0] = 113;
    pal[114][1] = 57;
    pal[114][2] = 0;
    pal[115][0] = 113;
    pal[115][1] = 85;
    pal[115][2] = 0;
    pal[116][0] = 113;
    pal[116][1] = 113;
    pal[116][2] = 0;
    pal[117][0] = 85;
    pal[117][1] = 113;
    pal[117][2] = 0;
    pal[118][0] = 57;
    pal[118][1] = 113;
    pal[118][2] = 0;
    pal[119][0] = 28;
    pal[119][1] = 113;
    pal[119][2] = 0;
    pal[120][0] = 0;
    pal[120][1] = 113;
    pal[120][2] = 0;
    pal[121][0] = 0;
    pal[121][1] = 113;
    pal[121][2] = 28;
    pal[122][0] = 0;
    pal[122][1] = 113;
    pal[122][2] = 57;
    pal[123][0] = 0;
    pal[123][1] = 113;
    pal[123][2] = 85;
    pal[124][0] = 0;
    pal[124][1] = 113;
    pal[124][2] = 113;
    pal[125][0] = 0;
    pal[125][1] = 85;
    pal[125][2] = 113;
    pal[126][0] = 0;
    pal[126][1] = 57;
    pal[126][2] = 113;
    pal[127][0] = 0;
    pal[127][1] = 28;
    pal[127][2] = 113;
    pal[128][0] = 57;
    pal[128][1] = 57;
    pal[128][2] = 113;
    pal[129][0] = 69;
    pal[129][1] = 57;
    pal[129][2] = 113;
    pal[130][0] = 85;
    pal[130][1] = 57;
    pal[130][2] = 113;
    pal[131][0] = 97;
    pal[131][1] = 57;
    pal[131][2] = 113;
    pal[132][0] = 113;
    pal[132][1] = 57;
    pal[132][2] = 113;
    pal[133][0] = 113;
    pal[133][1] = 57;
    pal[133][2] = 97;
    pal[134][0] = 113;
    pal[134][1] = 57;
    pal[134][2] = 85;
    pal[135][0] = 113;
    pal[135][1] = 57;
    pal[135][2] = 69;
    pal[136][0] = 113;
    pal[136][1] = 57;
    pal[136][2] = 57;
    pal[137][0] = 113;
    pal[137][1] = 69;
    pal[137][2] = 57;
    pal[138][0] = 113;
    pal[138][1] = 85;
    pal[138][2] = 57;
    pal[139][0] = 113;
    pal[139][1] = 97;
    pal[139][2] = 57;
    pal[140][0] = 113;
    pal[140][1] = 113;
    pal[140][2] = 57;
    pal[141][0] = 97;
    pal[141][1] = 113;
    pal[141][2] = 57;
    pal[142][0] = 85;
    pal[142][1] = 113;
    pal[142][2] = 57;
    pal[143][0] = 69;
    pal[143][1] = 113;
    pal[143][2] = 57;
    pal[144][0] = 57;
    pal[144][1] = 113;
    pal[144][2] = 57;
    pal[145][0] = 57;
    pal[145][1] = 113;
    pal[145][2] = 69;
    pal[146][0] = 57;
    pal[146][1] = 113;
    pal[146][2] = 85;
    pal[147][0] = 57;
    pal[147][1] = 113;
    pal[147][2] = 97;
    pal[148][0] = 57;
    pal[148][1] = 113;
    pal[148][2] = 113;
    pal[149][0] = 57;
    pal[149][1] = 97;
    pal[149][2] = 113;
    pal[150][0] = 57;
    pal[150][1] = 85;
    pal[150][2] = 113;
    pal[151][0] = 57;
    pal[151][1] = 69;
    pal[151][2] = 113;
    pal[152][0] = 81;
    pal[152][1] = 81;
    pal[152][2] = 113;
    pal[153][0] = 89;
    pal[153][1] = 81;
    pal[153][2] = 113;
    pal[154][0] = 97;
    pal[154][1] = 81;
    pal[154][2] = 113;
    pal[155][0] = 105;
    pal[155][1] = 81;
    pal[155][2] = 113;
    pal[156][0] = 113;
    pal[156][1] = 81;
    pal[156][2] = 113;
    pal[157][0] = 113;
    pal[157][1] = 81;
    pal[157][2] = 105;
    pal[158][0] = 113;
    pal[158][1] = 81;
    pal[158][2] = 97;
    pal[159][0] = 113;
    pal[159][1] = 81;
    pal[159][2] = 89;
    pal[160][0] = 113;
    pal[160][1] = 81;
    pal[160][2] = 81;
    pal[161][0] = 113;
    pal[161][1] = 89;
    pal[161][2] = 81;
    pal[162][0] = 113;
    pal[162][1] = 97;
    pal[162][2] = 81;
    pal[163][0] = 113;
    pal[163][1] = 105;
    pal[163][2] = 81;
    pal[164][0] = 113;
    pal[164][1] = 113;
    pal[164][2] = 81;
    pal[165][0] = 105;
    pal[165][1] = 113;
    pal[165][2] = 81;
    pal[166][0] = 97;
    pal[166][1] = 113;
    pal[166][2] = 81;
    pal[167][0] = 89;
    pal[167][1] = 113;
    pal[167][2] = 81;
    pal[168][0] = 81;
    pal[168][1] = 113;
    pal[168][2] = 81;
    pal[169][0] = 81;
    pal[169][1] = 113;
    pal[169][2] = 89;
    pal[170][0] = 81;
    pal[170][1] = 113;
    pal[170][2] = 97;
    pal[171][0] = 81;
    pal[171][1] = 113;
    pal[171][2] = 105;
    pal[172][0] = 81;
    pal[172][1] = 113;
    pal[172][2] = 113;
    pal[173][0] = 81;
    pal[173][1] = 105;
    pal[173][2] = 113;
    pal[174][0] = 81;
    pal[174][1] = 97;
    pal[174][2] = 113;
    pal[175][0] = 81;
    pal[175][1] = 89;
    pal[175][2] = 113;
    pal[176][0] = 0;
    pal[176][1] = 0;
    pal[176][2] = 65;
    pal[177][0] = 16;
    pal[177][1] = 0;
    pal[177][2] = 65;
    pal[178][0] = 32;
    pal[178][1] = 0;
    pal[178][2] = 65;
    pal[179][0] = 49;
    pal[179][1] = 0;
    pal[179][2] = 65;
    pal[180][0] = 65;
    pal[180][1] = 0;
    pal[180][2] = 65;
    pal[181][0] = 65;
    pal[181][1] = 0;
    pal[181][2] = 49;
    pal[182][0] = 65;
    pal[182][1] = 0;
    pal[182][2] = 32;
    pal[183][0] = 65;
    pal[183][1] = 0;
    pal[183][2] = 16;
    pal[184][0] = 65;
    pal[184][1] = 0;
    pal[184][2] = 0;
    pal[185][0] = 65;
    pal[185][1] = 16;
    pal[185][2] = 0;
    pal[186][0] = 65;
    pal[186][1] = 32;
    pal[186][2] = 0;
    pal[187][0] = 65;
    pal[187][1] = 49;
    pal[187][2] = 0;
    pal[188][0] = 65;
    pal[188][1] = 65;
    pal[188][2] = 0;
    pal[189][0] = 49;
    pal[189][1] = 65;
    pal[189][2] = 0;
    pal[190][0] = 32;
    pal[190][1] = 65;
    pal[190][2] = 0;
    pal[191][0] = 16;
    pal[191][1] = 65;
    pal[191][2] = 0;
    pal[192][0] = 0;
    pal[192][1] = 65;
    pal[192][2] = 0;
    pal[193][0] = 0;
    pal[193][1] = 65;
    pal[193][2] = 16;
    pal[194][0] = 0;
    pal[194][1] = 65;
    pal[194][2] = 32;
    pal[195][0] = 0;
    pal[195][1] = 65;
    pal[195][2] = 49;
    pal[196][0] = 0;
    pal[196][1] = 65;
    pal[196][2] = 65;
    pal[197][0] = 0;
    pal[197][1] = 49;
    pal[197][2] = 65;
    pal[198][0] = 0;
    pal[198][1] = 32;
    pal[198][2] = 65;
    pal[199][0] = 0;
    pal[199][1] = 16;
    pal[199][2] = 65;
    pal[200][0] = 32;
    pal[200][1] = 32;
    pal[200][2] = 65;
    pal[201][0] = 40;
    pal[201][1] = 32;
    pal[201][2] = 65;
    pal[202][0] = 49;
    pal[202][1] = 32;
    pal[202][2] = 65;
    pal[203][0] = 57;
    pal[203][1] = 32;
    pal[203][2] = 65;
    pal[204][0] = 65;
    pal[204][1] = 32;
    pal[204][2] = 65;
    pal[205][0] = 65;
    pal[205][1] = 32;
    pal[205][2] = 57;
    pal[206][0] = 65;
    pal[206][1] = 32;
    pal[206][2] = 49;
    pal[207][0] = 65;
    pal[207][1] = 32;
    pal[207][2] = 40;
    pal[208][0] = 65;
    pal[208][1] = 32;
    pal[208][2] = 32;
    pal[209][0] = 65;
    pal[209][1] = 40;
    pal[209][2] = 32;
    pal[210][0] = 65;
    pal[210][1] = 49;
    pal[210][2] = 32;
    pal[211][0] = 65;
    pal[211][1] = 57;
    pal[211][2] = 32;
    pal[212][0] = 65;
    pal[212][1] = 65;
    pal[212][2] = 32;
    pal[213][0] = 57;
    pal[213][1] = 65;
    pal[213][2] = 32;
    pal[214][0] = 49;
    pal[214][1] = 65;
    pal[214][2] = 32;
    pal[215][0] = 40;
    pal[215][1] = 65;
    pal[215][2] = 32;
    pal[216][0] = 32;
    pal[216][1] = 65;
    pal[216][2] = 32;
    pal[217][0] = 32;
    pal[217][1] = 65;
    pal[217][2] = 40;
    pal[218][0] = 32;
    pal[218][1] = 65;
    pal[218][2] = 49;
    pal[219][0] = 32;
    pal[219][1] = 65;
    pal[219][2] = 57;
    pal[220][0] = 32;
    pal[220][1] = 65;
    pal[220][2] = 65;
    pal[221][0] = 32;
    pal[221][1] = 57;
    pal[221][2] = 65;
    pal[222][0] = 32;
    pal[222][1] = 49;
    pal[222][2] = 65;
    pal[223][0] = 32;
    pal[223][1] = 40;
    pal[223][2] = 65;
    pal[224][0] = 45;
    pal[224][1] = 45;
    pal[224][2] = 65;
    pal[225][0] = 49;
    pal[225][1] = 45;
    pal[225][2] = 65;
    pal[226][0] = 53;
    pal[226][1] = 45;
    pal[226][2] = 65;
    pal[227][0] = 61;
    pal[227][1] = 45;
    pal[227][2] = 65;
    pal[228][0] = 65;
    pal[228][1] = 45;
    pal[228][2] = 65;
    pal[229][0] = 65;
    pal[229][1] = 45;
    pal[229][2] = 61;
    pal[230][0] = 65;
    pal[230][1] = 45;
    pal[230][2] = 53;
    pal[231][0] = 65;
    pal[231][1] = 45;
    pal[231][2] = 49;
    pal[232][0] = 65;
    pal[232][1] = 45;
    pal[232][2] = 45;
    pal[233][0] = 65;
    pal[233][1] = 49;
    pal[233][2] = 45;
    pal[234][0] = 65;
    pal[234][1] = 53;
    pal[234][2] = 45;
    pal[235][0] = 65;
    pal[235][1] = 61;
    pal[235][2] = 45;
    pal[236][0] = 65;
    pal[236][1] = 65;
    pal[236][2] = 45;
    pal[237][0] = 61;
    pal[237][1] = 65;
    pal[237][2] = 45;
    pal[238][0] = 53;
    pal[238][1] = 65;
    pal[238][2] = 45;
    pal[239][0] = 49;
    pal[239][1] = 65;
    pal[239][2] = 45;
    pal[240][0] = 45;
    pal[240][1] = 65;
    pal[240][2] = 45;
    pal[241][0] = 45;
    pal[241][1] = 65;
    pal[241][2] = 49;
    pal[242][0] = 45;
    pal[242][1] = 65;
    pal[242][2] = 53;
    pal[243][0] = 45;
    pal[243][1] = 65;
    pal[243][2] = 61;
    pal[244][0] = 45;
    pal[244][1] = 65;
    pal[244][2] = 65;
    pal[245][0] = 45;
    pal[245][1] = 61;
    pal[245][2] = 65;
    pal[246][0] = 45;
    pal[246][1] = 53;
    pal[246][2] = 65;
    pal[247][0] = 45;
    pal[247][1] = 49;
    pal[247][2] = 65;
    pal[248][0] = 0;
    pal[248][1] = 0;
    pal[248][2] = 0;
    pal[249][0] = 0;
    pal[249][1] = 0;
    pal[249][2] = 0;
    pal[250][0] = 0;
    pal[250][1] = 0;
    pal[250][2] = 0;
    pal[251][0] = 0;
    pal[251][1] = 0;
    pal[251][2] = 0;
    pal[252][0] = 0;
    pal[252][1] = 0;
    pal[252][2] = 0;
    pal[253][0] = 0;
    pal[253][1] = 0;
    pal[253][2] = 0;
    pal[254][0] = 0;
    pal[254][1] = 0;
    pal[254][2] = 0;
    pal[255][0] = 0;
    pal[255][1] = 0;
    pal[255][2] = 0;
}

int main()
{
    int xmax;
    int ymax;
    //graphdriver = DETECT;
    //initgraph (&graphdriver, &graphmode, "");
    resx = 1279;
    resy = 959;
    initwindow (1280, 960, "Gr�ficos");
    xmax = getmaxx();
    ymax = getmaxy();
    setlinestyle (SOLID_LINE, 0, NORM_WIDTH);
    rectangle (0, 0, xmax, ymax);
    genpal();
    int txtTam = 10;
    setcolor (15);
    //settextstyle (TRIPLEX_FONT, HORIZ_DIR, txtTam);
    //outtextxy (55, 40, "Hello world!");
    z = 15;

    do
    {
        x = 20;

        do
        {
            y = -10 * cos (2 * sqrt ( (x - 5) * (x - 5) + (z - 5) * (z - 5))) / 2 + 50;
            //y = -10 * sin (2 * sqrt ( (x - 5) * (x - 5) + (z - 5) * (z - 5))) / 2 + 50;
            //y = -10 * tan (2 * sqrt ( (x - 5) * (x - 5) + (z - 5) * (z - 5))) / 2 + 50;
            //y = -10 * log (2 * sqrt ( (x - 5) * (x - 5) + (z - 5) * (z - 5))) / 2 + 50;
            //y = -10 * cos (4 / sqrt ( (x - 5) * (x - 5) + (z - 5) * (z - 5))) / 2 + 50;
            //y = -10 * sin (4 / sqrt ( (x - 5) * (x - 5) + (z - 5) * (z - 5))) / 2 + 50;
            r = pal[(int)trunc (y)][0];
            g = pal[(int)trunc (y)][1];
            b = pal[(int)trunc (y)][2];
            putpixel (350 + trunc ( (resx / 640) * (2 * (x * 20 + z * 3 + 20))), 100 + trunc ( (resy / 200) * (y + z * 10)), COLOR (r, g, b)); //{    trunc( (2*y/((x+10)+(z+6)))+8 )); }
            x = x - 0.1;
        }
        while (x >= -10);

        z = z - 0.1;
    }
    while (z >= -6);

    //cout << "Hello world!" << endl;
    getch();
    return 0;
}
