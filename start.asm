global kernel_start
extern kernel_main

section .text

kernel_start:
    cli
    call kernel_main

hang:
    hlt
    jmp hang
