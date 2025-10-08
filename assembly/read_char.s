.data
input_buffer: .skip 16
.text
read_char:
    stp     lr, x0, [sp, #-16]!
    stp     x1, x2, [sp, #-16]!

    mov     x0, #0              // file descriptor for stdin
    ldr     x1, =input_buffer   // address of input buffer
    mov     x2, #1              // number of bytes to input

    mov     x8, #63             //63 is the syscall number for read
    svc     #0
    cmp     x0, #0
    bne     after_mark_eof
    mov     x0, #-1
    b       end_mark_eof
    after_mark_eof:
    ldrb    w0, [x1]
    end_mark_eof:
    ldp     x1, x2, [sp], #16
    add     sp, sp, #16
    ret
