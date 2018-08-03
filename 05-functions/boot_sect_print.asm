;function to print string stored in bx, null terminated
print:
    pusha

start:
    mov al, [bx]

    cmp al, 0 ;check for  null terminating char
    je done

    mov ah, 0x0e ;std print
    int 0x10

    add bx, 1 ;move pointer to next char in string
    jmp start

done:
    popa
    ret
;-------

;function to print newline
print_nl:
    pusha

    mov al, 0x0a
    mov ah, 0x0e ;std print
    int 0x10

    mov al, 0x0d
    mov ah, 0x0e ;std print
    int 0x10

    popa
    ret
