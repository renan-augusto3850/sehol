.text
.globl main

main:
    movl $.L.str, %edi
    xorl %eax, %eax
    callq printf@PLT

    # Exit the program
    movq $60, %rax
    xorl %edi, %edi
    syscall

.L.str:
    .asciz "Hello, LLVM!"