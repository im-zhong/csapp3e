	.file	"p-3.16.c"
	.text
	.globl	cond
	.type	cond, @function
cond:
.LFB0:
	.cfi_startproc
	endbr64
	testq	%rsi, %rsi
	je	.L1
	cmpq	%rdi, (%rsi)
	jge	.L1
	movq	%rdi, (%rsi)
.L1:
	ret
	.cfi_endproc
.LFE0:
	.size	cond, .-cond
	.globl	cond_goto
	.type	cond_goto, @function
cond_goto:
.LFB1:
	.cfi_startproc
	endbr64
	testq	%rsi, %rsi
	je	.L3
	cmpq	%rdi, (%rsi)
	jge	.L3
	movq	%rdi, (%rsi)
.L4:
.L3:
	ret
	.cfi_endproc
.LFE1:
	.size	cond_goto, .-cond_goto
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
