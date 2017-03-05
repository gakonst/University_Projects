.data 0x10008000
	.word 5,9,32

    buffer: .space 20
	input: .space 1
	tempChosen:
	.asciiz "\nYou chose: "
	tempScale:
	.asciiz "Select the temperature scale (f or c, lowercase):"
	tempNumber:
	.asciiz "\nType the desired temperature:" 
	tempOut:
	.asciiz "\nConverted Temperature = "

	

.text
main:
# input float
# Choose F or C
# if C
# F = C*(9/5)+32
# return F
# else if F
# C = (5/9)*(F-32)
# return C

	
#Initialization
	lui $gp, 0x1000
	ori $gp, $gp, 0x8000

#Load variables
	lwc1 $f16, 0($gp)  
	lwc1 $f18, 4($gp)
	lwc1 $f14, 8($gp) 
	#lwc1 $f12, 12($gp)

#Convert to floating-point number
	cvt.s.w $f16, $f16        
	cvt.s.w $f18, $f18
	cvt.s.w $f14, $f14
	#cvt.s.w $f12, $f12

#Select temp scale
	li $v0,4
	la $a0, tempScale
	syscall

#Read scale
	li $v0, 12
	syscall
	move $t5,$v0
	
	#li $v0,5
	#syscall
	#move $t5,$v0       # TESTING WITH 1 OR 2 INPUT, 1 = C, 2= F


#Show scale chosen
	li $v0,4
	la $a0, tempChosen
	syscall 

	move $a0, $t5        # C or F
	li $v0, 11
	syscall

#Input number text
	li $v0, 4
	la $a0, tempNumber
	syscall

	##-----------------CHECK----------##

#Read float number - temp in scale specified
	li $v0, 6
	syscall
	swc1 $f0, 0($sp)
	mov.s $f1, $f0

#show float number picked
	li $v0,4
	la $a0, tempChosen
	syscall 

	li $v0,2
	lwc1 $f12, 0($sp)
	syscall

#if input is C transform to F, else opposite
	#li $t1, 43
	#li $t2, 63
	beq $t5, 'c', cel
	beq $t5, 'C', cel
	#li $t1, 46
	#li $t2, 66
	beq $t5, 'f', far
	beq $t5, 'F', far

	
	#Testing without ifs
	#div.s $f20, $f16, $f18  # 5/9
	#sub.s $f1, $f1, $f14    # F-32
	#mul.s $f1, $f1, $f20    # (F-32)*(5/9)
	#swc1 $f1, 4($sp)
	#div.s $f21, $f18, $f16 # 9/5
	#mul.s $f1, $f21, $f1   # C*9/5
	#add.s $f1, $f1, $f14   # c*9/5 +32
	#swc1 $f1, 4($sp)
#Print result: Result is being printed in far/cel
	#li $v0, 4
	#la $a0, tempOut
	#syscall

	#li $v0, 2
	#lwc1 $f12, 4($sp)
	#syscall
#return 

	jr $ra


#Prakseis
far:

	lwc1 $f1, 0($sp)
	div.s $f20, $f16, $f18  # 5/9
	sub.s $f1, $f1, $f14    # F-32
	mul.s $f1, $f1, $f20    # (F-32)*(5/9)
	swc1 $f1, 4($sp)

	li $v0, 4
	la $a0, tempOut
	syscall

	li $v0, 2
	lwc1 $f12, 4($sp)
	syscall
	jr $ra 			        #return C


cel:
	lwc1 $f1, 0($sp)
	div.s $f21, $f18, $f16 # 9/5
	mul.s $f1, $f21, $f1   # C*9/5
	add.s $f1, $f1, $f14   # c*9/5 +32
	swc1 $f1, 4($sp)

	li $v0, 4
	la $a0, tempOut
	syscall

	li $v0, 2
	lwc1 $f12, 4($sp)
	syscall

	jr $ra 				   # return F
	
