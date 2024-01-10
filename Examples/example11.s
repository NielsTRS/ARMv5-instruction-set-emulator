.data
    .text
    .global main
main:
    @ Test 1 : TST avec N
    mov r0, #0xFFFFFFFF
    mov r1, #0xFFFFFFFF
    tst r0, r1

    @ Test 2 : TST avec Z
    mov r2, #0x0
    mov r3, #0x0
    tst r2, r3

    swi 0x123456