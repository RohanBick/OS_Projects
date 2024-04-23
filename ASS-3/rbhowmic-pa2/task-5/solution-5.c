/* Your code goes here */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sort.h"
typedef struct {
    uint32_t key;
    uint32_t values[24];
} BinObj;
int cmpBinObjs(const void *a, const void *b) {
    return ((BinObj*)a)->key - ((BinObj*)b)->key;
}
int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "File using: %s <inputfile> <outputfile>\n", argv[0]);
        exit(1);
    }
    FILE *inputFile = fopen(argv[1], "rb");
    if(inputFile == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(1);
    }
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    rewind(inputFile);
    if(fileSize % sizeof(BinObj) != 0) {
        fprintf(stderr, "Error has occurred\n");
        exit(1);
    }
    size_t numObjects = fileSize / sizeof(BinObj);
    BinObj *objects = (BinObj*) malloc(fileSize);
    if(objects == NULL) {
        fprintf(stderr, "Error has occurred\n");
        exit(1);
    }
    fread(objects, sizeof(BinObj), numObjects, inputFile);
    fclose(inputFile);
    qsort(objects, numObjects, sizeof(BinObj), cmpBinObjs);
    FILE *outputFile = fopen(argv[2], "wb");
    if(outputFile == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[2]);
        free(objects);
        exit(1);
    }
    fwrite(objects, sizeof(BinObj), numObjects, outputFile);
    fclose(outputFile);
    free(objects);
    return 0;
}

