#include "arena.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct MSarena {
	size_t capacity;
	size_t fill;
	char buffer[];
};

#define ARENA_DEFAULT_ALLOC_SIZE 0xFFF000
#define ARENA_DEFAULT_CAPACITY (ARENA_DEFAULT_ALLOC_SIZE - sizeof(MSarena))

MSarena* MS_make_arena() {
	MSarena* a = malloc(ARENA_DEFAULT_ALLOC_SIZE);
	a->capacity = ARENA_DEFAULT_CAPACITY;
	a->fill = 0;
	return a;
}

void MS_destroy_arena(MSarena* a) {
	free(a);
}

char* MS_a_push(MSarena* a, size_t size, size_t align) {
	uintptr_t align_mask = align - 1;
	uintptr_t initial = (uintptr_t)a->buffer + a->fill;
	uintptr_t rounded = (initial + align_mask) & ~align_mask;
	// Allocate (alignment padding) + (size) bytes
	a->fill += (rounded - initial) + size;
	if (a->fill > a->capacity) {
		fprintf(stderr, "TODO: arena reallocation not implemented\n");
		abort();
	}
	return (char*)rounded;
}

char* MS_a_push_zero(MSarena* a, size_t size, size_t align) {
	char* res = MS_a_push(a, size, align);
	memset(res, 0, size);
	return res;
}

char* MS_a_current_pointer(MSarena* a) {
	return a->buffer + a->fill;
}

size_t MS_a_push_section(MSarena* a) {
	return a->fill;
}

void MS_a_pop_section(MSarena* a, size_t section) {
	a->fill = section;
}
