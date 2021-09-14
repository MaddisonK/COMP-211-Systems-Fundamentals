// PID: 730322550
// I pledge the COMP211 honor code.

#include <stdio.h>
#include <stdlib.h>
int p2(int power);

int main(int argc, char *argv[])
{
	char bits[17]; //represents 16 bits
	int n = atoi(argv[1]); //the number inputted
	int x; //keep track of value
	
	//determine sign
	if (n >= 0) {
		bits[0] = '0';
	} else {
		bits[0] = '1';
		n *= -1;
	}
	for (int i = 1; i < 16; i++) {
		if (p2(15 - i) <= n) {
			bits[i] = '1';
			n -= p2(15 - i);
		} else {
			bits[i] = '0';
		} 
	}
	bits[16] = '\0';
	printf("The signed 16-bit representation of %s is: ", argv[1]);
	if (n==0) {
		printf("%s.\n", bits);
	} else {
		printf("not possible.\n");
	}
	
	return 0;
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

