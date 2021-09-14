// -----------------------------------
// COMP 211
//
// -----------------------------------

// -----------------------------------
// C-Style data structure 
//

#ifndef _MYSTRUCT_H_
#define _MYSTRUCT_H_
typedef struct {

	unsigned int priority;	// priority (zero or postive integer)
	int value;				// value (zero, postive, or negative integer)
	int count;

} v_struct;
#endif

// ------------------------------------
// Function prototype
// description: swaps two elements of a v_struct array and increments the count property of both elements.

void swap (v_struct* p1, v_struct* p2);

// ------------------------------------
// Function prototype
// description: creates a maximum heap with the value property;
// 		that is, if the array were organized as a binary tree, each element's value is greater than that of its children.
// arguments:
// 	v_array = array
// 	array_length = length of array

void max_heapify_value(v_struct* v_array, int array_length);

// ------------------------------------
// Function prototype
// description: creates a minimum heap with the value property;
// 		that is, if the array were organized as a binary tree, each element's value is smaller than that of its children.
// arguments:
// 	v_array = array
// 	array_length = length of array

void min_heapify_value(v_struct* v_array, int array_length);

// ------------------------------------
// Function prototype
// description: creates a maximum heap with the priority property;
// 		that is, if the array were organized as a binary tree, each element's priority is greater than that of its children.
// arguments:
// 	v_array = array
// 	array_length = length of array

void max_heapify_priority(v_struct* v_array, int array_length);

// ------------------------------------
// Function prototype
// description: creates a minimum heap with the priority property;
// 		that is, if the array were organized as a binary tree, each element's priority is smaller than that of its children.

void min_heapify_priority(v_struct* v_array, int array_length);

// ------------------------------------
// Function prototype
// description: sort (using in-place heapsort algo) the array using value field 
// return:
//	 error code: -1=error or 0=no error
// arguments:
//	 sort_type = "ascend" or "descend"
// 	 v_array = array 
//   array_length = number of v_struct elements in array

int sort_value( char* sort_type, v_struct* v_array, int array_length );

// ------------------------------------
// Function prototype
// description: sort (using in-place heapsort algo) the array using priority field
// return:
//	 error code: -1=error or 0=no error)
// arguments:
//	 sort_type = "ascend" or "descend"
// 	 v_array = array 
//   array_length = number of v_struct elements in array
int sort_priority( char* sort_type, v_struct* v_array, int array_length );

// ------------------------------------
// Function prototype
// print the array in standard format that can be easily graded
// no return
// v_struct* = array 
// array_length = number of v_struct elements in array
void print_array( v_struct* v_array, int array_length );

