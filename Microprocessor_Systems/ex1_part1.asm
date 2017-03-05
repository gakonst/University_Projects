.cseg
qAEM1:
.db 0x37, 0x33; 8 1 7 3 in ascii hex 
qAEM2:
.db 0x35, 0x39 ; 8 1 5 9in ascii hex

.include "m16def.inc"
reset:
	.def aem1 = r24
	.def aem2 = r23	
	.def aeMax = r18
	.def aeMin = r19
	.def maxones = r21
	.def maxtens = r22

	;LOADS 73 ON REGISTER AEM1
	ldi ZH, high(qAEM1<<1)
	ldi ZL, low(qAEM1<<1)
	lpm r16,Z
	mov aem1, r16
	subi aem1,48 ; ascii to int
	adiw Z,0x01
	lpm r16,Z
	subi r16,48 ; ascii to int
	lsl aem1 ; BCD
	lsl aem1 ; BCD
	lsl aem1 ; BCD
	lsl aem1 ; BCD
	or aem1,r16 ;BCD

	;LOADS 59 ON REGISTER AEM2
	ldi ZH, high(qAEM2<<1)
	ldi ZL, low(qAEM2<<1)
	lpm r16,Z
	mov aem2, r16
	subi aem2,48 ; ascii to int
	adiw Z,0x01
	lpm r16,Z
	subi r16,48 ; ascii to int
	lsl aem2    ; BCD
	lsl aem2    ; BCD
	lsl aem2    ; BCD
	lsl aem2    ; BCD
	or aem2,r16 ; BCD

	ldi r20,0xFF
	out ddrb,r20
	

start:
	 ;Compare two values. Branch with BRGE / BRLT
	 ; if AEM1<AEM2 bracnh X 
	cp aem1,aem2
	brlt max1 ; jumps to aeMax = 8173
	brge max2

max1:
	mov aeMax, aem2 ; AEM2 = aeMAX
	mov aeMin, aem1 ; AEM1 = aeMin

	rjmp calculations

max2:
	mov aeMax, aem1
	mov aeMin, aem2
	; Get digits - Not needed in BCD
	; andi maxones,0b00001111 ; 0xF
	; lsr maxtens,4
	; Set leds 7-4 to 3rd number of aeMax
	; set leds 3-0 to 4th number of aeMax
	out portb, aeMax
		
calculations:
	andi aeMax, 0b00000001 ; aeMax = 0 or aeMax = 1
	andi aeMin, 0b00000001 ; aeMin = 0 or aeMin = 1
	lsl aeMax ; Shift left 
	or aeMax,aeMin ; Keep the bits of each number
	com aeMax ; Take the complement
	out portb, aeMax

