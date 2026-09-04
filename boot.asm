bits 16
org 0x7c00

start:
    cli
    lgdt [gdt_desc]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

gdt_start:
dq 0
dq 0x00cf9a000000ffff
dq 0x00cf92000000ffff
gdt_end:

gdt_desc:
dw gdt_end-gdt_start-1
dd gdt_start

bits 32
protected_mode:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov ss,ax

    call 0x100000

hang:
    hlt
    jmp hang

times 510-($-$$) db 0
dw 0xaa55
