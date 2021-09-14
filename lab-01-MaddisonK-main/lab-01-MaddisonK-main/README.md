# Lab 01
You should be able to do these exercises with knowledge only from chapter 1 of the textbook *C Programming Language*.
If you're having trouble, revisit some of the relevant chapter sections, and study the code examples. At the end of this file there are some tips for
testing your code.

## Part 0: Getting Started

#### Command-line workflow
When you want to run a program you have written, you have to save the program, exit vim and compile the code to a machine language 
that your computer understands. We are using the `gcc` compiler to do this translation.
1. Start the learncli docker image and download this repository by entering `git clone < URL-to-repository >`
2. Enter the directory with `cd lab-01-<username>`
3. Start writing a C file in vim by entering `vim <name-of-program>.c`
4. When ever you want to run your program, you must first return to the terminal and compile it with the following steps:
  - `gcc <name-of-program>.c` will compile the program and store the binary in a file called `a.out`. If there were any compilation
  errors (like bad syntax), gcc will warn you and the code will not compile. Whenever you compile your code again, the previous `a.out`
  will be overwritten with the new program.
  - `./a.out` runs the program. In linux, typing `./` before the filename will typically run a binary file.
  - `echo <some-text-input> | ./a.out` this will pipe `<some-text-input>` into the standard input of the program. It's as if you
  typed `<some-text-input>` to the program yourself while it was running.
  - `./a.out <arg1> <arg2>` will run the program and supply some command line arguments. We will not be handling 
  arguments in Lab 01.


## Part 1: Going All Caps
Go to the directory named `1-going-all-caps`.
Your caps-lock key broke last week and you've gotten tired of holding down the shift key during your heated online
debates. Write a new program called `shout.c`.This program should convert all characters from the standard input to uppercase
and add three "!" to the end. For example:
```
learncli$ echo "tar heels" | ./a.out
TAR HEELS!!!
```
None of the test input will contain newlines.

*Hint: There are better and easier ways to do this than writing 26 `if` statements. Think about how to use ascii
codes to your advantage and remember that the `char` variable type is the same as an unsigned `short`.*

## Part 2: Compatibility
Go to the directory named `2-compatibility`. 
Your friend's monitor is too small and can only display up to 40 characters on a line before they run off 
the screen. They aren't good with computers, so you decide to write a program for them called `truncator.c` that
will reprint input so that no line is longer than 40 characters. 
Specifically, for lines less than 40 characters, simply reprint the line. For lines greater than 40 characters,
insert a newline at each 40-character interval.

**Note**: If the 41st character of a line in the input is a newline `'\n'`, then you should not print it
(to avoid unnatural double newlines in the text). Depending on your solution, you may not need extra code for this.
So worry about this case last.

***Tip**: Write your own file with test input for the case where the 41st character is a newline. In the bottom-right corner of VIM you
can see the line and column number of your cursor and plan your 41-character input accordingly.*

To give developers feedback on their programs for 40-character-displays, you decide to add additional functionality
to your program by displaying the line numbers of all lines over 40 characters long and the total number of these offending lines. 
For example:
```
learncli$ cat test0.txt 
This line is under 40 characters long
This line is over 40 characters long, see?
This line is soooooooooooooooooo looooouoooooooooooooooooooooooooooooooooooooooooooong!

learncli$ cat test0.txt | ./a.out
This line is under 40 characters long
This line is over 40 characters long, se
e?
This line is soooooooooooooooooo looooou
oooooooooooooooooooooooooooooooooooooooo
oooong!

Total lines over 40 chars: 2
Offending lines: 1, 2, 
learncli$
```
*Some additional things to note:*
 - The first line is the 0th line. 
 - You can assume that none of the test input will contain over 100 illegal lines, so use an 
array of length 100 to store the line numbers in.
 - There should be a newline `\n` after the reformatted output and a newline after the line that says "Offending lines..."*
 - tabs and spaces are both treated as a single character in C, even though a tab takes the space of 
multiple characters. None of the test input will contain tabs and you do not have to worry about this use case.

## Tips for testing
- Test inputs and results are included for you to check for correctness. You can also write your own tests.
- You can test for the correctness of your programs by running:
    ``` 
  learncli$ gcc <progam-name.c>
  learncli$ cat test0.txt | ./a.out > myresult
  learncli$ diff myresult test0-result.txt
  ```
  The first command compiles the c program to `a.out`. The second pipes the text in `test0.txt` to the standard input of your program and redirects the output of your program to a new file called `myresult` (instead of just printing it to the screen). And the third command runs the `diff` program on `myresult` and `test0-result.txt`, which will show any differences between the output from your program and
  the correct output. If nothing prints after running the diff command, then your output matches!
  
  You can also run all three commands on the same line by separating them with a semicolon `;`:
    ``` 
  learncli$ gcc <progam-name.c>; cat test0.txt | ./a.out > myresult; diff myresult test0-result.txt
  ```
  
## Submission
Submit the assignment to Gradescope the same way you did for Lab 00. Don't forget to add your PID and honor code to each .c file as below!
```
// PID: 9DigitPidNoSpacesOrDashes
// I pledge the COMP211 honor code.
```
1. Navigate to the base folder of the repository within Docker.
2. Type `git status`. You should see a list of changes that have been made to the repository.
3. Type `git add *`. This signals that you want to place all modified / new files on the "stage" so that their changes can take effect.
4. Type `git commit -m "Your Message Here"`. This shows that you are "committing" to the changes you put on the stage. Instead of Your Message Here, you should write a meaningful message about what changes you have made.
5. Type `git push origin main`. This takes the commit that was made locally on your machine and "pushes" it to GitHub. Now, when you view this repository on GitHub you should be able to see the changes you've made, namely the addition of your source files!
6. Go to the COMP 211 course in GradeScope and click on the assignment called **Lab 01**.
7. Click on the option to **Submit Assignment** and choose GitHub as the submission method.
8. You should see a list of your public repositories. Select the one named **c211s21/lab01-yourname** and submit it.
9. Your assignment should be autograded within a few seconds and you will receive feedback for the autograded portion.
10. If you receive all the autograder points, then you have completed this lab! Otherwise, you are free to keep pushing commits to your GitHub repository and submit for regrading up until the deadline of the lab.

**Note**: You may have noticed that after `git add *` your binary `a.out` files were not added to staging. This is because we specified in the gitignore to not include executable files. It is considered bad practice to have compiled binaries in your published git repository.
  - if an a.out or other file gets added accidentally to staging, `git reset <file-name>` will remove the file from staging without
  actually deleting the file.
  - Check which files are on the stage be commited at any time by entering `git status`.
 
 You shouldn't need to worry about this, the important thing for grading is that all your C source files are in the correct directories.

## Grading
- Autograding tests (8 pts): feedback is provided as soon as Gradescope grading completes. You are encouraged 
to continue improving your code and resubmiting until achieving full credit.

- Style (2 pts):  Code is often *read* more than it is *ran*. You do not have to adhere to an official coding style, but we expect readable code. TA's will grade this manually after the due date.
  - 1pt: reasonable function/variable names, using functions rather than having repetitive code in `main`, and returning the `EXIT_SUCCESS` constant or `0` at the end of `main`. 
  - 1pt: Your solution to part 1 does not use 26 `if` statements.
