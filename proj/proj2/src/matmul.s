.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

	# Error checks
	bge x0, a1, error
	bge x0, a2, error
	bge x0, a4, error
	bge x0, a5, error
	bne a2, a4, error

	# Prologue
	addi sp, sp, -40
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	sw s5, 20(sp)
	sw s6, 24(sp)
	sw s7, 28(sp)
	sw s8, 32(sp)
	sw ra, 36(sp)
	#
	mv s0, a0	# pointer to the start of m0
	mv s1, a1	# the # of rows (height) of m0
	mv s2, a2	# the # of columns (width) of m0
	mv s3, a3	# the pointer to the start of m1
	mv s4, a4	# the # of rows (height) of m1 
	mv s5, a5	# the # of columns (width) of m1
	mv s6, a6	# the pointer to the the start of d
	mv s7, x0 	# index of the outer loop


outer_loop_start:
	# pre
	bge s7, s1, outer_loop_end # s7 < s1 then continue
	mv s8, x0	# index of the inner loop
	


inner_loop_start:
	bge s8, s5, inner_loop_end
	# calculate a0-a4 again
	mul t0, s2, s7 # t0 = s2 * s7
	slli t0, t0, 2 # t0 *= 4
	add a0, s0, t0 # a0 = s0 + t0
	addi a3, x0, 1 # a3 = 1
	mv a4, s5 # a4 = s5
	slli t0, s8, 2	# t0 = s8 * 4
	add a1, s3, t0	# a1 = s3 + t0
	mv a2, s2
	jal ra, dot		# call dot
	sw a0, 0(s6)	# return the result matrix
	addi s6, s6, 4	# s6 += 4
	addi s8, s8, 1
	j inner_loop_start

inner_loop_end:
	addi s7, s7, 1
	j outer_loop_start


outer_loop_end:
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
	lw ra, 36(sp)
	addi sp, sp, 40

	ret

error:
	li a0, 38
	j exit