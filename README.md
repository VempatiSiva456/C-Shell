# OSN ASSIGNMENT 3 (Extension of 2nd Assignment)
### NAME &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Vempati Siva Koti Reddy 
### ROLLNO &nbsp; 2021101135
---

## ***REQUIREMENTS***:
- GCC compiler
- LINUX OS (Ubuntu) 

---
* To execute the program:
``` bash
$ make
$ ./a.out
```
## Specification 1 :
- Doing input output redirections using the symbols <, > and >>;
- Creating the output file (with permissions 0644) if it does not exist.
- Didn't handle multiple inputs and outputs as those are not required.
- Used execvp.
- Handling errors if file doesn't exist.

## Specification 2 :
- Implemented piping using dup2
- Handled Errors
- One or more commands can be piped

## Specification 3 :
- Implemented IO Redirection with command pipelines
- Getting output for io redirection within command lines

- First 3 specifications implemented in ioredirection.c

## Specification 4 :
- (i) jobs : 
- Printing jobs running & stopped with specified flags
- Didn't sort jobs in Alphabetical Order
- Printing with status and pid
- Doing further fg, bg and sig operations with assigned job numbers as mentioned

- (ii) sig :
- Passing job number of a running job and a corresponding signal number to do sig operation with kill.
- Handling all errors

- (iii) fg :
- Converting a job which is running in background to foreground
- Changing background running or stopped processes to foreground running processes
- Foreground processes obtained with fg conversion : Stored in seperate
- Handled all type of errors

- (iv) bg :
- Changing a stopped background process to running background process
- All errors handled like giving error if job with given number is not available, and intimating if the chosen process is already running in background.

- Implemented these parts in jobs.c

## Specification 5 :
- As mentioned in pdf, Handling signals with Ctrl-Z, Ctrl-C and Ctrl-D
- Ctrl - Z pushes currently running foreground job into background.
- Changing its state from running to stopped
- Ctrl - C to interrupt currently running foreground process
- No effects if no foreground running process
- Ctrl - C, Ctrl - Z makes Ctrl -D work to log out from shell

- Implemented this part in signals.c

## Specification 6 :
- Auto completing using stat, searching files starting with same characters using strncmp
- Used main.c provided in mail
- Handled all type of errors

- Implemented in scan.c


## Thank You :)
