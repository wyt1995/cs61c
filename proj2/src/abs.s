.globl abs

.text
# =================================================================
# FUNCTION: Given an int return its absolute value.
# Arguments:
#   a0 (int) is input integer
# Returns:
#   a0 (int) the absolute value of the input
# =================================================================
abs:
    # branch if non-negative
    bge a0, x0, done
    
    # Negate a0
    sub a0, x0, a0
    
    # Epilogue
done:
    ret
