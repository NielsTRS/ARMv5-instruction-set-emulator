.data
    .text
    .global main
main:
    mov r0, #0x1
    mov r1, #0xFFFFFFFF
    subs r2, r0, r1     @ vérification subs
    sbc r3, r0, r1      @ vérification sbc
    rsc r4, r1, r0      @ vérification rsc
    swi 0x123456