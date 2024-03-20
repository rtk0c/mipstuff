#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct MSstr {
	char* data;
	uint32_t len;
	uint32_t capacity;
} MSstr;

typedef struct MSstrview {
	const char* data;
	uint32_t len;
	uint32_t _unused;
} MSstrview;
