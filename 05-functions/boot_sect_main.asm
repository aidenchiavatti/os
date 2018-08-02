[org 0x7c00] ; tell the assembler that our offset is bootsector code

mov bx, HELLO
call print
call print_n1

mov bx, GOODBYE
call print
call print_n1

mov dx, 0x1234
call print_hex
call print_n1

mov dx, 0x3F7C
call print_hex
call print_n1

jmp $

%include "boot_sect_print.asm"
%include "boot_sect_print_hex.asm"

; data
HELLO:
    db 'Hello, World', 0

GOODBYE:
    db 'Goodbye', 0

; padding and magic number
times 510-($-$$) db 0
dw 0xaa55