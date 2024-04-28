.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (defined above).
# The return value should be stored in a0
f:
    # Your code here
    addi t0 a0 3 # index of a0 in the array
    slli t2 t0 2 # integer is 4 bytes wide
    addi t1 a1 0 # address of the first element
    add t2 t2 t1 # address of the operand element
    lw t3 0(t2)  # read the element
    mv a0 t3
    
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
