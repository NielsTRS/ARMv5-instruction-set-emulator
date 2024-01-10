.data
    .text
    .global main
main:
     mov r0, #0b0101
     mov r1, #0b1100
    
     bic r2, r0, r1       @ r2 = 0b0001 = 0x01
     bic r2, r0, #0b1100
     swi 0x123456