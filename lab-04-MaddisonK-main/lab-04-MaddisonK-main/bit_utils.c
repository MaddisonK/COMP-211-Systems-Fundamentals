// PID: 730322550
// I pledge the COMP211 honor code.

#include "bit_utils.h"

int p2(int power);

// Returns num after mask has been applied.
int mask_bits(int num, int mask) {
	return num & mask;
}

// Returns num after bits have been set.
int set_bits(int num, int bits) {
	return num | bits;
}

// Returns num after bits have been inversed.
int inverse_bits(int num, int bits) {
	return (~(num & bits) & bits) | (num & ~bits);
}

int bit_select(int num, int startbit, int endbit) {
	return ((unsigned) num << (SIZE - (startbit + 1))) >> (SIZE - (startbit + 1) + endbit);
}

char* itob(int n, int size) {
	char* bits = (char*) malloc(size + 1);
	char* p = bits;
	int x; //keep track of value, essentially counting
	
	//determine sign
	if (n >= 0) {
		bits[0] = '0';
		for (int i = 1; i < size; i++) {
			if (p2((size - 1) - i) <= n) {
				bits[i] = '1';
				n -= p2((size - 1) - i);
			} else {
				bits[i] = '0';
			}
		}
	} else {
		bits[0] = '1';
		x = -1 * p2((size - 1));
		for (int i = 1; i < size; i++) {
			if ((p2((size -1) - i) + x) <= n) {
				bits[i] = '1';
				x += p2((size - 1) - i);
			} else {
				bits[i] = '0';
			}
		}
		n -= x; // sets n to 0 if it worked out
	}
	bits[size] = '\0';
	return p;
}

int p2(int power) {
	int out = 1;
	if (power == 0) {
		return 1;
	}
	if (power == 1) {
		return 2;
	}
	for (int i = 0; i < power; i++) {
		out *= 2;
	}
	return out;
}