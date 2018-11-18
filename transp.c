#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const int LIMIT = (int) 9;
FILE* filePtr;

int* createMatrix(int width) {
    int size = width * width;
    int* arr = (int*) malloc(size * sizeof(int));
    int i;

    for (i = 0; i < size; i++) {
        arr[i] = rand() % LIMIT;
    }

    return arr;
}

void swapIntegers(int* a, int* b) {
    int temp = *a;

    *a = *b;
    *b = temp;
}

void printMatrix(int* arr, int width) {
    int i;

    for (i = 0; i < width * width; i++) {
        printf("%d ", arr[i]);

        if (i % width == (width - 1)) {
            printf("\n");
        }
    }

    printf("-------------------\n");
}

/*

width = 4

0 1 2 3             0 4 2 3
4 5 6 7     --      1 5 6 7
a b c d     --      a b c d
e f g h             e f g h

*/
void transpose(int width, int blockSize) {
    int* arr = createMatrix(width);
    int block, i, j;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    time_t startSeconds = tv.tv_sec;
    suseconds_t startMicroSeconds = tv.tv_usec;
    
    // transposes the top left block of the matrix, then takes 2 blocks - one to the right,
    // and another down of the top left one, and swaps them and puts them in their transposed
    // place.
    for (block = 0; block + blockSize - 1 < width; block += blockSize) {
        for (i = block; i < block + blockSize; i++) {
            for (j = i + 1; j < block + blockSize; j++) {
                swapIntegers(&arr[i * width + j], &arr[j * width + i]);
            }
        }

        for (i = block + blockSize; i < width; i++) {
            for (j = block; j < block + blockSize; j++) {
                swapIntegers(&arr[i * width + j], &arr[j * width + i]);
            }
        }
    }

    for (i = block; i < width; ++i) {
        for (j = i + 1; j < width; ++j) {
            swapIntegers(&arr[i * width + j], &arr[j * width + i]);
        }
    }

    gettimeofday(&tv, NULL);
    fprintf(filePtr, "%d,%d,%ld,%d\n", width, blockSize, (tv.tv_sec - startSeconds), (tv.tv_usec - startMicroSeconds));
}

void createStats() {
    int widths[3] = {2048, 4096, 8192};
    int i;

    filePtr = fopen("part1.csv", "w");
    fprintf(filePtr, "%s,%s,%s,%s\n", "Matrix Width", "Block Size", "Time Taken(s)", "Time Taken(µs)");

    for (i = 0; i < 3; i++) {
        int j;

        for (j = 2; j <= widths[i]; j <<= 1) {
            transpose(widths[i], j);
        }

        printf("width[%d] done.\n", widths[i]);
    }

    fclose(filePtr);
}

int main() {
/*     filePtr = fopen("stats.csv", "w");
    transpose(8, 4);
    fclose(filePtr); */

    createStats();

    return 0;
}