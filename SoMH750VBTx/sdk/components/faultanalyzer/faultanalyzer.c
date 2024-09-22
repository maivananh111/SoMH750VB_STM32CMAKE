/*
 * faultanalyzer.c
 *
 *  Created on: Aug 22, 2024
 *      Author: anh
 */

#include "faultanalyzer.h"
#include "stm32h7xx.h"
#include "stdio.h"
#include "stdbool.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "systeminfo.h"


extern const int _sstack;
extern const int _estack;
extern const int _stext;
extern const int _etext;

static __IO uint32_t main_stack_start_addr = 0;
static __IO size_t main_stack_size = 0;
static __IO uint32_t code_start_addr = 0;
static __IO size_t code_size = 0;

static char call_stack_info[CALL_STACK_MAX_DEPTH * (8 + 1)] = {0};
static __IO uint32_t saved_pc = 0, saved_lr = 0;

static __IO bool on_fault = false;
static __IO bool stack_is_overflow = false;
static __IO bool fault_on_thread = false;
static __IO bool stack_has_fpu_regs = false;



static uint32_t stack_del_fpu_regs(uint32_t lr, uint32_t sp) {
    /** The stack has S0~S15 and FPSCR registers when stack_has_fpu_regs is true (in EXEC_RETURN), double word align */
    stack_has_fpu_regs = ((lr & (1UL << 4)) == 0)? true : false;
    return stack_has_fpu_regs == true ? sp + sizeof(size_t) * 18 : sp;
}

static void dump_stack(uint32_t stack_start_addr, size_t stack_size, uint32_t *stack_pointer) {
    uint32_t deep = DUMP_STACK_DEPTH_SIZE/4;

	if (fault_on_thread)
		LOG_FAULT("Thread stack(0x%08x) was overflow", stack_pointer);
	else
		LOG_FAULT("Main stack(0x%08x) was overflow", stack_pointer);

	if ((uint32_t) stack_pointer < stack_start_addr)
		stack_pointer = (uint32_t *) stack_start_addr;
	else if ((uint32_t) stack_pointer > stack_start_addr + stack_size)
		stack_pointer = (uint32_t *) (stack_start_addr + stack_size);

	LOG_BASE("========================= Dump stack ==========================");
    for (; (uint32_t) stack_pointer < stack_start_addr + stack_size && deep; stack_pointer+=4, deep--) {
    	LOG_BASE("addr: 0x%08x-0x%08x    addr: 0x%08x-0x%08x    addr: 0x%08x-0x%08x    addr: 0x%08x-0x%08x",
    			stack_pointer, *stack_pointer, stack_pointer+1, *(stack_pointer+1), stack_pointer+2, *(stack_pointer+2), stack_pointer+3, *(stack_pointer+3));
    }
}

static void dump_registers(uint32_t *regs_addr){
	__IO uint32_t r0  = regs_addr[0];   // Register R0
	__IO uint32_t r1  = regs_addr[1];   // Register R1
	__IO uint32_t r2  = regs_addr[2];   // Register R2
	__IO uint32_t r3  = regs_addr[3];   // Register R3
	__IO uint32_t r12 = regs_addr[4];   // Register R12
	__IO uint32_t lr  = regs_addr[5];   // Link register LR
	__IO uint32_t pc  = regs_addr[6];   // Program counter PC
	__IO uint32_t psr = regs_addr[7];   // Program status word PSR

	saved_pc = pc;
	saved_lr = lr;

    LOG_BASE("======================== Dump Registers ========================");
    LOG_BASE("R0 : 0x%08x  R1: 0x%08x  R2: 0x%08x  R3 : 0x%08x", r0, r1, r2, r3);
    LOG_BASE("R12: 0x%08x  LR: 0x%08x  PC: 0x%08x  PSR: 0x%08x", r12, lr, pc, psr);
}

