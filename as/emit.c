#include "emit.h"

#include <assert.h>
#include <common/common.h>

// Macros prefixed with DO_ indicate it performs value validity checks
// otherwise, it is unchecked.

MSemitter MS_make_emitter() {
	MSemitter res;
	res.instructions = MS_make_buf(MS_KB(4));
	return res;
}

// N.B. these macros don't parenthesize their arguments. Be careful in usages to not put complex expressions in.

// This does not work for n=32 (filling the whole literal with bit 1), but we don't need it for the emitter
// WATCH OUT AND BE CAREFULY TO NOT USE IT THAT WAY IN THE FUTURE
#define MASK(n) ((1 << n) - 1)

#define PACK_BEGIN          \
	uint32_t _pack_val = 0; \
	int _write_nth_bit = 31
// TODO sanity check that v does not have more than `bits` bits
#define PACK_BITS(v, bits)                                                    \
	_pack_val |= (((uint32_t)v & MASK(bits)) << (_write_nth_bit - bits + 1)); \
	_write_nth_bit -= bits

// Known as "R types" in old educational MIPS manuals
// These are instructions with 0 in the op field, and have 5-5-5-5-5 bit fields coming after that
#define DO_PACK_SPECIAL(rs, rt, rd, shamt, funct) \
	PACK_BEGIN;                                   \
	PACK_BITS(0, 6);                              \
	PACK_BITS(rs, 5);                             \
	PACK_BITS(rt, 5);                             \
	PACK_BITS(rd, 5);                             \
	PACK_BITS(shamt, 5);                          \
	PACK_BITS(funct, 5)
// Known as "I types" in old educational MIPS manuals
#define DO_PACK_IMM(op, rs, rt, imm)              \
	assert(imm >= INT16_MIN && imm <= INT16_MAX); \
	PACK_BEGIN;                                   \
	PACK_BITS(op, 6);                             \
	PACK_BITS(rs, 5);                             \
	PACK_BITS(rt, 5);                             \
	PACK_BITS(imm, 16)
#define DO_PACK_J_TYPE(op, imm)       \
	assert(MS_INT_IN_RANGE(imm, 26)); \
	PACK_BEGIN;                       \
	PACK_BITS(op, 6);                 \
	PACK_BITS(imm, 26)
#define DO_PACK_REGIMM(rs, code, imm)             \
	assert(imm >= INT16_MIN && imm <= INT16_MAX); \
	PACK_BEGIN;                                   \
	PACK_BITS(0b000001, 6);                       \
	PACK_BITS(rs, 5);                             \
	PACK_BITS(code, 5);                           \
	PACK_BITS(imm, 16)

#if defined(__GNUC__) || defined(__clang__)
#	define PACK_GET_VALUE ({ assert(_write_nth_bit == 0); _pack_val; })
#else
// Best effort, assume that assert() expands to some kind of expression
#	define PACK_GET_VALUE _pack_val(assert(_write_nth_bit == 0), _pack_val)
#endif

