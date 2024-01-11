.data
    .text
    .global main
main:
    mov r1, #5
    mov r2, #6
    sub r3, r1, r2
    cmp r3, r1
    bmi loop @test bmi
    sub r5, r5, r5
    cmp r5, #0
    bpl loop2 @test bpl
    swi 0x123456

loop:
    sbc r6, r1, r2 @test sbc

loop2:
    rsc r7, r2, #1 @test rsc


    






