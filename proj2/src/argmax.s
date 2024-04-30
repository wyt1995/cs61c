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
    # Branch to exception if a1 is less than 1
    li t0, 1
    blt a1, t0, excp
    
    # Store maximum index and value
    li t0, 0
    lw t1, 0(a0)
    
    # Load current array index
    li t2, 0

loop_start:
    # Load current array element
    lw t3, 0(a0)
    
    # Branch if t1 < t3
    blt t1, t3, set_maximum
    j loop_continue    

set_maximum:
    # Set the current value to be the maximum
    mv t0, t2
    mv t1, t3
    j loop_continue

loop_continue:
    addi a0, a0, 4
    addi t2, t2, 1
    
    bge t2, a1, loop_end
    j loop_start

loop_end:
    mv a0, t0
    jr ra
    
excp:
    # Exit with error code 36
    li a0, 36
    j exit
