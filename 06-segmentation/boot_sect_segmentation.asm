mov ah, 0x0e ; tty

mov al, [the_secret]
int 0x10 ; we already saw this doesn't work, right?

mov bx, 0x7c0
mov ds, bx
mov al, [the_secret]
int 0x10

jmp $

the_secret:
    db "X"

; padding and magic number
times 510-($-$$) db 0
dw 0xaa55