#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct MSarena MSarena;

MSarena* MS_make_arena();
void MS_destroy_arena(MSarena*);

// Minimum alignment: alignof(unsigned long long)

char* MS_a_push(MSarena*, size_t size, size_t align);
char* MS_a_push_zero(MSarena*, size_t size, size_t align);

char* MS_a_current_pointer(MSarena*);

// _push() a point in allocations for restoring by _pop()
size_t MS_a_push_section(MSarena*);
void MS_a_pop_section(MSarena*, size_t section);

// Allocate string in arena, not null terminated
// Call this multiple times consequtively multiple times to concat multiple strings
#define APUSH_STRING(a, arg_str, arg_len) ({ auto len = arg_len; memcpy(MS_a_push((a), len, 1), (str), len); })
// Allocate string in arena, plus a null terminator
#define APUSH_STRING_Z(a, arg_str, arg_len) ({ auto len = arg_len; char* res = memcpy(MS_a_push(MS_a_push((a), len + 1, 1), (str), len); *res[len] = '\0'; res; })
#define APUSH_0(a) ()*MS_a_push((a), 1, 1) = '\0')

