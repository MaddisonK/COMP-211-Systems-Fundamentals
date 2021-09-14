# Lab 07

In this lab, we will model the ALU discussed in class and implement all of its functionality! At a high level, a ALU consists of an adder, a shifter, a logic circuit, and two multiplexers. But with the help of wires and control flags, all the pieces can be put together into a cohesive unit that performs our favorite arithmetic and logical operations. The general structure for all of the pieces has been provided to you; it is up to you to implement their control flag logic and actual behavior so that they work like a real ALU. 

### Reading
* C Programming Language Section 6.7 - typedef
* C Programming Language Section 5.11 - Function Pointers
* [ALU section of Circuit Design Lecture](https://sakai.unc.edu/access/content/group/bcb48c3b-5188-4ec4-ac95-ac2b53652fc7/Lecture%20Slides/Circuit_Design.pdf)

### File Structure
Three files are included in this lab: 
* `lab07.c` - This is the driver code that you can use to test your code. It takes inputs for the circuits from stdin and prints the output. **Do not modify.**
* `alu.h` - This header contains definitions of all the structs used in this lab (`arithmetic`, `shift`, `logic`, and `mux`) and their accompanying functions. Each struct has an associated `exe` (execution) and `cfg` (configuration) function; the implementation of these functions is your main task for the lab.  **Do not modify.**
* `alu.c` - Implementations of undefined functions in `alu.h` belong in this section. This is the only file that should be edited.

### Honor Code Header
Please include the honor code header at the top of the `alu.c` file.  Since we do grade manually for style we do not include names on code listings to avoid biasing the grading.
```
// PID: 9DigitPidNoSpacesOrDashes
// I pledge the COMP211 honor code.
```

## A Note on ALUs
The goal of this lab is to simulate an ALU, similar to how Lab 05 simulated a cache. 
However, ALUs are hardware components made up of wires and circuits that implement logical functions and there are many important distinctions between what a real ALU does and how we can simulate it with C code. 

In a real ALU, the inputs are electrical signals and the output is merely a combination of the inputs that have traversed through some circuits. In this lab, you will implement a `configure_alu` and `run_alu` function. The configuration function sets the appropriate ALUFN control signals and the `run_alu` function returns the output of the ALU with A and B as parameters. However in a real ALU, the ALUFN bits, A, and B would all be given as "inputs" simultaneously.


## Part 0: Problem Overview
The following is a picture of the ALU discussed in class, illustrating that the ALU is actually composed of four types of logical units, each of which contains two 32-bit inputs, some Boolean control flag inputs (i.e. `Sub`, `Bool`, `Shft`, and `Math`), and an output. The overall output of the ALU is determined by the particular values of each control flag, and all valid combinations of control flag inputs are shown in the table on the bottom right. Recall that an X means that the flag can be 0 or 1 as it has no effect on the output for that configuration.


![Here is an image.](https://user-images.githubusercontent.com/34545607/113628292-affc2280-9632-11eb-8786-edd9b8bc97d5.png)

Each of the four units has a corresponding struct defined in `alu.h`. For example, for the Add/Sub unit in the diagram, the corresponding struct is:
```
typedef struct arithmetic {
	
	int A;					// 32 bit input
	int B;					// 32 bit input
	int Y;					// 32 bit output
	char Sub;				// ALUFN: Sub = 0, 1, or X
	arithmetic_exe exe;		// execute circuit
	arithmetic_cfg cfg;		// configure circuit

} arithmetic;
```

Each struct has A and B as inputs, Y as the output, a field for the relevant control flag (i.e. `Sub`), and an associated `exe` and `cfg` function. The `cfg` function is used to initialize and validate inputs and control flags, and the `exe` function is used to set Y to the output of the circuit. Note that the `exe` function has a type of `arithmetic_exe` and the `cfg` function has a type of `arithmetic_cfg`. These are not built-in types, so where do they come from? Additionally, this is the first time we have seen functions as a field in a struct. What is going on here?

Notice nearby the following declarations:
```
typedef void (*arithmetic_exe) ( struct arithmetic* self );
typedef int (*arithmetic_cfg) ( struct arithmetic* self  );
```

We have used the `typedef` feature of C, which allows you to give alternate names to types of functions and structs. In this case, we are defining `arithmetic_exe` as the type for a void function that takes an `arithmetic` struct as a parameter. Similarly, `arithmetic_cfg` is a name for a function that returns an int and takes an `arithmetic` struct as a parameter. That's how we are able to get a field of type `arithmetic_exe` in the `arihtmetic` struct! Take a look at Section 6.7 in the Recommended Reading for a deeper understanding of this.

Also note the nearby declarations of the following functions:
```
void arithmetic_fun_exe( arithmetic* circuit );
int arithmetic_fun_cfg( arithmetic* circuit );
```

These declarations are associated with the actual function definitions, which are to be implemented in `alu.c`. But it turns out, we can assign a function pointer (see Section 5.11) as a field in a struct with a statement like: `arithmetic_circuit->exe = arithmetic_fun_exe;`. This lets us treat structs in a manner similar to Classes in Java, where a struct is just a combination of functions and values! You will have to bind the expected `cfg` and `exe` functions to the appropriate field in each struct.

Read carefully through the definition of each struct and understand how they relate to the diagram before proceeding.

## Part 1: Completing the Circuits

Your task for Part 1 is to complete all of the unimplemented "`exe`" functions: namely, `arithmetic_fun_exe`, `shift_fun_exe`, `logic_fun_exe`, and `mux_fun_exe`. Note that all of the `cfg` functions are already complete; all they do is validate inputs for a particular struct, returning `OK` if all inputs are valid and `CONFIG_ERROR` otherwise.

The behavior of each circuit when provided certain control flags is shown in the diagram in Part 0. All control flags are represented as characters, and each flag may only take the value `0`, `1`, or `X` (except for `Bool`, which is represented as a string with 2 of these values and a null terminator).

Each `exe` function expects an argument `circuit` which has already been initialized (all fields except the output of the `circuit`'s struct have been assigned to `'0'`, `'1'`, or `'X'`). In each `exe` function, you need to implement the expected behavior of the circuit, given the inputs provided from the `circuit` struct and then set `circuit->Y` to the result.

This simulates the "execution" of a circuit given certain inputs and control flags. 

#### Example
Suppose we have an `arithmetic` struct named `arithstruct` with fields initialized to the following values (note that this is not proper C syntax and is just used for illustration: 
```
arithstruct = arithmetic struct {
	int A = 0x00000010;					// 32 bit input
	int B = 0x00000020;					// 32 bit input
	int Y = ??????????;					// 32 bit output
	char Sub = 0;						// ALUFN: Sub = 0, 1, or X
	arithmetic_exe exe = arithmetic_fun_exe;		// execute circuit
	arithmetic_cfg cfg = arithmetic_fun_cfg;		// configure circuit
} 
```
Every field has a value that is initialized, except for Y. To "use" the arithmetic circuit `arithstruct` we configure and execute it as follows:
```
arithstruct->cfg(arithstruct);
arithstruct->exe(arithstruct);
```

Since the `Sub` flag is set to 0, we must perform the operation `A + B`, according to the ALUFN flag table. After this code executes, the state of `arithstruct` would be:
```
arithstruct {	
	int A = 0x00000010;					// 32 bit input
	int B = 0x00000020;					// 32 bit input
	int Y = 0x00000030;					// 32 bit output
	char Sub = 0;						// ALUFN: Sub = 0, 1, or X
	arithmetic_exe exe = arithmetic_fun_exe;		// execute circuit
	arithmetic_cfg cfg = arithmetic_fun_cfg;		// configure circuit
} 
```
Note that the only difference between this and the original state is that Y is now set to the sum of A and B. Getting Y to be the correct value for each combination of control flags is all you have to do for each `exe` function.

**Note**: Do not worry about implementing both right shift logical (>>) and right shift arithmetic (>>>). For these operations, you can simply use the `>>` operator.

### Testing
In `lab07.c`, note that the main function simply contains a call to `part1_test`. Read this function carefully to understand its behavior. In the directory `part1_tests`, there will be sample inputs and outputs for various executions of various circuits. Like in previous labs, you will be able to `diff` the expected output with your result to determine whether your code is correct.

#### Using the ALU
To "use" the ALU, type `make all` to compile the lab, then execute the `lab07` executable file. This will run the driver code from `lab07.c`. It will ask you for the 5 bit ALUFN, the 32 bit hex values for A and B and an ALU component to use, then it will give you the output from the component given those inputs.

## Part 2: Putting the ALU Together

Now that all of the pieces are in functioning, it's time to put them together! 

Each circuit in the diagram of Part 0 is instantiated as a global variable in `alu.h`:
```
struct mux* math_mux;
struct mux* shft_mux;
struct shift* shift_circuit;
struct logic* logic_circuit;
struct arithmetic* arithmetic_circuit;
```
For part 2, complete the `configure_alu` and `run_alu` functions in `alu.c`. When `configure_alu` and `run_alu` are called one after another, it is analagous to connecting all the ALUFN wires to their component parts and then connecting the wires for each bit in `A` and each bit in `B` and having the correct value finally flow through to the output `Y`.

`configure_alu` sets the control flags of every circuit to the user-supplied value. The return value should be an integer `1`, if there was an error (incorrect input) or `0`, if everything was configured correctly. Note that if you call a circuit's `cfc` function on itself, it will do this validation and return either `OK` (0) or `CONFIG_ERROR` (1, as defined in `alu.h`). Validate each circuit that you "configure" in `configure_alu` and return the correct value.

`run_alu` performs the desired operation on `A` and `B` based on the supplied control flags that were set in `configure_alu` and returns the result of the computation. Note that you shouldn't use any logic (like `if` statements, math, `&`s, `|`s) in `run_alu`, you must properly configure each circuit, then execute them using the dataflow in the diagram and return the correct output. 

For example, given the following inputs to the program:
```
A = 0x00000010
B = 0x00000020
Sub = 0
Bool = XX
Shft = X
Math = 1
```
The result returned by `run_alu` would be 0x0000030.

### Testing
Testing is identical to Part 1. Simply comment out the line calling `part1_test` and uncomment the line calling `part2_test`. The corresponding tests are in the directory `part2_tests`.

## Submit the Assignment
**You will need to comment out your main function for the grader to work properly.** Additionally, every function with a return value must return something in order for the grader to run. If you get a message saying the autograder failed to execute, please make sure you have included a well-formatted Honor Code header and that there are no compilation errors on your end.

1. Use git to push your finished code to this Github repository.
2. Go to the COMP 211 course in GradeScope and click on the assignment called **Lab 07**.
3. Click on the option to **Submit Assignment** and choose GitHub as the submission method.
4. You should see a list of your public repositories. Select the one named **lab-07-yourname** and submit it.
5. Your assignment should be autograded within a few seconds and you will receive feedback for the autograded portion.
6. If you receive all the points, then you have completed this lab! Otherwise, you are free to keep pushing commits to your GitHub repository and submit for regrading up until the deadline of the lab.

## Grading
#### Autograder Tests (8 pts)
#### Style (2pts)
* Variable Names (.5 pts)
	* Single-character variable names are only used for counting/indexing, or when used to represent a variable whose type has only one instance.
	* All "magic numbers" are defined as constants.
	* Variable names are either related to the usage of the variable, or the meaning is commented.
	* No unused variables are declared.
* Readability (.75 pts)
	* Proper indentation (use the following Vim trick: (1G=G) )
	* Consistent whitespace theme used throughout.
		* Always put consecutive curly braces one line apart (or two, depending on preference).
	* Logically distinct blocks of code are separated by a whitespace.
	* No more than two consecutive lines of empty whitespace.
	* No old debug code is present (including in the comments).	
* Correctness of Code (.75 pts)
	* For all non-void functions (including main if applicable), every path of execution leads to a return statement.
	* No libraries are included that weren't provided in the starter code or mentioned in the readme.
	* `run_alu` only sets circuit components and runs their "`exe`" functions, no C logic is used (i.e. no `if`s, no boolean logic, no +, -, etc).

We reserve the right to deduct points from the autograder if we notice that you have hard-coded any test cases and not actually fully implemented the functions.
