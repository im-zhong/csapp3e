	.file	"p-3.24.c"
	.text
	.globl	loop_while
	.type	loop_while, @function
loop_while:
.LFB0:
	.cfi_startproc
	endbr64
	cmpq	%rsi, %rdi
	jge	.L4
	addq	%rsi, %rdi
	addq	%rsi, %rsi
	movl	$1, %eax
.L3:
	imulq	%rdi, %rax
	addq	$1, %rdi
	cmpq	%rsi, %rdi
	jne	.L3
	ret
.L4:
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	loop_while, .-loop_while
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
