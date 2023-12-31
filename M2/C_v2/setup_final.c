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

    MPI_Init(NULL, NULL);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    // printf("rank %d\n", rank);
    // printf("size %d\n", size);
    // Number of rows handled by each process

    int Nperrank = N / size;
    int myfirstN = rank * Nperrank;
    int mylastN = (rank + 1) * Nperrank;

    // optimum threads is 32 I believe
    // omp_set_num_threads(4);

    // // // loop through lum array
    // # pragma omp parallel
    // {
    int blockSize = 64;
    int j, i, k;
    // //auto one = _mm256_set1_ps(1.0);
    // # pragma omp for

    // FILE *fp;
    // make a file name with rank
    // char filename[15];
    // sprintf(filename, "lum_ind_%d_v2.txt", rank);
    // fp = fopen(filename, "w");

    for (i = myfirstN; i < mylastN; i+=blockSize) {
        for (j = 0; j < N; j+=blockSize) {
            for (int di=0; di<blockSize; di++) {
                int I = di + i;
                for (int dj=0; dj<blockSize; dj++) {
                    int J = dj + j;
                    lum[I][J] = 0.0;
                    for (k = 0; k < L; k++) {
                        // bug here, light locations = inf        
                        lum[I][J] += 1 / (lum_x[k][I] + lum_y[k][J]);
                    }
                }
            }
        }
    }
    
    // FILE *fp;
    // // make a file name with rank
    // char filename[15];
    // sprintf(filename, "lum_ind_%d.txt", rank);
    // fp = fopen(filename, "w");
    // for (int i = myfirstN; i < mylastN; i++) {
    //     for (int j = 0; j < N; j++) {
    //         //float lum_sum = 0.0;
    //         // loop through lights array
    //         for (int k = 0; k < L; k++) {
    //             // calculate distance between light and point (
    //             // inverse square law)
    //             lum[i][j] += 1 / (lum_x[k][i] + lum_y[k][j]);
    //             // fprintf(fp, "%d, %d\n", i, j);
    //         }
    //     }
    // }
    // fclose(fp);

    // print each element in array with index if not none
    // FILE *fp;
    // fp = fopen("lum_testing.txt", "w");
    // for (int i = myfirstN; i < mylastN; i++) {
    //     for (int j = 0; j < N; j++) {
    //         fprintf(fp, "%f ", lum[i][j]);
    //     }
    //     fprintf(fp, "\n");
    // }
    // fclose(fp);

    // FILE *fp;
    // fp = fopen("luminosity.txt", "w");
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N-1; j++) {
    //         fprintf(fp, "%f,", lum[i][j]);
    //     }
    //     fprintf(fp, "%f\n", lum[i][N-1]);
    // }
    // fclose(fp);

    if (rank == 0) {

        MPI_Recv(&lum[Nperrank][0], Nperrank*N, MPI_FLOAT, 1, 101, MPI_COMM_WORLD, &status);
        MPI_Send(&lum[0], Nperrank*N, MPI_FLOAT, 1, 100, MPI_COMM_WORLD);
    } else {
        MPI_Send(&lum[Nperrank][0], Nperrank*N, MPI_FLOAT, 0, 101, MPI_COMM_WORLD);
        MPI_Recv(&lum[0], Nperrank*N, MPI_FLOAT, 0, 100, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();
    
    }



    //         // for (k = 0; k < L; k+=blockSize) {
    //         //     for (int di=0; di<blockSize; di++) {
    //         //         for (int dj=0; dj<blockSize; dj++) {

    //         //             for (int dk=0; dk<blockSize; dk++) {
    //         //                 // calculate distance between light and point (
    //         //                 // inverse square law)
    //         //                 lum[i+di][j+dj] += 1 / (
    //         //                     lum_x[k+dk][i+di] + lum_y[k+dk][j+dj]);
    //         //             }
    //         //             //lum[i + di][j + dj] = lum_sum * SCALE_FACTOR;
    //         //             // calculate distance between light and point (
    //         //             // inverse square law)
    //         //             // lum_sum += 1 / (
    //         //             //     lum_x[k][i] + lum_y[k][j]) + 1 / (
    //         //             //         lum_x[k + 1][i] + lum_y[k + 1][j]) + 1 / (
    //         //             //             lum_x[k + 2][i] + lum_y[k + 2][j]) + 1 / (
    //         //             //                 lum_x[k + 3][i] + lum_y[k + 3][j]) + 1 / (
    //         //             //                     lum_x[k + 4][i] + lum_y[k + 4][j]);
    //         //         }
    //         //     }
    //         // }
            
    //     }
//     }
// }
// }
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
