# flame_cp - File Copy Utility

## Description

<!-- Contributors: Mukund and Kunth
Last Edited: 1:35 PM 29th January 2023 -->

// my_copy.c program uses system calls such as open read write and close to copy contents;
// from the standard input to the standard output. And return either 0 for succesfull copy or -1 for an error. 
// The error is stored in ec which is returned by the C program and the error can be found by using the bash script. 

// Firstly Our program uses the copy system call and ensures it gets 5 arguments including -i and -o ; it the argument requirements are not met then it returns Wrong_argument. we use the strcmp command which compares two string and is a standard C function. 

// 2. Then we temporarily store the copied data as we need to do that between read and write
// system calls using a buffer of 1024 bytes. we check if both the source and destination files can be accessed by us using the access function. The access function checks if we have the permissions to obtain its data. 

// 3. We then use the open system call to open the source file in read only. And then use the open system call to open the destination file in write only, also  with flags like create -> which creates a new destination file if it doesn't exist but we let the system know if this is happening.

//4. We run a loop and copy the contents using read in source with buffer as the argument and write in destination with buffer as the argument. And then close both the files using the system call close. 

// 5. We try to cover all the errors that might have occured in the process as not able to read the source file or length of source file bytes is not same as destination file and return the desired error. 

// flame_cp is the compiled binary version of my_copy.c program. And save the errors in errno. 

`flame_cp` is the command-line based C program that copies the contents of one file to another.  
The usage syntax is as follows:

```bash 
// Open the directory of the assignment in the terminal and type make.
// After successful completion type:

flame_cp -i <source file pathname> -o <destination file pathname>

// In Our case we used :
// ./flame_cp -i input_message.txt -o output_message.txt

// To run the bash script you first need to grant permnission (I had to do it. )
// chmod +x handle_errors.sh
// Also in mac there is a application called gatekeeper which prevents harmfull scripts from running so you might have to ovveride it. (I had to do it.); this is the command I used:
// xattr -d com.apple.quarantine handle_errors.sh

// Run the script with the desired error code ; for example
// ./handle_errors.sh 2 
// in case the C program returns 2. To see the message of the error. 

// If you want to do it simultaneosly (return error) then run both commands together like this:
// ./flame_cp -i input_message.txt -o output_message.txt ./handle_errors.sh $?



// Apart from the obvious 7 errors we presented in this code of:

# 1. Presenting wrong arguments.
# 2. Source file not found.
# 3. Not able to open file.
# 4. Unnable to read file.
# 5. Unnable to write to file. 
# 6. Unnable to close file. 
# 7. No destination found. 

We faced another plethora of errors like:

1. As mentioned above no permission given to run the bash script
2. The Makefile was not working. We had to individually type the commands each time. Finally we fixed it. 
3. We got more permission related errors such as operation not permitted. EPERM
4. When we accidentally put a file instead of a directory we go ENOTDIR which is Not a directory. 
and many more.


