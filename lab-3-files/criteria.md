# Lab 3 Criteria

## Purpose

The purpose of this lab is to become familiar with Linux and C/C++ standard I/O. You will implement a program that reads a given file using the Linux `read()` system call as well as the C/C++ `fgetc()` and `fread()` functions.

## Required System Calls and Functions

Use the following Unix and C/C++ standard I/O functions. Check each function with `man`.

- `open()`: Unix system call that opens a file.
- `read()`: Unix system call that reads data into a buffer.
- `close()`: Unix system call that closes a file descriptor.
- `fopen()`: C/C++ standard I/O function that opens a file.
- `fgetc()`: C/C++ standard I/O function that reads one byte from a file.
- `fread()`: C/C++ standard I/O function that reads data into a buffer.
- `fclose()`: C/C++ standard I/O function that closes a file.

## Statement of Work

The provided template measures the time required to read a file using Linux `read()` and C/C++ standard I/O. The program accepts:

1. A file name.
2. The number of bytes to read per `read()` or `fread()` call.

Requirements:

1. Complete `main()` so the program runs and compares the two file-reading mechanisms.
2. The command-line byte-count argument controls the chunk size for each call.
3. The program must always read the entire file, regardless of chunk size.
4. When the second argument is `1`, you must use `fgetc()` instead of `fread()`.

## What to Turn In

- One submission for both students.
- Both student names included on the assignment.
- The completed `lab3.cpp`.
- Execution output.

You can test against `hamlet.txt` from Canvas.

## Example Execution Output

The following sample shows execution output when reading with chunk sizes of `512`, `1024`, `2048`, and `4096` bytes. Times will vary by machine, but the measurements should be proportional.

```text
[css503@uw1-320-18 lab3]$ ./lab3 hamlet.txt 512
Unix read's elapsed time        = 969
Standard fread's elapsed time   = 230

[css503@uw1-320-18 lab3]$ ./lab3 hamlet.txt 1024
Unix read's elapsed time        = 528
Standard fread's elapsed time   = 219

[css503@uw1-320-18 lab3]$ ./lab3 hamlet.txt 2048
Unix read's elapsed time        = 358
Standard fread's elapsed time   = 219

[css503@uw1-320-18 lab3]$ ./lab3 hamlet.txt 4096
Unix read's elapsed time        = 245
Standard fread's elapsed time   = 222
```

## Rubric

| Criteria | Points |
| --- | ---: |
| Follows Coding Guidelines | 5 |
| Compiles | 3 |
| Runs | 2 |
| Correct Output | 5 |

## Current Project Evaluation

This evaluation is based on the files in this directory, especially `lab3.cpp`, `lab3-output.txt`, and `run.sh`, plus live compile and run checks performed in this workspace.

### Requirement Check

| Requirement | Status | Evidence |
| --- | --- | --- |
| `lab3.cpp` is included | Met | `lab3.cpp` is present in the project root. |
| Execution output is included | Met | `lab3-output.txt` is present in the project root. |
| Program compiles | Met | `g++ -Wall -Wpedantic -Werror -Wextra lab3.cpp -o /tmp/lab3_check` succeeded. |
| Program runs | Met | `./run.sh` completed successfully, and `./bin/lab3 hamlet.txt 1` also ran successfully. |
| Uses `open()`, `read()`, and `close()` | Met | `lab3.cpp:39-47` uses the Unix I/O path. |
| Uses `fopen()` and `fclose()` | Met | `lab3.cpp:52-68` opens and closes the file with standard I/O. |
| Uses `fread()` when byte count is greater than `1` | Met | `lab3.cpp:62-66` reads in a loop with `fread()`. |
| Uses `fgetc()` when byte count is `1` | Partially met | `lab3.cpp:58-61` does call `fgetc()`, but only once. |
| Reads the entire file regardless of chunk size | Not met | In the `bytes == 1` case, the standard I/O path reads a single byte instead of looping to EOF. |
| Correctly compares both reading mechanisms | Not met | The standard I/O timing for `bytes == 1` is not comparable because it does not process the full file. |
| Output is clean and close to the sample | Partially met | Timing output exists, but labels differ from the sample and `lab3-output.txt` includes terminal control characters from `script`. |
| Both student names are included | Unable to determine | No student names are visible in the project files reviewed here. |
| Follows coding guidelines | Partially met / unclear | The program compiles cleanly, but the rubric does not define the guidelines here. There are minor code quality issues such as duplicate includes, `using namespace std;`, and heap allocations that are not freed before exit. |

### Overall Verdict

The project is close, but it does **not fully meet** the assignment requirements yet. The key missing requirement is that the standard I/O path must read the **entire file** even when the byte count is `1`, using `fgetc()` in a loop until EOF.
