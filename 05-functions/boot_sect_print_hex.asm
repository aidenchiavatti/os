; prints hex number stored in dx. 
; loops to convert the last digit to hex ascii (1234 -> 0123 -> 0012 -> 0001)
print_hex:
    pusha

    mov cx, 0 ; counter, indicates current index from the right of original number
hex_loop:
    cmp cx, 4
    je hex_done ; ends after 4 digits

    mov ax, dx
    and ax, 0x000f ; bitwise function to isolate last digit
    add al, 0x30 ; '0' == ASCII 0x30
    cmp ax, 0x39 ; '9' == ASCII 0x39
    jle hex_step_2
    add al, 0x07 ; 'A' == ASCII 0x41. need to add 7 for letters (A-F)

hex_step_2: ; replaces HEX_OUT character with corresponding ASCII at current index
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al
    
    shr dx, 4 ; shift to next index
    add cx, 1 ; increment counter
    jmp hex_loop

hex_done:
    mov bx, HEX_OUT
    call print
    popa
    ret

HEX_OUT:
    db '0x0000', 0