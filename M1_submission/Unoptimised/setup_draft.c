#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "setup_draft.h"

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
    // loop through lum array
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // loop through lights array
            for (int k = 0; k < L; k++) {
                // calculate distance between light and point (inverse square law)
                float d_squared = pow(lights[k][0] - i, 2) + pow(lights[k][1] - j, 2);
                // scale factor needs to be researched
                lum[i][j] += 1 / d_squared * SCALE_FACTOR;
            }
        }
    }
}