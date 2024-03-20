#pragma once

#include <common/buffer.h>
#include <common/mips.h>

typedef struct MSemitter {
	MSbuf instructions;
} MSemitter;

MSemitter MS_make_emitter();

void MS_emit_add(MSemitter* e, MSreg dst, MSreg op1, MSreg op2);
void MS_emit_addi(MSemitter* e, MSreg dst, MSreg op, MSimmediate imm);
void MS_emit_sub(MSemitter* e, MSreg dst, MSreg op1, MSreg op2);
