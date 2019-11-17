	.text
	.global	_start

_start:
	sub	$4, %esp

again:
	mov	$3, %eax
	mov	$0, %ebx
	mov	%esp, %ecx
	mov	$1, %edx
	int	$0x80

	test	%eax, %eax
	jle	done

	mov	$4, %eax
	mov	$0, %ebx
	mov	%esp, %ecx
	mov	$1, %edx
	int	$0x80
	jmp	again

done:
	mov	$1, %eax
	mov	$42, %ebx
	int	$0x80
