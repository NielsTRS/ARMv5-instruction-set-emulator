.data
    .text
    .global main
main:
    mov r1, #5
    mov r2, #6
    mov r6, #0xfffffff
    mov r3, r2, ASR #2 @test ASR
    mov r4, r2, LSR #2 @test LSR
    mov r5, r2, ROR #2 @test ROR
    cmp r1, r6
    bcc loop @test bcc

loop:
    add r7, r6, r1
    bvc loop2 @test bvc 

loop2:
    add r1, r2
    bhi loop3 @test bhi 

loop3:
    mov r1, #0






