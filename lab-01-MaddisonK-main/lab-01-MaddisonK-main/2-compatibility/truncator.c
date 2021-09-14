#include <stdio.h>

int main()
{
	int i, l, c; // i is the word index, l is the line index, c is the character
	int list_l[100]; // list of l indices
	i, l = 0;
	for (int j = 0; j<100; ++j) //set line index array to all 101
		list_l[j] = 101;

	c = getchar();
	while (c != EOF) {
		putchar(c);
		++i;
		if (c == '\n') {
			++l;
			i = 0;
		}
		c = getchar();
		if (i == 40) {
			if (c == '\n')
				continue;
			printf("\n");
			i = 0;
			list_l[l] = l;
		}
	}
	int total = 0; // sum of lines that are not 101
	for (int j = 0; j<100; ++j) {
		if (list_l[j] != 101)
			++total;
	}
	printf("\nTotal lines over 40 chars: %d", total);
	printf("\nOffending lines: ");
	for (int j = 0; j<100; ++j) { // printing each non 101 line index
		if (list_l[j] != 101)
			printf("%d, ", list_l[j]);
	}
	printf("\n");
	return 0;
}

// PID: 730322550
// I pledge the COMP211 honor code.