.include "print_num.s"
.text
.global _start
_start:
mov x0, #1

mov x1, #412
bl print_num
mov x0, #0
mov w8, #93
svc #0
