#include <stdio.h>

int main()
{
	int c;
	while ((c = getchar()) > 10) {
		if (c < 97) {
			putchar(c);
		} else {
			putchar(c-32);
		}
	}
	printf("!!!\n");
	return 0;
}

// PID: 730322550
// I pledge the COMP211 honor code.