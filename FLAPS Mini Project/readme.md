
Github Link: https://github.com/kunthshah21/System-Programming.git

--------------------------------------------------------------
# Universal Process Function
--------------------------------------------------------------
This function opens the proc folder and then iterartes through each of the processes listed in the library. This information is then printed in the terminal.

--------------------------------------------------------------
# Pid Specific Details Function
--------------------------------------------------------------

# Method and Approach 
This function utilizes the /proc filesystem, a virtual filesystem that provides detailed information about running processes in Linux. Each process has its directory within /proc named after its PID. By accessing specific files within these directories, such as cmdline, status, stat, statm, environ, and /fd, the function retrieves various details about the process. This approach was chosen because it provides a straightforward and standardized way to access process-related information in Linux without requiring specialized libraries or system calls. Additionally, it allows for easy extensibility and customization by reading different files within the /proc/[pid] directory.

# Specifics of each sub-part of the funciton

1. **Process ID**: The PID is directly passed to the function as an argument.

2. **Command Name**: The command name is retrieved by opening the `/proc/[pid]/cmdline` file, which contains the command-line arguments passed to the process when it was started. The content of this file is read into a buffer and represents the command name of the process.

3. **Status Information**: Detailed status information about the process is obtained by reading the `/proc/[pid]/status` file. This file contains various fields such as the process state, memory usage, parent process ID, and more. Each line of this file is read into a buffer and presented as status information.

4. **Memory Information**: Memory usage information is extracted from the `/proc/[pid]/statm` file. This file contains memory statistics for the process, including total program size, resident set size, and shared text size. The information is read and parsed directly into variables representing memory usage.

5. **CPU Information**: CPU usage information is obtained from the `/proc/[pid]/stat` file. The required information, namely user and system CPU times, is extracted from specific fields within this file.

6. **Command Line Arguments**: Command-line arguments passed to the process are retrieved from the `/proc/[pid]/cmdline` file. This file contains the command and arguments used to invoke the process. The content of this file is directly read into a buffer.

7. **Start Time and Uptime**: The start time and uptime of the process are read from the `/proc/[pid]/stat` file. These values represent the time the process started (measured in clock ticks since system boot) and the uptime of the system. The start time is then converted into seconds and the elapsed time since the process started is calculated.

8. **Environment Variables**: Environment variables associated with the process are obtained from the `/proc/[pid]/environ` file. This file contains a null-delimited list of environment variable settings for the process. Each environment variable is read individually from this file.

9. **Open File Descriptors**: Information about the process's open file descriptors is retrieved by scanning the `/proc/[pid]/fd` directory. Each entry in this directory represents an open file descriptor. The type of each entry is checked, and if it's a symbolic link (indicating an open file descriptor), its name is printed.

10. **Process Group and Session Information**: Additional process group and session information is obtained by parsing the `/proc/[pid]/status` file. This file contains various fields including user ID, group ID, supplementary groups, and file descriptor size. Specific lines containing this information are selectively read and presented.





--------------------------------------------------------------
# Session Tree Printing Function
--------------------------------------------------------------

This function recursively prints the process tree in a tree-like format, starting from a given process ID (pid) and going down to its child processes up to a certain depth level (level).

# Steps and Approach: 
1. The function first checks if the current level of indentation exceeds a predefined maximum level (MAX_LEVEL). If it does, the function returns, avoiding further recursion to prevent infinite looping. 
2. It constructs the path to the /proc directory corresponding to the given PID.
3. Opens the directory and iterates over its contents, which are symbolic links representing individual tasks (threads) of the process.
4. Prints the PID of the current process with appropriate indentation based on the current level.
5. Recursively calls itself for each child process (task) found in the directory.





--------------------------------------------------------------
# Processes in the same Group Function
--------------------------------------------------------------

This function identifies and prints the processes within the same process group as the target process specified by its PID. It traverses the /proc directory, which contains directories named after process IDs, and checks each process's group ID to determine if it matches the group ID of the target process. 

# Steps and Approach: 
1. Opens the /proc directory to access information about running processes and iterates through each entry in the directory, assuming each entry corresponds to a process.
2. Checks if the entry is a directory and its name is a numerical string, implying it represents a process.
3. Excludes the target process and its parent process from consideration to avoid redundant comparisons.
4. Retrieves the process group ID of the current process by reading the appropriate entry in the /proc/<pid>/stat file.
5. Compares the process group ID of the current process with the process group ID of the target process obtained using getpgid. If the process group IDs match, prints a message indicating that the current process is in the same group as the target process.

--------------------------------------------------------------
# Processes of the User using RealID and EffectiveID
--------------------------------------------------------------

These functionalities are made up of two seperate functions based on the need

Both functions aim to identify and print processes based on user identification (UID). They achieve this by iterating through the /proc directory, which contains information about running processes on the system. They extract the UID associated with each process from the /proc/<pid>/status file and compare it with the specified UID. Processes matching the specified UID are then printed.


# Difference between the two approach:

Certainly. The key difference between the `process_user_realID` and `process_user_effectiveID` functions lies in how they interpret and compare user identification (UID) information obtained from the `/proc/<pid>/status` files. `process_user_realID` focuses on determining whether a process is executing on behalf of a specific real user ID, representing the actual user who initiated the process or owns the resource. In contrast, `process_user_effectiveID` examines the effective user ID of each process, reflecting the user identity the process effectively possesses during execution, which may differ from the real user ID due to privilege elevation mechanisms like `setuid`. Consequently, while `process_user_realID` assists in authorization checks based on user permissions `process_user_effectiveID` aids in access control decisions by considering the effective permissions of processes.





