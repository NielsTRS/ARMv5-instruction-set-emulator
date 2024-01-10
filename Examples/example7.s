.data
entier:  .byte 7 

    .global main
    .text
main:
    mov r0, #8                @ vérification mov
    ldr r1, =entier
    mov r2, #18
    mov r3, #3
    mov r4, #0

tantque1: cmp r4, r3           @ vérification cmp entre 2 registres
          beq fintq1
            ldrb r5, [r1]        
            strb r5, [r0, r4]  @ vérification multiple et strb
            ldrb r6, [r0, r4]  @ vérification multiple et ldrb
            add r1, r1, #1        
            add r4, r4, #1 
          b tantque1

fintq1:
    swi 0x123456