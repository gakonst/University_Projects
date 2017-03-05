; Delay reserved registers -> r18-r21
; prewash Delay regs -> r22-r24

.include "m16def.inc"
.def input = r16 
.def temp = r17 ; led value 




reset:
	ldi r21, 0xFF
	out ddrb, r21 				; Set output port
	ldi temp, 0b01111111  ; Led 7 is activated forever
	out portb, temp
	clr r21
	ldi r16, low(RAMEND)
	out SPL, r16
	ldi r16, high(RAMEND)
	out SPH, r16

door:
	in input, PIND
	cpi input, 0b11111110 ; Close door, wait for sw0
	brne door
	
start:
	in input, PIND
	cpi input, 0b10111111 ; Press start, wait for sw 6
	brne start

overloadCheckPrewash:
	in input, PIND
	cpi input, 0b11111101 ; overload button = sw1 -> blinkLed1
	breq blinkLed1Close

	sbrs input, 2         ; set flag if button 2 is pressed -> proplisi enabled
	set                   ; gets skipped if button 2 wasn't pressed
	
	ldi  r22, 41
    ldi  r23, 150
    ldi  r24, 128
 L5: 
	dec  r24
    brne L5
    dec  r23
    brne L5
    dec  r22
    brne L5
programSelection:
	in input, PIND
	mov temp, input ; Store input at temporary value
	
	andi temp, 0b01111111 ; Activate the corresponding LEDs to the pressed switches + LED7
	out portb, temp

	cpi input, 0b11000111 ; 000 5 4 3 pressed
	breq setDelay1
	cpi input, 0b11001111 ; 001 5 4 pressed
	breq setDelay2 
	cpi input, 0b11010111 ; 010 5 3 pressed
	breq setDelay3
	cpi input, 0b11011111 ; 011 5 pressed
	breq setDelay4 
	cpi input, 0b11100111 ; 100 4 3 pressed
	breq setDelay1
	cpi input, 0b11101111 ; 101 4 pressed
	breq setDelay2
	cpi input, 0b11110111 ; 110 3 pressed
	breq setDelay3
	cpi input, 0b11111111 ; 111 no button pressed
	breq setDelay4

setDelay1: 				; 8 sec delay
	ldi  r18, 163 
    ldi  r19, 87
    ldi  r20, 3
    rjmp preWash
setDelay2:				; 16 sec delay
	ldi  r18, 2
    ldi  r19, 69
    ldi  r20, 173
    ldi  r21, 7
    rjmp preWash

setDelay3:				; 32 sec delay
	ldi  r18, 3
    ldi  r19, 138
    ldi  r20, 89
    ldi  r21, 16
    rjmp preWash
setDelay4:				; 64 sec delay
    ldi  r18, 6
    ldi  r19, 19
    ldi  r20, 177
    ldi  r21, 35

blinkLed1Close:
	rjmp blinkLed1

preWash:				; If r21 (reg that is non-zero when )
	brts proplisi
	cpi r21, 0b00000000
	breq shortDelayClose
	rjmp longDelay



proplisi:
    ldi  r22, 41
    ldi  r23, 150
    ldi  r24, 128
L1:
    rcall errorCheck
	dec  r24
    brne L1
    dec  r23
    brne L1
    dec  r22
    brne L1
    cpi r21, 0b00000000
	breq shortDelayClose
	rjmp longDelay



ksevgalma:
    ldi  r22, 21
    ldi  r23, 75
    ldi  r24, 191
L2: 
    rcall errorCheck
	dec  r24
    brne L2
    dec  r23
    brne L2
    dec  r22
    brne L2

check:
	sbrs temp, 5 ; If bit 5 is set (sw5 wasnt pressed, skip stragisma, end program)
	rcall stragisma ; else go to stragisma
	rjmp end

stragisma: ; Wait 2 seconds

    ldi  r22, 41
    ldi  r23, 150
    ldi  r24, 128

L3: 
    rcall errorCheck
	dec  r24
    brne L3
    dec  r23
    brne L3
    dec  r22
    brne L3
    
    ret

    ; cpi r2L31, 0b00000000

doorOpened:                      ; Blink led 1 forever with period 1s
	ldi temp, 0b01111110
	out portb, temp
	rcall delay1s
	ldi temp, 0b01111111
	out portb, temp
	rcall delay1s
	rjmp doorOpened

shortDelayClose:
	rjmp shortDelay

waterStopped:
	ldi temp, 0b00111101
	out portb, temp
	rcall delay1s
	ldi temp, 0b00111111
	out portb, temp
	rcall delay1s
	rjmp waterStopped
	

blinkLed1:                      ; Blink led 1 forever with period 1s
	ldi temp, 0b11111101
	out portb, temp
	rcall delay1s
	ldi temp, 0b11111111
	out portb, temp
	rcall delay1s
	rjmp blinkLed1

delay1s:
	ldi  r22, 21
    ldi  r23, 75
    ldi  r24, 191
L4: dec  r24
    brne L4
    dec  r23
    brne L4
    dec  r22
    brne L4

    ret


shortDelay: ; Delay with 3 regs
	rcall errorCheck
    dec  r20
    brne shortDelay
    dec  r19
    brne shortDelay
    dec  r18
    brne shortDelay

    rjmp ksevgalma

longDelay:   ; Delay with 4 regs
	rcall errorCheck
 	dec  r21
    brne longDelay
    dec  r20
    brne longDelay
    dec  r19
    brne longDelay
    dec  r18
    brne longDelay

    rjmp ksevgalma

errorCheck:
	in input, PIND
	cpi input, 0b11111110
	breq doorOpened
	cpi input, 0b01111111
	breq waterStopped
	ret
end:
	nop