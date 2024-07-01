.text
.include "print_num.s"
.global _start
_start:
    mov     x0, #1
    mov     x1, #-1234
    bl      print_num

    mov     x0, #0      
    mov     w8, #93
    svc     #0
