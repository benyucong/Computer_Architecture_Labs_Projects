.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

	# Prologue
	addi sp, sp, -28
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	sw s5, 20(sp)
	sw ra, 24(sp)
	mv s0, a0
	mv s1, a1
	mv s2, a2
	#fopen
	mv a0, s0 # A pointer to the filename string.
	mv a1, x0 # Read-only
	jal ra, fopen
	li t0, -1
	beq t0, a0, error_27
	mv s3, a0 # store the file descriptor t0 s0
	# ===========================================
	mv a1, s1 # address of # of rows
	addi a2, x0, 4
	jal ra, fread
	li t0, 4
	bne t0, a0, error_29
	#====
	mv a0, s3
	mv a1, s2 # address of # of cols
	addi a2, x0, 4
	jal ra, fread
	li t0, 4
	bne t0, a0, error_29
	#====
	lw t1, 0(s1)
	lw t2, 0(s2)
	mul a0, t1, t2
	slli a0, a0, 2 
	mv s4, a0 # store the total num of the bytes to read
	jal ra, malloc
	beq a0, x0, error_26
	mv s5, a0 # store the pointer to the allocated memory to s5
	mv a1, s5 # address
	mv a0, s3 # file id
	mv a2, s4 # nums
	jal ra,fread
	bne s4, a0, error_29
	#==============fclose===============
	mv a0, s3
	jal ra, fclose
	li t0, -1
	beq t0, a0, error_28
	mv a0, s5

	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	lw s5, 20(sp)
	lw ra, 24(sp)	
	addi sp, sp, 28

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