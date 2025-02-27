#ifndef PIPE_H
#define PIPE_H

#include "kernel/types.h"
#include "list.h"

struct pipe {
	char *buffer;
	int read_pos;
	int write_pos;
	int flushed;
	int refcount;
	struct list queue;
};

struct pipe *pipe_create();
struct pipe *pipe_addref( struct pipe *p );
void pipe_delete(struct pipe *p);
void pipe_flush(struct pipe *p);

int pipe_write(struct pipe *p, char *buffer, int size);
int pipe_write_nonblock(struct pipe *p, char *buffer, int size);
int pipe_read(struct pipe *p, char *buffer, int size);
int pipe_read_nonblock(struct pipe *p, char *buffer, int size);
int pipe_size( struct pipe *p);

#endif
