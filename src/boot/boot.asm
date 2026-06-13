; Multiboot2 header and entry point
section .multiboot_header
header_start:
    dd 0xe85250d6                ; Magic number
    dd 0                         ; Architecture 0 (protected mode i386)
    dd header_end - header_start ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum

    ; Framebuffer tag
    align 8
    dw 5                         ; Type
    dw 0                         ; Flags
    dd 20                        ; Size
    dd 1024                      ; Width
    dd 768                       ; Height
    dd 32                        ; Depth

    ; End tag
    align 8
    dw 0                         ; Type
    dw 0                         ; Flags
    dd 8                         ; Size
header_end:

section .text
bits 32
global _start
extern kmain

_start:
    cli
    ; GRUB passes magic in EAX and info pointer in EBX.
    ; We need to save them before we potentially overwrite them or change segments.
    mov edi, eax
    mov esi, ebx

    lgdt [gdt_ptr]
    jmp 0x08:.reload_cs

.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up our own stack
    mov esp, stack_top
    
    ; Push the saved multiboot magic and information structure pointer
    push esi ; Info pointer
    push edi ; Magic

    call kmain

.halt:
    hlt
    jmp .halt

section .data
align 8
gdt_start:
    ; Null descriptor
    dq 0
gdt_code:
    ; Code descriptor: base=0, limit=0xfffff, type=0x9a (exec/read), flags=0xc (32-bit, 4k gran)
    dw 0xffff
    dw 0
    db 0
    db 0x9a
    db 0xcf
    db 0
gdt_data:
    ; Data descriptor: base=0, limit=0xfffff, type=0x92 (read/write), flags=0xc (32-bit, 4k gran)
    dw 0xffff
    dw 0
    db 0
    db 0x92
    db 0xcf
    db 0
gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd gdt_start

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:
