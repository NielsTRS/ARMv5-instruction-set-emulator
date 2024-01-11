.data
    .text
    .global main
main:
    mov r1, #4
    mov r2, #6
    mov r3, #7
    mvn r6, #5
    
    cmp r2, r1
    bge loop
    cmp r2, r3
    ble loop2
    swi 0x123456

loop: 
    rsb r2, r2, #1 @test rsb
    
loop2: 
    sbc r4, r3, r6 @test sbc
    bcs loop3 

loop3:
    rsc r1, r2, r6 @test rsc






