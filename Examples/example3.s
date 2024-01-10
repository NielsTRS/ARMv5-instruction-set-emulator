.global main
.text
main:
    mov r0, #0
    ldr r1, [r0]
    swi 0x123456
.data
limite:
    .word 0x12345678