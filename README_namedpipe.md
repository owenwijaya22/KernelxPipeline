# Named PIPE Implementation in Basekernel

This project extends the existing PIPE mechanism in Basekernel to implement Named PIPEs (also known as FIFOs). Named PIPEs allow for inter-process communication between unrelated processes using a file system name.

## Background

Basekernel already implements a basic PIPE mechanism with the following properties:

- Unidirectional byte stream
- Sequential data passing
- No message boundaries
- Blocking reads on empty PIPEs
- End-of-file detection when write-end is closed
- Blocking writes when PIPE is full
- Potential segmentation of large writes

## Named PIPE Features

The Named PIPE implementation builds upon the existing PIPE mechanism and adds the following features:

1. File system representation: Named PIPEs have a name within the file system.
2. Open and delete operations: Named PIPEs can be opened and deleted like regular files.
3. Inter-process communication: Allows communication between unrelated processes using the file name.

## Implementation Details

The Named PIPE implementation includes:

1. Extension of the existing PIPE mechanism (kernel/pipe.h and kernel/pipe.c)
2. File system integration for Named PIPEs
3. Open and close operations for Named PIPEs
4. Read and write operations maintaining PIPE semantics

## Usage

To use a Named PIPE:

1. Create a Named PIPE using a system call (e.g., `mkfifo`)
2. Open the Named PIPE for reading or writing using its file system name
3. Perform read and write operations as with regular PIPEs
4. Close the Named PIPE when finished

## Testing

A user-level test program (similar to user/test/pipetest.c) will be provided to demonstrate the functionality of Named PIPEs, including:

1. Creating a Named PIPE
2. Writing to the Named PIPE from one process
3. Reading from the Named PIPE in another process
4. Demonstrating inter-process communication

## Future Work

- Implement support for non-blocking I/O operations on Named PIPEs
- Add support for multiple readers and writers on a single Named PIPE
- Implement additional file system operations (e.g., permissions, ownership)

## References

- Basekernel project: [https://github.com/dthain/basekernel](https://github.com/dthain/basekernel)
- PIPE implementation: kernel/pipe.h and kernel/pipe.c
- PIPE test program: user/test/pipetest.c