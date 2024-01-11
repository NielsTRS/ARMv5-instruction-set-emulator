.data
    .text
    .global main
main:
    mov r1, #5
    mov r4, #4
    mvn r2, r1 @test mvn
    adc r3, r1, #6 @test adc
    add r4, #2 @test add
    cmp r4,r1
    bgt loop @test bgt
    swi 0x123456

loop: 
    sub r4, #1 @test sub
    cmp r2,r4
    blt loop2 @test blt

loop2:
    sub r4, r4, r1 @test sub



