/*
 * FAME - Fault Aware Micro-hypervisor Environment
 *
 * Author: Andreas Heinig <andreas.heinig@gmx.de>
 *
 * Copyright (C) 2011,2012 Department of Computer Science,
 * Design Automation of Embedded Systems Group
 * Dortmund University of Technology
 *
 * This program is proprietary software: you must not redistribute it.
 * Using this software is only allowed inside the DFG SPP1500 F.E.H.L.E.R project,
 * ls12-www.cs.tu-dortmund.de/daes/forschung/dependable-embedded-real-time-systems
 *
 * The complete license is depicted in the LICENSE file in the top level folder.
 */

#ifndef ARM_OPCODE_H__INCLUDED__
#define ARM_OPCODE_H__INCLUDED__

#include <stdint.h>

typedef struct {
  uint32_t r[16];  // registers of current mode only!
  uint32_t cpsr;
  uint32_t spsr;
} arm_regs_t;


extern int verbose_opcode;
extern char opcode_string[128];
extern char * opcode_string_tmp;

#define OP_PRINTF(fmt, arg...) { if(verbose_opcode) printf(fmt, ##arg); }
//#define OP_PRINTF(fmt, arg...) printf(fmt, ##arg)
//#define OP_PRINTF(fmt, arg...) opcode_string_tmp += sprintf(opcode_string_tmp, fmt, ##arg)

#define BIT_IS_SET(bit)		  (inst & (1 << bit))
#define BIT_IS_CLEAR(bit)	(!(inst & (1 << bit)))

#define ARM_MODE_MASK	0x1F

#define	ARM_MODE_SVC	0x13
#define ARM_MODE_UND	0x1B
#define ARM_MODE_ABT	0x17
#define ARM_MODE_IRQ	0x12
#define ARM_MODE_FIQ	0x11
#define ARM_MODE_USR	0x10
#define ARM_MODE_SYS	0x1F

#define IS_OP_FIQ(op) ((((op)->regs->cpsr) & ARM_MODE_MASK) == ARM_MODE_FIQ)


#define I_BIT		(1 << 25)
#define P_BIT		(1 << 24)
#define U_BIT		(1 << 23)
#define B_BIT		(1 << 22)
#define S_BIT		(1 << 22)
#define ALU_S_BIT	(1 << 20)
#define W_BIT		(1 << 21)
#define L_BIT		(1 << 20)

#define I_SET		(inst & I_BIT)
#define P_SET		(inst & P_BIT)
#define U_SET		(inst & U_BIT)
#define B_SET		(inst & B_BIT)
#define S_SET		(inst & S_BIT)
#define ALU_S_SET	(inst & ALU_S_BIT)
#define W_SET		(inst & W_BIT)
#define _L_SET		(inst & L_BIT)


#define OP_FLAG_WRITE		0x00000001
#define OP_FLAG_READ		0x00000002

#define OP_FLAG_CONDITION_FALSE	0x80000000

typedef struct {
	uint32_t inst;
	arm_regs_t * regs;
	uint32_t flags;
	uint32_t regs_r;
	uint32_t regs_r_fiq;
	uint32_t regs_w;
	uint32_t regs_w_fiq;
	uint32_t mem_addr;
	uint32_t mem_size;
}arm_instruction_t;

uint32_t op_cond_test(uint32_t inst, uint32_t cpsr);

uint32_t decode_addressing_mode1(arm_instruction_t * op);
void decode_addressing_mode2(arm_instruction_t * op);
void decode_addressing_mode3(arm_instruction_t * op);
void decode_addressing_mode5(arm_instruction_t * op);

int decode_data_processing(arm_instruction_t * op);
int decode_load_store(arm_instruction_t * op);
int decode_load_store_multiple_and_branch(arm_instruction_t * op);
int decode_coprocessor(arm_instruction_t * op);

int decode_instruction(uint32_t inst, arm_regs_t * regs, arm_instruction_t * op);
void dump_instruction(const arm_instruction_t * op);

static inline void arm_op_add_reg_r(arm_instruction_t * op, uint32_t r)
{
	if(r >= 8 && IS_OP_FIQ(op))
		op->regs_r_fiq |= (1 << r);
	else
		op->regs_r |= (1 << r);
}

static inline void arm_op_add_reg_w(arm_instruction_t * op, uint32_t r)
{
	if(r >= 8 && r != 15 && IS_OP_FIQ(op))
		op->regs_w_fiq |= (1 << r);
	else
		op->regs_w |= (1 << r);
}

#endif /* ARM_OPCODE_H__INCLUDED__ */
