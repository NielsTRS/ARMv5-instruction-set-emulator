.data
    .text
    .global main
main:
     mvn r0, #2   @ r0 = 0xfffffffd
     mvn r1, #8   @ r1 = 0xfffffff7
     swi 0x123456