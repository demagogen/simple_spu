in
pop rax
push rax
call FACTORIAL:
out
hlt

FACTORIAL:
    push rax
    push 1
    sub
    pop rax
    push rax
    push 1
    je EQUAL_TO_ONE:
        pop
        call FACTORIAL:
        mult
        push 1
        push 1
    EQUAL_TO_ONE:
    pop
    pop
ret
