Contributors: Kunth Shah, Varsha Gunturu
Last Updated: 5th March 2024
Last Tested Device: Macbook pro 16 M1 Pro 
Last Tested OS: MACOS SONAMA 14.4

------------------------------------------------------------------------  
------------------------ HOW TO RUN ------------------------------------
------------------------------------------------------------------------

gcc -Wall -o my_bfm my_bfm.c

./my_bfm -c demo
chmod +rx demo //needs to be given to get access to directory

./my_bfm -c t.txt -s “Hello World” //appends text for all text files with the same name

./my_bfm -c binary.bin -e 53 //appends even numbers for all binary files with the same file name

./my_bfm -r t.txt text.txt  //renames all t.txt files to text.txt

./my_bfm -d text.txt  //deletes all files with the same text file name


Alternatively you may use the make file: 

make

./my_bfm -c demo
chmod +rx demo
./my_bfm -c t.txt -s "Hello World"
./my_bfm -c binary.bin -e 53
./my_bfm -r t.txt text.txt
./my_bfm -d text.txt

make clean       #This will clean the compiled files

------------------------------------------------------------------------  
------------------------ Code Explaination -----------------------------
------------------------------------------------------------------------

General Breakdown: 
This code uses system calls to perform multiple file operations. This builds on the previous assignment of single file operations and extends those operations to all files and directories within a specified directory. 
This is implemented with the help of recursive_file_search function, which recursively searches folders and files within the directory to identify the correct file names and types to perform the operations. 

This code explaination will mainly focus on that function as that is the core unique part of this code, seperating it from the previous single file operation code. 

---------------- Search File Recursively Explaination -------------------

It is designed to perform file operations within a specified directory and its subdirectories. The function takes four parameters: dir_name represents the base directory, operation specifies the action to be performed (either deletion with -d or renaming with -r), filename denotes the target file or directory, and new_name is utilized when renaming files or directories. The code begins by attempting to open the specified directory using the opendir system call. In case of failure, it prints an error message using perror and returns from the function.

Within the function, a loop iterates through each entry in the opened directory, skipping entries for the current directory (.) and parent directory (..). For each entry, a file path is constructed by appending the entry name to the base directory using snprintf. The code then checks if the current entry matches the specified filename. If there's a match, it further evaluates the specified operation. In the case of deletion (-d), the function calls another function delete_file_or_directory to remove the file or directory. For renaming (-r), a new path (new_path) is constructed by appending the new name to the base directory, and the rename system call is used to perform the renaming operation. Success or failure messages are printed accordingly.

The code also handles the scenario where the specified operation is neither -d nor -r, printing an "Invalid operation" message. Additionally, if the current entry is identified as a directory, the function recursively calls itself with the new directory path, allowing it to traverse through subdirectories and perform the specified file operations. Finally, after processing all entries in the directory, the function closes the directory using the closedir system call. In essence, this recursive file operation code utilizes various system calls, such as opendir, readdir, stat, rename, and closedir, to navigate through directories, identify and operate on specific files, and manage file-related tasks within a given directory structure.

---------------- Log file Challange -------------------

This challange dealt with generating a log file for all operations performed and this was tackled by implementing an fprintf funciton that prints appropriate updates throughout the code, to the log file which is generated. These funcitons will keep appending to the log file. 

This is a sample output of renaming files in a particular directory: 
[2024-03-05 11:58:43] RECURSIVE_SEARCH: ./demo
[2024-03-05 11:58:43] RECURSIVE_SEARCH: ./test.txt
[2024-03-05 11:58:43] RECURSIVE_SEARCH: ./.vscode
[2024-03-05 11:58:43] RECURSIVE_SEARCH: .
[2024-03-05 11:58:43] RENAME: test3.txt (test.txt)


------------- Edge Case Scenarios -----------------------

Delete Function:
When it has deleted all files, the code throws a message to the system output that there was an error deleting, this happens because there are no more files left to delete and is an indication that all files have been deleted. 

Rename Funciton: 
The same case as deletion, happens for the rename function as well. 
