#include "parse.h"

void MS_init_compile(MSparsestate* state) {
	state->arena = MS_make_arena();
	state->emitter = MS_make_emitter();
}

void MS_finish_compile(MSparsestate* state) {
	MS_destroy_emitter(&state->emitter);
	MS_destroy_arena(&state->arena);
}
