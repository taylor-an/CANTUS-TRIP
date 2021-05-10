## -----------------------------------------
## -----------------------------------------
    .section .text
    .global _Reset_Start
	.type _Reset_Start, @function
_Reset_Start:

    ldi     _stack, %r0 /* supervisor stack pointer */
    mov     %r0, %sp

    clr     13 # disable interrupt
    set     12

    ## Flash Memory Access Time Control
    ldi    	0x80000410,  %R6    # Local ROM Control Register
    ldi    	0x00005550,  %R7
    st    	%R7, (%R6,0)


    ## Set Clock
    ldi     0x80020400, %r0 
    ldi     0x00000043, %r1 # USB, MCLK(voice,I2S)
    #ldi	    0x00000003, %r1 #default
    st      %r1, (%r0, 0x24)   # 0x80020424

    ldi     0x00000000, %r1
	st   	%r1, (%r0, 0x4)     # 0x80020404
	
	ldi     0x00000702, %r1 # 25.4016MHz ON, XIN: 11.2896MHz 
    st      %r1, (%r0, 0)
	ldi     0x00010702, %r1 # 25.4016MHz OFF, XIN: 11.2896MHz 
    st      %r1, (%r0, 0)
	ldi     0x00000702, %r1 # 25.4016MHz ON, XIN: 11.2896MHz 
    st      %r1, (%r0, 0)
	
#	ldi     0x00015343, %r1 # 96MHz OFF, XIN: 11.2896MHz 
	ldi     0x00012002, %r1 # 96MHz OFF, XIN: 11.2896MHz 
    st      %r1, (%r0, 0)
	
#	ldi     0x00005343, %r1 # 96MHz ON, XIN: 11.2896MHz 
	ldi     0x00002002, %r1 # 96MHz ON, XIN: 11.2896MHz 
    st      %r1, (%r0, 0)


	ldi		0x4000, %r3
	ldi		0	,%r4
	
.dly:
	addq	1,%r4
	cmp		%r4,%r3
	jnz		.dly
	
    ldi	    0x00000001, %r1 
    st   	%r1, (%r0, 0x4)     # 0x80020404

    ldi     0x00000000, %r1
    st      %r1, (%r0, 0x24)

	## Cache ON/OFF
	jal _CacheInit
##################################################################

    # Initialzie data and bss secton
    .extern __rom_data_start
    .extern __ram_data_start
    .extern __ram_data_end
    .extern __bss_start
    .extern __bss_end
__copy_data:
    ldi  __rom_data_start, %r0
    ldi  __ram_data_start, %r1
    ldi  __ram_data_end, %r2

1:  cmp  %r2, %r1
    jz  __init_bss
    ld    (%r0, 0), %r3
    st    %r3, (%r1, 0)
    addq 4, %r0
    addq 4, %r1
    jmp  1b

__init_bss:
    ldi  __bss_start, %r0
    ldi  __bss_end, %r1
    ldi  0, %r2

2:  cmp  %r1, %r0
    jz   3f
    st    %r2, (%r0, 0)
    addq 4, %r0
    jmp  2b

3:

##################################################################

    jal     _main

4:  jmp     4b

.size _Reset_Start, .-_Reset_Start
.section ._stack
_stack:    .long     1


