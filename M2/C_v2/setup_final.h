#include <omp.h> //openMP library
#include <immintrin.h>//AVX library

#define SCALE_FACTOR 2 // set as 0 temporarily
// MUST BE A FACTOR OF 64
#define N 128 // size of the world
#define L 10 // number of lights
#define M 5 // number of moths
#define L_SIZE 2 // size of the light array
#define M_SIZE 3 // size of the moth array

void generateLuminosity (float lum[N][N], int lights[L][L_SIZE]);
void generateLights (int pos[L][L_SIZE]);
void generateMoths (int pos[M][M_SIZE]);