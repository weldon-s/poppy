.text
.include "print_num.s"
print_line:
    stp     lr, x0, [sp, #-16]!
    stp     x1, x2, [sp, #-16]!

    mov     x0, #1           // x0 is the file descriptor (1 for stdout)
    mov     x2, #1           // x2 is the length of the string (always 1 for our case)
    mov     x8, #64          // 64 is the syscall number for write
    mov     x11, #10         // '\n' is 10
    str     x11, [sp, #-16]!
    mov     x1, sp           // set x1 to the top of the stack
    add     sp, sp, #16      // pop the top value from the stack
    svc     #0               // call the write syscall
    ldp     x1, x2, [sp], #16
    ldr     x0, [sp], #16
    ret
print_char:
    stp     lr, x0, [sp, #-16]!
    stp     x1, x2, [sp, #-16]!

    mov     x0, #1           // x0 is the file descriptor (1 for stdout)
    mov     x2, #1           // x2 is the length of the string (always 1 for our case)
    mov     x8, #64          // 64 is the syscall number for write
    str     x1, [sp, #-16]!
    mov     x1, sp           // set x1 to the top of the stack
    add     sp, sp, #16      // pop the top value from the stack
    svc     #0               // call the write syscall
    ldp     x1, x2, [sp], #16
    ldr     x0, [sp], #16
    ret
