#include "emit.h"

#include <assert.h>
#include <common/common.h>

MSemitter MS_make_emitter() {
	MSemitter res;
	res.instructions = MS_make_buf(MS_KB(4));
	return res;
}

// N.B. these macros don't parenthesize their arguments. Be careful in usages to not put complex expressions in.

#define MASK(n) (1 << n)

#define PACK_BEGIN uint32_t _pack_val = 0; int _write_nth_bit = 31
// TODO sanity check that v does not have more than `bits` bits
#define PACK_BITS(v, bits) _pack_val |= (((uint32_t)v & MASK(bits)) << (_write_nth_bit - bits + 1)); _write_nth_bit -= bits
// Known as "R types" in old educational MIPS manuals
// These are instructions with 0 in the op field, and have 5-5-5-5-5 bit fields coming after that
#define PACK_BITS_SPECIAL(rs, rt, rd, shamt, funct) PACK_BITS(0, 6); PACK_BITS(rs, 5); PACK_BITS(rt, 5); PACK_BITS(rd, 5); PACK_BITS(shamt, 5); PACK_BITS(funct, 5)
// Known as "I types" in old educational MIPS manuals
#define PACK_BITS_IMM(op, rs, rt, imm) PACK_BITS(op, 6); PACK_BITS(rs, 5); PACK_BITS(rt, 5); PACK_BITS(imm, 16);

#if defined(__GNUC__) || defined(__clang__)
#	define PACK_GET_VALUE ({ assert(_write_nth_bit == 0); _pack_val; })
#else
#	define PACK_GET_VALUE _pack_val
#endif

void MS_emit_add(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	PACK_BEGIN;
	PACK_BITS_SPECIAL(op1, op2, dst, 0, 0b100000);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

void MS_emit_addu(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	PACK_BEGIN;
	PACK_BITS_SPECIAL(op1, op2, dst, 0, 0b100001);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

void MS_emit_addi(MSemitter* e, MSreg dst, MSreg op, MSimmediate imm) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS_IMM(0b001000, op, dst, imm);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

void MS_emit_addiu(MSemitter* e, MSreg dst, MSreg op, MSimmediate imm) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS_IMM(0b001001, op, dst, imm);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

void MS_emit_addiupc(MSemitter* e, MSreg dst, MSimmediate imm) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS(0b111011, 6);
	PACK_BITS(dst, 5);
	PACK_BITS(0b00, 2);
	PACK_BITS(imm, 19);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// TODO should we keep this? or write the BGEZAL and warn on arguments that doesn't match the BAL special case
void MS_emit_bal(MSemitter* e, MSimmediate offset) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS_IMM(0b000001, 0b00000, 0b10001, imm);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// TODO BALC

// TODO are these deprecated?
void MS_emit_beq(MSemitter* e, MSreg a, MSreg b, MSimmediate offset) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS_IMM(0b000100, a, b, imm);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

void MS_emit_bne(MSemitter* e, MSreg a, MSreg b, MSimmediate offset) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS_IMM(0b000100, a, b, imm);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

void MS_emit_sub(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	PACK_BEGIN;
	PACK_BITS_SPECIAL(op1, op2, dst, 0, 0b100000);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}
