#define FAULT_DIVISION_ERROR            0x0
#define INSTR_DEBUG                     0x1
#define INT_NMI                         0x2
#define INSTR_BREAK                     0x3
#define TRAP_OVERFLOW                   0x4
#define FAULT_BOUND_EXCEED              0x5
#define FAULT_INVALID_OPCODE            0x6
#define FAULT_NO_MATH_PROCESSOR         0x7
#define ABORT_DOUBLE_FAULT              0x8
#define FAULT_RESERVED_0                0x9
#define FAULT_INVALID_TSS               0x10
#define FAULT_SEG_NOT_PRESENT           0x11
#define FAULT_STACK_SEG_FAULT           0x12
#define FAULT_GENERAL_PROTECTION        0x13
#define FAULT_PAGE_FAULT                0x14
#define FAULT_RESERVED_1                0x15
#define FAULT_X87_FAULT                 0x16
#define FAULT_ALIGNMENT_CHECK           0x17
#define ABORT_MACHINE_CHECK             0x18
#define FAULT_SIMD_FP_EXCEPTION         0x19
#define FAULT_VIRTUALIZATION_EXCEPTION  0x20
#define FAULT_CONTROL_PROTECTION        0x21