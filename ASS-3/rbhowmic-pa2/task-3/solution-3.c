/* Your code goes here */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *convert_bin_to_hex(const char *binaryNumber) {
  // Calculate the length of the binary number.
  int binaryNumberLength = strlen(binaryNumber);
  // Allocate memory to store the hexadecimal number.
  char *hexadecimalNumber = malloc(sizeof(char) * (binaryNumberLength / 4 + 1));
  if (hexadecimalNumber == NULL) {
    return NULL;
  }
  int hexadecimalNumberIndex = 0;
  for (int i = 0; i < binaryNumberLength; i += 4) {
    int hexadecimalDigit = 0;
    for (int j = 0; j < 4; j++) {
      hexadecimalDigit |= (binaryNumber[i + j] - '0') << (3 - j);
    }
    hexadecimalNumber[hexadecimalNumberIndex++] = hexadecimalDigit >= 10 ? hexadecimalDigit - 10 + 'a' : hexadecimalDigit + '0';
  }
  hexadecimalNumber[hexadecimalNumberIndex] = '\0';
  return hexadecimalNumber;
}
int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./sol3exe input-filename output-filename\n");
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
  char binaryNumber[1024];
  while (fgets(binaryNumber, sizeof(binaryNumber), input_fp) != NULL) {
    if (binaryNumber[strlen(binaryNumber) - 1] == '\n') {
      binaryNumber[strlen(binaryNumber) - 1] = '\0';
    }
    char *hexadecimalNumber = convert_bin_to_hex(binaryNumber);
    if (hexadecimalNumber == NULL) {
      fprintf(stderr, "Error: Cannot allocate memory\n");
      exit(1);
    }
    fprintf(output_fp, "%s\n", hexadecimalNumber);
    free(hexadecimalNumber);
  }
  fclose(input_fp);
  fclose(output_fp);

  return 0;
}
