	.text
	.file	"example.c"
	.globl	otherfunc
	.align	16, 0x90
	.type	otherfunc,@function
otherfunc:                              # @otherfunc
# BB#0:
	subl	$12, %esp
	movl	$.Lstr, (%esp)
	calll	puts
	addl	$12, %esp
	retl
.Lfunc_end0:
	.size	otherfunc, .Lfunc_end0-otherfunc

	.globl	setfunc
	.align	16, 0x90
	.type	setfunc,@function
setfunc:                                # @setfunc
# BB#0:
	retl
.Lfunc_end1:
	.size	setfunc, .Lfunc_end1-setfunc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
# BB#0:
	subl	$12, %esp
	movl	$.Lstr, (%esp)
	calll	puts
	xorl	%eax, %eax
	addl	$12, %esp
	retl
.Lfunc_end2:
	.size	main, .Lfunc_end2-main

	.type	.Lstr,@object           # @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"RED ALERT!"
	.size	.Lstr, 11


	.ident	"clang version 3.7.0 (tags/RELEASE_370/final)"
	.section	".note.GNU-stack","",@progbits
