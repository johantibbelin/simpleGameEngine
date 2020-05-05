.globl _vbl
.globl _jvbli
_vbl:
	addi.l #1,_vblcount
_jvbl:	jmp 0x000000
