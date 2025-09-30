This is a mini shell implemented in C with only system calls. The purpose of the project was to start learning something about processes and low level C.

## USAGE

1. From the initial directory type on the command line `make` to run the Makefile, this will create a `/build` directory with all the object files and an executable `run_shell`
2. Now on the command line type `./run_shell` to run the shell
3. To clear both the `/build` and `run_shell` you can simply run `make clean` 

---

## DEMO

![Demo](demo/demo.gif)

---

## FEATURES 

The shell supports: 
- Simple file executions, both from `PATH` and from a relative path
- Redirection of stdin, stdout and stderr with the classics `< > >> 2> 2>>`
- Piping (single pipe without concatenation)
- Tracking a history of commands scrollable with arrow keys, to use this feature: enter `his` in the cmd line, now you can scroll over the last 100 commands used and then enter to launch the selected command.
- `cd` and `exit`

---

## CREATION PROCESS

I searched the internet to understand how a shell should work, then i used the linux manual and the internet to learn the most common system calls and c library function. At that point i started writing the code feature by feature. At the end of the project i learned a little bit about how a real shell is implemented and found out the differences with respect to the code i wrote. Overall the learning process was good.

I also ended up learning more about <termios.h> and how to handle the mode of reading the input and displaying it on the cmd line. In this context i encountered a big difficulty: if you want to detect arrow keys for scrolling over the history you must both disable buffering and echoing, but you can't do it in a traditional shell unless you decide to handle echoing by yourself. I found this compromise of using a command (`his`) to pass from traditional mode to raw mode.

Another interesting thing I learned about is writing safer code, since I thought it would have been a mess to check each system call's return value i wrote a function (`check_syscall`) that handles every problem by typing on the screen a red prompt with the error. 

## IMPLEMENTATION DETAILS

The shell is entirely built with system calls, except for:
- Strings functions which i thought would have been a waste of time and space to write by myself
- <termios.h>'s functions (used in the `built_in` module for the command `his`) which would have been hard to implement from scratch. 
I wrote some library functions by myself whenever i thought it was appropriate and useful for my implementation, those functions start with a "my".