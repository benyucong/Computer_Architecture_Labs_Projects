.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
    li t0, 5
	bne t0, a0, error_31
	addi sp, sp, -52
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	sw s5, 20(sp)
	sw s6, 24(sp)
	sw s7, 28(sp)
	sw s8, 32(sp)
	sw s9, 36(sp)
	sw s10, 40(sp)
	sw s11, 44(sp)
	sw ra, 48(sp)
	mv s0, a0
	mv s1, a1
	mv s2, a2
	# malloc an array of length 6 to store the dimensions of matrices
	li a0, 24
	jal ra, malloc
	beq a0, x0, error_26
	mv s4, a0 # the pointer to array

	# Read pretrained m0
	lw a0, 4(s1)
	mv a1, s4
	addi a2, s4, 4
	jal ra, read_matrix
	mv s5, a0

	# Read pretrained m1
	lw a0, 8(s1)
	addi a1, s4, 8
	addi a2, s4, 12
	jal ra, read_matrix
	mv s6, a0

	# Read input matrix
	lw a0, 12(s1)
	addi a1, s4, 16
	addi a2, s4, 20
	jal ra, read_matrix
	mv s7, a0

	# Compute h = matmul(m0, input)
	lw t0, 0(s4)
	lw t1, 20(s4)
	mul a0, t0, t1
	mv s9, a0 # num of elements 
	slli a0, a0, 2 # num of bytes
	jal, ra, malloc
	beq a0, x0, error_26
	mv s8, a0 # target matrix pointer
	# matmul
	mv a0, s5
	lw a1, 0(s4)
	lw a2, 4(s4)
	mv a3, s7
	lw a4, 16(s4)
	lw a5, 20(s4)
	mv a6, s8
	jal, ra, matmul

	# Compute h = relu(h)
	mv a0, s8
	mv a1, s9
	jal ra, relu

	# malloc for o
	lw t0, 8(s4)
	lw t1, 20(s4)
	mul a0, t0, t1
	mv s11, a0 # num of elems
	slli a0, a0, 2 # num of bytes
	jal ra, malloc
	beq a0, x0, error_26
	mv s10, a0

	# Compute o = matmul(m1, h)
	mv a0, s6
	lw a1, 8(s4)
	lw a2, 12(s4)
	mv a3, s8
	lw a4, 0(s4)
	lw a5, 20(s4)
	mv a6, s10
	jal ra, matmul

	# Write output matrix o
	lw a0, 16(s1)
	mv a1, s10
	lw a2, 8(s4)
	lw a3, 20(s4)
	jal, ra, write_matrix

	# Compute and return argmax(o)
	mv a0, s10
	mv a1, s11
	jal ra, argmax
	mv s11, a0

	# If enabled, print argmax(o) and newline
	bne s2, x0, Done # s2 != 0
	jal, ra, print_int
	li a0, '\n'
	jal, ra, print_char

Done:
	mv a0, s4
	jal, ra, free
	mv a0, s8
	jal, ra, free
	mv a0, s10
	jal ra, free
	mv a0, s11

	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	lw s5, 20(sp)
	lw s6, 24(sp)
	lw s7, 28(sp)
	lw s8, 32(sp)
	lw s9, 36(sp)
	lw s10, 40(sp)
	lw s11, 44(sp)
	lw ra, 48(sp)
	addi sp, sp, 52
	ret

error_26:
	li a0, 26
	j exit

error_31:
	li a0, 31
	j exit