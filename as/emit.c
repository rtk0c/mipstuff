#include "emit.h"

#include <common/common.h>

MSemitter MS_make_emitter() {
	MSemitter res;
	res.instructions = MS_make_buf(MS_KB(4));
	return res;
}

#define PACK_VAR _pack_var
#define PACK_BEGIN uint32_t PACK_VAR = 0; int write_nth_bit = 31
// TODO sanity check that v does not have more than `bits` bits
#define PACK_BITS(v, bits) PACK_VAR |= ((v) << (write_nth_bit - bits + 1)); write_nth_bit -= bits;
#define PACK_BITS_SPECIAL(rs, rt, rd, shamt, funct) PACK_BITS(0, 6); PACK_BITS(rs, 5); PACK_BITS(rt, 5); PACK_BITS(rd, 5); PACK_BITS(shamt, 5); PACK_BITS(funct, 5)
#define PACK_BITS_I(op, rs, rt, imm) PACK_BITS(op, 6); PACK_BITS(rs, 5); PACK_BITS(rt, 5); PACK_BITS(imm, 16);

void MS_emit_add(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	PACK_BEGIN;
	PACK_BITS_SPECIAL(op1, op2, dst, 0, 0b100000);
	MS_buf_append_u32(&e->instructions, PACK_VAR);
}

void MS_emit_addi(MSemitter* e, MSreg dst, MSreg op, MSimmediate imm) {
	PACK_BEGIN;
	// TODO handle large imm
	PACK_BITS_I(0b001000, op, dst, imm);
	MS_buf_append_u32(&e->instructions, PACK_VAR);
}

void MS_emit_sub(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	PACK_BEGIN;
	PACK_BITS_SPECIAL(op1, op2, dst, 0, 0b100000);
	MS_buf_append_u32(&e->instructions, PACK_VAR);
}
