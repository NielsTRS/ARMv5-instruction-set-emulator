   .data
debutTAB: .skip 5*4

   .text
   .global main
main:
   ldr r0, ptr_debutTAB         

     mov r3, #11                
     mov r2, #0                 
tq:  cmp r2, #5                     @ vérification cmp valeur immédiate       
     beq fintq                      @ vérification beq
        @ i-5 <> 0
        ldr r0, ptr_debutTAB
        add r0, r0, r2, LSL #2  @ vérification lsl (r0 <- r0 + r2*4 = debutTAB + i*4)
        str r3, [r0]
        add r2, r2, #1
        add r3, r3, #11
     b   tq                     @ vérification b
fintq:  @ i-5 = 0

fin:
   swi 0x123456

ptr_debutTAB : .word debutTAB