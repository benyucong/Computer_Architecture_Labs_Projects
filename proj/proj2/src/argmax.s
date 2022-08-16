.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
	bge x0, a1, error # 0 >= s1
	addi sp, sp, -8
	sw s0, 0(sp)
	sw s1, 4(sp)
	add s0, a0, x0
	add s1, a1, x0


loop_start:
	mv t0, x0  # t0 marks the index of the array
	mv t4, x0	# t4 stores the answer index
	lw t5, 0(s0)	# t5 stores the first max element


loop_continue:
	slli t1, t0, 2	# t1 = t0 * 4
	add t2, s0, t1	# t2 = s0 + t1 = s0 + t0 * 4, current address
	lw t3, 0(t2)	# grab the current element to t3
	blt t5, t3, loop_end # if t5 < t3 then loop_end
	addi t0, t0, 1
	blt t0, s1, loop_continue
	j done

loop_end:
	mv t4, t0
	mv t5, t3
	addi t0, t0, 1
	blt t0, s1, loop_continue


done:
	addi a0, t4, 0
	lw s0, 0(sp)
    lw s1, 4(sp)
    addi sp, sp, 8
	jr ra	# Return to caller


error:
	li a0, 36
	j exit