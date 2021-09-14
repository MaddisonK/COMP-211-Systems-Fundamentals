// PID: 730322550
// I pledge the COMP211 honor code.

#include <stdio.h>

int main(int argc, char *argv[])
{
	char bits[33];
	// inversion
	for (int i = 0; i < 32; i++) {
		if (argv[1][i] == '1') {
			bits[i] = '0';
		} else {
			bits[i] = '1';
		}
	}
	//
	// add 1
	int i = 31;
	while (i >= 0) {
		if (bits[i] == '0') {
			bits[i] = '1';
			break;
		}
		i--;
	}
	bits[32] = '\0';
	printf("The negation of the 32-bit two's complement integer %s is: %s.\n", argv[1],bits);
	return 0;
}