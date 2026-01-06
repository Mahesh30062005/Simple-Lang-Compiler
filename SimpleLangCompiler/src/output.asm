    LDI 10
    STA 0
    LDI 20
    STA 1
    LDA 1
    STA 255
    LDA 0
    ADD 255
    STA 2
    LDI 30
    STA 255
    LDA 2
    SUB 255
    JZ L_END_IF_13
    LDI 1
    STA 255
    LDA 2
    ADD 255
    STA 2
L_END_IF_13:
    HLT
