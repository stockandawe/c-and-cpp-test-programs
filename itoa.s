	.file	"itoa.c"
.globl value
	.data
	.align 4
	.type	value, @object
	.size	value, 4
value:
	.long	1234
	.text
.globl reverse
	.type	reverse, @function
reverse:
.LFB2:
	pushq	%rbp
.LCFI0:
	movq	%rsp, %rbp
.LCFI1:
	movq	%rdi, -24(%rbp)
	movl	$0, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	$-1, %rcx
	movq	%rax, -32(%rbp)
	movl	$0, %eax
	cld
	movq	-32(%rbp), %rdi
	repnz
	scasb
	movq	%rcx, %rax
	notq	%rax
	decq	%rax
	decl	%eax
	movl	%eax, -4(%rbp)
	jmp	.L2
.L3:
	movl	-8(%rbp), %eax
	cltq
	addq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al,%eax
	movl	%eax, -12(%rbp)
	movl	-8(%rbp), %eax
	cltq
	movq	%rax, %rdx
	addq	-24(%rbp), %rdx
	movl	-4(%rbp), %eax
	cltq
	addq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	movl	-4(%rbp), %eax
	cltq
	movq	%rax, %rdx
	addq	-24(%rbp), %rdx
	movl	-12(%rbp), %eax
	movb	%al, (%rdx)
	leaq	-8(%rbp), %rax
	incl	(%rax)
	leaq	-4(%rbp), %rax
	decl	(%rax)
.L2:
	movl	-8(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jl	.L3
	leave
	ret
.LFE2:
	.size	reverse, .-reverse
.globl my_itoa
	.type	my_itoa, @function
my_itoa:
.LFB3:
	pushq	%rbp
.LCFI2:
	movq	%rsp, %rbp
.LCFI3:
	pushq	%rbx
.LCFI4:
	movl	%edi, -28(%rbp)
	movq	%rsi, -40(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L7
.L8:
	movl	-12(%rbp), %eax
	cltq
	movq	%rax, %rsi
	addq	-40(%rbp), %rsi
	movl	-28(%rbp), %ecx
	movl	$1717986919, -52(%rbp)
	movl	-52(%rbp), %eax
	imull	%ecx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ebx
	subl	%eax, %ebx
	movl	%ebx, -44(%rbp)
	movl	-44(%rbp), %eax
	addl	%eax, %eax
	leal	0(,%rax,4), %edx
	addl	%edx, %eax
	movl	%ecx, %edx
	subl	%eax, %edx
	movl	%edx, -44(%rbp)
	movzbl	-44(%rbp), %eax
	addl	$48, %eax
	movb	%al, (%rsi)
	leaq	-12(%rbp), %rax
	incl	(%rax)
	movl	-28(%rbp), %ecx
	movl	$1717986919, -52(%rbp)
	movl	-52(%rbp), %eax
	imull	%ecx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -28(%rbp)
.L7:
	cmpl	$0, -28(%rbp)
	jne	.L8
	movl	-12(%rbp), %eax
	cltq
	addq	-40(%rbp), %rax
	movb	$0, (%rax)
	movq	-40(%rbp), %rax
	popq	%rbx
	leave
	ret
.LFE3:
	.size	my_itoa, .-my_itoa
	.section	.rodata
.LC0:
	.string	"Reversed string is %s\n"
.LC1:
	.string	"Desired string is %s\n"
	.text
.globl main
	.type	main, @function
main:
.LFB4:
	pushq	%rbp
.LCFI5:
	movq	%rsp, %rbp
.LCFI6:
	subq	$16, %rsp
.LCFI7:
	movl	value(%rip), %edi
	leaq	-16(%rbp), %rsi
	call	my_itoa
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leaq	-16(%rbp), %rdi
	call	reverse
	leaq	-16(%rbp), %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %eax
	leave
	ret
.LFE4:
	.size	main, .-main
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.byte	0x4
	.long	.LCFI0-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.byte	0x4
	.long	.LCFI2-.LFB3
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.quad	.LFB4
	.quad	.LFE4-.LFB4
	.byte	0x4
	.long	.LCFI5-.LFB4
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE5:
	.ident	"GCC: (GNU) 4.0.0 20050519 (Red Hat 4.0.0-8)"
	.section	.note.GNU-stack,"",@progbits
