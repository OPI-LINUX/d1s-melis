/*
* =====================================================================================
*
*       Filename:  linker.lds
*
*    Description:  This file describes the memory layout (memory blocks) as virtual
*                  memory addresses.This linker script is passed to the binutils ld
*                  with '-T' options and then to link compiler output into elf run-
*                  able output file.
*                  
*
*        Version:  1.0
*        Created:  2017年08月26日 11时52分57秒
*       Revision:  none
*
*         Author:  czl
*   Organization: AllWinner Technology.
*
* =====================================================================================
*/

MEMORY
{
    /* dram0 area, mmu enabled, 64M size, flat mapping of dram0_0_seg. */
    dram0_0_seg (RWX) :                 org = 0x40000000, len = 0x04000000

    /* modules area, mmu enabled, 256M size. */
    dram0_1_seg (RWX) :                 org = 0xe0000000, len = 0x10000000

    /* desktop mod area, mmu enabled, 64M size, flat mapping of dram0_0_seg. */
    dram0_2_seg (RWX) :                 org = 0xe1f00000, len = 0x00100000

    /* MAGIC section area. just use in loader. */
    magic_info_seg (R) :                org = 0xffff0000, len = 0x00000100
}

INCLUDE configuration.lds

/* must be defined in the code. */
ENTRY(DESKTOP_MInit)

__DRAM0_MOD_BASE = ORIGIN(dram0_2_seg);
__DRAM0_INF_BASE = ORIGIN(magic_info_seg);


SECTIONS
{
    . = __DRAM0_MOD_BASE;
    DESKTOP_API_TBL __DRAM0_MOD_BASE : AT(__DRAM0_MOD_BASE)
    {
        KEEP(*desktop_api.o(DESKTOP_API_TBL))
    } > dram0_2_seg 
    
    .mod.text ADDR(DESKTOP_API_TBL) + SIZEOF(DESKTOP_API_TBL) : AT(LOADADDR(DESKTOP_API_TBL) + SIZEOF(DESKTOP_API_TBL))
    {
        *(.text)
        *(.text.*)
        /* compat. with ARMCC library */
        *(x$fpl$*)
        *(.stub)
        *(.gnu.warning)
        *(.gnu.linkonce.t*)
        . = ALIGN(4);
    } > dram0_2_seg 

    .mod.rodata  ADDR(.mod.text) + SIZEOF(.mod.text) : AT(LOADADDR(.mod.text) + SIZEOF(.mod.text))
    {
        *(.rodata)
        *(.rodata.*)
        *(.const*)
        *(.gnu.linkonce.r*)
        . = ALIGN(4);
    } > dram0_2_seg
    
    .mod.data  ADDR(.mod.rodata) + SIZEOF(.mod.rodata) : AT(LOADADDR(.mod.rodata) + SIZEOF(.mod.rodata))
    {
        *(.data) 
        *(.data.*) 
        *(.gnu.linkonce.d.*)
        *(.sdata) 
        *(.sdata.*) 
        *(.gnu.linkonce.s.*)
        *(.sdata2) 
        *(.sdata2.*) 
        *(.gnu.linkonce.s2.*)
        . = ALIGN(4);
    } > dram0_2_seg
    
    .mod.bss ADDR(.mod.data) + SIZEOF(.mod.data) (NOLOAD) :
    {
        __bss_start = ABSOLUTE(.);
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)
        *(.sbss)
        *(.sbss.*)
        *(.gnu.linkonce.sb.*)
        *(.sbss2)
        *(.sbss2.*)
        *(.gnu.linkonce.sb2.*)
        *(COMMON)
        *(.scommon)
        . = ALIGN(4);
    } > dram0_2_seg

    __bss_end = ABSOLUTE(.);
    _end = ABSOLUTE(.);

    MAGIC (INFO) :
    {
        KEEP(*magic.o(.magic))
    } > magic_info_seg

    .reginfo 0 : { *(.reginfo) }
    .mdebug 0 : { *(.mdebug) }
    .note 0 : { *(.note) }
    .comment 0 : { *(.comment) }
    .ARM.attributes 0 : { *(.ARM.attributes) }
    /DISCARD/ : { *(.note.GNU-stack) *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
}
