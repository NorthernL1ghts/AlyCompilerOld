.section .data
b: .space 8
a: .space 8
.section .text
.global main
main:
push %rbp
mov %rsp, %rbp
sub $32, %rsp
movq $420, a(%rip)
movq $42, b(%rip)
add $32, %rsp
pop %rbp
ret
