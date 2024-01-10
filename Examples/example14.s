.data
    .text
    .global main
main:
    @ Initialisation des registres avec des valeurs spécifiques
    mov r0, #0          @ Valeur initiale pour r0
    mov r1, #0x0F       @ Valeur initiale pour r1
    mov r2, #0x80       @ Valeur initiale pour r2

    @ Test 1 : ORR avec une valeur immédiate
    orr r0, r1, #0xFF   @ r0 = r1 OR 0xFF (0x0F OR 0xFF)

    @ Test 2 : ORR avec un autre registre
    orr r0, r0, r2      @ r0 = r0 OR r2 (0xFF OR 0x80)

    swi 0x123456