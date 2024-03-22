#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct MSbuf {
	char* data;
	size_t fill;
	size_t capacity;
} MSbuf;

MSbuf MS_make_buf(size_t initialCapacity);
void MS_destroy_buf(MSbuf* buf);

void MS_buf_resize(MSbuf* buf, size_t new_size);

void MS_buf_append(MSbuf* buf, const char* data, size_t len);
// `static inline` behave as-if C++ `inline`
// https://stackoverflow.com/questions/31108159/what-is-the-use-of-the-inline-keyword-in-c
static inline void MS_buf_append_u32(MSbuf* buf, uint32_t v) { MS_buf_append(buf, (char*)&v, sizeof(v)); }
static inline void MS_buf_append_u16(MSbuf* buf, uint16_t v) { MS_buf_append(buf, (char*)&v, sizeof(v)); }
static inline void MS_buf_append_u8(MSbuf* buf, uint8_t v) { MS_buf_append(buf, (char*)&v, sizeof(v)); }

void MS_dump_to_FILE(FILE* file, MSbuf* buf);
