	.text

############################################################
## CSO1 Spring 2023 - Homework 5 
## 
## Computing ID: sub5gd
## 
## You must update your ID above to receive credit.  Note
## that this is an individual assignment and you may NOT
## use a compiler.
############################################################                         

############################################################
##   modulo routine (calculates mod using add/subtract)   ##
############################################################

.globl  modulo
modulo:
        cmpq    $0, %rsi         # if y equals 0, return 0
        je      return_zero

        cmpq    $0, %rdi         # if x equals 0, return 0
        je      return_zero

loop:
        cmpq    %rsi, %rdi       # if x is less than  y, exit loop
        jl      done
        subq    %rsi, %rdi       # x -= y
        jmp     loop

done:
        movq    %rdi, %rax       # return x
        retq

return_zero:
        xorq    %rax, %rax       # return 0
        retq
#############################################################
##                 end of modulo routine                  ##
############################################################



############################################################
##            gcd routine (gcd using modulo)              ##
############################################################


.globl  gcd

gcd:
        cmpq    %rdi, %rsi       # if x equals y, return y
        je      return_y

        cmpq    $0, %rsi         # if y equals 0, return x
        je      return_x

        pushq   %rdi             # Save x
        pushq   %rsi             # Save y

        call    modulo           # Compute x % y

        popq    %rsi             # Restore y
        popq    %rdi             # Restore x

        movq    %rsi, %rdi       # New x = old y
        movq    %rax, %rsi       # New y = old x % y

        call    gcd              # Recursively call gcd
        retq

return_y:
        movq    %rsi, %rax
        retq

return_x:
        movq    %rdi, %rax
        retq
############################################################
##                 end of gcd routine                     ##
############################################################



############################################################
##           prime routine (prime using gcd)              ##
############################################################


        .globl  prime
prime:
        cmpq    $2, %rdi         # If n is less than 2, it's not prime
        jl      not_prime

        movq    $2, %rdx         # Set i = 2 (starting divisor)

check_loop:
        cmpq    %rdi, %rdx       # If i equals n, it's prime
        jge     is_prime

        movq    %rdx, %rsi       # Set y = i
        pushq   %rdi             # Save n
        call    gcd              # gcd(n, i)
        popq    %rdi             # Restore n

        cmpq    $1, %rax         # If gcd(n, i) != 1, not prime
        jne     not_prime

        incq    %rdx             # increment i
        jmp     check_loop

not_prime:
        xorq    %rax, %rax       # Return 0 (not prime)
        retq

is_prime:
        movq    $1, %rax         # Return 1 (prime)
        retq
############################################################
##                end of prime routine                    ##
############################################################




############################################################
	.globl	printNum
printNum:
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$24, %rsp
	movq	%fs:40, %rax
	movq	%rax, 16(%rsp)
	movb	$48, 15(%rsp)
	testq	%rdi, %rdi
	je	.LBB0_9
	movq	%rdi, %r14
	movabsq	$1000000000000000, %rbx
	cmpq	%rbx, %rdi
	jae	.LBB0_5
	movabsq	$-3689348814741910323, %rsi
.LBB0_3:
	movq	%rbx, %rcx
	movq	%rbx, %rax
	mulq	%rsi
	movq	%rdx, %rbx
	shrq	$3, %rbx
	cmpq	%r14, %rbx
	ja	.LBB0_3
	cmpq	$10, %rcx
	jb	.LBB0_7
.LBB0_5:
	movabsq	$-3689348814741910323, %r12
	leaq	15(%rsp), %r15
.LBB0_6:
	xorl	%edx, %edx
	movq	%r14, %rax
	divq	%rbx
	movq	%rax, %rcx
	mulq	%r12
	shrl	$2, %edx
	andl	$-2, %edx
	leal	(%rdx,%rdx,4), %eax
	subl	%eax, %ecx
	orb	$48, %cl
	movb	%cl, 15(%rsp)
	movl	$1, %edi
	movl	$1, %edx
	movq	%r15, %rsi
	callq	write@PLT
	movq	%rbx, %rax
	mulq	%r12
	shrq	$3, %rdx
	cmpq	$9, %rbx
	movq	%rdx, %rbx
	ja	.LBB0_6
	jmp	.LBB0_7
