in
pop rax
push rax
call FACTORIAL:
out
hlt

FACTORIAL:
    push rax
    push 1
    jbe EQUAL_TO_ONE:

    push rax
    push rax
    push 1
    sub
    pop rax
    call FACTORIAL:
    mult

    ret

EQUAL_TO_ONE:
    push 1
    ret
