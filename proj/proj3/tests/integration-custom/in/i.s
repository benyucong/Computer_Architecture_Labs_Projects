addi t0, x0, 4
andi t1, t0, 7
ori t2, t0, 7
xori t2, t0, 9
slli t0, t0,1
srli t0, t0,1
srai t0, t0, 1
slti t0, t0,1
##############
add t0, t1, t2
mul t2, t0, t1
sub t2, t2, t1
sll t1, t1, t0
srl t1, t1, t0
sra t2, t1, t2
slt t2, t2, t1
and t2,t1,t0
or t1,t2,t0
xor t2, t0, t1
mul t1, t2, t0
addi sp, x0, 2000
##############
sw t0, 8(sp)
lw t0, 8(sp)
lh t0, 8(sp)
lb t0, 8(sp)