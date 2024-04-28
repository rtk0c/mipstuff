#pragma once

#define MIPS_R_ZERO 0

// Assembler Temporary
#define MIPS_R_AT 1

// Values for Function Results
#define MIPS_R_V0 2
#define MIPS_R_V1 3

// Arguments
#define MIPS_R_A0 4
#define MIPS_R_A1 5
#define MIPS_R_A2 6
#define MIPS_R_A3 7
#define MIPS_R_AN(n) (MIPS_R_A0 + (n))

// Temporaries
#define MIPS_R_T0 8
#define MIPS_R_T1 9
#define MIPS_R_T2 10
#define MIPS_R_T3 11
#define MIPS_R_T4 12
#define MIPS_R_T5 13
#define MIPS_R_T6 14
#define MIPS_R_T7 15
#define MIPS_R_T8 24
#define MIPS_R_T9 25
#define MIPS_R_TN(n) ((n) <= 7 ? MIPS_R_T0 + (n) : MIPS_R_T8 + ((n) - 8))

// Saved Temporaries
#define MIPS_R_S0 16
#define MIPS_R_S1 17
#define MIPS_R_S2 18
#define MIPS_R_S3 19
#define MIPS_R_S4 20
#define MIPS_R_S5 21
#define MIPS_R_S6 22
#define MIPS_R_S7 23
#define MIPS_R_SN(n) (MIPS_R_S0 + (n))

// Reserved for OS Kernel
#define MIPS_R_K0 26
#define MIPS_R_K1 27
#define MIPS_R_KN(n) (MIPS_R_K0 + (n))

#define MIPS_R_GP 28 //< Global Pointer
#define MIPS_R_SP 29 //< Stack Pointer
#define MIPS_R_FP 30 //< Frame Pointer
#define MIPS_R_RA 31 //< Return Address

typedef int MSreg;
typedef int MSimmediate;
