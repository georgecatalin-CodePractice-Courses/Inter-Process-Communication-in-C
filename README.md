# Inter-Process-Communication
Inteprocess-Communication (IPC) examples in C programming language.

## Theoretic aspects
The following Interprocess Communication mechanisms (IPC) can be used:
* pipes
* named pipes (FIFO)
* sockets
* message queues
* shared memory

### Unidirectional communication:
* pipes
* named pipes (FIFO)

## Bidirectional communication:
* sockets
* message queues
* shared memory

## Communicating between processes using pipes
### Vocabulary
The most commonly used functions and terms for communication between processes  are 
* pipe()
* mkfifo()
* write()
* read()
* perror()
* fork()


#### pipe()
A pipe is used for connecting two processes. The output from one process can be sent as an input to another process. The flow is unidirectional, that is one process can write to the pipe, and another process can read from the pipe. Writing and reading are done in an area of main memory which is called VIRTUAL FILE.

Pipes have a First in First Out (FIFO) or queue structure, that means what is written first, is read first too.
A process should not try to read from a pipe before something is written into it, otherwise it will suspend until something is written onto the pipe.

* syntax
int pipe(int arr[2]); where arr[0] is a file descriptor for the read end of the pipe and arr[1] is a file descriptor for the write end of the pipe. Function pipe() returns 0 on successful completion and otherwise -1.

#### mkfifo()
This function creates a new FIFO special file. 
* syntax
int mkfifo(const char *filename, mode_t permission) where filename represents the filename along with complete path, and permission represents the permission bits of the new FIFO file. The default permissions are read and write permission for owner, group and others that is 0666

#### write()
This function is used for writing the specified file or pipe whose descriptor is supplied.
* syntax
write(int file_descriptor, const void *buf,size_t n) where it writes the n number of bytes into the file that is being pointed by the file pointer file_descriptor, from the buffer buf

#### read()
This function reads from the specified file or pipe by the file descriptor that is supplied
*  syntax
read(int file_descriptor, void *buf, size_t n) where it tries to read up to a number of n bytes from a file that is being pointed by the file_descriptor. The bytes that are read are then assigned to the buffer buf.

#### perror()
This function displays the error message indicating the error that might have occurred while invoking a function or system call. The error message is displayed to stderr, that is, the standard error output stream.
* syntax
void perror(const char *str) where the error message displayed is optionally preceded by the string 'str'  which is provided by the function.

#### fork()
This function is used to create a new process. The newly created process is called the child process, and runs concurrently with the new process.
After the execution of the fork() function, the execution of the program continues and the instruction following the fork() is executed in both the parent and the child process. If such a system call is successful , it will return a process ID of the child process in the parent process and it returns 0 in the newly created process. The fork() function returns a negative value if a child process is not created.

