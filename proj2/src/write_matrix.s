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
    addi sp, sp, -24
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    
    mv s0, a1       # matrix pointer
    mv s1, a2       # number of rows
    mv s2, a3       # number of columns
    mul s3, s1, s2  # number of entries
    
    # Open the file with write permissions
    li a1, 1
    jal fopen
    li t0, -1
    beq a0, t0, fopen_error
    mv s4, a0       # file descriptor

    
    # Write the number of rows and columns to file
    addi sp, sp, -8
    sw s1, 0(sp)
    sw s2, 4(sp)
    
    addi a1, sp, 0
    li a2, 1
    li a3, 4
    jal fwrite
    li t0, 1
    bne a0, t0, fwrite_error
    
    mv a0, s4
    addi a1, sp, 4
    li a2, 1
    li a3, 4
    jal fwrite
    li t0, 1
    bne a0, t0, fwrite_error
    
    addi sp, sp, 8
    
    
    # Write the data to file
    mv a0, s4
    mv a1, s0
    mv a2, s3
    li a3, 4
    jal fwrite
    mv t0, s3
    bne a0, t0, fwrite_error
    
    
    # Close the file
    mv a0, s4
    jal fclose
    bne a0, x0, fclose_error
    

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    addi sp, sp, 24

    jr ra


fopen_error:
    li a0, 27
    j exit


fclose_error:
    li a0, 28
    j exit
    
    
fwrite_error:
    li a0, 30
    j exit
