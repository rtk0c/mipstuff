#include "emit.h"

#include <assert.h>
#include <common/common.h>

MSemitter MS_make_emitter() {
	MSemitter res;
	res.instructions = MS_make_buf(MS_KB(4));
	return res;
}

#define MASK(n) (1 << (n))

#define PACK_BEGIN uint32_t _pack_val = 0; int _write_nth_bit = 0
// TODO sanity check that v does not have more than `bits` bits
#define PACK_BITS(v, bits) _pack_val |= (((uint32_t)(v) & MASK(bits)) << _write_nth_bit); _write_nth_bit += bits

#define PACK_R_TYPE(opcode, rd, funct3, rs1, rs2, funct7) \
	PACK_BITS(opcode, 7); \
	PACK_BITS(rd, 5); \
	PACK_BITS(funct3, 3); \
	PACK_BITS(rs1, 5); \
	PACK_BITS(rs2, 5); \
	PACK_BITS(funct7, 7)

#define PACK_I_TYPE(opcode, rd, funct3, rs1, imm) \
	PACK_BITS(opcode, 7); \
	PACK_BITS(rd, 5); \
	PACK_BITS(funct3, 3); \
	PACK_BITS(rs1, 5); \
	PACK_BITS(imm, 12) /* Produces bits 0..=11 in the resulting value */

#define PACK_S_TYPE(opcode, funct3, rs1, rs2, imm) \
	PACK_BITS(opcode, 7); \
	PACK_BITS(imm, 5); /* Produces bits 0..=4 */ \
	PACK_BITS(funct3, 3); \
	PACK_BITS(rs1, 5); \
	PACK_BITS(rs2, 5); \
	PACK_BITS(imm >> 7, 7) /* Produces bits 5..=11 */

#define PACK_U_TYPE(opcode, rd, imm) \
	PACK_BITS(opcode, 7); \
	PACK_BITS(rd, 5); \
	PACK_BITS(imm, 20) /* Produces bits 12..=31 */

#if defined(__GNUC__) || defined(__clang__)
#	define PACK_GET_VALUE ({ assert(_write_nth_bit == 0); _pack_val; })
#else
#	define PACK_GET_VALUE _pack_val
#endif
