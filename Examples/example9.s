.data
    .text
    .global main
main:
    mov r0, #0xFFFFFFFF
    mov r1, #0xFFFFFFFF
    adds r2, r0, r1    @ vérification adds
    adc r3, r0, r1     @ vérification adc

    mov r0, #0
    mov r1, #0
    adds r2, r0, r1
    adc r3, r0, r1
    swi 0x123456