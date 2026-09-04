global multiboot_header

section .multiboot
align 8

multiboot_header:
    dd 0xe85250d6        ; multiboot2 magic
    dd 0                 ; architecture i386
    dd -(0xe85250d6)     ; checksum
    dd 0

    ; end tag
    dw 0
    dw 0
    dd 8
