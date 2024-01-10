.data
    .text
    .global main
main:
    @ Initialisation des registres avec des valeurs spécifiques
    mov r0, #0          @ Valeur zéro
    mov r1, #5          @ Valeur positive non nulle
    mov r2, #5          @ Valeur positive non nulle
    mov r3, #0xFFFFFFFF @ Valeur maximale pour un entier non signé 32 bits

    @ Test 1 : TEQ avec zéro
    teq r0, #0          @ Doit positionner le bit Z (Zero) car 0 XOR 0 = 0

    @ Test 2 : TEQ avec valeurs positives non nulles égales
    teq r1, r2          @ Doit positionner le bit Z car 5 XOR 5 = 0

    @ Test 3 : TEQ avec valeurs différentes
    teq r1, r3          @ Ne doit pas positionner le bit Z car 5 XOR 0xFFFFFFFF ≠ 0

    swi 0x123456