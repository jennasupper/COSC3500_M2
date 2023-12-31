#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "setup_draft.h"

// CONSTANTS //
#define THRESHOLD 0.01
#define TIMESTEPS 1500

/**
 * Calculates mean coordinate of moths and returns as a weight
 * @param moths: array of moth positions
 * @param coord: coordinate to calculate mean of
 * @return: mean of the coordinate
*/
float meanMothPosition(int moths[M][M_SIZE], int coord) {
    float mean = 0;
    for (int i = 0; i < M; i++) {
        mean += moths[i][coord];
    }
    mean = mean / M;
    return mean;
}

/**
 * Calculates closest light to x and y coordinates
 * @param lights: array of light positions
 * @param x: x coordinate
 * @param y: y coordinate
 * @return: index of closest light
*/
int calculateClosestLight (int lights[L][L_SIZE], int x, int y) {
    // Loop through lights and calculate closest light
    int closest_light = 0;
    float closest_distance = pow((lights[0][0] - x), 2) + pow((lights[0][1] - y), 2);
    for (int i = 1; i < L; i++) {
        float distance = pow((lights[i][0] - x), 2) + pow((lights[i][1] - y), 2);
        if (distance < closest_distance) {
            closest_light = i;
            closest_distance = distance;
        }
    }
    return closest_light;
}

/**
 * Calculates the direction of the light relative to the moth (-1 for behind the moth, 0 for same direction, 1 for in front of the moth)
 * @param closest_light_coord: coordinate of the closest light
 * @param coord: coordinate of the moth
 * @return: direction of the light relative to the moth
*/
int calculateDirection(int closest_light_coord, int coord) {
    int direction = 0;
    if (closest_light_coord > coord) {
        direction = 1;
    } else if (closest_light_coord < coord) {
        direction = -1;
    }
    return direction;
}

/**
 * Edits the moth coordinates
 * @param lum: array of luminosity values
 * @param lights: array of light positions
 * @param moth: array of moth positions
 * @param mean_x: mean x coordinate of moths
 * @param mean_y: mean y coordinate of moths
 * @return: 1 if moth is on a light (dead), 0 otherwise
*/
int move (float lum[N][N], int lights[L][L_SIZE], int moth[M_SIZE], float mean_x, float mean_y) {
    // Extract information
    int x = moth[0];
    int y = moth[1];
    int dir = moth[2];

    float luminosity = lum[x][y];

    int closest_light = calculateClosestLight(lights, x, y);

    // convert mean_x and mean_y to ints
    int mean_x_int = (int) mean_x;
    int mean_y_int = (int) mean_y;
    int mean_x_dir = calculateDirection(mean_x_int, x);
    int mean_y_dir = calculateDirection(mean_y_int, y);

    // Calculate the direction of the closest light
    int closest_light_x = lights[closest_light][0];
    int closest_light_y = lights[closest_light][1];
    int closest_light_dir_x = calculateDirection(closest_light_x, x);
    int closest_light_dir_y = calculateDirection(closest_light_y, y);

    if (closest_light_dir_x == 0 && closest_light_dir_y == 0) {
        // moth is on a light
        // return function such that the moth cannot move anymore
        return 1;
    }

    // Binary variable to determine if moth moves randomly
    // set a random seed
    int escape = rand() % 2;
    int near_light = 0;

    if (THRESHOLD * 10 > luminosity && luminosity > THRESHOLD && escape == 0) {
        // Move towards the light
        x += closest_light_dir_x;
        y += closest_light_dir_y;
        near_light = 1;

    } else if (THRESHOLD * 10 < luminosity && luminosity > THRESHOLD && escape == 0) {
        // circle the light
        // if dir == 0 move clockwise
        // if dir == 1 move anti-clockwise
        if (dir == 0) {
            // move clockwise
            if ((closest_light_dir_x == 1 && closest_light_dir_y == 1) || (closest_light_dir_x == 1 && closest_light_dir_y == 0)) {
                x += 1;
                y -= 1;
            } else if ((closest_light_dir_x == 0 && closest_light_dir_y == 1) || (closest_light_dir_x == -1 && closest_light_dir_y == 1)) {
                x += 1;
                y += 1;
            } else if ((closest_light_dir_x == 0 && closest_light_dir_y == -1) || (closest_light_dir_x == 1 && closest_light_dir_y == -1)) {
                x -= 1;
                y -= 1;
            } else if ((closest_light_dir_x == -1 && closest_light_dir_y == -1) || (closest_light_dir_x == -1 && closest_light_dir_y == 0)) {
                // move right
                x -= 1;
                y += 1;
            }
        } else if (dir == 1) {
            // move anti-clockwise
            if ((closest_light_dir_x == 1 && closest_light_dir_y == 1) || (closest_light_dir_x == 0 && closest_light_dir_y == 1)) {
                x -= 1;
                y += 1;
            } else if ((closest_light_dir_x == 0 && closest_light_dir_y == -1) || (closest_light_dir_x == -1 && closest_light_dir_y == -1)) {
                x += 1;
                y -= 1;
            } else if ((closest_light_dir_x == 1 && closest_light_dir_y == 0) || (closest_light_dir_x == 1 && closest_light_dir_y == -1)) {
                x += 1;
                y += 1;
            } else if ((closest_light_dir_x == -1 && closest_light_dir_y == 0) || (closest_light_dir_x == -1 && closest_light_dir_y == 1)) {
                // move right
                x -= 1;
                y -= 1;
            }
        near_light = 1;
        }

    } else if (escape == 1 && luminosity > (THRESHOLD)) {
        // Move away from the light
        x -= closest_light_dir_x;
        y -= closest_light_dir_y;
        near_light = 1;
    }
    
    else {
        // Move randomly
        y += rand() % 3 - 1;
        x += rand() % 3 - 1;
    }



    // Update moth array
    if (x < 0) {
        x = 0;
    } else if (x >= N) {
        x = N - 1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= N) {
        y = N - 1;
    }

    if (near_light == 0 && escape == 0) {
        x += mean_x_dir;
        y += mean_y_dir;
    }

    moth[0] = x;
    moth[1] = y;

}

/**
 * Main function
*/
int main () {

    // initialisation of the 'luminosity' array
    float lum[N][N] = {0.0};

    // initialisation of the 'light position' array
    int lights[L][L_SIZE] = {0};
    generateLights(lights);

    // initialisation of the 'moth position' array
    int moths[M][M_SIZE] = {0};
    generateMoths(moths);

    // populate luminosity array
    generateLuminosity(lum, lights);

    // Means
    float mean_x;
    float mean_y;

    // Begin simulation
    int *allMoths = (int*)malloc(M*TIMESTEPS*M_SIZE*sizeof(int));
    for (int t = 0; t < TIMESTEPS; t++) {
        // Loop through lights and initialise moth count to 0
        for (int i = 0; i < L; i++) {
            lights[i][2] = 0;
        }

        mean_x = meanMothPosition(moths, 0);
        mean_y = meanMothPosition(moths, 1);

        // Loop through moths
        for (int i = 0; i < M; i++) {
            move(lum, lights, moths[i], mean_x, mean_y);

        }

        // Save moth state to allMoths
        // allMoths[M*t] = moths;
        for (int i = 0; i < M; i++) {
            allMoths[M*t*M_SIZE + i*M_SIZE] = moths[i][0];
            allMoths[M*t*M_SIZE + i*M_SIZE + 1] = moths[i][1];
            allMoths[M*t*M_SIZE + i*M_SIZE + 2] = moths[i][2];
        }
    }
}