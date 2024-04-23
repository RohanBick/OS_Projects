/* Your code goes here */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *convert_hex_to_bin(const char *hexNumber) {
  int hexNumLength = strlen(hexNumber);
  char *binaryNumber = malloc(sizeof(char) * (hexNumLength * 4 + 1));
  if (binaryNumber == NULL) {
    return NULL;
  }
  int binaryNumberIndex = 0;
  for (int i = 0; i < hexNumLength; i++) {
    int binaryDigit = hexNumber[i] >= 'a' ? hexNumber[i] - 'a' + 10 : hexNumber[i] - '0';
    for (int j = 0; j < 4; j++) {
      binaryNumber[binaryNumberIndex++] = (binaryDigit >> (3 - j)) & 1 ? '1' : '0';
    }
  }
  binaryNumber[binaryNumberIndex] = '\0';
  return binaryNumber;
}
int main(int argc, char *argv[]) {
  // Check if the correct number of command line arguments have been provided.
  if (argc != 3) {
    fprintf(stderr, "Usage: ./sol2exe input-filename output-filename\n");
    exit(1);
  }
  FILE *input_fp = fopen(argv[1], "r");
  if (input_fp == NULL) {
    fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
    exit(1);
  }
  FILE *output_fp = fopen(argv[2], "w");
  if (output_fp == NULL) {
    fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
    exit(1);
  }
  char hexNumber[1024];
  while (fgets(hexNumber, sizeof(hexNumber), input_fp) != NULL) {
    if (hexNumber[strlen(hexNumber) - 1] == '\n') {
      hexNumber[strlen(hexNumber) - 1] = '\0';
    }
    char *binaryNumber = convert_hex_to_bin(hexNumber);
    if (binaryNumber == NULL) {
      fprintf(stderr, "Error: Cannot allocate memory\n");
      exit(1);
    }
    fprintf(output_fp, "%s\n", binaryNumber);
    free(binaryNumber);
  }
  fclose(input_fp);
  fclose(output_fp);

  return 0;
}
