#pragma once

#include <common/buffer.h>
#include <common/mips.h>
#include <stdbool.h>

typedef struct MSemitter {
	MSbuf instructions;
} MSemitter;

MSemitter MS_make_emitter();

void MS_emit_add(MSemitter* e, MSreg dst, MSreg op1, MSreg op2);
void MS_emit_addu(MSemitter* e, MSreg dst, MSreg op1, MSreg op2);
void MS_emit_addiu(MSemitter* e, MSreg dst, MSreg op, MSimmediate imm);
void MS_emit_addiupc(MSemitter* e, MSreg dst, MSimmediate imm);
void MS_emit_sub(MSemitter* e, MSreg dst, MSreg op1, MSreg op2);

/* Section: Regular Branches */

void MS_emit_bal(MSemitter* e, MSimmediate offset);
void MS_emit_beq(MSemitter* e, MSreg a, MSreg b, MSimmediate offset);
void MS_emit_bne(MSemitter* e, MSreg a, MSreg b, MSimmediate offset);
void MS_emit_bgez(MSemitter* e, MSreg rs, MSimmediate offset);
void MS_emit_bltz(MSemitter* e, MSreg rs, MSimmediate offset);

/* Section: Compact Branches */

void MS_emit_bc(MSemitter* e, MSimmediate offset);
void MS_emit_balc(MSemitter* e, MSimmediate offset);

typedef enum {
	MScomp_LESSER,
	MScomp_LESSER_EQ,
	MScomp_GREATER,
	MScomp_GREATER_EQ,
	MScomp_EQUALS,
	MScomp_NOT_EQUALS,
} MScomp;

// TODO expose raw emitters?
void MS_emit_bcompc(MSemitter* e, MScomp comp, bool is_unsigned, MSreg rs, MSreg rt, MSimmediate offset);

void MS_emit_blezalc(MSemitter* e, MSreg reg, MSimmediate offset);
void MS_emit_bgezalc(MSemitter* e, MSreg reg, MSimmediate offset);
void MS_emit_bgtzalc(MSemitter* e, MSreg reg, MSimmediate offset);
void MS_emit_bltzalc(MSemitter* e, MSreg reg, MSimmediate offset);

void MS_emit_beqzalc(MSemitter* e, MSreg reg, MSimmediate offset);
void MS_emit_bnezalc(MSemitter* e, MSreg reg, MSimmediate offset);

void MS_emit_bcompc_link(MSemitter* e, MScomp comp, MSreg rs, MSreg rt, MSimmediate offset);

void MS_emit_beqc(MSemitter* e, MSreg rs, MSreg rt, MSimmediate offset);
void MS_emit_bnec(MSemitter* e, MSreg rs, MSreg rt, MSimmediate offset);
