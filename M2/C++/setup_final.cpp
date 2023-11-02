#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "setup_final.h"


// FUNCTIONS //
/**
 * Randomly generates L lights
 * @param pos array of positions 
*/
void generateLights (int pos[L][L_SIZE]) {
    for (int i = 0; i < L; i++) {
        // randomly generate x and y coordinates
        pos[i][0] = rand() % N;
        pos[i][1] = rand() % N;
    }
}

/**
 * Randomly generates M moths
 * @param pos array of positions
*/
void generateMoths (int pos[M][M_SIZE]) {
    for (int i = 0; i < M; i++) {
        // randomly generate x and y coordinates
        pos[i][0] = rand() % N;
        pos[i][1] = rand() % N;
        // direction tendancy (either 0 = clockwise or 1 = anticlockwise)
        pos[i][2] = rand() % 2;
    }
}

/**
 * Generates luminosity array
 * @param lum array of luminosity values
 * @param lights array of light positions
*/
void generateLuminosity (float lum[N][N], int lights[L][L_SIZE]) {

    // Pre calculate luminosity values
    float lum_x[L][N] = {0};
    float lum_y[L][N] = {0};
    for (int n = 0; n < N; n++) {
        for (int l = 0; l < L; l++) {
            lum_x[l][n] = (lights[l][0] - n) * (lights[l][0] - n);
            lum_y[l][n] = (lights[l][1] - n) * (lights[l][1] - n);
        }
    }

    // optimum threads is 32 I believe
    omp_set_num_threads(32);

    // optimum loop sizes
    // size_t ib = std::min(256, N);
    // size_t jb = std::min(512, N);
    // size_t kb = std::min(16, N);
    // loop through lum array
    # pragma omp parallel
    {
    int blockSize = 64;
    int j, i, k;
    //auto one = _mm256_set1_ps(1.0);
    # pragma omp for
    for (int i = 1; i < N; i+=blockSize) {
        for (int j = 1; j < N; j+=blockSize) {
            float lum_sum = 0.0;
            // loop through lights array
            for (int k = 1; k < L; k+=5) {
                for (int di=0; di<blockSize; di++) {
                    for (int dj=0; dj<blockSize; dj++) {
                        // calculate distance between light and point (
                        // inverse square law)
                        lum_sum += 1 / (
                            lum_x[k][i] + lum_y[k][j]) + 1 / (
                                lum_x[k + 1][i] + lum_y[k + 1][j]) + 1 / (
                                    lum_x[k + 2][i] + lum_y[k + 2][j]) + 1 / (
                                        lum_x[k + 3][i] + lum_y[k + 3][j]) + 1 / (
                                            lum_x[k + 4][i] + lum_y[k + 4][j]);
                    }
                }
            }
            lum[i][j] = lum_sum * SCALE_FACTOR;
        }
    }
    }
    // for (int i = 1; i < N; i+=blockSize) {
    //     for (int j = 1; j < N; j+=blockSize) {
    //         //float lum_sum = 0.0;
    //         __m256 lum_sum = _mm256_setzero_ps();
    //         __m256 one = _mm256_set1_ps(1.0);
    //         // loop through lights array
    //         for (int k = 1; k < L; k++) {
    //             // rolled to a factor of 5
    //             for (int di=0; di<blockSize; di++) {
    //                 for (int dj=0; dj<blockSize; dj++) {
    //                     // calculate distance between light and point (
    //                     // inverse square law)
    //                     // lum_sum += 1 / (
    //                     //     lum_x[k][i] + lum_y[k][j]) + 1 / (
    //                     //         lum_x[k + 1][i] + lum_y[k + 1][j]) + 1 / (
    //                     //             lum_x[k + 2][i] + lum_y[k + 2][j]) + 1 / (
    //                     //                 lum_x[k + 3][i] + lum_y[k + 3][j]) + 1 / (
    //                     //                     lum_x[k + 4][i] + lum_y[k + 4][j]);
    //                     lum_sum = _mm256_add_ps(lum_sum, _mm256_div_ps(one, _mm256_add_ps(_mm256_loadu_ps(&lum_x[k][i]), _mm256_loadu_ps(&lum_y[k][j]))));
    //                 }
    //             }
    //         }
    //         //lum[i][j] = lum_sum * SCALE_FACTOR;
    //         //_mm256_mul_ps(lum_sum, SCALE_FACTOR);
    //         _mm256_storeu_ps(_mm256_loadu_ps(&lum_x[i][j]), _mm256_mul_ps(lum_sum, SCALE_FACTOR));
    //     }
    // }
    // for (jj = 0; jj < N; jj += jb) {
    //     for (ii = 0; ii < N; ii += ib) {
    //         for (kk = 0; kk < N; kk += kb) {



    // for (jj = 0; jj < N; jj += jb) {
    //     for (ii = 0; ii < N; ii + ib) {
    //         for (kk = 0; kk < N; kk += kb) {
                // for (j = 0; j < N; j += 2) {
                //     for (i = 0; i < N; i += 16)
                //     {
                //         __m256 sumA, sumB;

                        // if (kk == 0) {
                        //     sumA = sumB = _mm256_setzero_ps();
                        // } else {
                        //     // update sums
                        //     sumA = _mm256_loadu_ps(&lum[j][i]);
                        //     sumB = _mm256_loadu_ps(&lum[j][i + 8]);
                        // }

                        // for (k = kk; k < kk+kb; k++) {

                        //     auto k_j = _mm256_set1_ps(lum_x[k][j]);
                        //     auto k_i = _mm256_set1_ps(lum_y[k][i]);
                        //     auto k_j_8 = _mm256_set1_ps(lum_x[k][j + 8]);
                        //     auto k_i_8 = _mm256_set1_ps(lum_y[k][i + 8]);
                        //     sumA = _mm256_add_ps(sumA, _mm256_div_ps(one, _mm256_add_ps(k_j, k_i)));
                        //     sumB = _mm256_add_ps(sumB, _mm256_div_ps(one, _mm256_add_ps(k_j_8, k_i_8)));

                        //  }
                        // _mm256_storeu_ps(&lum[j][i], sumA);
    //                 }
    //             }
    //         }
    // //     }
    // }
    // }




    // for (int i = 1; i < N; i+=blockSize) {
    //     for (int j = 1; j < N; j+=blockSize) {
    //         //float lum_sum = 0.0;
    //         __m256 lum_sum = _mm256_setzero_ps();
    //         __m256 one = _mm256_set1_ps(1.0);
    //         // loop through lights array
    //         for (int k = 1; k < L; k++) {
    //             for (int di=i; di<blockSize; di++) {
    //                 for (int dj=j; dj<blockSize; dj++) {
    //                     // calculate distance between light and point (
    //                     // inverse square law)
    //                     // lum_sum += 1 / (
    //                     //     lum_x[k][i] + lum_y[k][j]);
    //                     lum_sum = _mm256_add_ps(lum_sum, _mm256_div_ps(one, _mm256_add_ps(_mm256_loadu_ps(&lum_x[k][di]), _mm256_loadu_ps(&lum_y[k][dj]))));
    //                 }
    //                 _mm256_storeu_ps(&lum_x[i][j], lum_sum);
    //             }
    //         }
    //         //lum[i][j] = lum_sum * SCALE_FACTOR;
            
    //     }
    // }
    // }
    // }
    // }
    // }
}