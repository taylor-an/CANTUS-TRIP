

	.section .text
	.global _get_cur_sp
	.type _get_cur_sp, @function
	
_get_cur_sp:
    mov %sp, %r8
	jplr
.size _get_cur_sp, .-_get_cur_sp


