// -----------------------------------
// COMP 211
//
// -----------------------------------


// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <stdio.h>	// IO functions
#include <stdlib.h> // atof function
#include "sort.h"	// sort function prototypes

int main( int argc, char** argv ) {
	// DO NOT MODIFY THIS PART OF THE CODE
	int n;
	scanf("%d", &n);
	v_struct items[n];

	// Scan in number of v_structs in array, followed by priority and value for each v_struct.
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &items[i].priority, &items[i].value);
		items[i].count = 0;
	}
	
	// The section below is meant to be edited so that you can call your implemented functions on the array and then print it.
	// You can comment / uncomment sections of code based on what you are testing and add new function calls / print statements if you wish to test in a different order.
	
	printf("+++++++++++++++++++++++\n");
	printf("Initial state of heap:\n");
	print_array( items, n );
	printf("+++++++++++++++++++++++\n");
	
	// printf("Max heapification on value:\n");
	// max_heapify_value( items, n );
	// print_array( items, n);
	// printf("+++++++++++++++++++++++\n");
	
	// printf("Min heapification on value:\n");
	// min_heapify_value( items, n );
	// print_array( items, n);
	// printf("+++++++++++++++++++++++\n");
	
	// printf("Max heapification on priority:\n");
	// max_heapify_priority( items, n );
	// print_array( items, n);
	// printf("+++++++++++++++++++++++\n");
	
	// printf("Min heapification on priority:\n");
	// min_heapify_priority( items, n );
	// print_array( items, n);
	// printf("+++++++++++++++++++++++\n");
	
	// printf("Sorting values in ascending order:\n");
	// sort_value( "ascend", items, n );
	// print_array( items, n );
	// printf("+++++++++++++++++++++++\n");
	
	// printf("Sorting values in descending order:\n");	
	// sort_value( "descend", items, n );
	// print_array( items, n );
	// printf("+++++++++++++++++++++++\n");
	
	// printf("Sorting priority in ascending order:\n");
	// sort_priority( "ascend", items, n );
	// print_array( items, n );
	// printf("+++++++++++++++++++++++\n");
	
	printf("Sorting priority in descending order:\n");
	sort_priority( "descend", items, n );
	print_array( items, n );
	printf("+++++++++++++++++++++++\n");   
   return 0;

}
