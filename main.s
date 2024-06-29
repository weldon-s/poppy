.data
text:
    .ascii "I am a test\n"
len = . - text

.text
.global _start
_start:
    mov     x0, #1
    ldr     x1, =text
    ldr     x2, =len
    mov     w8, #64
    svc     #0

    mov     x0, #0      
    mov     w8, #93
    svc     #0
