.globl _vbl_hbl_init

_vbl_hbl_init:
	move.l 0x70,_svbl
	move.l 0x70,jvbl+2
	move.l _vbl,0x70
	rts

_svbl:	.long 0
	
