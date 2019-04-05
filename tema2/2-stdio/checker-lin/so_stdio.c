#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "so_stdio.h"

#define _BUFLEN 4096
#define OP_NONE 0
#define OP_READ 1
#define OP_WRITE 2

struct _so_file {
	uint8_t *buf;
	uint8_t *bcur;
	uint8_t *bend;
	int fd;
	int last_op;
	int eof;
	int error;
};

//TODO _BUFLEN sau _BUFLEN*sizeof(uint8_t)

int is_buf_empty(SO_FILE *stream)
{
	return stream->buf == stream->bcur;
}

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	struct _so_file *new_so_file;
	int o_flags = -1;

	switch (mode[0]) {
	case 'r':
		if (mode[1] == '+')
			o_flags = O_RDWR;
		else
			o_flags = O_RDONLY;
		break;
	case 'w':
		if (mode[1] == '+')
			o_flags = O_RDWR | O_CREAT | O_TRUNC;
		else
			o_flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;
	case 'a':
		if (mode[1] == '+')
			o_flags = O_RDWR | O_CREAT | O_APPEND;
		else
			o_flags = O_WRONLY | O_CREAT | O_APPEND;
		break;
	default:
		return NULL;
	}

	new_so_file = malloc(sizeof(struct _so_file));
	if (new_so_file == NULL)
		return NULL;

	new_so_file->buf = malloc(_BUFLEN);
	if (new_so_file->buf == NULL) {
		free(new_so_file);
		return NULL;
	}

	if (o_flags & O_CREAT)
		new_so_file->fd = open(pathname, o_flags, 0666);
	else
		new_so_file->fd = open(pathname, o_flags);


	if (new_so_file->fd < 0) {
		free(new_so_file->buf);
		free(new_so_file);
		return NULL;
	}

	new_so_file->bend = new_so_file->bcur = new_so_file->buf;
	new_so_file->last_op = OP_NONE;
	new_so_file->eof = 0;
	new_so_file->error = 0;
	return new_so_file;
}

int so_fclose(SO_FILE *stream)
{
	int tmp;

	if (stream == NULL)
		return SO_EOF;
	if (!is_buf_empty(stream) && stream->last_op == OP_WRITE) {
		tmp = so_fflush(stream);
		if (tmp != 0) {  // nu ar trebui sa close(stream->fd)?
			free(stream->buf);
			free(stream);
			return SO_EOF;
		}
	}
	if (close(stream->fd) != 0) {
		free(stream->buf);
		free(stream);
		return SO_EOF;
	}
	free(stream->buf);
	free(stream);
	return 0;
}

int so_fgetc(SO_FILE *stream)
{
	ssize_t result;

	if (stream->buf == NULL)
		return SO_EOF; // TODO trebuie sa setez error?

	so_fflush(stream);
	stream->last_op = OP_READ;

	if (stream->bcur == stream->bend) {
		result = read(stream->fd, stream->buf, _BUFLEN);
		if (result < 0) {
			stream->error = 1;
			return SO_EOF;
		} else if (result == 0) {
			stream->eof = 1;
			return SO_EOF;
		}
		stream->bend = stream->buf + result;
		stream->bcur = stream->buf;
	}
	return (int)*stream->bcur++;
}

int so_fputc(int c, SO_FILE *stream)
{
	ssize_t result;

	if (stream->buf == NULL)
		return SO_EOF;

	stream->last_op = OP_WRITE;

	*stream->bcur++ = (uint8_t)c;
	if (stream->bcur == stream->buf + _BUFLEN) {
		result = write(stream->fd, stream->buf, _BUFLEN);
		if (result < 0) {
			stream->error = 1;
			return SO_EOF;
		}
		// TODO verifica daca nu s-a scris tot
		stream->bcur = stream->buf;

	}
	return c;
}

void invalidate_buf(SO_FILE *stream)
{
	stream->bcur = stream->bend = stream->buf;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	size_t i;
	int c;
	uint8_t *p = ptr;

	for (i = 0; i < nmemb; i++) {
		c = so_fgetc(stream);
		if (c == SO_EOF)
			return i;
		p[i] = (uint8_t)c;
	}
	return i;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	size_t i;
	int result;
	uint8_t *p = (uint8_t *)ptr;

	for (i = 0; i < nmemb; i++) {
		result = so_fputc(p[i], stream);
		if (result < 0)
			return 0;
	}
	return i;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
	off_t tmp;

	if (stream->last_op == OP_WRITE) {
		so_fflush(stream);
	} else {
		invalidate_buf(stream);
		offset -= stream->bend - stream->bcur;
	}

	stream->last_op = OP_NONE;

	tmp = lseek(stream->fd, offset, whence);

	if (tmp < 0)
		return -1;
	else
		return 0;
}

long so_ftell(SO_FILE *stream)
{
	off_t tmp = lseek(stream->fd, 0, SEEK_CUR);

	if (tmp < 0)
		return -1;
	else
		return tmp;
}

int so_fflush(SO_FILE *stream)
{
	ssize_t result;

	if (stream == NULL)
		return SO_EOF;
	if (stream->last_op != OP_WRITE) // TODO
		return SO_EOF;
	result = write(stream->fd,
			stream->buf,
			stream->bcur - stream->buf);
	if (result < 0) {
		stream->error = 1;
		return SO_EOF;
	}
	stream->bcur = stream->buf;
	stream->last_op = OP_NONE;
	return 0;
}

int so_fileno(SO_FILE *stream)
{
	return stream->fd;
}

int so_feof(SO_FILE *stream)
{
	return stream->eof;
}

int so_ferror(SO_FILE *stream)
{
	return stream->error;
}

SO_FILE *so_popen(const char *command, const char *type)
{
	return NULL;
	/*if (fork() != 0) {
	 *	execlp("/bin/sh", "sh", "-c", command, NULL);
	 *}
	 */
}

int so_pclose(SO_FILE *stream)
{
	return 0;
}

