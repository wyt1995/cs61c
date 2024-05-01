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
    addi sp, sp, -36
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
    sw s7, 32(sp)
    
    # Save arguments
    mv s0, a0
    mv s1, a1
    mv s2, a2

    # Open file with read-only permission
    li a1, 0
    jal fopen
    
    # File descriptor
    li t0, -1
    beq a0, t0, fopen_error
    mv s3, a0
    
    
    # Read the number of rows from file
    mv a1, s1
    li a2, 4
    jal fread
    li t0, 4
    bne a0, t0, fread_error
    
    # Read the number of columns from file
    mv a0, s3
    mv a1, s2
    li a2, 4
    jal fread
    li t0, 4
    bne a0, t0, fread_error
    
    
    # Allocate space to store the matrix
    lw s1, 0(s1)    # number of rows
    lw s2, 0(s2)    # number of columns
    mul s5, s1, s2  # number of entries
    slli a0, s5, 2  # number of bytes
    jal malloc
    beq a0, x0, malloc_error
    mv s4, a0       # pointer to the matrix
    
    
    # Read matrix from file
    li s6, 0
    mv s7, s4
    
loop:
    beq s5, s6, done
    
    mv a0, s3
    mv a1, s7
    li a2, 4
    jal fread
    li t0, 4
    bne a0, t0, fread_error
    
    addi s6, s6, 1
    addi s7, s7, 4
    j loop


done:
    # Close the file
    mv a0, s3
    jal fclose
    bne a0, x0, fclose_error
    
    # Return a pointer to the matrix in memory
    mv a0, s4
    
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    addi sp, sp, 36

    jr ra


malloc_error:
    li a0, 26
    j exit


fopen_error:
    li a0, 27
    j exit


fclose_error:
    li a0, 28
    j exit


fread_error:
    li a0, 29
    j exit