static void fault_diagnosis(void) {
	__IO uint32_t hfsr = SCB->HFSR;      // Hard Fault Status Register
	__IO uint32_t cfsr = SCB->CFSR;      // Configurable Fault Status Register
	__IO uint32_t mfsr = cfsr;    		 // MemManage Fault Status Register
	__IO uint32_t mfar = SCB->MMFAR;     // MemManage Fault Address Register
	__IO uint32_t bfsr = (cfsr >> 8U);   // Bus Fault Status Register
	__IO uint32_t bfar = SCB->BFAR;      // Bus Fault Address Register
	__IO uint32_t ufsr = (cfsr >> 16U);  // Usage Fault Status Register
	(void)cfsr;


    if (hfsr & (1<<1U))
        LOG_FAULT("Hard fault is caused by failed vector fetch from vector table");

    /* Memory Management Fault */
	if (mfsr) {
		if (mfsr & (1<<0U)) //IACCVIOL
			LOG_FAULT("Memory management fault is caused by instruction access violation");
		if (mfsr & (1<<1U)) //DACCVIOL
			LOG_FAULT("Memory management fault is caused by data access violation");
		if (mfsr & (1<<3U)) //MUNSTKERR
			LOG_FAULT("Memory management fault is caused by unstacking error");
		if (mfsr & (1<<4U)) //MSTKERR
			LOG_FAULT("Memory management fault is caused by stacking error");
		if (mfsr & (1<<5U)) //MLSPERR
			LOG_FAULT("Memory management fault is caused by floating-point lazy state preservation");
		if ((mfsr & (1<<7U)) && ((mfsr & (1<<0U)) || (mfsr & (1<<1U)))) //MMARVALID && (IACCVIOL || DACCVIOL)
			LOG_FAULT("The memory management fault occurred address is 0x%08x", mfar);
	}

	/* Bus Fault */
	if (bfsr) {
		if (bfsr & (1<<0U)) //IBUSERR
			LOG_FAULT("Bus fault is caused by instruction access violation");
		if (bfsr & (1<<1U)) //PRECISERR
			LOG_FAULT("Bus fault is caused by precise data access violation");
		if (bfsr & (1<<2U)) //IMPREISERR
			LOG_FAULT("Bus fault is caused by imprecise data access violation");
		if (bfsr & (1<<3U)) //UNSTKERR
			LOG_FAULT("Bus fault is caused by unstacking error");
		if (bfsr & (1<<4U)) //STKERR
			LOG_FAULT("Bus fault is caused by stacking error");
		if (bfsr & (1<<5U)) //LSPERR
			LOG_FAULT("Bus fault is caused by floating-point lazy state preservation");
		if ((bfsr & (1<<7U)) && (bfsr & (1<<1U))) //BFARVALID && PRECISERR
			LOG_FAULT("The bus fault occurred address is 0x%08x", bfar);
	}

	/* Usage Fault */
	if (ufsr) {
		if (ufsr & (1<<0U)) //UNDEFINSTR
			LOG_FAULT("Usage fault is caused by attempts to execute an undefined instruction");
		if (ufsr & (1<<1U)) //INVSTATE
			LOG_FAULT("Usage fault is caused by attempts to switch to an invalid state (e.g., ARM)");
		if (ufsr & (1<<2U)) //INVPC
			LOG_FAULT("Usage fault is caused by attempts to do an exception with a bad value in the EXC_RETURN number");
		if (ufsr & (1<<3U)) //NOCP
			LOG_FAULT("Usage fault is caused by attempts to execute a coprocessor instruction");
		if (ufsr & (1<<8U)) //UNALIGNED
			LOG_FAULT("Usage fault is caused by indicates that an unaligned access fault has taken place");
		if (ufsr & (1<<9U)) //DIVBYZERO0
			LOG_FAULT("Usage fault is caused by Indicates a divide by zero has taken place");
	}

	/* Force Hard Fault */
    if (hfsr & (1<<30U))
    	LOG_FAULT("Force hard fault");
}

static bool disassembly_ins_is_bl_blx(uint32_t addr) {
    uint16_t ins1 = *((uint16_t *)addr);
    uint16_t ins2 = *((uint16_t *)(addr + 2));

#define BL_INS_MASK         0xF800
#define BL_INS_HIGH         0xF800
#define BL_INS_LOW          0xF000
#define BLX_INX_MASK        0xFF00
#define BLX_INX             0x4700

	if ((ins2 & BL_INS_MASK) == BL_INS_HIGH && (ins1 & BL_INS_MASK) == BL_INS_LOW)
		return true;
	else if ((ins2 & BLX_INX_MASK) == BLX_INX)
		return true;
	else
		return false;
}

