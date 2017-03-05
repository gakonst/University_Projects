.globl main
.data
	inputString1:
	.asciiz "Please insert an integer number: \n"
	resultString:
	.asciiz "\nThe factorial of ("
	resultString2:
	.asciiz ") is : "


.text
main:

#Print first input message
	li $v0, 4
	la $a0, inputString1
	syscall

#Read integer and save it in a1
	li $v0, 5
	syscall
	move $t0,$v0
	move $a0,$t0
	addi $sp, $sp,-12
	sw $t0, 0($sp)
	sw $ra, 8($sp)	

#Call fact subroutine:
	jal fact
	lw $s0, 4($sp) #Load return value to $s0

#Print result
	li $v0, 4
	la $a0, resultString # The factorial of 
	syscall

	lw $a0, 0($sp)
	li $v0, 1           # X 
	syscall

	li $v0, 4
	la $a0, resultString2 # is : 
	syscall

	move $a0,$s0
	li $v0, 1 
	syscall

	#return0
	li $v0,10
	syscall

.text
fact:

#Adjust stack for 2 items, check if value is 0
	lw $t0, 0($sp)
	beq $t0, $zero, one
	addi $t0, $t0, -1


	addi $sp, $sp,-12 #move stack pointer 2 words up
	sw $t0, 0($sp)
	sw $ra, 8($sp)
	jal fact

#Factorial *n-1 etc
	lw $ra, 8($sp)

	lw $t1, 4($sp) #previous call's return value
	lw $t2 12($sp) #start value
	mul $t3, $t1, $t2 # x= (x-1)*x
	sw $t3, 16($sp)
	addi $sp, $sp, 12
	jr $ra

.text
one:
  li      $t0, 1            # load 1 into register $t0
  sw      $t0, 4($sp)       # store 1 into the parent's return value register
  jr      $ra               # jump to parent call