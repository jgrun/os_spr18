# Programming Assignment 1
### Jacob Grunwald

## Contact Information
Email: jacob.grunwald@colorado.edu

## File Breakdown
'compile_os.sh': Short script that runs all of the commands in the sequence to compile the OS. Should be placed in the linux-hwe-4.13.0 folder.

'csci3753_add.c': Adds two numbers together using system calls. Should be placed in the /arch/x86/kernel/ folder.

'Makefile': Replaces the standard Makefile for the kernel. Should be placed int eh /arch/x86/kernel/ folder.

'README.md': This file

'syscall_64.tbl': The look-up table for the system calls. Should be placed in the /arch/x86/entry/syscalls/ to replace the existing syscall_64.tbl file.

'syscalls.h': Holds the function prototypes for the system calls. Should be placed in the /include/linux/ folder replacing the existing syscalls.h file.

'sys_hello_test.c': Runs the test program for the helloworld syscall. Compile with 'gcc -o sys_hello_test sys_hello_test.c'.

'syslog': Contains the system logs from my machine after running the problem.

'sys_simple_add_test.c': Runs the test program for the csci3753_add syscall. Compile with 'gcc -o sys_simple_add_test sys_simple_add_test.c'.

## Build Instructions
After placing all of the appropriate files in the correct locations, run the compile_os.sh script. Give the configuration a unique name, then let the operating system compile. Once the script exits and returns control to the user, reboot the VM.

## Running the Test Program
After compiling 'sys_hello_test.c' and 'sys_simple_add_test.c' run them by typing 'sys_hello_test' and 'sys_simple_add_test'. Check the system log for correctness once finished by typing 'dmesg | tail'.
