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
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
    # Prologue: check args
    li t0, 1
    blt a2, t0, excp_number
    blt a3, t0, excp_stride
    blt a4, t0, excp_stride
    
    # dot product
    li t0, 0
    
    # strides
    slli t3, a3, 2
    slli t4, a4, 2

loop_start:
    lw t1, 0(a0)
    lw t2, 0(a1)
    mul t1, t1, t2
    add t0, t0, t1

    addi a2, a2, -1
    beq a2, x0, loop_end
    
    # Increment array addresses
    add a0, a0, t3
    add a1, a1, t4
    j loop_start

loop_end:
    mv a0, t0
    ret

    # Epilogue
excp_number:
    li a0, 36
    j exit
    
excp_stride:
    li a0, 37
    j exit
