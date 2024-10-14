#include <stddef.h>
#include "../include/library/syscalls.h"
#include "named_pipe.h"
#include "kmalloc.h"
#include "string.h"

static struct named_pipe_mapping named_pipe_map[MAX_NAMED_PIPES];
static int find_free_mapping_index(void);
#define MAX_PATH_SIZE 256

/*  Splits a file path into the parent directory path and the file name. */
void extract_parent_and_file(const char *fullPath, char *parentPathBuffer, char *fileNameBuffer) {
    const char *lastSlash = fullPath;
    const char *current = strchr(fullPath, '/');

    size_t parentBufferSize = MAX_PATH_SIZE;
    size_t fileNameBufferSize = MAX_PATH_SIZE;

    while (current != NULL) {
        lastSlash = current;
        current = strchr(current + 1, '/');
    }

    // If no slash was found, or the fullPath is the root directory
    if (lastSlash == fullPath) {
        // Handle special case, such as setting parentPathBuffer to "/"
        strncpy(parentPathBuffer, "/", parentBufferSize - 1);
        parentPathBuffer[parentBufferSize - 1] = '\0';
        fileNameBuffer[0] = '\0'; // No file name in this case
    } else {
        size_t parentLength = lastSlash - fullPath;
        if (parentLength >= parentBufferSize) parentLength = parentBufferSize - 1;

        strncpy(parentPathBuffer, fullPath, parentLength);
        parentPathBuffer[parentLength] = '\0';

        strncpy(fileNameBuffer, lastSlash + 1, fileNameBufferSize - 1);
        fileNameBuffer[fileNameBufferSize - 1] = '\0';
    }
}

/* Finds a mapping entry to store the named pipe mapping */
static int find_free_mapping_index(void) { // uses a linear search
    for (int i = 0; i < MAX_NAMED_PIPES; i++) {
        if (named_pipe_map[i].pipe == NULL) {
            return i;
        }
    }
    return -1;
}

/* Map named pipe and dirent together */
static void map_named_pipe(struct named_pipe *pipe, struct fs_dirent *dirent) {
    int index = find_free_mapping_index();
    if (index >= 0) {
        named_pipe_map[index].pipe = pipe;
        if (!named_pipe_map[index].pipe) {
            printf("Failed to map the IPC pipe to mapping system.\n");
        } 

        named_pipe_map[index].dirent = dirent;
        if (!named_pipe_map[index].dirent) {
            printf("Failed to map the dirent to mapping system.\n");
        }
    }
}

/* Find the named pipe by path */
static struct named_pipe *find_named_pipe_by_path(const char *path) {
    for (int i = 0; i < MAX_NAMED_PIPES; i++) {
        // Ensure the pipe entry in the map is not NULL
        if (named_pipe_map[i].pipe != NULL) {
            char *current_path = named_pipe_map[i].pipe->path;
            if (strcmp(current_path, path) == 0) { // compare the paths using strcmp
                    return named_pipe_map[i].pipe;
            }
        }
    }
    return NULL; // Not found
}

/* Create a named pipe */
int named_pipe_create(const char *fname) {
    char parentPath[MAX_PATH_SIZE] = {0};
    char fileName[MAX_PATH_SIZE] = {0};

    // Step 1: Extract the path to get parentPath and fileName
    extract_parent_and_file(fname, parentPath, fileName);

    // Step 2: Get the fs_dirent of parentPath
    struct fs_dirent *parentDir = fs_resolve(parentPath);
    if (!parentDir) {
        return -1;  // failed to resolve the parent directory
    }

    // Step 3: Create file within that fs_dirent
    struct fs_dirent *pipe_dirent = fs_dirent_mkfile(parentDir, fileName);
    fs_dirent_close(parentDir);  // close the parent directory dirent
    if (!pipe_dirent) {
        return -1; // pipe dirent creation failed
    }

    // Step 4: Allocate and initialize named_pipe structure
    struct named_pipe *np = kmalloc(sizeof(struct named_pipe));
    if (!np) {
        printf("Failed to allocate named pipe structure.\n");
        fs_dirent_close(pipe_dirent); // cleanup
        return -1; // failed to allocate named_pipe
    }

    np->base = pipe_create(); // Step 4.1: create IPC pipe
    if (!np->base) {
        printf("Failed to create IPC pipe for named pipe.\n");
        kfree(np);
        fs_dirent_close(pipe_dirent); // cleanup
        return -1; // failed to create IPC mechanism
    }

    np->path = strdup(fname); // Step 4.2: copy path for the named pipe
    if (!np->path) {
        printf("Failed to copy path to named pipe.\n");
        pipe_delete(np->base); // cleanup
        kfree(np);
        fs_dirent_close(pipe_dirent); // cleanup
        kfree(parentPath);
        return -1; // failed to copy path
    }

    map_named_pipe(np, pipe_dirent); // map the named pipe to its directory entry
    return 0;  // success
}

/* Open an existing named pipe */
int named_pipe_open(const char *path, struct named_pipe **np_out) {
    char parentPath[MAX_PATH_SIZE] = {0};
    char fileName[MAX_PATH_SIZE] = {0};

    // Step 1: Extract the path to get parentPath and fileName
    extract_parent_and_file(path, parentPath, fileName);

    // Step 1: Resolve the named pipe dirent
    struct fs_dirent *pipe_dirent = fs_resolve(parentPath);
    if (!pipe_dirent) {
        return -1; // failed to resolve named pipe
    }

    // Step 2: Find the named pipe associated with this dirent
    struct named_pipe *np = find_named_pipe_by_path(path);
    if (!np) {
        printf("Named pipe structure not found for %s.\n", path);
        return -1;
    }

    // Step 3: Increase the refcount
    if (np->base) {
        pipe_addref(np->base);
    } else {
        printf("np->base is not valid.\n");
    }

    *np_out = np; 
    return 0; // success
}

/* Delete a named pipe */
int named_pipe_delete(const char *path) {
    // Step 1: Find the named pipe by path
    struct named_pipe *np = find_named_pipe_by_path(path);
    if (!np) {
        printf("Named pipe not found for deletion: %s.\n", path);
        return -1; // named pipe not found
    }

    // Step 2: Decrease the reference count and possibly delete the underlying pipe
    if (np->base) {
        pipe_delete(np->base);
    }

    // Step 3: Free the memory allocated for the path
    if (np->path) {
        kfree(np->path);
        np->path = NULL;
    }

    // Step 4: Remove the named pipe from the mapping array
    for (int i = 0; i < MAX_NAMED_PIPES; i++) {
        if (named_pipe_map[i].pipe == np) {
            named_pipe_map[i].pipe = NULL;
            named_pipe_map[i].dirent = NULL;
            break;
        }
    }

    // Step 5: Free the named_pipe structure
    kfree(np);

    return 0; // success
}
