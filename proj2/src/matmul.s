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
#
# Equivalent code in C:
# void matmul(int* a0, int a1, int a2, int* a3, int a4, int a5, int* a6) {
#     for (int i = 0; i < a1; i++) {
#         for (int j = 0; j < a5; j++) {
#             int* d_ptr = a6 + a5 * i + j;
#             int* m0_ptr = a0 + a2 * i;
#             int* m1_ptr = a3 + j;
#             &d_ptr = dot(m0_ptr, m1_ptr, a2, 1, a5);
#         }
#     }
# }
# =======================================================
matmul:

    # Error checks
    li t0, 1
    blt a1, t0, excp
    blt a2, t0, excp
    blt a4, t0, excp
    blt a5, t0, excp
    bne a2, a4, excp

    # Prologue
    addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)

    # Loop counter
    li s0, 0
    li s1, 0
    
    # Matrix address
    mv s2, a6
    

outer_loop_start:
    bge s0, a1, outer_loop_end


inner_loop_start:
    bge s1, a5, inner_loop_end
    
    # m0 pointer
    slli t0, s0, 2
    mul t0, t0, a2
    add t0, a0, t0

    # m1 pointer
    slli t1, s1, 2
    add t1, a3, t1
    
    # Save non-preserved registers
    addi sp, sp, -28
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    sw a3, 12(sp)
    sw a4, 16(sp)
    sw a5, 20(sp)
    sw a6, 24(sp)
    
    # Call dot function
    mv a0, t0
    mv a1, t1
    li a3, 1
    mv a4, a5
    jal dot
    sw a0, 0(s2)
    
    # Restore temporary registers
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    lw a3, 12(sp)
    lw a4, 16(sp)
    lw a5, 20(sp)
    lw a6, 24(sp)
    addi sp, sp, 28
    
    # Increment counter
    addi s1, s1, 1
    addi s2, s2, 4
    j inner_loop_start


inner_loop_end:
    addi s0, s0, 1
    li s1, 0  # reset inner loop counter
    j outer_loop_start
    

outer_loop_end:

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    addi sp, sp, 16
    
    jr ra

excp:
    # Exit with error code 38
    li a0 38
    j exit
