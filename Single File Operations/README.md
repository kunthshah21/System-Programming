HOW TO RUN: 

Open the folder in a new terminal tab (or switch to the directorhy of the Program)
Type: make
Run the executable ./my_fm 



***********************************************************************************************************************************
--------------------------------------------------- Exectuion ---------------------------------------------------------------------
***********************************************************************************************************************************

./my_fm [function flags and input]

------ DEMO CASES SCENARIOS -------
[replace file names and paths]

write/append 50 BYTES
    ./my_fm -w /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/nest.txt "New new new new new" // write in text
    ./my_fm -w file.bin 75                                                                      // write start with num in binary

Display first 50 BYTES
    ./my_fm -f /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newnewtest.txt

Rename File or Directory
     ./my_fm -r /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newnewtest.txt /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newworld.txt

Delete File or Directory
    ./my_fm -d /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newworld.txt   // deletes a file
    ./my_fm -d /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newFolder      // deletes a folder

Create File or Directory
    ./my_fm -c /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newnewtest.txt 0  // creates a text file
    ./my_fm -c /Users/kunthshah/Desktop/Assignment2_Kunth_Mukund/newfolder 1      // creates a folder


***********************************************************************************************************************************
----------------------------------------------- Code Explaiantion -----------------------------------------------------------------
***********************************************************************************************************************************

Information about the Functions and the overall code.
The code has 5 funcitons:

1. Create file or directory
2. Delete file or directory
3. Rename file or directory
4. Display first 50 bytes
5. Write 50 bytes at EOF 

--------------------
-----Explaination---
--------------------

The code parses command-line arguments to determine the desired operation and its corresponding parameters. The operations are implemented as separate functions (create_file_or_directory, delete_file_or_directory, rename_file_or_directory, display_first_50_bytes, and write_50byte_function).

-----------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------create_file_or_directory--------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------

The function takes two parameters: path, representing the target location, and is_dir, a flag indicating the type of creation (directory if true, file if false). 

The code utilizes system calls, such as 'mkdir' for directory creation and 'open' for file creation, with appropriate flags and permissions. In case of an error during the creation process, the program prints an error message along with the associated error number using the errno variable and exits with a failure status. 

The open system call to create a file with the given path.
O_CREAT: Creates the file if it does not exist.
O_EXCL: Ensures that the call fails if the file already exists.
S_IRUSR | S_IWUSR: Sets read and write permissions for the owner of the file.

read (4), write (2), and execute (1) permissions -> permision code 0777 is used for the mkdir file permision. 
Laslty there is also a check for extension, so there should not be '.' in a directory name


-----------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------rename_file_or_directory--------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------

The logic behind this function is that the function is called to change the file path. 
The rename_file_or_directory function takes two arguments: old_path and new_path, representing the paths of the file or directory to be renamed and the new name or path, respectively. The function first checks the type of the file or directory specified by old_path using the stat function and the S_ISDIR and S_ISREG. 

Based on the file type, it then uses the rename function to perform the renaming operation. If the operation is successful, it prints a success message; otherwise, it updates errno message and exits the function.

-----------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------display_first_50_bytes----------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------

The display_first_50_bytes function takes a file path as an argument, attempts to open the file in read-only mode using open, reads the first 50 bytes of the file into a buffer using read, and then prints those bytes along with the file path. It also handles potential errors during file opening and reading. 

After reading, it ensures that the buffer is null-terminated and then closes the file. This was very important step and was a major bug solve. This is explained in the bug solve section of the readme. 

-----------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------write_50byte_function----------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------

The write_50byte_function function opens a file specified by file_path in read-write mode, determines whether it's a regular text file or a binary file, and writes either user-inputted text or generated binary data to the beginning of the file. The function first checks if the file is a regular file using the stat function. 

If it is, the function tries to detect if it's a text or binary file by inspecting the content and file extension. 
Understanding whether the file is binary or Text:

It does that by two main methods -> Inspecting the content and checking the file extension. 

----------------Content Inspection-----------------
The part of the function's process is to open the file and reads its contents character by character using fgetc until the end of the file is reached or a non-printable ASCII character (outside the range [32, 126]) is encountered. If any character in the file falls outside the printable ASCII range and is not a tab (\t), newline (\n), or carriage return (\r), the file is considered a binary file.

This is because binary files often contain non-printable characters, and text files typically consist of printable characters and control characters like tabs and newlines.


----------File Extension Analysis-----------------
The function also checks the file extension by extracting it from the file_path using strrchr. 
If the file has an extension:
1. Common text file extensions (e.g., ".txt", ".c", ".cpp", ".py"), the file is considered a text file.
2. Common binary file extensions (e.g., ".bin", ".dat"), the file is considered a binary file.

The extension analysis provides an additional hint about the file type, but it doesn't guarantee the file's actual content, therefore we have combined both of these verification checks to garuntee result. 

---------Edge Case Scenario for Binary------------

We are writing 50 bytes of odd numbers in the binary file. 
In cases where the inputted number is outside the range of 50 and 200, error is thrown. 
On the other hand, lets say that the 50 bytes of numbers input increases more than 200 number, then we stop at 200 or 50 bytes, whichever comes first. 
    For example an input of 195, will only write 195, 197, 199 and not the whole 50 bytes.  

-----------Combining Results----------------------
The function combines the results from content inspection and file extension analysis to make a final determination:
If the content inspection suggests a binary file or the extension matches a binary extension, the file is considered a binary file.
If the content inspection suggests a text file and the extension matches a text extension, the file is considered a text file.
If there is a discrepancy between content inspection and extension analysis, the function relies on the content inspection result, prioritizing the detection of non-printable characters as an indicator of a binary file.


-----------Limitiaton of write50bytefunction-----------
Since we are looking at both the file type and extension, some cases where there is not data (content) in the file, causes us to make a decision that it is a binary file, even though it could have been a text file, as we are automaticaly detecting between thet two rather than user inputted. 

------------CODES GENERATED BY AI----------------------
------------If you feel like reading the Contents of the Binary files in ASCII Values then use our readbinaryfile program provided------
------------If you feel like creating a binary file with random characters then use our create binary file program provided--------

***********************************************************************************************************************************
----------------------------------------------- Errors and how we solved ----------------------------------------------------------
***********************************************************************************************************************************

To handle errors we have written the name of the system call used in the output message along with the errno error return value so that one can go to the man page of the system call and find the error. 

*** BUG : We are not checking the return values of scanf or fgets wherein it will be a good practice to see if scanf which reads certain bytes, has read the number of bytes that you expect it to read. Or fgets which returns a pointer to an array which stores the collection of characters that form a particular string, returns NULL on failure. 


ERROR: If the user created a directory and named it with an extension, say ".txt"

EXPLANATION: An error should arise as the extension is going into the name of the directory and not symboising the file type. A Directory shouldn't have a .txt at the end. 

SOLUTION: We inserted a checking condition after the user selected a directory creating option. The issue is that we are checking for a period and if the directory name contains a period and not a extension then that won't be created. We can use Regular Expressions to improve this check.



