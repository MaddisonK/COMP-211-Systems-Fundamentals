# Lab 03

## Introduction
In this lab, you will gain familiarity with many concepts, which are pertinent to both C Programming and Systems as a whole. To be concise, you will make use of the following concepts:
* C Concepts:
  * Arrays
  * Structs
  * Functions
  * Interfaces (Headers)
  * Pointers (Passing Variables by Reference)
* Systems Concepts:
  * Sorting
  * Priority Queues / Binary Heaps

In this lab, you will implement the familiar HeapSort algorithm from Data Structures, as well as the more "object-oriented" features of C. Motivating this idea is the abstract idea of a priority queue - a data structure in which jobs are scheduled at different priorities that an operating system uses to determine what jobs it should focus on first. For example, an operating system would likely give high priority to I/O operations - it's important that the CPU reacts as soon as you type a key on the keyboard. On the other hand, a long download may have lower priority because the result is not expected to be immediate, so a little "slowness" is okay. So now that we've motivated this lab, let's get started!

#### Background Reading
* Refer to the following for information on the necessary C concepts to complete this lab:
  * Read section 4.5 in *The C Programming Language* for an explanation about how header files work in C. In this lab, you are provided with a header file that specifies the functions that you must implement.
  * Read Chapter sections 5 in *The C Programming Language* to understand the relationship between pointers and arrays. This lab involves swapping structs within an array, and because structs are complex data types they are copied by reference (pointer) instead of value. Specifically sections 5.0 to 5.5 have relevance to the lab.
  * Read Chapter sections 6 in *The C Programming Language* to understand the concept of a struct and how structs can be manipulated. Specifically sections 6.0 to 6.5 have relevance to the lab.
* Refer to the following for a review on binary heaps, and the mapping between an array and a heap:
	* Provided slides in this repository.
	
#### Compiling with a Make File
In Lab02, you learned that you could use the following command to give a custom name to your compiled program's executable file:
```
gcc -o ExecutableName SourceProgram
./ExecutableName
```
This is a perfectly reasonable way to compile programs, but it is a bit lengthy. Fortunately, GNU has created a tool called GNU Make, which lets you compile a program and generate an executable with one short command: `make`.

