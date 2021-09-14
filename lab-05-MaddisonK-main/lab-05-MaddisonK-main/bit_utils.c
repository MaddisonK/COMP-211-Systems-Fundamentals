#include "bit_utils.h"

char* itob(int num, int size){
    char *output = (char *) malloc(size - 1);
    int isnegative = 0;

    if (num < 0)
        isnegative = 1;

    // assign string terminator
    output[size-1] = '\0'; 

    // Convert integer to binary string output
    for (int i = size -1; i >= 0; i--) {
        if (num % 2)
            output[i] = '1';
        else 
            output[i] = '0';
        num = num / 2;
    }

    if (isnegative) {
        // reverse all 0s and 1s
        for (int i=0; i < size; i++){
            if (output[i] == '0')
                output[i] = '1';
            else
                output[i] = '0';
        }

        // binary addition to add 1
        for (int i=size-1; i >= 0; i--){
            if (output[i] == '0'){
                output[i] = '1';
                return output;
            } else {
                output[i] = '0';
            }
        }
    }

    return output;
}


int mask_bits(int num, int mask){
    return num & mask;
}

int set_bits(int num, int bits){
    return num | bits;
}

int inverse_bits(int num, int bits){
    return num ^ bits;
}

int bit_select(int num, int startbit, int endbit){
    int size_of_int = 32;
    startbit++;

    unsigned int shifted = num << (size_of_int - startbit);
    shifted = shifted >> (size_of_int - startbit + endbit);

    return shifted;
}