static size_t backtrace_call_stack(uint32_t *buffer, size_t size, uint32_t sp, uint32_t stack_start_addr, uint32_t stack_size) {
    uint32_t pc;
    size_t depth = 0;
    bool regs_saved_lr_is_valid = false;

	if (!stack_is_overflow) {
		/* first depth is PC */
		buffer[depth++] = saved_pc;
		/* fix the LR address in thumb mode */
		pc = saved_lr - 1;
		if ((pc >= code_start_addr) && (pc <= code_start_addr + code_size) && (depth < CALL_STACK_MAX_DEPTH) && (depth < size)) {
			buffer[depth++] = pc;
			regs_saved_lr_is_valid = true;
		}
	}

	/* program is running on thread before fault */
	if (!fault_on_thread) {
		stack_start_addr = main_stack_start_addr;
		stack_size = main_stack_size;
	}

    /* copy called function address */
    for (; sp < stack_start_addr + stack_size; sp += sizeof(size_t)) {
        /* the *sp value may be LR, so need decrease a word to PC */
        pc = *((uint32_t *) sp) - sizeof(size_t);
        /* the Cortex-M using thumb instruction, so the pc must be an odd number */
        if (pc % 2 == 0) {
            continue;
        }
        /* fix the PC address in thumb mode */
        pc = *((uint32_t *) sp) - 1;
        if ((pc >= code_start_addr + sizeof(size_t)) && (pc <= code_start_addr + code_size) && (depth < CALL_STACK_MAX_DEPTH)
                /* check the the instruction before PC address is 'BL' or 'BLX' */
                && disassembly_ins_is_bl_blx(pc - sizeof(size_t)) && (depth < size)) {
            /* the second depth function may be already saved, so need ignore repeat */
            if ((depth == 2) && regs_saved_lr_is_valid && (pc == buffer[1])) {
                continue;
            }
            buffer[depth++] = pc;
        }
    }

    return depth;
}

static void print_call_stack(uint32_t sp, uint32_t stack_start_addr, uint32_t stack_size) {
    size_t i, cur_depth = 0;
    uint32_t call_stack_buf[CALL_STACK_MAX_DEPTH] = {0};

    cur_depth = backtrace_call_stack(call_stack_buf, CALL_STACK_MAX_DEPTH, sp, stack_start_addr, stack_size);

    for (i = 0; i < cur_depth; i++) {
        sprintf(call_stack_info + i * (8 + 1), "%08lx", (unsigned long)call_stack_buf[i]);
        call_stack_info[i * (8 + 1) + 8] = ' ';
    }

    if (cur_depth)
    	LOG_FAULT("Show more call stack info by run: addr2line -e %s%s -afpiC %.*s", get_build_target(), ".elf", cur_depth * (8 + 1), call_stack_info);
    else
    	LOG_FAULT("Dump call stack has an error");
}








void fault_analyzer_init(void){
	SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;


    main_stack_start_addr = (uint32_t)(&_sstack);
    main_stack_size       = (uint32_t)(&_estack) - main_stack_start_addr;
    code_start_addr       = (uint32_t)(&_stext);
    code_size             = (uint32_t)(&_etext) - code_start_addr;
}


void fault_analyze(uint32_t lr, uint32_t sp){
	uint32_t stack_pointer = sp, saved_regs_addr = stack_pointer;
    uint32_t stack_start_addr = main_stack_start_addr;
    size_t stack_size = main_stack_size;

    if(!on_fault){
		on_fault = true;

		/** Fault on thread mode or handler mode */
		/** Check EXEC_RETURN value in LR register */
		fault_on_thread = lr & (1UL << 2);

		if (fault_on_thread) {
			TaskHandle_t current_task;
			TaskStatus_t current_task_info;

			/** Get curent thread task info */
			current_task = xTaskGetCurrentTaskHandle();
			vTaskGetInfo(current_task, &current_task_info, pdTRUE, eInvalid);
			LOG_FAULT("Fault on thread mode [%s]", (current_task_info.pcTaskName != NULL)? current_task_info.pcTaskName : "Unknown");

			/** Get curent thread stack info */
			stack_start_addr = (uint32_t)current_task_info.pxStackBase;
			stack_size = (uint32_t)current_task_info.usStackDepth * sizeof( StackType_t );
		}
		else
			LOG_FAULT("Fault on handler mode");

		/** Ignore Core register */
		stack_pointer += sizeof(size_t) * 8;
		/** Ignore FPU register if bit-4 is reset */
		stack_pointer = stack_del_fpu_regs(lr, stack_pointer);
		LOG_FAULT("Stack info - pointer: 0x%08x   start: 0x%08x   end: 0x%08x", stack_pointer, stack_start_addr, stack_start_addr + stack_size);
		/** Dump stack */
		if (stack_pointer < stack_start_addr || stack_pointer > stack_start_addr + stack_size) {
			stack_is_overflow = true;
			dump_stack(stack_start_addr, stack_size, (uint32_t *) stack_pointer);
		}

		dump_registers((uint32_t *)saved_regs_addr);
		fault_diagnosis();
		print_call_stack(stack_pointer, stack_start_addr, stack_size);
    }

    for(uint64_t i = 0; i < 120000000; i++){
        __asm__("nop");
    }
    while(1);
//    NVIC_SystemReset();
}




