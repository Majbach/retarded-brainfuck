global _start
global _add_ptr
global _sub_ptr
global _add
global _sub
global _display
global _input


section .text

_start:
	;rsi = pointer

	;here goes the initialization code, to be added

_add_ptr:
	add		rsi,0x01

_sub_ptr:
	sub		rsi,0x01

_add:
	add		byte[rsi],0x01

_sub:
	sub		byte[rsi],0x01

_display:
	push	rax
	mov		rax,0x01
	mov		rdi,0x01
	mov		rdx,0x01
	syscall
	pop		rax

_input:
	push	rax
	mov		rax,0x00
	mov		rdi,0x00
	mov		rdx,0x01
	syscall
	pop		rax

_loop_start:
	cmp		byte[rsi],0x00
	je		0x00000004

_loop_end:
	jmp		0x00000004

_init_seq:
	push	rbp
	mov		rbp,rsp

	mov		rbx,rsp
	sub		rsp,0x400
	mov		rax,rsp
	mov		rsi,rsp

_init_l_start:
	cmp		rax,rbx
	je		_init_l_end

	mov		byte[rax],0x00
	add		rax,0x01
	jmp		_init_l_start
_init_l_end:
	nop

_exit_seq:
	mov		rsp,rbp
	pop		rbp
	ret
