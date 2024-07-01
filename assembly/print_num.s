.text
// prints a number to the console
// we do this by pushing each ASCII digit to the stack, then popping them off and printing them
// can definitely be optimized

//x1 is the number to print (passed in as NON-ASCII!)
print_num:
    str     lr, [sp, #-8]!
    str     x0, [sp, #-8]!
    stp     x1, x2, [sp, #-16]!

    //x1 is our parameter (integer to print) + will store the ASCII-converted value
    //x2 is the length of the string (always 1 for our case)

    //get things set up
    mov     x9, x1      //x9 is the current value we're working with
    mov     x0, #1      //x0 is the file descriptor (1 for stdout)
    mov     x1, sp      //x1 is the address of the text buffer
    mov     x2, #1      //x2 is the length of the string (always 1 for our case)
    mov     x8, #64     //64 is the syscall number for write
    mov     x10, #10    //10 is the base for our division
    mov     x13, #0     //x13 is the number of digits we've printed (for popping off the stack later)

    //first, let's check if x9 is negative
    //i could negate it here too, but that doesnt handle the edge case of the smallest possible value
    //so i just deal with that digit-by-digit (which i should optimize at some point)

    cmp     x9, #0
    bge     print_loop_stack

    //if it is, print a '-'
    mov     x11, #45
    strb    w11, [x1]

    svc     #0


    print_loop_stack:
        //divide x9 by 10, store in x11
        sdiv    x11, x9, x10

        //get the remainder, store in x12
        //this is equal to x9 - (x11 * x10)
        msub    x12, x11, x10, x9
        mov     x9, x11
        mov     x11, x12

        //if x11 is negative, negate it
        cmp     x11, #0
        bge     print_loop_stack_after_negate
        neg     x11, x11

        print_loop_stack_after_negate:
        //add 48 to x11 to convert to ASCII
        add     x11, x11, #48

        // push this value to the stack
        str     x11, [sp, #-8]!
        add     x13, x13, #1

        //if x9 is 0, we're done
        cmp     x9, #0
        bne     print_loop_stack

    print_loop_out:
        mov     x1, sp     // set x1 to the top of the stack
        add     sp, sp, #8 // pop the top value from the stack
        svc     #0         //call the write syscall

        subs    x13, x13, #1
        bne     print_loop_out
    
    ldp     x1, x2, [sp], #16
    ldr     x0, [sp], #8
    add     sp, sp, #8
    ret
