
.text
_start:

# ----------------- Stub to test functions in QtRVSim -----------------
    # These lines are not executed by the Artemis tester.
    # Uncomment the parts testing the function you want to test and comment
    # out the other parts. Feel free to adjust values for testing.

    # uncomment following lines to test tribonacci_iter
      li a0, 3
      call tribonacci_iter

    # uncomment following lines to test tribonacci_rec
      #li a0, 6
      #call tribonacci_rec

    ebreak  # stop execution in QtRVSim to inspect the result
# -------------- End of stub to test functions in QtRVSim -------------


# first (and only) argument: n
# return value: a(n)    (n-th tribonacci number)
tribonacci_iter:
    # TODO: Implement iterative tribonacci number calculation
    // while(count < n)
    li t0, 0
    
    // T0
    li t1, 0
    // T1
    li t2, 0
    // T2
    li t3, 1
    
    
    j cond
loop:
	addi t0, t0, 1
	li t4, 0
	add t4, t1, t2
	add t4, t4, t3
	mv t1, t2
	mv t2, t3
	mv t3, t4
	
    
cond:
	blt t0, a0, loop
	
	mv a0, t1
	ret
	
	
    


# first (and only) argument: n
# return value: a(n)    (n-th tribonacci number)
tribonacci_rec:
    # TODO: Implement recursive tribonacci number calculation
    // if n == 0 ret 0
    bne a0, zero, cond2
    li a0, 0
    ret

cond2:
	// if n == 1 ret 1
	li t0, 1
	bne a0, t0, cond 3
	li a0, 0
	ret

cond3:
	// if n == 2 ret 1
	li t0, 2
	bne a0, t0, body
	li a0, 1
	ret
body:
	add t0, zero, zero
	addi sp, sp, -16
	sw ra, 12(sp)
	sw a0, 8(sp)
	sw t0, 4(sp)
	
	// trib(n-1)
	addi a0, a0, -1
	call tribonacci_rec
	
	lw t0, 4(sp)
	add t0, t0, a0
	lw a0, 8(sp)
	
	sw t0, 4(sp)
	
	// trib(n-1)
	addi a0, a0, -2
	call tribonacci_rec
	
	lw t0, 4(sp)
	add t0, t0, a0
	lw a0, 8(sp)
	
	sw t0, 4(sp)
	
	// trib(n-2)
	addi a0, a0, -3
	call tribonacci_rec
	
	lw t0, 4(sp)
	add t0, t0, a0
	
	add a0, t0, zero
	lw ra, 12(sp)
	
	addi sp, sp, 16
	ret
 
    
    
    // trib(n-1) + trib(n - 2) + trib( n - 3)
    
