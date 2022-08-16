.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
	# Prologue
	bge x0, a2, error1
	bge x0, a3, error2
	bge x0, a4, error2
	addi sp, sp, -20
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	mv s0, a0	# start of array0
	mv s1, a1	# start of array1
	mv s2, a2
	mv s3, a3
	mv s4, a4

loop_start:
	mv t0, x0 # number of element being used
	mv t6, x0 # the sum of the pair product

loop_continue:
	#array0
	mul t1, t0, s3 # t1 = t0 * s3
	slli t2, t1, 2 # t2 = t1*4
	add t3, s0, t2 # t3 = s0 + t2
	lw t4, 0(t3)	# grab the current array0 element to t4
	# array1
	mul t1, t0, s4 # t1 = t0 * s4
	slli t2, t1, 2 # t2 = t1*4
	add t3, s1, t2 # t3 = s1 + t2
	lw t5, 0(t3)	# grab the current array1 element to t5
	#
	mul t4, t4, t5
	add t6, t6, t4 # t6 += t4
	# upgrade t0
	addi t0, t0, 1
	blt t0, s2, loop_continue

loop_end:
	mv a0, t6
	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	addi sp, sp, 20
	ret

error1:
	li a0, 36
	j exit

error2:
	li a0, 37
	j exit