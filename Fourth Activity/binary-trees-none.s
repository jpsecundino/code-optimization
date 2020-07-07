	.file	"binary-trees.c"
	.text
	.type	create_Tree, @function
create_Tree:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$16, %esi
	movq	%rax, %rdi
	call	apr_palloc@PLT
	movl	$16, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -24(%rbp)
	jle	.L2
	movq	-24(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	create_Tree
	movq	-8(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-24(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	create_Tree
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
.L2:
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	create_Tree, .-create_Tree
	.type	compute_Tree_Checksum, @function
compute_Tree_Checksum:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L5
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L5
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	compute_Tree_Checksum
	movq	%rax, %rbx
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	compute_Tree_Checksum
	addq	%rbx, %rax
	addq	$1, %rax
	jmp	.L6
.L5:
	movl	$1, %eax
.L6:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	compute_Tree_Checksum, .-compute_Tree_Checksum
	.section	.rodata
	.align 8
.LC0:
	.string	"stretch tree of depth %jd\t check: %jd\n"
	.align 8
.LC1:
	.string	"%jd\t trees of depth %jd\t check: %jd\n"
	.align 8
.LC2:
	.string	"long lived tree of depth %jd\t check: %jd\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$144, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 12, -40
	.cfi_offset 3, -48
	movl	%edi, -148(%rbp)
	movq	%rsi, -160(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -40(%rbp)
	xorl	%eax, %eax
	movq	%rsp, %rax
	movq	%rax, %r12
	movq	$4, -96(%rbp)
	movq	-96(%rbp), %rax
	leaq	1(%rax), %rbx
	movq	-160(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	cltq
	cmpq	%rax, %rbx
	jl	.L8
	movq	-96(%rbp), %rax
	addq	$2, %rax
	jmp	.L9
.L8:
	movq	-160(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	cltq
.L9:
	movq	%rax, -88(%rbp)
	call	apr_initialize@PLT
	leaq	-144(%rbp), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	apr_pool_create_unmanaged_ex@PLT
	movq	-144(%rbp), %rax
	movq	-88(%rbp), %rdx
	addq	$1, %rdx
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	create_Tree
	movq	%rax, -80(%rbp)
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	compute_Tree_Checksum
	movq	%rax, %rdx
	movq	-88(%rbp), %rax
	addq	$1, %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-144(%rbp), %rax
	movq	%rax, %rdi
	call	apr_pool_clear@PLT
	movq	-88(%rbp), %rax
	subq	-96(%rbp), %rax
	addq	$2, %rax
	movq	%rax, %rdx
	shrq	$63, %rdx
	addq	%rdx, %rax
	sarq	%rax
	subq	$1, %rax
	movq	%rax, -72(%rbp)
	movq	-88(%rbp), %rax
	subq	-96(%rbp), %rax
	addq	$2, %rax
	movq	%rax, %rdx
	shrq	$63, %rdx
	addq	%rdx, %rax
	sarq	%rax
	movq	%rax, -176(%rbp)
	movq	$0, -168(%rbp)
	movq	-88(%rbp), %rax
	subq	-96(%rbp), %rax
	addq	$2, %rax
	movq	%rax, %rdx
	shrq	$63, %rdx
	addq	%rdx, %rax
	sarq	%rax
	movq	%rax, %r14
	movl	$0, %r15d
	movq	-88(%rbp), %rax
	subq	-96(%rbp), %rax
	addq	$2, %rax
	movq	%rax, %rdx
	shrq	$63, %rdx
	addq	%rdx, %rax
	sarq	%rax
	leaq	0(,%rax,8), %rdx
	movl	$16, %eax
	subq	$1, %rax
	addq	%rdx, %rax
	movl	$16, %ebx
	movl	$0, %edx
	divq	%rbx
	imulq	$16, %rax, %rax
	movq	%rax, %rdx
	andq	$-4096, %rdx
	movq	%rsp, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
.L10:
	cmpq	%rdx, %rsp
	je	.L11
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L10
.L11:
	movq	%rax, %rdx
	andl	$4095, %edx
	subq	%rdx, %rsp
	movq	%rax, %rdx
	andl	$4095, %edx
	testq	%rdx, %rdx
	je	.L12
	andl	$4095, %eax
	subq	$8, %rax
	addq	%rsp, %rax
	orq	$0, (%rax)
.L12:
	movq	%rsp, %rax
	addq	$7, %rax
	shrq	$3, %rax
	salq	$3, %rax
	movq	%rax, -64(%rbp)
	movq	-144(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	create_Tree
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	compute_Tree_Checksum
	movq	%rax, -48(%rbp)
	leaq	-136(%rbp), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	apr_pool_create_unmanaged_ex@PLT
	movq	-96(%rbp), %rax
	movq	%rax, -104(%rbp)
	jmp	.L13
.L16:
	movq	$0, -112(%rbp)
	movq	-88(%rbp), %rax
	movl	%eax, %edx
	movq	-104(%rbp), %rax
	subl	%eax, %edx
	movq	-96(%rbp), %rax
	addl	%edx, %eax
	movl	$1, %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	cltq
	movq	%rax, -120(%rbp)
	jmp	.L14
.L15:
	movq	-136(%rbp), %rax
	movq	%rax, %rdi
	call	apr_pool_clear@PLT
	movq	-136(%rbp), %rdx
	movq	-104(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	create_Tree
	movq	%rax, %rdi
	call	compute_Tree_Checksum
	addq	%rax, -112(%rbp)
.L14:
	movq	-120(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	%rdx, -120(%rbp)
	testq	%rax, %rax
	jg	.L15
	movq	-104(%rbp), %rax
	subq	-96(%rbp), %rax
	movq	%rax, %rdx
	shrq	$63, %rdx
	addq	%rdx, %rax
	sarq	%rax
	movq	%rax, %rcx
	movq	-64(%rbp), %rax
	movq	-112(%rbp), %rdx
	movq	%rdx, (%rax,%rcx,8)
	addq	$2, -104(%rbp)
.L13:
	movq	-104(%rbp), %rax
	cmpq	-88(%rbp), %rax
	jle	.L16
	movq	-136(%rbp), %rax
	movq	%rax, %rdi
	call	apr_pool_destroy@PLT
	movq	-96(%rbp), %rax
	movq	%rax, -128(%rbp)
	jmp	.L17
.L18:
	movq	-128(%rbp), %rax
	subq	-96(%rbp), %rax
	movq	%rax, %rdx
	shrq	$63, %rdx
	addq	%rdx, %rax
	sarq	%rax
	movq	%rax, %rdx
	movq	-64(%rbp), %rax
	movq	(%rax,%rdx,8), %rdx
	movq	-88(%rbp), %rax
	movl	%eax, %ecx
	movq	-128(%rbp), %rax
	subl	%eax, %ecx
	movq	-96(%rbp), %rax
	addl	%ecx, %eax
	movl	$1, %esi
	movl	%eax, %ecx
	salq	%cl, %rsi
	movq	-128(%rbp), %rax
	movq	%rdx, %rcx
	movq	%rax, %rdx
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$2, -128(%rbp)
.L17:
	movq	-128(%rbp), %rax
	cmpq	-88(%rbp), %rax
	jle	.L18
	movq	-48(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-144(%rbp), %rax
	movq	%rax, %rdi
	call	apr_pool_destroy@PLT
	call	apr_terminate@PLT
	movl	$0, %eax
	movq	%r12, %rsp
	movq	-40(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L20
	call	__stack_chk_fail@PLT
.L20:
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
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
