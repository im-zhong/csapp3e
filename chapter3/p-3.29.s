	.file	"p-3.29.c"
	.text
	.globl	fun
	.type	fun, @function
fun:
.LFB0:
	.cfi_startproc
	endbr64
	movl	$0, %eax
	movl	$0, %edx
	jmp	.L2
.L3:
	addq	$1, %rax
.L2:
	cmpq	$9, %rax
	jg	.L5
	testb	$1, %al
	jne	.L3
	addq	%rax, %rdx
	jmp	.L3
.L5:
	movq	%rdx, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	fun, .-fun
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
