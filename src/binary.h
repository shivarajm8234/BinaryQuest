#ifndef BINARY_H
#define BINARY_H

// Conversion types
typedef enum {
    CONVERSION_DECIMAL,
    CONVERSION_OCTAL,
    CONVERSION_HEXADECIMAL
} ConversionType;

int binaryConvert(int number, int* bits, int maxBits);
int octalToBinary(int octalNumber, int* bits, int maxBits);
int hexToBinary(int hexNumber, int* bits, int maxBits);
int convertToBinary(int number, int* bits, int maxBits, ConversionType type);

#endif
