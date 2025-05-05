    jmp afterfoo
foo:
    push %rbp
    mov %rsp, %rbp
    sub $32, %rsp
    add $32, %rsp
    pop %rbp
    ret
afterfoo:
    a(%rip)
    b(%rip)
