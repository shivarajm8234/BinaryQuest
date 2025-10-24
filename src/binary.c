#include "binary.h"

// Convert decimal to binary
int binaryConvert(int number,int *bits,int maxBits){
    int count=0;
    if(number==0){
        bits[0]=0;
        return 1;
    }
    while (number>0 && count<maxBits )
    {
        bits[count++]=number%2;
        number/=2;
    }
    //reverse bits for correct order
    for (int i=0;i<count/2;i++){
        int temp=bits[i];
        bits[i]=bits[count-i-1];
        bits[count-i-1]=temp;
    }
    return count;
}

// Convert octal to binary (octal number is given as decimal representation of octal)
// For example: 17 (octal) = 15 (decimal) = 1111 (binary)
int octalToBinary(int octalNumber, int* bits, int maxBits) {
    // Handle zero case
    if (octalNumber == 0) {
        bits[0] = 0;
        return 1;
    }
    
    // First convert octal to decimal
    int decimal = 0;
    int base = 1;
    
    while (octalNumber > 0) {
        int digit = octalNumber % 10;
        if (digit >= 8) {
            // Invalid octal digit - treat as 0
            bits[0] = 0;
            return 1;
        }
        decimal += digit * base;
        base *= 8;
        octalNumber /= 10;
    }
    
    // Then convert decimal to binary
    return binaryConvert(decimal, bits, maxBits);
}

// Convert hexadecimal to binary (hex number is given as integer value)
// For example: 0x1F = 31 (decimal) = 11111 (binary)
int hexToBinary(int hexNumber, int* bits, int maxBits) {
    // Hex number is already in decimal form, just convert to binary
    return binaryConvert(hexNumber, bits, maxBits);
}

// Universal conversion function
int convertToBinary(int number, int* bits, int maxBits, ConversionType type) {
    switch (type) {
        case CONVERSION_DECIMAL:
            return binaryConvert(number, bits, maxBits);
        case CONVERSION_OCTAL:
            return octalToBinary(number, bits, maxBits);
        case CONVERSION_HEXADECIMAL:
            return hexToBinary(number, bits, maxBits);
        default:
            return binaryConvert(number, bits, maxBits);
    }
}