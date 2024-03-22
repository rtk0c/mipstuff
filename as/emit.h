#pragma once

#include <common/buffer.h>
#include <common/mips.h>

typedef struct MSemitter {
	MSbuf instructions;
} MSemitter;

MSemitter MS_make_emitter();
