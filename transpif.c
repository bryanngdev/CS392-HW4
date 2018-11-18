#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const int LIMIT = (int) 9;
FILE* filePtr;

int findMin(int a, int b) {
    if (a <= b) {
        return a;
    }

    return b;
}

int* createMatrix(int height, int width) {
    int size = height * width;
    int* arr = (int*) malloc(size * sizeof(int));
    int i;

    for (i = 0; i < size; i++) {
        arr[i] = rand() % LIMIT;
    }

    return arr;
}

void printMatrix(int* arr, int height, int width) {
    int i;

    for (i = 0; i < height * width; i++) {
        printf("%d ", arr[i]);

        if (i % width == (width - 1)) {
            printf("\n");
        }
    }

    printf("-------------------\n");
}

/*

height = 6; width = 4

0 1 2 3             0 4 8 b f j
4 5 6 7     --      1 5 9 c g k
8 9 0 a     --      2 6 0 d h l
b c d e             3 7 a e i m
f g h i
j k l m

*/
void transpose(int height, int width, int blockSize) {
    int* arr = createMatrix(height, width);
    struct timeval tv;

    gettimeofday(&tv, NULL);

    time_t startSeconds = tv.tv_sec;
    suseconds_t startMicroSeconds = tv.tv_usec;

    int* temp = (int*) malloc((height * width) * sizeof(int));
    int i, j, k;

    // We take blocks from left to right(starting at (0, 0)) from the original array, and
    // transpose them and put them in their right positions from top to bottom.
    for (i = 0; i < height; i += blockSize) {
        for (j = 0; j < width; j++) {
            for (k = 0; i + k < height && k < blockSize; k++) {
                temp[j * height + i + k] = arr[(i + k) * width + j];
            }
        }
    }

    arr = temp;
    gettimeofday(&tv, NULL);
    fprintf(filePtr, "%d,%d,%d,%ld,%d\n", height, width, blockSize, (tv.tv_sec - startSeconds), (tv.tv_usec - startMicroSeconds));
}

void createStats() {
    int heights[3] = {2001, 4099, 8100};
    int widths[3] = {2049, 4090, 8195};
    int i, j;

    filePtr = fopen("part2.csv", "w");
    fprintf(filePtr, "%s,%s,%s,%s,%s\n", "Matrix Height", "Matrix Width", "Block Size", "Time Taken(s)", "Time Taken(µs)");

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            int k;

            for (k = 2; k <= findMin(heights[i], widths[j]); k <<= 1) {
                transpose(heights[i], widths[j], k);
            }
            
            printf("height[%d] x width[%d] done.\n", heights[i], widths[j]);
        }
    }

    fclose(filePtr);
}

int main() {
/*     int height, width, blockSize;

    height = 6;
    width = 3;
    blockSize = 2;
    transpose(height, width, blockSize); */

    createStats();

    return 0;
}