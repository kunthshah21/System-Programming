#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// These declarations hold pointers to the source and destination path. 
char *source_path;
char *dest_path;

/*
We could be using these different errors codes and handle the errors within the C program itself;
but as written in the assignment; we will always return 0 (check  int main ) and -1 for the error;
while storing the error in ec and handling it through the bash script. This was certainly the tricky part. 

Error codes will be returned and then put them in the bash script to run them.
*/

#define OKAY              0
#define WRONG_ARGUMENT   1
#define FILE_NOT_FOUND  2
#define FILEOPEN_ERROR 3
#define FILEREAD_ERROR 4
#define WRITE_ERROR 5
#define CLOSE_ERROR 6
#define NULL_DESTINATION_ERROR  7

int ec = OKAY;

int copy_file(const char *src, const char *dest);

int main(int argc, char *argv[]) {
    if (argc == 5) {
        if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0) {
            source_path = argv[2];
            dest_path = argv[4];
            ec = copy_file(source_path, dest_path);
        } else {
            ec = WRONG_ARGUMENT;
        }
    } else {
        ec = WRONG_ARGUMENT;
    }
    /*
    This part of the code checks if the correct number of arguments (5) is provided. 
    It verifies that the options -i and -o are used in the correct positions and then calls the copy_file function with the source and destination paths. 
    If any condition fails, it sets ec to WRONNG_ARGUMENT call
    
    */
    
    printf("%d\n", ec); // Print error code to stdout (will be captured by the calling script and not by C program itself)

    return 0;
}

int copy_file(const char *src, const char *dest) {
    int source_fd, dest_fd;
    char buffer[1024];                      // This will be used to temporarily store data in between the system read and write: 128 bytes
    ssize_t read_bytes, write_bytes;        // Store no. of TOTAL bytes to be read and wrote for comparision 

    // Check if source file exists
    if (access(src, F_OK) == -1) {
        return FILE_NOT_FOUND;
    }

    // Check if destination file exists
    if (access(dest, F_OK) == -1) {
        return NULL_DESTINATION_ERROR;
    }

    source_fd = open(src, O_RDONLY);
    if (source_fd == -1) {
        ec = errno;
        return FILEOPEN_ERROR;
    }
    /*
    This line attempts to open the source file in read-only mode (O_RDONLY). 
    The open function returns a file descriptor, which is assigned to source_fd.
    The errno variable is set by the system to indicate the specific error. 
    The function sets ec (error code) to the value of errno and returns an appropriate error code (FILEOPEN_ERROR).
    */

    dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dest_fd == -1) {
        ec = errno;
        close(source_fd);
        return FILEOPEN_ERROR;
    }

    while ((read_bytes = read(source_fd, buffer, sizeof(buffer))) > 0) {
        write_bytes = write(dest_fd, buffer, read_bytes);
        if (write_bytes != read_bytes) {
            ec = errno;
            close(source_fd);
            close(dest_fd);
            return WRITE_ERROR;
        }
    }
    /*
        This loop reads the file in chunks of bytes specifified in the buffer. It ends when the file is read completely by matching total byte size. 
        It then write this data in the source file,
        if the number of bytes does not match, indicated some error and the ec is updated and the error is returned. 
    */

    if (read_bytes == -1) {
        ec = errno;
        close(source_fd);
        close(dest_fd);
        return FILEREAD_ERROR;
    }

    /*
    Ater the read loop, it checks if any error occurred during the read operation. 
    If so, it sets ec to errno, closes both files, and returns an appropriate error code (FILEREAD_ERROR).
    */

    if (close(source_fd) == -1 || close(dest_fd) == -1) {
        ec = errno;
        return CLOSE_ERROR;
    }

    /*
    Finally, it attempts to close both the source and destination files. 
    If any of the close calls fail, it sets ec to errno and returns an appropriate error code (CLOSE_ERROR).
    */

    return OKAY;
    // return okay, if no errors were caught in the entire operation
}

