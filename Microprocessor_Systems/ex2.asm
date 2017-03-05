.include "m16def.inc"


.def input = r16
.def storedPassword = r17
.def lastInput = r18
.def led = r19
.def errorCounter = r21
.def inputPassword = r22
.def temp = r23
.def loops = r24

reset:
  ldi r20,0xFF
  out ddrb,r20
  clr temp
  clr lastInput
  clr errorCounter
  clr inputPassword ; set input pass to 11111111
  com inputPassword

 ;Sets password for the first time
setPasswd:
  in input,PIND                   ; Input password
  cpi input, 0b01111111
  breq storePasswd
  com input					  ; input=1000xxxx
  cpi input,0b00000000	      ; When the buttons are pressed there is a significant delay between them and the sw7 button's pressing
  breq setPasswd			  ; This loop makes sure that temp will not get a blank value
  mov temp, input
  rjmp setPasswd                  ; Loops until sw7 is pressed.

;Free up temp
storePasswd:
  com temp
  out portb, temp
  com temp
  mov storedPassword,temp         ; Saves password
  com storedPassword

start:
  in input,PIND                   ; Wait for sw0 to start
  com input
  cpi input,0b00000001            ; loop until sw0 is pressed
  brne start
  ldi led, 0b11111110
  out portb,led           ; flash led0 and go to triggered label
  rjmp triggered

triggered:
  clr lastinput 				  ; If an error occurs after the first password make sure that lastinput is cleared.
  clr loops 					  ; Same logic as above, if error has been made need to clear loop count.
  cpi errorCounter, 0b00000010    ; If error counter is 2 go to invalid state 1
  breq invalidState
  rcall givePassword 				  ; password check routine, checks if last switch is less than current


;If errorcounter = 1 - blink per 1 sec 5 times.
wrong:
  inc errorCounter				  ; errorCounter++
  cpi errorCounter, 0b00000001
  breq blinkLed0
  rjmp triggered

;Blinks 5 times with period 1s
blinkLed0:
  ldi led,0b11111110 
  out portb,led
  rcall delay
  ldi led, 0b11111111
  out portb,led
  rcall delay
  ldi led,0b11111110 
  out portb,led
  rcall delay
  ldi led, 0b11111111
  out portb,led
  rcall delay
  
  ldi led,0b11111110 
  out portb,led
  rcall delay
  ldi led, 0b11111111
  out portb,led
  rcall delay
  
  ldi led,0b11111110 
  out portb,led
  rcall delay
  ldi led, 0b11111111
  out portb,led
  rcall delay
  
  ldi led,0b11111110 
  out portb,led
  rcall delay
  ldi led, 0b11111111
  out portb,led
  rcall delay
  rjmp triggered ; 5 blinks with period = 1s

near_start:rjmp start

invalidState:
  ldi led, 0b00000000
  out portb,led
  rcall delay
  ldi led, 0b11111111
  out portb,led
  rcall delay
  rjmp invalidState ; Keep blinking 1sec.


; Get input reads digits, waits until sw7 is pressed
; If no button is pressed within 5 seconds, blinkled0
; 5 sec  4mhz = 2010^6 cycles
; Per loop - 16 cycles
; Loops = 1.250.000
; If loops == 1.250.000 blinkLed0
; Implement with 3 8-bit registers
; while (toc - tic  5 && !sw7.isPressed())
; if toc-tic  5 - blinkled0
; if sw7 is pressed proceed to check if password is wrong

;How to calculate X
;	branches are never taken so they take 1 instrution cycle
; 	sbis takes 3 cycles to skip ret. Ret is always skipped in our case since sw7 doesnt get pressed
;	other commands take 1 instruction cycle
; 3(ret)+2(rjmp)+9(other commands) = 14 cycles.
; Tota loops = 1.428.571, let it be rounded to int.
; '0010101 11001100 01011011' need 3 8bit registers



; We use regs r18-19 as a word, which counts up to 65535 (~65ms) for counting up to 65ms
; Then we'll increment r20 76 times in order to count 5s (4.94s actually but is OK)
; clr r18 
; ON EACH LOOP INREMENT R18-R19 WORD, 
; WHEN IT'S FULL -> CLR WORD -> INC R20. IF R20 = 76, blinkled0


givePassword:
  cpi r20, $76  						; X = 2010^6cycles per loop
  breq blinkled0
; inc loops
  adiw r18, 0x01
  cpi r19, 0xFF
  breq full

  in input,PIND						; INPUTPASSWORD = 1111XXXX, X = 0 MEANS ACTIVATED
  cpi input,0b11111111
  breq givePassword

  mov temp, input 					; Use temp variable to compare with stored password
  and temp,storedPassword 			; Mask out the bits of input that match the stored password
  cp temp,storedPassword 			; If the two values are equal, proceed
  brne wrong
  and inputPassword, input
  cp inputPassword, storedPassword  ; DONE, Arm the system
  brne givePassword 				; More digits to be given

full:
  cpi r18,0xFF
  bne givePassword ; if elapsed time <65ms keep looping
  clr r18
  clr r19 
  inc r20 ; 65ms -> clear 65ms -> increase r20 until it reaches 76
  rjmp givePassword

; System is armed only if correct password is given 
armed:
  ldi led, 0b11111101
  out portb,led
  in input,PIND
  cpi input,0b11111110 ; If button 0 is pressed, restart
  breq near_start
  cpi input,0b11111101
  breq breach2before1				; If last input was 0b00000010 and input is 0b00000001 + led2
  cpi input,0b11100111				; If input is 3 AND 4 at the same time blink led 4 + led2 
  breq breach34
  cpi input,0b11011111  
  breq blinkLed
  cpi input,0b10111111	; If input is 5 or 6 or 7 blink 5 or 6 or 7 respectively + led2
  breq blinkLed
  cpi input,0b01111111
  breq blinkLed

  mov lastInput, input
 ; or inputPassword, input
  rjmp armed

breach34:
	ori input,0b00010000
	rjmp blinkLed

;breach5
;	ori input,0b00100000
;	rjmp blinkLed

;breach6
;	ori input,0b01000000
;	rjmp blinkLed

;breach7
;	ori input,0b1000000
;	rjmp blinkLed

breach2before1:
	cpi lastinput,0b11111011
	brne armed
	andi input, 0b11110111

blinkLed:
  andi input,0b11111011 ;  maintain led2 open
  out portb, input
  rjmp blinkLed

;Ret takes 4 machine cycles, we can ignore the time error this induces as it is in the range of 10^-6 seconds
delay:
  ldi  r22, 21
  ldi  r23, 75
  ldi  r24, 191
L1:
  dec  r24
  brne L1
  dec  r23
  brne L1
  dec  r22
  brne L1
  ret