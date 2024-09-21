#pragma once

#include <stddef.h>
#include <stdint.h>

// Internal
// FIXME This is horrible. Replace with flexible array member + attempt to realloc?
typedef struct MSarenablock {
	struct MSarenablock* prev;
	char data[4096 - sizeof(struct MSarenablock*)];
} MSarenablock;

typedef struct MSarena {
	MSarenablock* curr;
	size_t curr_fill;
} MSarena;

MSarena MS_make_arena();
void MS_destroy_arena(MSarena*);

// Minimum alignment: alignof(unsigned long long)

char* MS_push(MSarena*, size_t size, size_t alignment);
static inline char* MS_push_string(MSarena* a, size_t size) { return MS_push(a, size, _Alignof(char)); }
