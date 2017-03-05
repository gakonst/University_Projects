;tmima2 
;Konstantinos Keramaris
;Giorgos Konstantopoulos
;Omada 11

.cseg
qAEM1:
.db 0x38, 0x31, 0x35, 0x39 ; 8 1 5 9 in ascii hex 
qAEM2:
.db 0x38, 0x31, 0x37, 0x33 ; 8 1 7 3 in ascii hex

.include "m16def.inc"
reset:
	
	.def aem1h = r24 ;  LET R24-R25= ALWAYS THE MAX
	.def aem1l = r25  
	.def aem2h = r23 ;  LET R22-23 = ALWAYS THE MIN
	.def aem2l = r22
	.def temp = r17 ; temp value for swaps	

	;LOADS 59 ON REGISTER AEM1
	ldi ZH, high(qAEM1<<1)
	ldi ZL, low(qAEM1<<1)
	lpm r16,Z
	mov aem1h, r16
	subi aem1h,48 ; ascii to int
	adiw Z,0x01
	lpm r16,Z
	subi r16,48 ; ascii to int
	lsl aem1h    ; BCD
	lsl aem1h    ; BCD
	lsl aem1h    ; BCD
	lsl aem1h    ; BCD
	or aem1h,r16 ; BCD
	
	adiw Z,0x01
	lpm r16,Z
	mov aem1l, r16
	subi aem1l,48 ; ascii to int
	adiw Z,0x01
	lpm r16,Z
	subi r16,48 ; ascii to int
	lsl aem1l    ; BCD
	lsl aem1l    ; BCD
	lsl aem1l    ; BCD
	lsl aem1l    ; BCD
	or aem1l,r16 ; BCD


	
	;LOADS 73 ON REGISTER AEM2
	ldi ZH, high(qAEM2<<1)
	ldi ZL, low(qAEM2<<1)
	lpm r16,Z
	mov aem2h, r16
	subi aem2h,48 ; ascii to int
	adiw Z,0x01
	lpm r16,Z
	subi r16,48 ; ascii to int
	lsl aem2h ; BCD
	lsl aem2h ; BCD
	lsl aem2h ; BCD
	lsl aem2h ; BCD
	or aem2h,r16 ;BCD

	adiw Z,0x01
	lpm r16,Z
	mov aem2l, r16
	subi aem2l,48 ; ascii to int
	adiw Z,0x01
	lpm r16,Z
	subi r16,48 ; ascii to int
	lsl aem2l    ; BCD
	lsl aem2l    ; BCD
	lsl aem2l    ; BCD
	lsl aem2l    ; BCD
	or aem2l,r16 ; BCD
	
	ldi r21,0xFF
	out ddrb,r21
	

start:
	cp aem1l,aem2l
    brlt swapAEM ; aem1l currently smaller than aem2l, this is going to change.
 	rjmp loop

swapAEM:
; When called, aem1l and aem1h are the low values. 
; We use the register temp to store aem2l and aem2h
; and swap its values with aem1l and aem1h respectively.
; Finally on aem1l and aem1h are the max values
; and on aem2l and aem2h are the min values,

	mov temp, aem2l
    mov aem2l, aem1l; AEM1 = MAX
	mov aem1l, temp 

	mov temp, aem2h
	mov aem2h, aem1h ; AEM2= MIN
	mov aem1h, temp

loop: 
	in r16, PIND ; 
	; 5 switches. 
	; 11111110 0 
	; 11111101 1
	; 11111011 2
	; 11110111 3 
	; 01111111 7
	cpi r16, 0b11111110
	breq sw0
	cpi r16, 0b11111101
	breq sw1
	cpi r16, 0b11111011
	breq sw2
	cpi r16, 0b11110111
	breq sw3
	cpi r16, 0b01111111
	breq near_sw7
	rjmp loop

sw0:
	com aem2l
	out portb,aem2l
 	com aem2l  
    ldi  r18, 203
    ldi  r19, 236
    ldi  r20, 133

L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
    nop

	ldi r22,0b11111111
	out portb,r22

	rjmp loop

sw1:
	com aem2h
	out portb,aem2h
	com aem2h
    ldi  r18, 203
    ldi  r19, 236
    ldi  r20, 133

L2: dec  r20
    brne L2
    dec  r19
    brne L2
    dec  r18
    brne L2
    nop

	ldi r22,0b11111111
	out portb,r22

	rjmp loop

sw2:
	com aem1l
	out portb,aem1l
	com aem1l
    ldi  r18, 203
    ldi  r19, 236
    ldi  r20, 133

L3: dec  r20
    brne L3
    dec  r19
    brne L3
    dec  r18
    brne L3
    nop

	ldi r22,0b11111111
	out portb,r22
	
	rjmp loop

near_sw7:rjmp sw7

sw3:
	com aem1h
	out portb,aem1h
	com aem1h
    ldi  r18, 203
    ldi  r19, 236
    ldi  r20, 133

L4: dec  r20
    brne L4
    dec  r19
    brne L4
    dec  r18
    brne L4
    nop

	ldi r22,0b11111111
	out portb,r22

	rjmp loop

sw7:
	andi aem1l, 0b00000001 ; aeMax = 0 or aeMax = 1
	andi aem2l, 0b00000001 ; aeMin = 0 or aeMin = 1
	lsl aem1l ; Shift left 
	or aem1l,aem2l ; Keep the bits of each number
	com aem1l ; Take the complement
	out portb, aem1l
	com aem1l

	rjmp loop