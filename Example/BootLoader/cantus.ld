/******************************************************************************
	Copyright (C) 2013	Advanced Digital Chips Inc. 
						http://www.adc.co.kr
						
	Author : Software Team.
******************************************************************************/

OUTPUT_FORMAT("elf32-ae32000-little", "elf32-ae32000-big",
			"elf32-ae32000-little")

OUTPUT_ARCH(ae32000)

ENTRY(_Reset_Start)
/*GROUP(-lc -lgcc)*/
/*STARTUP(crt0.o)*/
/*SEARCH_DIR(.);*/

SEARCH_DIR(~/local/ae32000-elf/lib);

MEMORY
{
    /* 512KB Flash */
	rom : ORIGIN = 0x00000000, LENGTH = 512K

	ram : ORIGIN = 0x20000000, LENGTH = 80K
}

SECTIONS
{
  .text : {
    *(.vects)
    *(.text)
    *(.text.*)
    . = ALIGN(4);
    /* *(.flash_code) */
    . = ALIGN(4);
    *(.stub)
    *(.rodata)
    *(.rodata.str1.*)
    *(.rodata1)
    ___ctors = . ;
    *(.ctors)
    ___ctors_end = . ;
    ___dtors = . ;
    *(.dtors)
    ___dtors_end = . ;
  } > rom
  _etext = . ;
  PROVIDE (etext = .);

  .data : AT ( (LOADADDR (.text) + SIZEOF(.text) + 3) & ~ (4 - 1)) {
    __ram_data_start = . ;
    /*PROVIDE (___ram_data_start = .);*/
  *(.flash_code)
    *(.data)
    *(.sdata)
    CONSTRUCTORS
    . = ALIGN(4);
    __ram_data_end = .;
    /*PROVIDE (___ram_data_end = .);*/
  } > ram
  __rom_data_start  =  LOADADDR (.data);
  /*PROVIDE (___rom_data_start = .);*/

  .bss ALIGN(0x4) : AT ((LOADADDR (.data) + SIZEOF(.data) + 3) & ~ (4 - 1) ) {
    __bss_start = .;
    /*PROVIDE (__bss_start = .);*/
    *(.dynbss)
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
    /*PROVIDE (___bss_end = .);*/
  } > ram

  _end = . ;
  PROVIDE (end = .);

  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* These must appear regardless of  .  */

  ._stack 0x20013FFC : {
    _stack = .;
    /*PROVIDE (__stack = .);*/
    *(._stack)
  } 
}
PROVIDE(___data_size = SIZEOF(.data));
PROVIDE(___bss_size = SIZEOF(.bss));


