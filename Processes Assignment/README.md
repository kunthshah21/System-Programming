CODE CONTRIBUTORS: 
# Kunth Shah 
# Varsha Gunturu 

# Detailed Technical Justification

## Command Line Parsing using getopt:
- The getopt function from the `unistd.h` library is used to parse command line options. This is a standard and efficient way to handle command line arguments in C programs. getopt allowed me to specify the expected options and their corresponding arguments, making the code concise and easy to understand.

## Error Handling:
- Error handling is implemented to handle various scenarios like missing required options (`-e`), unknown options, and errors during forking.
- Error messages are printd to `stderr` to distinguish them from normal program output in the console. 

## Forking to Create Child Process:
- The `fork()` system call is used to create a new process (child process). 
- The child process inherits a copy of the parent process's memory space, including the code segment, data segment, and stack. This allows the child process to execute a different program using `exec()` without affecting the parent process.

## Building the Command to Execute:
- The command to execute is constructed dynamically using `snprintf()`. This allows us to concatenate the executable path and options (if provided) into a single string, which can then be passed to `system()` for execution.
- Using `snprintf()` prevents buffer overflow issues by specifying the maximum size of the command buffer. This was an issue before in the previous codes which were submitted. The reason being that, characters overflow from one input to the other. 

## Executing the Command using system():
- The `system()` function is used to execute the command in the child process. This function invokes a command shell and runs the specified command in it.
- This approach is simple and straightforward, allowing us to execute arbitrary commands provided by the user.

## Handling Background vs. Foreground Execution:
- The `-b` and `-f` options are used to specify whether the command should be run in the background or foreground, respectively.
- If the command is to be run in the background, the parent process does not wait for its completion and immediately prints the child process ID.
- If the command is to be run in the foreground, the parent process waits for the child process to finish and then prints the command line, child process ID, exit code, and any error codes.

## Printing Output:
- Output is printed to `stdout` using `printf()`. This includes the command line, child process ID, exit code (if applicable), and any error codes.

