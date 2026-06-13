global _start
extern kmain

section .text

_start:
    call kmain

.hang:
    hlt
    jmp .hang
