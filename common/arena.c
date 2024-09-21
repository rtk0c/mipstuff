#include "arena.h"

MSarena MS_make_arena() {
	MSarena res;
	res.curr = malloc(sizeof(MSarenablock));
	res.curr->prev = NULL;
	res.curr_fill = 0;
	return res;
}

void MS_destroy_arena(MSarena* arena) {
	MSarenablock* b = arena->curr;
	while (b) {
		MSarenablock* prev = b->prev;
		free(b);
		b = prev;
	}
}