.LBB0_9:
	leaq	15(%rsp), %rsi
	movl	$1, %edi
	movl	$1, %edx
	callq	write@PLT
.LBB0_7:
	leaq	15(%rsp), %rsi
	movb	$10, (%rsi)
	movl	$1, %edi
	movl	$1, %edx
	callq	write@PLT
	movq	%fs:40, %rax
	cmpq	16(%rsp), %rax
	jne	.LBB0_8
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	retq
.LBB0_8:
	callq	__stack_chk_fail@PLT
############################################################




############################################################
	.globl	main
main:
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	leaq	.L.str(%rip), %rsi
	movl	$1, %edi
	movl	$9, %edx
	callq	write@PLT
	leaq	7(%rsp), %rbx
	movb	$48, (%rbx)
	movb	$48, %al
	xorl	%r14d, %r14d
.LBB1_1:
	movsbq	%al, %rax
	leaq	(%r14,%r14,4), %rcx
	leaq	(%rax,%rcx,2), %r14
	addq	$-48, %r14
	xorl	%edi, %edi
	movl	$1, %edx
	movq	%rbx, %rsi
	callq	read@PLT
	movb	7(%rsp), %al
	cmpb	$10, %al
	jne	.LBB1_1
	leaq	.L.str.1(%rip), %rsi
	xorl	%ebx, %ebx
	xorl	%edi, %edi
	movl	$9, %edx
	callq	write@PLT
	leaq	7(%rsp), %r15
	movb	$48, (%r15)
	movb	$48, %al
.LBB1_3:
	movsbq	%al, %rax
	leaq	(%rbx,%rbx,4), %rcx
	leaq	(%rax,%rcx,2), %rbx
	addq	$-48, %rbx
	xorl	%edi, %edi
	movl	$1, %edx
	movq	%r15, %rsi
	callq	read@PLT
	movb	7(%rsp), %al
	cmpb	$10, %al
	jne	.LBB1_3
	movq	%r14, %rdi
	movq	%rbx, %rsi
	callq	modulo@PLT
	movq	%rax, %r15
	leaq	.L.str.2(%rip), %rsi
	movl	$1, %edi
	movl	$8, %edx
	callq	write@PLT
	movq	%r15, %rdi
	callq	printNum
	movq	%r14, %rdi
	movq	%rbx, %rsi
	callq	gcd@PLT
	movq	%rax, %r15
	leaq	.L.str.3(%rip), %rsi
	movl	$1, %edi
	movl	$12, %edx
	callq	write@PLT
	movq	%r15, %rdi
	callq	printNum
	movq	%r14, %rdi
	callq	prime@PLT
	movq	%rax, %r15
	leaq	.L.str.4(%rip), %rsi
	movl	$1, %edi
	movl	$10, %edx
	callq	write@PLT
	movq	%r15, %rdi
	callq	printNum
	movq	%rbx, %rdi
	callq	prime@PLT
	movq	%rax, %rbx
	leaq	.L.str.5(%rip), %rsi
	movl	$1, %edi
	movl	$10, %edx
	callq	write@PLT
	movq	%rbx, %rdi
	callq	printNum
	movq	%fs:40, %rax
	cmpq	8(%rsp), %rax
	jne	.LBB1_6
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	retq
.LBB1_6:
	callq	__stack_chk_fail@PLT
############################################################




############################################################
	.section	.rodata
.L.str:
	.asciz	"Enter x: "
.L.str.1:
	.asciz	"Enter y: "
.L.str.2:
	.asciz	"x % y = "
.L.str.3:
	.asciz	"gcd(x, y) = "
.L.str.4:
	.asciz	"x prime = "
.L.str.5:
	.asciz	"y prime = "
############################################################