To make use of Make, you must provide a file called `Makefile`, which includes information about the source code - essentially, it includes all of the information that you previously would have entered on the command line. For this lab, we have provided the `Makefile` for you, and it is not necessary to understand all the details but you should read the `Makefile` to get an idea of what it is doing. If you are interested in learning more, check out the [official documentation](https://www.gnu.org/software/make/).

In simple terms, you just need to enter the command `make` and your code will compile, generating an executable called `lab03`.

#### Lab 03 Structure
Unlike previous labs, you will not be asked to implement a program in `main`- instead, you will implement a set of functions to be called by an external program. That is, we will run your code on our own `main` file to test the correctness of your functions. We have provided a file called `lab03.c` in which you can call your functions and test with examples we provide, as well as your own tests (information about testing is further down). There are two additional starter files for this lab: `sort.h` and `sort.c`.

`sort.h` is an example of a header file. You may be familiar with the notion of header files from previous labs, in which you used the `#include` directive to include headers such as `stdio.h` and `stdlib.h` in order to use certain functions. The gcc manual defines two different types of header files:

* System header files declare the interfaces to parts of the operating system. You include them in your program to supply the definitions and declarations you need to invoke system calls and libraries.
* Your own header files contain declarations for interfaces between the source files of your program. Each time you have a group of related declarations and macro definitions all or most of which are needed in several different source files, it is a good idea to create a header file for them.

`stdio.h` and `stdlib.h` are examples of system header files, while `sort.h` is a self-defined header file. Essentially, `sort.h` acts as an interface which promises that the function declarations contained within will be implemented in a separate source file. Your job for this lab is to modify `sort.c` such that you implement the functions which have declarations in `sort.h` but are not present in `sort.c`.

In summary, here is a description of each file and what you are expected to do with each one:
* `Makefile`- contains information used to compile your program with one command. **Do not modify.**
* `sort.h`- contains declarations of each function that will be included in the final program. **Do not modify.**
* `sort.c`- contains implementations of each function. You are to implement the following: `max_heapify_value`, `min_heapify_value`, `max_heapify_priority`, `min_heapify_priority`, `sort_value`, and `sort_priority`. **Do not modify the `print_array`and `swap` functions.**
* `lab03.c` - contains a main function that scans in values. You may modify the specified section to test your functions, but **do not modify the loop that scans in values**.

#### Source Code Header
Each source file in COMP211 will begin with the standard header comment below. Note this header is checked by the
autograder for an exact match. Please be sure to format your PID as a single 9-digit number with no spaces nor
dashes. Additionally, be sure your capitalization and punctuation of the honor code pledge are correct. Since we do
grade manually for style we do not include names on code listings to avoid biasing the grading.
```
// PID: 9DigitPidNoSpacesOrDashes
// I pledge the COMP211 honor code.
```

#### Vim Tips
If you have decided to stick with vim for this course, you should be used to basic navigation in vim with keystrokes like `w`,`b`,`e`, deleting words and lines by combining the delete key with directional keystrokes like `dw`, `dd`, `d2j`, undoing things with `u`, and directional movements like `gg` and `G`. If you are not familiar, it is recommended that you revisit the `vimtutor` from time to time so that editing documents becomes easier.

This assignment has multiple c files that you may want to refer to while you're coding. Luckily, vim supports window panes and tabs. While editing a file in vim you can type `:tabe <file_name>` to open <file_name> in a new tab (Note that using the \<TAB\> key for autocompletion works here too). In normal mode, navigate between tabs with `gt` to go to the next tab, and `gT` to go to the previous tab. `:q` will still work to close the single file you are editing, `:qa` will close all the tabs you have open.

Additionally, from the command line, you can open several vim tabs automatically when you start vim. Instead of `vim <file>`, do `vim -p <file1> <file2> <file3> ...`. You could also do `vim -p *.c *.h` to open all c and header files in your current directory in vim tabs.

# The Assignment

## Part 0. Problem Description
On a high level, this lab defines a structure called `v_struct` and asks you to write functions that will perform heap operations on an array of `v_struct`s. The definition of `v_struct` is as follows:
```
typedef struct {

	unsigned int priority;	// priority (zero or postive integer)
	int value;		// value (zero, postive, or negative integer)
	int count;		// number of times the v_struct has been swapped with another v_struct

} v_struct;
```
In other words, `v_struct` is a struct containing three properties: `priority`, `value`, and `count`. This struct is meant to abstractly represent a job and its priority. To help motivate this abstraction, we might think of a shop that is fulfilling orders, where `value` is the revenue generated by an order and `priority` is how important a particular order is relative to other orders (with higher priority indicating greater importance). With this example, you might expect a greatly positive `value` to correspond to high `priority` but you might also imagine an order with negative `value` (e.g. loss of revenue) that has a high `priority` (e.g. the person who placed the order is one of your most valued customers). The point is, high `priority` indicates higher importance of a job, and `value` is some attribute related to that job. 

In this lab, you will implement four functions to create a maximum or mininimum heap of an array of `v_struct`s. When making calls to these functions, you will need to swap `v_structs` in an array. The function to swap `v_struct`s has already been provided:
```
void swap (v_struct* p1, v_struct* p2) {
  v_struct  temp = *p1;
  *p1 = *p2;
  *p2 = temp;
  p1->count++;
  p2->count++;
}
```
Read through it to make sure you understand the syntax, but do not modify it or the `print_array` function as they will both be used for grading. Each `v_struct` has a property called `count` that starts at 0 and is incremented whenever the `v_struct` is swapped during heapification / sorting. This is used to check that you have implemented the heapsort.

### Testing

You will be provided with a driver file called `lab03.c` that reads in an integer and a list of paired values and automatically creates the `v_struct` array to pass to calls of your functions. This driver file expects inputs in the following format:
```
n (number of vstructs)
priority_0 value_0
priority_1 value_1
...
priority_n value_n
```

For example, the input
```
2
1 4
5 9
```
will create an array of the following structure:
```
+++++++++++++++++++++++
Initial state of heap:
[priority: 1 value: 4 count: 0]
[priority: 5 value: 9 count: 0]
+++++++++++++++++++++++
```
You can use the `lab03.c` file to initialize a `v_struct` array, then call your functions on the array to test for correctness. Try showing the output from `print_array` for one of the provided lists!
```
make
./lab03 < tests/heapsize7.txt
+++++++++++++++++++++++
Initial state of heap:
[priority: 1 value: 4 count: 0]
[priority: 5 value: 9 count: 0]
[priority: 2 value: -5 count: 0]
[priority: 3 value: 24 count: 0]
[priority: 9 value: 11 count: 0]
[priority: 20 value: 5 count: 0]
[priority: 8 value: -2 count: 0]
+++++++++++++++++++++++
```

## Part 1. Heapification
Implement each of the four `heapify` functions in the `sort.c` file: `max_heapify_value`, `min_heapify_value`, `max_heapify_priority`, and `min_heapify_priority`. 
Each function should take an array of `v_structs` as an argument and organize the array by swapping elements so that it satisfies the given properties for:
- `max_heapify_value`: maximum binary heap based on the value of `value`.
- `min_heapify_value`: minimum binary heap based on the value of `value`.
- `max_heapify_priority`: maximum binary heap based on the value of `priority`.
- `min_heapify_priority`: minimum binary heap based on the value of `priority`.

Consult the provided pdf for a refresher on how an array can be treated as a binary heap.

**Note**: These functions do not have a return value. Since we are passing in a pointer to a v_struct array, any changes to the array change the original array.

#### Testing Heapification
In the folder `tests`, we have provided two sample inputs - `heapsize7.txt` and `heapsize15.txt`. These inputs are used to create arrays of 7 and 15 `v_struct`s respectively. You may use these inputs along the provided `lab03.c` driver code to see how your heapification is working for these arrays. Expected outputs after one call of each `heapify` function are provided. 

In order to test `max_heapify_value`, for example, you would first uncomment the following lines in `lab03.c`, ensuring that they are the only uncommented print statements:
```
// printf("Max heapification on value:\n");
// max_heapify_value( items, n );
// print_array( items, n);
```
Then, compile your code and provide one of the test files as input. You can then use the `diff` command to see if the result matches what was expected:
```
make
./lab03 < tests/heapsize7.txt > myresult
diff myresult tests/7_maxheap_value.txt
```
**Note**: The provided tests assume that you have called ONLY the function being tested. If you make multiple function calls, you will end up with different `count`s because the elements of the array will have been swapped more times than if the function had been called once.

## Part 2. In-Place HeapSort
Heapification in Part 1 rearranged the array into a min or max binary tree, but did not completely sort the array to be in ascending or descending order, which is what we will do now. Implement the `sort_value` and `sort_priority` functions as specified in `sort.c` such that each function takes in three parameters: `sort_type` (a string with the value `"ascend"` or `"descend"`), `v_array` (your array of v_structs), and `v_length` (the length of `v_array`). Your implementation should be in-place; that is, you are only allowed to swap elements within the heap and you may not use any extra variables in your sort. You should return a value of -1 if there is an error, and a value of 0 otherwise. Here are some hints on how the HeapSort algorithm is implemented (more details are available in the provided pdf):

* Start with a `heapify` call so that the beginning of the array starts with a max / min value.
* You will make `n` calls to a `heapify` function over the course of your sort.
* `heapify` results in a maximum / minimum value at the beginning of the array, and it can be called on a subset of the array. How can this property be used to sort in-place?

#### Testing HeapSort
Testing for heapsort works the same way as testing for heapification. For example, if you want to test how your heapsort is sorting values in ascending order, you would uncomment these lines of code:
``` 
// printf("Sorting values in ascending order:\n");
// sort_value( "ascend", items, n );
// print_array( items, n );
```
And then run these commands:
```
make
./lab03 < tests/heapsize7.txt > myresult
diff myresult tests/7_heapsort_value_ascend.txt
```

## Submit your assignment
Assignment submissions will be made through [GradeScope](https://www.gradescope.com).

To submit your assignment, you must commit and push your work to this repository using git. The steps are the same as previous labs.

1. Navigate to the base folder of the repository within Docker.
2. Type `git status`. You should see a list of changes that have been made to the repository.
3. Type `git add *`. This signals that you want to place all modified / new files on the "stage" so that their changes can take effect.
4. Type `git commit -m "Your Message Here"`. This shows that you are "committing" to the changes you put on the stage. Instead of Your Message Here, you should write a meaningful message about what changes you have made.
5. Type `git push origin main`. This takes the commit that was made locally on your machine and "pushes" it to GitHub. Now, when you view this repository on GitHub you should be able to see the changes you've made, namely the addition of your source files!
6. Go to the COMP 211 course in GradeScope and click on the assignment called **Lab 03**.
7. Click on the option to **Submit Assignment** and choose GitHub as the submission method.
8. You should see a list of your public repositories. Select the one named **lab-03-yourname** and submit it.
9. Your assignment should be autograded within a few seconds and you will receive feedback for the autograded portion.
10. If you receive all the points, then you have completed this lab! Otherwise, you are free to keep pushing commits to your GitHub repository and submit for regrading up until the deadline of the lab.

## Grading
- Autograder (8pts): Check correctness of the algorithms. Submit as many times as you like before the due date to get full credit.
  - *Note:* during manual checking, we will make sure you didn't add more include statements and that you actually implemented the algorithms rather than hard-coded around the test cases. We reserve the right to take back autograder points if your algorithms are specific to the test cases and will not work generally. 
- Style (2pts): Code readability.
  - 1 pt: Reasonable function/variable names. No global variables.
  - 1 pt: No commented old debug code or unused variables, correct indentation. (*Hint*: If you go to the top of the file in vim and type `=G` in normal-mode, it will auto-format your c file)
