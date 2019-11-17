	.att_syntax	noprefix
	.global	_start
_start:
	mov	$4, eax
	mov	$1, ebx
	mov	$str, ecx
	mov	$estr-str, edx
	int	$0x80

	mov	$1, eax
	mov	$42, ebx
	int	$0x80
str:	.asciz	"Hello\n"
estr:
