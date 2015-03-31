#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE    0.1
#define MAX_ITERATION    100
#define NUM_LINES_FILE   30

float randomFloat()
{
    return (float)rand() / (float)RAND_MAX;
}

int calculateOutput(float weights[], float x, float y, float z)
{
    float sum = x * weights[0] + y * weights[1] + z * weights[2] + weights[3];
    return (sum >= 0) ? 1 : -1;
}

int main(int argc, char *argv[])
{

    float outputs[NUM_LINES_FILE], x[NUM_LINES_FILE], y[NUM_LINES_FILE], z[NUM_LINES_FILE], weights[4], localError, globalError;
    int patternCount, i, p, iteration, output;
    char c;
    FILE *fp;

    srand(time(NULL));

    if ((fp = fopen("data.txt", "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }

   //skip first line
   while ((c = fgetc(fp)) != '\n');

    i = 0;
    while (fscanf(fp, "%f %f %f %f", &x[i], &y[i], &z[i], &outputs[i]) != EOF) {
        if (outputs[i] == 0) {
            outputs[i] = -1;
        }
        i++;
    }
    patternCount = i;

    weights[0] = randomFloat();
    weights[1] = randomFloat();
    weights[2] = randomFloat();
    weights[3] = randomFloat();

    iteration = 0;
    do {
        iteration++;
        globalError = 0;
        for (p = 0; p < patternCount; p++) {
            output = calculateOutput(weights, x[p], y[p], z[p]);

            localError = outputs[p] - output;
            weights[0] += LEARNING_RATE * localError * x[p];
            weights[1] += LEARNING_RATE * localError * y[p];
            weights[2] += LEARNING_RATE * localError * z[p];
            weights[3] += LEARNING_RATE * localError;

            globalError += (localError*localError);
        }

  /* Root Mean Squared Error */
  printf("Iteration %d : RMSE = %.4f\n", iteration,
                     sqrt(globalError/patternCount));
    } while (globalError != 0 && iteration<=MAX_ITERATION);

 printf("\nDecision boundary (line) equation: %.2f*x + %.2f*y + %.2f*z + %.2f = 0\n",
             weights[0], weights[1], weights[2], weights[3]);

    return 0;
}
