	.file	"p-3.17.c"
	.text
	.globl	goto_diff_se
	.type	goto_diff_se, @function
goto_diff_se:
.LFB0:
	.cfi_startproc
	endbr64
	movq	%rsi, %rax
	cmpq	%rsi, %rdi
	jge	.L2
	addq	$1, lt_cnt(%rip)
	subq	%rdi, %rax
	ret
.L2:
	addq	$1, ge_cnt(%rip)
	subq	%rsi, %rdi
	movq	%rdi, %rax
.L3:
	endbr64
	ret
	.cfi_endproc
.LFE0:
	.size	goto_diff_se, .-goto_diff_se
	.globl	ge_cnt
	.bss
	.align 8
	.type	ge_cnt, @object
	.size	ge_cnt, 8
ge_cnt:
	.zero	8
	.globl	lt_cnt
	.align 8
	.type	lt_cnt, @object
	.size	lt_cnt, 8
lt_cnt:
	.zero	8
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
