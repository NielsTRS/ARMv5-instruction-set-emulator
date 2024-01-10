.data
    .text
    .global main
main:
    @ Initialisation des registres avec des valeurs spécifiques
    mov r0, #0          @ Valeur zéro
    mov r1, #5          @ Valeur positive non nulle
    mov r2, #5          @ Valeur positive non nulle
    mov r3, #0xFFFFFFFA @ Valeur maximale pour un entier non signé 32 bits

    @ Test 1 : CMN avec zéro
    cmn r0, #0          @ Doit positionner le bit Z (Zero) car 0 + 0 = 0

    @ Test 2 : CMN avec valeurs positives non nulles égales
    cmn r1, r2          @ Ne doit pas positionner le bit Z car 5 + 5 = 10 (non nul)

    @ Test 3 : CMN avec valeurs positives non nulles différentes
    cmn r1, r3          @ Doit positionner le bit N (Negative) car 5 + 0xFFFFFFFF est négatif

    swi 0x123456