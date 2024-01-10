.data
    .text
    .global main
main:
    mov r0, #0xFFFFFFFF
    mov r1, #0xFFFFFFFF
    adds r3, r0, r1
    adc r2, r0, r1
    @ Test mrs
    mrs r4, CPSR
    mrs r5, SPSR
    swi 0x123456