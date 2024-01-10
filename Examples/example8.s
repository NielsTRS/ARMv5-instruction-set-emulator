.data
    .text
    .global main
main:
    mov r0, #0x3A   @0b00111010
    mov r1, #0x25   @0b00100101

    and r2, r0, r1  @ vérification and (r2 = 0b00100000 = 0x20)
    and r3, r0, #0x25

    eor r2, r0, r1  @ vérification eor (r2 = 0b00011111 = 0x1F)
    eor r2, r0, #0x25

    sub r2, r0, r1  @ vérification sub (r2 = 0xb0010101 = 0x15)
    rsb r2, r1, r0  @ vérification rsb (r2 = 0xb0010101 = 0x15)
    add r2, r0, r1  @ vérification add (r2 = 0b01011111 = 0x5F)
    swi 0x123456