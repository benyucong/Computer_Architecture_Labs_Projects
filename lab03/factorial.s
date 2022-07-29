# test
.globl factorial

.data
n: .word 8

.text
main:
    la t0, n    # loads the address of the label where n is located.
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial:
    # YOUR CODE HERE
    addi s0, x0, 1
loop:
    beq a0, x0, exit
    mul s0, s0, a0
    addi a0, a0, -1
    j loop
exit:
    mv a0, s0
    jr ra




