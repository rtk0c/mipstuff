#pragma once

#include "emit.h"

#include <common/arena.h>

#include <stddef.h>

typedef struct MSparsestate {
	// Fill [in] fields before calling MS_init_compile()
	/*[in]*/ const char* src;
	/*[in]*/ size_t src_len;

	MSarena* a;
	MSemitter emitter;
} MSparsestate;

void MS_init_compile(MSparsestate*);
void MS_finish_compile(MSparsestate*);

typedef enum {
	MScompresult_SUCCESS,
	MScompresult_FAILURE,
} MScompresult;

// TODO
//MScompresult MS_asm_compile_ins(MSparsestate*);
