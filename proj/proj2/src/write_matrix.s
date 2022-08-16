.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

	# Prologue
	addi sp, sp, -36
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	sw s5, 20(sp)
	sw ra, 24(sp)
	mv s0, a0
	mv s1, a1
	mv s2, a2 # num of rows
	mv s3, a3 # num of cols
	# fopen
	mv a0, s0
	addi a1, x0, 1
	jal ra, fopen
	li t0, -1
	beq t0, a0, error_27
	mv s4, a0 # file id
	# ===========================
	sw s2, 28(sp)
	sw s3, 32(sp)
	addi a1, sp, 28
	li a2, 2
	li a3, 4
	jal ra, fwrite

	mv a0, s4
	mv a1, s1
	mul a2, s2, s3 # a2 = s2 * s3
	mv s5, a2
	li a3, 4
	jal ra, fwrite
	bne a0, s5, error_30
	# ==========fclose===============
	mv a0, s4
	jal ra, fclose
	li t0, -1
	beq t0, a0, error_28

	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	lw s5, 20(sp)
	lw ra, 24(sp)	
	addi sp, sp, 36


	ret

error_26:
	li a0, 26
	j exit

error_27:
	li a0, 27
	j exit

error_28:
	li a0, 28
	j exit

error_29:
	li a0, 29
	j exit
	
error_30:
	li a0, 30
	j exit