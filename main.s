.data
text:
    .ascii " "
len = . - text

.text

//prints a number to the console
//x1 is the number to print (passed in as NON-ASCII!)
//TODO negative handling
print:
    str lr, [sp, #-8]!
    stp x1, x2, [sp, #-16]!

    //x1 is our parameter (integer to print) + will store the ASCII-converted value
    //x2 is the length of the string (always 1 for our case)

    //first, let's check if x1 is negative

    //get things set up
    mov     x9, x1
    ldr     x1, =text
    mov     x2, #1
    mov     x10, #10
    mov     x13, #0

    print_loop_stack:
        //divide x9 by 10, store in x11
        sdiv    x11, x9, x10

        //get the remainder, store in x12
        //this is equal to x9 - (x11 * x10)
        msub    x12, x11, x10, x9
        mov     x9, x11
        mov     x11, x12

        //add 48 to x11 to convert to ASCII
        add     x11, x11, #48
        strb    w11, [x1]
        ldr     x3,  [x1]
        ldr     x2, =len

        // push this value to the stack
        str     x11, [sp, #-8]!
        add     x13, x13, #1

        //if x9 is 0, we're done
        cmp     x9, #0
        bne     print_loop_stack

    print_loop_out:
        //pop the stack
        ldr     x11, [sp], #8
        strb    w11, [x1]
        
        //call the write syscall
        mov     x8, #64
        svc     #0

        subs    x13, x13, #1
        bne     print_loop_out
    
    ldp x1, x2, [sp], #16
    ret

.global _start
_start:
    mov     x0, #1
    mov     x1, #0xFFFF
    bl      print

    mov     x0, #0      
    mov     w8, #93
    svc     #0
