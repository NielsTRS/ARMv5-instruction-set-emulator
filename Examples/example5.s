    .data
D1:    .word  266
D2:    .hword 42
D3:    .byte 12

    .text
    .global main
main:
      LDR r3, LD_D1
      LDR r4, [r3]    @ vérification ldr

      LDR r5, LD_D2
      LDRH r6, [r5]   @ vérification ldrh

      LDR r7, LD_D3
      LDRB r8, [r7]   @ vérification ldrb

LD_D1:   .word D1
LD_D2:   .word D2
LD_D3:   .word D3
