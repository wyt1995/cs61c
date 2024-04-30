.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    # Branch to exception if a1 is less than 1
    addi t0, x0, 1
    blt a1, t0, excp

    # Load array index
    li t1, 0
    
loop_start:
    # Load array element
    lw t2, 0(a0)
    
    # Branch if element is negative
    blt t2, x0, loop_continue
    
    # Increment array address and index
    addi a0, a0, 4
    addi t1, t1, 1
    
    # End loop if no more element
    beq t1, a1, loop_end
    j loop_start

loop_continue:
    # Set negative element to 0
    sw x0, 0(a0)
    j loop_start

loop_end:
    jr ra

    # Epilogue
excp:
    # Exit with error code 36
    li a0, 36
    j exit
