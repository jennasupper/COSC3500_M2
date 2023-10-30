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

    // loop through lum array
    int blockSize = 64;
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