// Add Word (trapping)
void MS_emit_add(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	DO_PACK_SPECIAL(op1, op2, dst, 0, 0b100000);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Add Word (non-trapping)
void MS_emit_addu(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	DO_PACK_SPECIAL(op1, op2, dst, 0, 0b100001);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Add Immediate (unsigned - non-trapping)
void MS_emit_addiu(MSemitter* e, MSreg dst, MSreg op, MSimmediate imm) {
	DO_PACK_IMM(0b001001, op, dst, imm);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Add Immediate to PC (unsigned - non-trapping)
void MS_emit_addiupc(MSemitter* e, MSreg dst, MSimmediate imm) {
	assert(imm >= INT16_MIN && imm <= INT16_MAX);
	PACK_BEGIN;
	PACK_BITS(0b111011, 6);
	PACK_BITS(dst, 5);
	PACK_BITS(0b00, 2);
	PACK_BITS(imm, 19);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// SUBtract
void MS_emit_sub(MSemitter* e, MSreg dst, MSreg op1, MSreg op2) {
	DO_PACK_SPECIAL(op1, op2, dst, 0, 0b100000);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// TODO should we keep this? or write the BGEZAL and warn on arguments that doesn't match the BAL special case
// Branch And Link
void MS_emit_bal(MSemitter* e, MSimmediate offset) {
	DO_PACK_IMM(0b000001, 0b00000, 0b10001, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Branch on EQuals
void MS_emit_beq(MSemitter* e, MSreg a, MSreg b, MSimmediate offset) {
	DO_PACK_IMM(0b000100, a, b, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Branch on Not Equals
void MS_emit_bne(MSemitter* e, MSreg a, MSreg b, MSimmediate offset) {
	DO_PACK_IMM(0b000101, a, b, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Branch on Greater Than or Equal to Zero
void MS_emit_bgez(MSemitter* e, MSreg rs, MSimmediate offset) {
	DO_PACK_REGIMM(rs, 0b00001, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Branch on Less Than Zero
void MS_emit_bltz(MSemitter* e, MSreg rs, MSimmediate offset) {
	DO_PACK_REGIMM(rs, 0b00000, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Branch Compact
void MS_emit_bc(MSemitter* e, MSimmediate offset) {
	DO_PACK_J_TYPE(0b110010, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Branch And Link Compact
void MS_emit_balc(MSemitter* e, MSimmediate offset) {
	DO_PACK_J_TYPE(0b111010, offset);
	MS_buf_append_u32(&e->instructions, PACK_GET_VALUE);
}

// Compact Compare-and-Branch, 16 bit offset
#define DO_PACK_BCONDC_16(op, rs, rt, offset)                \
	{                                                        \
		DO_PACK_IMM(op, rs, rt, offset);                     \
		MS_buf_append_u32(&e->instructions, PACK_GET_VALUE); \
	}
// Compact Compare-and-Branch, 21 bit offset
#define DO_PACK_BCONDC_21(op, rs, offset)                    \
	{                                                        \
		assert(rs != 0);                                     \
		PACK_BEGIN;                                          \
		PACK_BITS(op, 6);                                    \
		PACK_BITS(rs, 5);                                    \
		PACK_BITS(offset, 21);                               \
		MS_buf_append_u32(&e->instructions, PACK_GET_VALUE); \
	}

#define POP26 0b010110
#define RAW_EMIT_BLEZC(e, reg, offset) DO_PACK_BCONDC_16(POP26, 0, reg, offset)
#define RAW_EMIT_BGEZC(e, reg, offset) DO_PACK_BCONDC_16(POP26, reg, reg, offset)
#define RAW_EMIT_BGEC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP26, rs, rt, offset)
#define RAW_EMIT_BLEC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP26, rt, rs, offset)

#define POP27 0b010111
#define RAW_EMIT_BGTZC(e, reg, offset) DO_PACK_BCONDC_16(POP27, 0, reg, offset)
#define RAW_EMIT_BLTZC(e, reg, offset) DO_PACK_BCONDC_16(POP27, reg, reg, offset)
#define RAW_EMIT_BLTC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP27, rs, rt, offset)
#define RAW_EMIT_BGTC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP27, rt, rs, offset)

#define POP06 0b000110
#define RAW_EMIT_BGEUC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP06, rs, rt, offset)
#define RAW_EMIT_BLEUC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP06, rt, rs, offset)
#define RAW_EMIT_BLEZALC(e, reg, offset) DO_PACK_BCONDC_16(POP06, 0, reg, offset)
#define RAW_EMIT_BGEZALC(e, reg, offset) DO_PACK_BCONDC_16(POP06, reg, reg, offset)

void MS_emit_blezalc(MSemitter* e, MSreg reg, MSimmediate offset) {
	RAW_EMIT_BLEZALC(e, reg, offset);
}

void MS_emit_bgezalc(MSemitter* e, MSreg reg, MSimmediate offset) {
	RAW_EMIT_BGEZALC(e, reg, offset);
}

#define POP07 0b000111
#define RAW_EMIT_BLTUC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP07, rs, rt, offset)
#define RAW_EMIT_BGTUC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP07, rt, rs, offset)
#define RAW_EMIT_BGTZALC(e, reg, offset) DO_PACK_BCONDC_16(POP07, 0, reg, offset)
#define RAW_EMIT_BLTZALC(e, reg, offset) DO_PACK_BCONDC_16(POP07, reg, reg, offset)

void MS_emit_bgtzalc(MSemitter* e, MSreg reg, MSimmediate offset) {
	RAW_EMIT_BGTZALC(e, reg, offset);
}

void MS_emit_bltzalc(MSemitter* e, MSreg reg, MSimmediate offset) {
	RAW_EMIT_BLTZALC(e, reg, offset);
}

#define POP10 0b001000
#define RAW_EMIT_BEQC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP10, rs, rt, offset)
#define RAW_EMIT_BEQZALC(e, reg, offset) DO_PACK_BCONDC_16(POP10, 0, reg, offset)

void MS_emit_beqzalc(MSemitter* e, MSreg reg, MSimmediate offset) {
	RAW_EMIT_BEQZALC(e, reg, offset);
}

#define POP30 0b011000
#define RAW_EMIT_BNEC(e, rs, rt, offset) DO_PACK_BCONDC_16(POP30, rs, rt, offset)
#define RAW_EMIT_BNEZALC(e, reg, offset) DO_PACK_BCONDC_16(POP30, 0, reg, offset)

void MS_emit_bnezalc(MSemitter* e, MSreg reg, MSimmediate offset) {
	RAW_EMIT_BNEZALC(e, reg, offset);
}

#define POP66 0b110110
#define RAW_EMIT_BEQZC(e, rs, offset) DO_PACK_BCONDC_21(POP66, rs, offset)

#define POP76 0b111110
#define RAW_EMIT_BNEZC(e, rs, offset) DO_PACK_BCONDC_21(POP76, rs, offset)

// Change the operator as if doing a `a < b` => `b > a` flip
static MScomp MS_flip_comp(MScomp comp) {
	switch (comp) {
		case MScomp_LESSER: return MScomp_GREATER;
		case MScomp_GREATER: return MScomp_LESSER;
		case MScomp_LESSER_EQ: return MScomp_GREATER_EQ;
		case MScomp_GREATER_EQ: return MScomp_LESSER_EQ;
		default: return comp;
	}
}

void MS_emit_bcompc(MSemitter* e, MScomp comp, bool is_unsigned, MSreg rs, MSreg rt, MSimmediate offset) {
	if (rs == MIPS_R_ZERO || rt == MIPS_R_ZERO) {
		if (rs == MIPS_R_ZERO && rt == MIPS_R_ZERO) {
			assert(false);
		} else if (rs == MIPS_R_ZERO) {
			// Convert to `x (<|>) 0` form
			MS_SWAP_VARS(rs, rt);
			comp = MS_flip_comp(comp);
		} else /* if (rt == MIPS_R_ZERO) */ {
			// No-op, this is the order that we want
		}

		switch (comp) {
			case MScomp_LESSER: RAW_EMIT_BLTZC(e, rs, offset); return;
			case MScomp_GREATER: RAW_EMIT_BGTZC(e, rs, offset); return;
			case MScomp_LESSER_EQ: RAW_EMIT_BLEZC(e, rs, offset); return;
			case MScomp_GREATER_EQ: RAW_EMIT_BGEZC(e, rs, offset); return;
			case MScomp_EQUALS: RAW_EMIT_BEQZC(e, rs, offset); return;
			case MScomp_NOT_EQUALS: RAW_EMIT_BNEZC(e, rs, offset); return;
		}
		assert(false);
	} else {
		switch (comp) {
			case MScomp_LESSER:
				if (is_unsigned)
					RAW_EMIT_BLTUC(e, rs, rt, offset)
				else
					RAW_EMIT_BLTC(e, rs, rt, offset);
				return;

			case MScomp_GREATER:
				if (is_unsigned)
					RAW_EMIT_BGTUC(e, rs, rt, offset)
				else
					RAW_EMIT_BGTC(e, rs, rt, offset);
				return;

			case MScomp_LESSER_EQ:
				if (is_unsigned)
					RAW_EMIT_BLEUC(e, rs, rt, offset)
				else
					RAW_EMIT_BLEC(e, rs, rt, offset);
				return;

			case MScomp_GREATER_EQ:
				if (is_unsigned)
					RAW_EMIT_BGEUC(e, rs, rt, offset)
				else
					RAW_EMIT_BGEC(e, rs, rt, offset);
				return;

			case MScomp_EQUALS:
				// Comparison between registers
				// beqc needs rs < rt
				if (rt > rs)
					MS_SWAP_VARS(rs, rt);
				RAW_EMIT_BEQC(e, rs, rt, offset);
				return;

			case MScomp_NOT_EQUALS:
				// Comparison between registers
				// bnec needs rs < rt
				if (rt > rs)
					MS_SWAP_VARS(rs, rt);
				RAW_EMIT_BNEC(e, rs, rt, offset);
				return;
		}
		assert(false);
	}
}

void MS_emit_bcompc_link(MSemitter* e, MScomp comp, MSreg rs, MSreg rt, MSimmediate offset) {
	if (rs == MIPS_R_ZERO && rt == MIPS_R_ZERO) {
		assert(false);
	} else if (rs == MIPS_R_ZERO) {
		MS_SWAP_VARS(rs, rt);
		comp = MS_flip_comp(comp);
	} else if (rt == MIPS_R_ZERO) {
		// No-op, this is the order that we want
	} else /* if (rs == MIPS_R_ZERO || rt == MIPS_R_ZERO) */ {
		// No such instructions for the branch-and-link variants
		assert(false);
	}

	switch (comp) {
		case MScomp_LESSER: RAW_EMIT_BLTZALC(e, rs, offset); return;
		case MScomp_GREATER: RAW_EMIT_BGTZALC(e, rs, offset); return;
		case MScomp_LESSER_EQ: RAW_EMIT_BLEZALC(e, rs, offset); return;
		case MScomp_GREATER_EQ: RAW_EMIT_BGEZALC(e, rs, offset); return;
		case MScomp_EQUALS: RAW_EMIT_BEQZALC(e, rs, offset); return;
		case MScomp_NOT_EQUALS: RAW_EMIT_BNEZALC(e, rs, offset); return;
	}
	assert(false);
}

void MS_emit_beqc(MSemitter* e, MSreg rs, MSreg rt, MSimmediate offset) {
	MS_emit_bcompc(e, MScomp_EQUALS, false, rs, rt, offset);
}

void MS_emit_bnec(MSemitter* e, MSreg rs, MSreg rt, MSimmediate offset) {
	MS_emit_bcompc(e, MScomp_NOT_EQUALS, false, rs, rt, offset);
}
