# Lab 05

## Introduction
Caches are implemented in almost all non-trivial computing systems, as they mitigate the lengthy time that it takes to retrieve memory from disk.
This fact serves as the primary motivation for this lab, in which you will implement a simulator for a cache that can support either the direct mapping or fully associative design scheme. In this lab, you will also make use of the bit masking and selection functions from the `bit_utils.c` library that you implemented in Lab 04.

In addition to reinforcing concepts from previous labs, this lab also emphasizes new systems concepts learned in class:

* Old Concepts
  * Structs
  * Interfaces (Headers)
  * Pointers (Passing Values by Reference)
* New Concepts
  * Caches / Cache Algorithms
  * Addressing Physical Memory

The size of the cache and physical memory in this simulator will be much smaller than it would be in practice, which will make it easier to reason about.
In particular, you will be working with 8 cache lines and a physical memory with 256 addresses (each of which stores a byte). Let's start working with our compact cache and begin!

#### Background Reading

* Refer to the following for a review on caches, and the differences between the direct mapping and fully associative cache mapping functions.
  * [Memory Organization slides from lecture](https://sakai.unc.edu/access/content/group/bcb48c3b-5188-4ec4-ac95-ac2b53652fc7/Lecture%20Slides/Memory_Organization.pdf).
  * [Lecture 14](https://uncch.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=b284a627-8255-4eff-8a40-ace1011ed305).
  * [Lecture 15](https://uncch.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=115247f5-b5a5-4ad9-8411-ace6011ea950).
  * [Supplemental reading on caches on Sakai](https://sakai.unc.edu/access/content/group/bcb48c3b-5188-4ec4-ac95-ac2b53652fc7/Readings/Supp_Cache_Memory.pdf) (in particular, Section 13.4). 
  * If you need to review the relationship between pointers and arrays, see Chapter 5 of The C Programming Language.


#### VIM Tips: Moving around the page and moving the page around
The following keystrokes will help you navigate your code faster in vim. This description will make more sense if you open up a file and follow along to see what they do!

You can use the `H`, `M`, `L` keys to move your cursor to the *h*ighest, *m*iddlest, and *l*owest part of the page that is in you field of view without moving the page.

The `z` key is used for moving the page around from your current cursor position. `zz` will move the code so that the line your cursor is on is at the center of the screen. `zt` will move the line that your cursor is on to the *t*op of the screen and `zb` will move the line your cursor is at to the *b*ottom of the screen.

### Lab 05 Structure
In lab 04, we implemented a C library called bit_utils, which could be imported and run in the `main` body of other C programs. Similarly, in this lab all the implemented functions will be written in files that can be imported and called by an external program. We have provided a driver file called `lab05.c` which calls the functions you implement and serves as a top-level interface to the cache simulator.

There are nine starter files included with this lab, whose purposes are listed below: 

* `Makefile`- contains information used to compile your program with the `make` command. **Do not modify.**
* `memory.c` - contains functions that deal with physical memory (on-disk), such as memory allocation and dividing memory into blocks.
* `memory.h` - contains declarations for the functions implemented in `memory.c`. **Do not modify.**
* `cache.c` - contains functions that deal with cache and cache lines, including cache initialization and cache reading / writing.
* `cache.h` - contains declarations for the functions implemented in `cache.c`, as well as the definition for the `cache_line` struct. **Do not modify.**
* `bit_utils.c` - contains our implementation of the functions you implemented in Lab 04. You are welcome to replace this file with your `bit_utils.c` solution from Lab 04.
* `bit_utils.h` - contains declarations for the functions implemented in Lab 04. **Do not modify.**
* `lab05.c` - contains the main method which will run an interactive, text-based program that continuously reads memory addresses (provided by user input) and modifies the cache based on whether the read address results in a cache hit or miss. **Do not modify.**
* `memory.txt` - a sample memory file with 256 lines to be read in by the code in `memory.c`. The index of each line represents a distinct memory address, and the hexadecimal value on each line is the value of the byte stored at that address. **Do not modify.**

Open all of these files and familiarize yourself with them. Recall that you can open all the .c and .h files in the current working directory in vim tabs with the command `vim -p *.c *.h` and you can navigate the tabs with `gt` and `gT` (see vim tips section of the lab3 readme file). Read through each file carefully, as they contain function specifications and comments that will help you understand how everything works together.

In particular, pay attention to the following:
* The defined constants and how they correspond with the cache mapping algorithms and the size of blocks, address length, cache size, etc.
* The control flow of the main `lab05.c` function.
* The `cache_line` struct, defined as follows:
```
typedef struct {

	int tag;
	int hit_count;
	unsigned int block[4];

} cache_line;
```

While there are a lot of moving parts in this lab, you will only need to actually implement two functions!

#### Compiling and Running 
Similarly to Lab 03, you will be provided a Makefile with this lab that will compile your code automatically. So you only need to enter the command `make` to compile your code.

#### Honor Code Header
Please include the honor code header at the top of the `memory.c` and `cache.c` files.  Since we do grade manually for style we do not include names on code listings to avoid biasing the grading.
```
// PID: 9DigitPidNoSpacesOrDashes
// I pledge the COMP211 honor code.
```

## Part 0. Starter Code and numberOfBlocks
Once you are familiar with the organization of the code, implement the following function prototype from `memory.h` in the `memory.c` file:

```
int numberOfBlocks( unsigned int addr_bits, unsigned int num_block_offset_bits );
```

The specification for this function is simple. Given the number of bits it takes to represent the number of addresses used in the simulation and the number of offset bits used in the cache, how many blocks can your address space be divided into?

`numberOfBlocks` should return the `FAIL` constant (defined in memory.h) if either of the arguments are not positive or if addr_bits is smaller than num_block_offset_bits.

**Note**: For this particular simulation, the number of addresses is 256 and the number of offset bits is 2 (resulting in a block size of 2^2 = 4).
This means that our address space can be divided into 64 blocks of 4 addresses each, but your function should generalize to any size of address space and number of offset bits.

### Testing

To test your `numberOfBlocks` functions, write some sample function calls with print statements at the beginning of the `main` function in `lab05.c`. Once you are convinced you are calculating the correct number of blocks, delete your debug code and submit to GradeScope to confirm that your function is correct. Note that you'll get some warnings from gcc because not all the functions have been implemented. You can ignore these for now.

## Part 1. Direct Mapping

You will implement the `cread` function in two phases, each of which corresponds with a cache mapping design scheme. The function prototype is defined as:

```
int cread( unsigned int cmf, unsigned int* hex_addr, unsigned int* found, unsigned int* replace);
```

The `cread` function will apply a particular mapping function to the cache depending on the value of the `cmf` parameter. Note that in `cache.h`, the following constants are defined: 

```
#define DM 1
#define FA 2	
```

If the `cmf` parameter supplied is DM (represented internally as 1), then `cread` should apply the direct mapping algorithm, and if the parameter is FA (represented internally as 2), then it will apply the fully associative algorithm.

For Part 1, you only need to implement the direct mapping algorithm, the details of which can be found in the background reading and class lectures. A paraphrased version of the algorithm is supplied here, but some details (such as dealing with the `found` and `replace` parameters and modifying some properties of `cache_line` structs) have been elided and are left for you to figure out.

* Use the appropriate `bit_utils` functions to compute the block_offset, line, and tag bits (derived from `NUM_BLOCK_OFFSET_BITS`, `NUM_OF_LINE_BITS` and `NUM_OF_TAG_BITS_DM`) based on the supplied hexadecimal address.
* See if the cache line corresponding to the line bits has tag bits that match with the tag you are searching for.
	 * If the tag bits match, then the address you are searching for has already been cached, so you do not need to replace anything.
	 * If the cache line is empty, cache the memory block in this line and initialize the line's tag.
	 * If the cache line has different tag bits, then you must replace the line's memory block and tag with those of the block you are searching for.
 * Now that the appropriate memory block has been cached, do the following:
 	 * Increment the hit count of the appropriate cache line.
 	 * Set `found` and `replace` to appropriate values based on whether there was a cache hit and whether an existing value in the cache line was replaced.
 	 * Set retVal to be the value in memory corresponding to the requested `hex_addr`.
 * Return retVal.


#### Testing Direct Mapping

To test your code, you will run the executable generated by the `lab05.c` file, which takes in `memory.txt` as a command line argument in order to initialize the simulator's physical memory with the contents of `memory.txt`. The command to run your program in this manner after compiling is `./lab05 memory.txt`. Or you can simply enter the command `make run`, which maps to that command in the make file.

Your testing of the algorithm will consist of entering hexadecimal addresses into the simulator and seeing if the output of the cache acts as you would expect it. In particular, look out for the following:

 * Does the tag referenced in the cache correspond to the appropriate hexadecimal address from `memory.txt`?
 * Is the return value the same as what is present in the corresponding hexadecimal address in `memory.txt`?
 * If a block of addresses is already stored in the cache, does entering an address from that block result in a cache hit (and vice versa)?
 * Does a particular hexadecimal address always map to the same cache line?

Like previous labs, you will be given sample input and output files. However, you will not be able to simply output your result and compare with `diff`. This is partly because the main loop of `lab05.c` loops infinitely without keyboard intervention, and because we want you to carefully sample the examples to understand the cache behavior. Thus, the input files include a sequence of values that you should enter manually and the output files contain everything that should be output to the console.

As an example, let us take a look at the `directmapping_input0.txt` file:
```
1  
00
01
02
03
09
0A
0B
10
20
20
40
```

The 1 indicates that we have chosen 1 (direct mapping) as our cache mapping function. Every subsequent number is a hexadecimal address in physical memory that the cache attempts to read. The expected results would be found in `directmapping_output1.txt`. The following snippet from this file demonstrates how the flow between inputs and outputs interact:

```
------------------------
[STEP 1] Setting up physical memory
------------------------
Physical memory addressable bits = 8, total number of blocks = 64

------------------------
[STEP 2] Determining physical memory block locations
------------------------

------------------------
[STEP 3] Select cache mapping function (CMF)
------------------------
1 = Direct mapping
2 = Fully associative
------------------------
Please enter 1 or 2: 1 				           // Input on this line.
------------------------
[STEP 4] initializing cache
------------------------

------------------------
[STEP 5] Starting simulation
------------------------
CMF is Direct Mapping
To exit simulation press the 'Ctrl C' keys on your keyboard

Please enter 8-bit hexidecimal address: 00                 // Input on this line.

Entered Hexidecimal (Base-16) address 00 (Base-10 value 0)

---------------------------------------------
line	tag	num of hits
---------------------------------------------
0	00	1
1	-1	0
2	-1	0
3	-1	0
4	-1	0
5	-1	0
6	-1	0
7	-1	0
[MISS:No Replacement] The byte value at memory address 00 is 7F
```
The way to interpret this sequence is that the user entered `1` to choose the direct mapping cache algorithm. Then, they entered the hexadecimal address `00` as the address whose value should be read from the cache. Since the cache was empty, this was a cache miss and the block of physical memory corresponding to address `00` was loaded into cache line 0. If you examine `memory.txt`, you will see that the value stored in the first line is indeed `7F`!

## Part 2. Fully Associative Mapping

You will now implement the second component of the `cread` function, which corresponds with the fully associative mapping algorithm. If the user supplies a `cmf` parameter of FA (represented internally as 2) to the `cread` function, then you should apply the fully associative algorithm. As was the case in Part 1, you can find more details in the lecture and supplied reading, but here is a paraphrased version of the algorithm:

* Use the appropriate `bit_utils` functions to compute the block_offset and tag bits (derived from `NUM_BLOCK_OFFSET_BITS` and `NUM_OF_TAG_BITS_FA`) based on the supplied hexadecimal address.
* Go through each of the cache lines and determine which, if any, of the cache lines needs to be replaced.
	* If the tag of any cache line matches the tag you are searching for, then the address you are searching for has already been cached and you do not need to replace anything.
	* If there are cache lines that have not been used yet, cache the value in the first available open line (i.e. the one of lowest index in your array of cache lines).
	* If every cache line has been used, use the Least Frequently Used replacement strategy. That is, use the cache line with the fewest number of hits. In the event of a tie, use the cache line with a lower index in the array of cache lines.
* Now that you know which cache line to use, do the following:
	* Set the new tag of the cache line to the desired value, and reset the number of hit counts to 1.
	* Set the blocks in the cache line to be the values of the corresponding block in physical memory.
	* Set `found` and `replace` to appropriate values based on whether there was a cache hit and whether an existing value in the cache line was replaced.
	* Set retVal to be the value in memory corresponding to the requested `hex_addr`. 
* Return retVal.

#### Testing Fully Associative Mapping

Testing for Part 2 works the same way as testing in Part 1; you just need to use the appropriate sample files. Since the fully associative algorithm differs from direct mapping, it will not be the case that a particular hexadecimal address always maps to the same cache line.

## Submit your assignment
1. Use git to push your finished code to this Github repository.
2. Go to the COMP 211 course in GradeScope and click on the assignment called **Lab 05**.
3. Click on the option to **Submit Assignment** and choose GitHub as the submission method.
4. You should see a list of your public repositories. Select the one named **lab-05-yourname** and submit it.
5. Your assignment should be autograded within a few seconds and you will receive feedback for the autograded portion.
6. If you receive all the points, then you have completed this lab! Otherwise, you are free to keep pushing commits to your GitHub repository and submit for regrading up until the deadline of the lab.

## Grading
#### Autograder tests (8 pts)
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
	* For all functions (including main if applicable), every path of execution leads to a return statement.
	* No libraries are included that weren't provided in the starter code or mentioned in the readme.

We reserve the right to deduct points from the autograder if we notice that you have hard-coded any test cases and not actually fully implemented the functions.
