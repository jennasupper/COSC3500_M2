#include <thread>
#include <omp.h> //openMP library
#include <immintrin.h>//AVX library

#define SCALE_FACTOR 2 // set as 0 temporarily
#define N 500 // size of the world
#define L 100 // number of lights
#define M 100 // number of moths
#define L_SIZE 2 // size of the light array
#define M_SIZE 3 // size of the moth array

void generateLuminosity (float lum[N][N], int lights[L][L_SIZE]);
void generateLights (int pos[L][L_SIZE]);
void generateMoths (int pos[M][M_SIZE]);