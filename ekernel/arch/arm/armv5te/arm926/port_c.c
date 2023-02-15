/*
 * =====================================================================================
 *
 *       Filename:  port_c.c
 *
 *    Description:  porinting layer of arm926ej-s processor platform.
 *
 *        Version:  Melis3.0
 *         Create:  2017-11-02 11:58:23
 *       Revision:  none
 *       Compiler:  gcc version 6.3.0 (crosstool-NG crosstool-ng-1.23.0)
 *
 *         Author:  caozilong@allwinnertech.com
 *   Organization:  BU1-PSW
 *  Last Modified:  2018-11-29 14:56:20
 *
 * =====================================================================================
 */

#include <stddef.h>
#include <stdint.h>

/*****************************/
/* CPU Mode                  */
/*****************************/
#define USERMODE        0x10
#define FIQMODE         0x11
#define IRQMODE         0x12
#define SVCMODE         0x13
#define SYSMODE         0x1f
#define ABORTMODE       0x17
#define UNDEFMODE       0x1b
#define MODEMASK        0x1f
#define NOINT           0xc0
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
uint8_t *awos_arch_stack_init(void *tentry, void *parameter,
                              uint8_t *stack_addr, void *texit)
{
    uint32_t *stk;

    //stk      = (uint32_t*)stack_addr;
    stack_addr += sizeof(uint32_t);
    stack_addr = (uint8_t *)ALIGN_DOWN((uint32_t)stack_addr, 8);
    stk        = (uint32_t *)stack_addr;

    *(--stk) = (uint32_t)tentry;            /* entry point */
    *(--stk) = (uint32_t)texit;             /* lr */
    *(--stk) = 0xdeadbeef;                  /* r12 */
    *(--stk) = 0xdeadbeef;                  /* r11 */
    *(--stk) = 0xdeadbeef;                  /* r10 */
    *(--stk) = 0xdeadbeef;                  /* r9 */
    *(--stk) = 0xdeadbeef;                  /* r8 */
    *(--stk) = 0xdeadbeef;                  /* r7 */
    *(--stk) = 0xdeadbeef;                  /* r6 */
    *(--stk) = 0xdeadbeef;                  /* r5 */
    *(--stk) = 0xdeadbeef;                  /* r4 */
    *(--stk) = 0xdeadbeef;                  /* r3 */
    *(--stk) = 0xdeadbeef;                  /* r2 */
    *(--stk) = 0xdeadbeef;                  /* r1 */
    *(--stk) = (uint32_t)parameter;         /* r0 : argument */

    /* cpsr setting, clear N,Z,C,V,I,F. use SVC mode. */
    if ((uint32_t)tentry & 0x01)
    {
        *(--stk) = SVCMODE | 0x20;    /* thumb mode */
    }
    else
    {
        *(--stk) = SVCMODE;           /* arm mode   */
    }

    /*  FCSE did not enabled at present. */
    *(--stk) = 0x0;

    /* return task's current stack address */
    return (uint8_t *)stk;
}

/**
 * This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @return return the index of the first bit set. If value is 0, then this function
 * shall return 0.
 */
int32_t awos_arch_ffs(int32_t value)
{
    register uint32_t x;

    if (value == 0)
    {
        return value;
    }

    asm volatile
    (
        "rsb %[temp], %[val], #0\n"
        "and %[temp], %[temp], %[val]\n"
        "clz %[temp], %[temp]\n"
        "rsb %[temp], %[temp], #32\n"
        :[temp] "=r"(x)
        :[val] "r"(value)
    );
    return x;
}

