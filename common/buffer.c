#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MSbuf MS_make_buf(size_t initialCapacity) {
	MSbuf res;
	res.data = malloc(initialCapacity);
	res.fill = 0;
	res.capacity = initialCapacity;
	return res;
}

void MS_destroy_buf(MSbuf* buf) {
	free(buf->data);
	buf->data = NULL;
	buf->fill = 0;
	buf->capacity = 0;
}

void MS_buf_resize(MSbuf* buf, size_t new_size) {
	buf->data = realloc(buf->data, new_size);
	buf->capacity = new_size;
}

static void buf_resize_more(MSbuf* buf) {
	MS_buf_resize(buf, buf->capacity * 2);
}

void MS_buf_append(MSbuf* buf, const char* data, size_t len) {
	size_t avail = buf->capacity - buf->fill;
	if (avail < len) {
		buf_resize_more(buf);
	}

	memcpy(buf->data + buf->fill, data, len);
	buf->fill += len;
}

void MS_dump_to_FILE(FILE* file, MSbuf* buf) {
	fwrite(buf->data, 1, buf->fill, file);
}
