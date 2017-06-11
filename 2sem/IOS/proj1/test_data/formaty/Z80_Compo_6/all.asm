;***********************************************************
;* Size Coding #6 Entry
;*
;* Adrian Brown - adrian@apbcomputerservices.co.uk
;*
;***********************************************************

				ORG	08000h

;***********************************************************

Start:			
				; Clear the screen (plus the printer buffer which will be used as variables)
				ld		hl, $4000
				ld		de, $4001
				ld		(hl), l
				ld		bc, &1bff
				ldir

				; Draw the game border
				ld		hl, $5829
				ld		(hl), h
				ld		de, $5834
				ld		a, d
				ld		(de), a
				ld		e, $49
				ld		bc, 32*21
				ldir
				ld		b, 12
_Board:
				ld		(de), a
				inc		e
				djnz	_Board

				;******************************************
				; See if we want to drop any rows down
				;******************************************
CheckDrop:
				ld		hl, $5aca
				ld		c, 22
_NextRow:
				; See if this row is empty
				ld		b, 10
				push	hl
_CheckRow:
				ld		a, (hl)
				and		a
				jr		z, _NotFull
				inc		l
				djnz	_CheckRow

				; We can now drop the blocks down
				ld		a, c
_NotFull:
				pop		de
				ld		hl, -32
				add		hl, de

				; Cheeky - use the flag from the djnz above the first time to exit and the jr nz below after that
				jr		z, _DoNextRowCheck
				push	hl
				ld		c, 10
				ldir
				dec		a
				jr		nz, _NotFull

				pop		hl
				jr		CheckDrop

_DoNextRowCheck:
				dec		c
				jr		nz, _NextRow

				; We cant move down - redraw the piece where it was and check for falling
_GetBlock:		ld		a, r
				and		$38
				jr		z, _GetBlock
				ld		e, a
				rrca
				rrca
				rrca
				ld		(BlockNum + 1), a
				
				;******************************************
				; Set the starting position
				;******************************************
NewBlockLoop:
				ld		d, 1
				ld		ixh, d
				ld		hl, $582c		; (Block Starting Position)

				; End of game
				call	TestPiece
				jr		nz, Start

MainLoopResetFlag:
				;******************************************
				; Now clear the z flag - remember the flags (or h as h will never be 0)
				;******************************************
				or		h
				ex		af, af'

				;******************************************
				; Draw the block
				;******************************************
MainLoop:
				ld		a, e
				call	DrawPiece

				;******************************************
				; Wait a frame
				;******************************************
				halt

				;******************************************
				; Check for drop
				;******************************************
				ex		af, af'
				jr		z, CheckDrop

				;******************************************
				; Clear the block
				;******************************************
				call	ClearPiece

				; Store the current details
				push	de
				push	hl

				;******************************************
				; Keyboard system
				;******************************************
				ld		bc, $dffe
				in		a, (c)
				bit		3, a
				jr		z, _DroppingDown

				; Debounce the keys
				cpl
				ld		b, a
				xor		ixl
				ld		ixl, b
				and		b

				rra
				jr		nc, _NoRight
				inc		hl
_NoRight:
				rra
				jr		nc, _NoLeft
				dec		hl
_NoLeft:
				rra
				jr		nc, _NoRotate
				inc		d
_NoRotate:

				;******************************************
				; Handle the delay
				;******************************************
_DropDelay:		dec		ixh
				jr		nz, _NotDropping
_DroppingDown:
				; Handle the dropping - restore HL/DE as we cant move to the side and drop
				; else a block can get stuck on the side
				pop		hl
				pop		de
				push	de
				push	hl
				ld		bc, 32
				add		hl, bc
				ld		ixh, 16				
_NotDropping:
				; Check if it can be here
				ex		af, af'
				call	TestPiece

				; Restore our old positions into the exx pairs
				exx
				pop		hl
				pop		de

				; If we can move then start the loop again
				jr		nz, MainLoop

				; Get our new positions back
				exx

				jr		MainLoopResetFlag

;***********************************************************

ClearPiece:
				xor		a
				out		(254), a
DrawPiece:
				ld		b, $77			; OPCODE FOR ld (hl), a
				jr		BlockFunction
TestPiece:
				ld		b, $b6			; OPCODE FOR or (hl)
				xor		a
BlockFunction:
				push	de
				push	hl
				push	af
				ld		hl, NoData - 1
				ld		(hl), b

				;************************************
				; Get the block data
				;************************************
				ld		hl, Blocks
BlockNum:		ld		bc, 1
				add		hl, bc

				; Get rotations
				ld		a, d
				and		3
				inc		a
				rla
				ld		c, a

				;  Get AE as the data byte shifted 4
				ld		b, (hl)
				xor		a
				rld
				ld		e, (hl)
				ld		(hl), b

				;************************************
				; Rotate the block
				;************************************
_Twiddle1:
				ld		b, 8
_Twiddle2:		
				rl		e
				rla
				rr		h
				rl		e
				rla
				rr		l
				djnz	_Twiddle2

				ex		de, hl
				ld		a, d
				dec		c
				jr		nz, _Twiddle1

				;************************************
				; Get the position to draw the piece
				;************************************
BlockPosition:
				pop		af
				pop		hl
				push	hl

				ld		b, 4
_Loop1:
				push	bc
				ld		b, 4
_Loop2:
				; Wonder if we can link the get block data and this?
				sla		e
				rl		d
				jr		nc, NoData
				ld		(hl), a
NoData:
				inc		l
				djnz	_Loop2

				ld		c, 28
				add		hl, bc

				pop		bc
				djnz	_Loop1

				pop		hl
				pop		de

				; Do the test
				and		a
				ret

;***********************************************************
; DATA
;***********************************************************

Blocks:			EQU		$ - 1

				db		%11000110			; S
				db		%00110110			; Z
				db		%01100110			; O
				db		%11101000			; L
				db		%11100010			; J
				db		%01001110			; T
				db		%11110000			; I

;***********************************************************
; VARIABLES
;***********************************************************


				
				
org	0x8100
;SPACE RACE by Arcadiy Gobuzov. Made for COMPO #6 www.facebook.com/groups/z80asm
;First (?) Spectrum multiplayer game in 256 bytes
;
; Control keys:
; <ENTER> - Start
; <1,2> RED ROCKET
; <A,S> GREEN ROCKET
;
;tech details: we only draw in memory buffer [de00..ff3f]
;but on screen we see only 32x24 window
;     0123456789ABCDEF
;de00
;df00
;...
;e600    16 bytes     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
;e700    offscreen    XX this window will on screen XX
;e800                 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
;...
;fc00                 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
;fd00                 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
;fe00
;ff00
STAR		equ	255	;/// colors
ASTEROID	equ	8	;
MAX_ASTEROIDS	equ	217	; 222-217 = 5
startpoint:
;	ld	hl,end-startpoint	;temp to know size
	ei
	ld	hl,0x5f5b	;hl,c as random seeds
	exx
;
;	jr	$
start_game:
	ld	a,8	; start energy
	ld	(player1+2),a
	ld	(player2+2),a
	ld	a,221	; one bad guy for begin
	ld	(update+1),a
	ld	d,0

loop:	ld	bc,0xbffe
	in	a,(c)
	rra
	jr	nc,start_game

clr_buf:xor	a
	ld	h,221
clrloop:ld	l,d	;first time 0; others 91
	ld	(hl),a
	dec	l
	jr	nz,$-2
	inc	h
	jr	nz,clrloop

;object struct: [dy, y, dx, x, color]
update:	ld	l,218	; 256-objects ; 220
updloop:ld	h,221
;
; 1: update & check only y
	ld	a,(hl)	; dy
	inc	h
	add	a,(hl)	; add y
	ld	(hl),a
	ld	d,a
	jr	nz,update_normal
; 2: if offscreen, init new object:
; now h=222
rand:	exx
	ld	a,h
	rra
	rrca
	rrca
	xor	l
	rra		
	rl	c
	adc	hl,hl
	exx
;//rand
	and	63
	ld	e,a	; newx
	and	3
	rla             ; a: [0,2,4,6]
	add	a,h	; h=DE
	ld	d,a	; newy
	ld	a,l
	cp	h	; DE: stars from XXDE..XXFF
	ld	bc,0x100; dx = 0; dy = 1
	jr	nc,init_star
;init_asteroid:
	res	0,e	; e= [0,2,4,6..62]
	inc	c	; c=dx; +1 from left
	ld	a,e     ;
	cp	16
	jr	c,asteroids	
	dec	c       ; dx=0 central direction
	cp      48
	jr	c,asteroids
	dec	c       ; c=-1 from right
asteroids:
	ld	a,ASTEROID
	jr	init_object
init_star:
	rra	;a-random
	ld	a,STAR
	jr	nc,$+3
	inc	b	; randomly iy++
;in:	a: color [STAR, ASTEROID]
;	bc: b=dy, c=dx
;	de: d=newy, e=newx
;object struct: [dy, y, dx, x, color]
init_object:
	dec	h	; h=221 again
	ld	(hl),b	; dy
	inc	h
	ld	(hl),d	; newy
	inc	h
	ld	(hl),c	; dx
	inc	h
	ld	(hl),e	; newx
	inc	h
	ld	(hl),a	; color
;
update_normal:
	ld	h,223	; skip dy & y; d already = y
	ld	a,(hl)	; dx
	inc	h
	add	a,(hl)
	ld	(hl),a	; newx
	inc	h
	ld	e,a	; xcoor
	ld	a,(hl)	; a-color
;de  e-[0..63] x;  d-[222..255] y;
;/// draw STAR or ASTEROID
	ld	(de),a
	cp	ASTEROID
	jr	nz,nextspr
	inc	e
	ld	(de),a
	inc	d
	ld	(de),a
	dec	e
	ld	(de),a
;
nextspr:inc	l
	jr	nz,updloop
;
player1:ld	de,0x1e	; D-energy; E-x coordinate
	ld	bc,0xf750 ; B-keyport; C-color
	ld	l,0x11
	call	player
	ld	(player1+1),de
;
player2:ld	de,0x21	; D-energy; E-x coordinate
	ld	bc,0xfd60
	ld	l,0x2e
	call	player
	ld	(player2+1),de
;
	halt
; check time for increment asteroids
	dec	ixl
	jr	nz,no_incr
	ld	hl,update+1	;ld hl,update
	ld	a,(hl)
	cp	MAX_ASTEROIDS   ;ld a,(hl)
	jr	z,no_incr       ;cp N
	dec	(hl)
no_incr:
moveonscr:
	ld	de,22528	;16
	ld	h,222+8
movloop:ld	bc,32	;!
	ld	l,16	;!
	ldir
	inc	h
	jr	nz,movloop
;
	jp	loop

;in: de:lives,x; l:xbar; bc:keyport,color
;out: de:lives,x;
player:	inc	d
	dec	d
	ret	z       ; no lives return
	push	bc	; store keyport
	ld	b,d     ; lives
	ld	h,0xe7	; second line in buffer

barloop:ld	(hl),c
	inc	l
	bit	5,l     ; right or left side of screen (32-center of buffer)
	jr	z,barend
	dec	l
	dec	l
barend:	djnz	barloop
;// draw rocket
	ld	l,e	; xcoor
	ld	h,0xfb  ; ycoor
	ld	(hl),c
	inc	h
	inc	l
	ld	b,3
check:	ld	a,(hl)  ; collision detect here
	cp	ASTEROID
	jr	nz,draw
	dec	d
	jr	z,cont
draw:	ld	(hl),c
	dec	l
	djnz	check
;
cont:	pop	bc	; restore keyport
	ld	c,$fe
	in	a,(c)   ; check 0 & 1 bits
	rra
	jr	c,isright
	ld	a,e
	cp	18
	ret	z
	dec	e
	ret
isright:	
	rra
	ret	c
	ld	a,e
	cp	45	; right end x
	ret	z
	inc	e
	ret

	
	
	;  pasmo -d piskvorky.asm piskvorky.bin > test.asm ; ./bin2tap piskvorky.bin; ls -l piskvorky.bin
 
PROGSTART 		EQU	$8200		 ; 33280
ORG PROGSTART


PLAYER_COLOR	EQU	1*8
AI_COLOR		EQU	2*8
MASK_COLOR		EQU	$38

; Start game from basic
; RANDOMIZE USR 33473 (0x82C1) = progStart + 193 (+ $00C1)

; ----------------------------------------------------
; Vstup: 	HL adresa kurzoru
;		A priznaky posunu
; Vystup:	pokud neni stisknuto FIRE tak zmeni jen registry AF,HL

;	4	3	2	1	0
;	F(5)	↑(4)	↓(3)	→(2)	←(1)	Sinclair 1 (Sinclair left)
MOVE:
; P_LEFT:
	PUSH	BC
	PUSH	DE
	
	LD	BC,$001F
	RRCA					; nastavi carry
	LD	D,A				; maska

	LD	A,L
	SBC	A,B
; P_RIGHT:
	SRL	D				; nastavi carry
	ADC	A,B
	
	AND	C				; orezani do 0..31 pokud pretekl/podtekl
	XOR	L
	AND	C
	XOR	L
	LD	L,A

; P_DOWN:
	SRL	D
	JR	nc,P_UP
	ADC	HL,BC
P_UP:
	SRL	D
	JR	nc,P_FIRE
	SBC	HL,BC
P_FIRE:

; 57 0101 0111 +3 -> 5A 0101 1010
; 58 0101 1000
; 59 0101 1001
; 5A 0101 1010
; 5B 0101 1011 -3 -> 58 0101 1000

	LD	A,H
	CP	$57
	JR	nz,P_NO_UNDERFLOW		; nepodtekl
	LD	H,$5A				; Y max
P_NO_UNDERFLOW:
	CP	$5B
	JR	nz,P_NO_OVERFLOW		; nepretekl
	LD	H,$58				; Y min
P_NO_OVERFLOW:

	SRL	D
	
	POP	DE
	POP	BC
	RET	nc				; pokud neni stisknuto FIRE tak zmeni jen registry A,HL

	LD	A,(HL)
	OR	A
	RET	nz				; neprepisujeme kameny
	LD	(HL),PLAYER_COLOR		; PLAYER_COLOR = 2

; Zjednodusene nalezeni nejlepsiho tahu
; Find_best:
	LD	HL,$5800
	LD	C,L				; = 0 hodnota nejlepsi pozice, $FF = konec hry, existuje rada 5 kamenu
	PUSH	HL				; souradnice nejlepsi pozice

BRAIN_LOOP:


; ------------------------------------
; Vystup:	ixh
; Zmeni:	IX, AF, DE
; COUNT_VALUE:	
	LD	IXH,0				; vynulujeme hodnotu aktualni pozice
	LD	DE,DATA_DIRECTION-1
CV_LOOP:
	INC	DE
	
; "zpetny chod" na okraj usecky v jejimz stredu je testovana pozice
	PUSH	HL				; na zasobnik adresu zkoumane pozice, protoze ji budeme menit jak prochazime usecky
	LD	B,5
EL_TO_EDGE:
	LD	A,(DE)
	CALL	MOVE				;
	DJNZ	EL_TO_EDGE
	
	INC	DE
; prepnuti do stinovych registru
	EXX

; ------------------------------------
; Vstup:	shadow nothing, HL = cursor address, IYH value address, (DE) direction, C best value, HL on stack
; Zmeni:	IX, AF, maybe C = $ff
; EXPLORE_LINE:
	
; inicializace smycky
		LD	BC,$0900		; B = citac, C = nalezeny kamen
		LD	HL,$0010		; hl hodnota souvisle rady, dame bonus pro ; dame bonus pro xxxx_ xxx._ xx.._ x..._
		LD	D,C			; = 0 hodnota linie
		LD	E,C			; = 0 prazdnych
		LD	IXL,C			; = 0 delka rady

EL_LOOP:
		INC	IXL			; zvedneme delku rady

	EXX
	; navrat do "normalnich" registru
	LD	A,(DE)			; nastaveni smeru
	CALL	MOVE		
	LD	A,(HL)			; 
	AND	MASK_COLOR			; chceme jen barvu PAPER
	EXX
		JR	nz,EL_FOUND_STONE

; ---------------
; EL_EMPTY:
	
		INC	E			; prodlouzime radu prazdnych
		ADD	HL,HL			; bonus za stred

		LD	A,B			; 
		CP	5
		JR	z,EL_NEXT		;

		ADD	HL,DE			; H = H + D, v L a E (pocet prazdnych) je "smeti" jehoz soucet nikdy nepretece pres bajt. Pokud se sejdou jednickove bity tak je L nizke. 
		LD	D,H
	
		LD	HL,$0010		; dostane bonus i pro priste
		JR	EL_NEXT	

; ---------------
EL_FOUND_STONE:				; A je nenulove
		CP	C			; shodne kameny?
		JR	z,EL_IDENTIC

		INC	C
		DEC	C			; pokud tam byla 0 mame prvni kamen
		LD	C,A			; ulozime novy kamen
		JR	z,EL_IDENTIC	; prvni kamen, jinak by to blblo pri rade ._x. udelalo by to .x.

; ---------------
; EL_DIFFERENT:
		CALL	ADD_VALUE_SERIES
	
		LD	HL,$0008
		LD	D,H

		INC	E			; 
		LD 	IXL,E			; delka nove rady = prazdnych + 1 kamen
		DEC	E
		JR	z,EL_IDENTIC
	
		ADD	HL,HL			; dame bonus 2x protoze zacal prazdnym

; ---------------
EL_IDENTIC:

		LD	E,0			; vymazeme radu prazdnych
		ADD	HL,HL
		ADD	HL,HL
	
		LD	A,H
		CP	$20
		JR	c,EL_NEXT
	
; Nalezena rada 5 kamenu (asi)
		EX	(SP),HL		; do HL adresa stredu zkoumane usecky
		LD	A,(HL)
		OR	A			;
		JR	z,EL_5_NOT_FOUND
; Fakt nalezena
		SET	6,(HL)		; zesvetlime kamen
	EXX
	LD	C,$FF				; Existuje_rada_5_kamenu = True
	EXX
EL_5_NOT_FOUND:
		EX	(SP),HL		; vratime jak to bylo

EL_NEXT:
		DJNZ	EL_LOOP
	
		ADD	HL,HL			; dame bonus pro _xxxx _.xxx _..xx _...x
		CALL	ADD_VALUE_SERIES
	
; end Explore_line --------------------------------------------
	EXX
	
	POP	HL				; obnovime pozici pred zkoumanim dalsiho smeru
	LD	A,(DE)			; posledni je 1
	DEC	A
	JR	nz,CV_LOOP

; end Count_value ----------------------------------------------

	LD	A,(HL)
	OR	A
	JR	nz,B_NEXT			; pokud je na pozici kamen tak uz vse ignorujeme a deme dal, test na existenci rady 5 kamenu uz probehl

	LD	A,IXH
	CP	C				; porovname s nejlepsim
	JR	c,B_NEXT			; pokud zname lepsi tak ignorujeme

; aktualne nejlepsi pozice
	POP	DE				; vytahneme nejlepsi a zahodime
	PUSH	HL				; ulozime lepsi
	LD	C,A				; nejlepsi hodnota je ulozena v C
B_NEXT:
	INC	HL
	LD	A,$5B				; $5800 + 3 * 256 = $5800 + $0300
	CP	H
	JP	nz,BRAIN_LOOP

	INC	C				; Existuje rada 5 kamenu == $FF -> $00
	
	POP	HL				; vytahneme nejlepsi ze zasobniku
	LD	(HL), AI_COLOR		;

	RET	nz				;
	LD	(HL),C			; = 0, zmensime pravdepodobnost ze 1/50 vteriny bude videt pixel navic nez smazem obrazovku
						; neni to korektni protoze teoreticky muzeme mit v HL uz obsazenou pozici pokud je $5800 uz rada peti
; propadnuti na Repeat_game


; --------------------------------
REPEAT_GAME:
	POP	HL				; vytahneme nepouzitou adresu navratu pro ret

NEW_GAME:
; clear screen
	LD	HL,$4000			; 3
	LD	A,$5B				; 2
CLEAR_LOOP:
	LD	(HL),$00			; 2
	INC	HL				; 1
	CP	H				; 1
	JR	nz,CLEAR_LOOP		; 2

; umistime na stred a polozime kamen AI
	LD	HL,$598F
	LD	(HL),AI_COLOR		
					
READ_INPUT:
	LD	E,(HL)
	LD	(HL),$B8			; 

	LD	BC,0XF7FE			;
	IN	A,(C)				;
	CPL
	CP	D
	LD	D,A
	
	LD	(HL),E			; vratim puvodni
	
	CALL	nz,MOVE	
	JR	READ_INPUT


; -------------------------------
ADD_VALUE_SERIES:

		LD	A,IXL			; delka rady
		CP	$06			; je tam pricten uz i odlisny kamen
		RET	c			; pokud ma rada i s mezerama delku kratsi jak 5 tak nema zadnou hodnotu

		ADD	HL,DE			; H = H + D, v L a E (pocet prazdnych) je "smeti" jehoz soucet nikdy nepretece pres bajt. Pokud se sejdou jednickove bity tak je L nizke. 
		LD	D,H
		ADD	IX,DE			; IXH = IXH + D, IXL si zaneradime souctem puvodni delky rady s poctem jeho prazdnych poli, ale bude se menit

		RET


;	4	3	2	1	0
;	F(5)	↑(4)	↓(3)	→(2)	←(1)	Sinclair 1 (Sinclair left)
DATA_DIRECTION:
DB	9,6,10,5,8,4,2,1			; posledni je cislo 1! Pouzito zaroven jako zarazka











carx		equ	$c000
roadx		equ	$c001
gameframe	equ	$c002
roadw		equ	$c003
score		equ	$c100

	org	$8300
numbers
    db	%11101011	;0
    db	%10101010	;1
    db	%11011011	;2
    db	%01110111	;3
    db	%10111101	;4
    db	%11100111	;5
    db	%10101111	;6
    db	%11010101	;7
    db	%11100111	;8
    db	%11110101	;9

start
.menu
	ld	a,%10111111
	in	a,(254)
	rra
	jr	c,.menu

	ld	hl,carx
	ld	(hl),15	;carx
	inc	l
	ld	(hl),8	;roadx
	ld	l,roadw & 255
    ld 	(hl),12
    xor	a
    call cls_car

    ld	hl,score
.clrlp ld	(hl),a
    inc	l
    jr	nz,.clrlp

    ld	a,15
    ld	(.lp+1),a
        
.lp
    ld b,15
.lop  halt
	ld	a,($c300)
	xor	24
	ld	($c300),a
	and	24
	out	(254),a

    djnz .lop

	ld	hl,$5ADF
	ld	de,$5AFF
	ld	bc,$2E0
	lddr
	ld  hl,$5800
	ld	bc,31
	ld	a,4*8
	call fill
	ld	a,(roadx)
	ld	l,a
	ld	bc,(roadw)
	ld	b,0
	ld	a,7
	call fill
	ld	hl,(carx)
	ld	h,$5a
	ld	a,(hl)
	cp	4*8
	jp	z,.menu
	ld	(hl),2*8
	
	ld	a,%11011111
	in 	a,(254)
	ld	c,a
	ld	a,l
	rr	c
	jr	c,.nr
	cp	31
	jr	z,.nr
	inc	l
.nr	rr	c
	jr	c,.nl
	or	a
	jr	z,.nl
	dec	l
.nl	ld	a,l
	ld	(carx),a
	
	ld	a,r
	ld	b,a
.del	djnz .del
		
	ld	a,r
	rlca
	rlca
	rlca
	and	1
	add	a,a
	dec	a
	ld	h,a
.ss	
	ld	bc,(roadx)
	ld	a,31
	sub	c
	ld	b,a
	ld	a,(roadx)
	add	a,h
	or	a
	jr	z,.skp
	cp	b
	jr	z,.skp
	ld	(roadx),a
.skp

    ld	hl,score+1
    ld	a,9
.sl inc	(hl)
    cp	(hl)
    jr	nc,.ok
    ld	(hl),0
    inc	l
    jr	nz,.sl
.ok

	ld	de,$4000
	ld	hl,score+9
.sc
    ld c,(hl)
    ld	b,numbers/256
    ld	a,(bc)
	ld	b,4
.bl	ld	c,a
	and	$c0
	ld	(de),a
	ld	a,c
	rla
	rla
    inc d
	djnz .bl
	ld	d,$40
	inc	e  

	dec	l
	jr	nz,.sc

	ld	hl,gameframe
	inc	(hl)
	jr	nz,.skp1

	inc	l	; roadw
	ld	a,(hl)
	cp	4
	jr	nz,.skp2
	ld	a,(.lp+1)
	dec	a
	jr	z,.skp1
	ld	(.lp+1),a
	db	62
.skp2
	dec	(hl)
.skp1
	jp	.lp

cls_car	ld	hl,$5800
	ld	bc,$2ff
fill
	ld	d,h
	ld	e,l
	inc	de
	ld	(hl),a
	ldir
	ret


	
	
	
	
	
	
	
	
;Asteroidbelt

;Travel your ship trough the asteroidbelt.

;Controls:
;(Re)Startgame : 5, T, Y, G, H, B, N
;Left: Q, A, Shift
;Right: W, S, Z



; order of nrs going down                                       
    
    
ASTEROID_MEM	Equ	0x8400
ORG  ASTEROID_MEM                                           
                                                                
; characterset: ship, nr in reverse order, asteroid             
                                                                
ship       DEFB %00011000         ; ship                        
           DEFB %00100100                                       
           DEFB %01000010                                       
           DEFB %10011001                                       
           DEFB %11100111                                       
                                                                
           DEFB %00111100         ; 9                           
           DEFB %00100100                                       
           DEFB %00111100                                       
           DEFB %00000100                                       
           DEFB %00000100                                       
                                                                
           DEFB %00111100         ; 8                           
           DEFB %00100100                                       
           DEFB %00111100                                       
           DEFB %00100100                                       
           DEFB %00111100                                       
                                                                
           DEFB %00111100         ; 7                           
           DEFB %00000100                                       
           DEFB %00000100                                       
           DEFB %00000100                                       
           DEFB %00000100                                       
                                                                
           DEFB %00111100         ; 6                           
           DEFB %00100000                                       
           DEFB %00111100                                       
           DEFB %00100100                                       
           DEFB %00111100                                       
                                                                
           DEFB %00111100         ; 5                           
           DEFB %00100000                                       
           DEFB %00111100                                       
           DEFB %00000100                                       
           DEFB %00111100                                       
                                                                
           DEFB %00100100         ; 4                           
           DEFB %00100100                                       
           DEFB %00111100                                       
           DEFB %00000100                                       
           DEFB %00000100                                       
                                                                
           DEFB %00111100         ; 3                           
           DEFB %00000100                                       
           DEFB %00111100                                       
           DEFB %00000100                                       
           DEFB %00111100                                       
                                                                
           DEFB %00111100         ; 2                           
           DEFB %00000100                                       
           DEFB %00111100                                       
           DEFB %00100000                                       
           DEFB %00111100                                       
                                                                
           DEFB %00000100         ; 1                           
           DEFB %00000100                                       
           DEFB %00000100                                       
           DEFB %00000100                                       
           DEFB %00000100                                       
                                                                
           DEFB %00111100         ; 0                           
           DEFB %00100100                                       
           DEFB %00100100                                       
           DEFB %00100100                                       
           DEFB %00111100                                       
                                                                
rock       DEFB %00101100         ; rock                        
           DEFB %01011010                                       
           DEFB %10000010                                       
           DEFB %10011001                                       
           DEFB %01100110                                       
                                                                
; no game yet, but already 60 bytes "gone"                      
                                                                
asteroid_score      EQU  64000             ; somewhere in RAM #nn00      
                                                                
; the start of the game, first time just read A from any port                                         
                                                                
asteroid_start     IN   A,(254)           ; 1st read can be skipped 
           AND  16                ; start bit? 5 T Y G H B N    
           JR   NZ,asteroid_start         ; next read all rows but 6-0  
                                                                
asteroid_cls        LD   DE,23295          ; erase screen and attr					??? $5AFF = 0101 1010 1111 1111, D = $5A = 90
           LD   HL,asteroid_score+7        ; score in RAM and ATTR mask
asteroid_cls1       LD   A,87												; $57
           CP   D
           SBC  A,A													; 
           AND  L  													; AND 7              
           LD   (DE),A                         									; 0 ($5800 - $5AFF), 7 white ink, black paper ($4000 - $57FF)
           DEC  DE                                              
           BIT  6,D               ; out of RAM                  
           JR   NZ,asteroid_cls1           ; only clear RAM-screen  					16384 =$4000 = 0100 0000 0000 0000, D = $40 = 64     
                                                                
clscore    DEC  L                                               
           LD   (HL),10           ; set char "0"                
           JR   NZ,clscore        ; clear all positions         
                                                                
; from above Z-flag set, from below possible collision          
; placing test here makes a relative jump possible              
; saves a byte in the code.                                     
                                                                
scroll     JR   NZ,asteroid_start         ; test from dispship          
                                                                
           LD   BC,#BF80          ; 191 lines, right side       
                                                                
scr1       CALL field             ; calculate screen            
           EX   DE,HL             ; set destination             
           PUSH BC                ; save original linenumber    
           LD   A,B                                             
           SUB  6                 ; calculate from line         
onscreen   LD   B,A               ; set from or use leftscreen               
           CALL NC,field          ; calculate from address      
           LD   BC,16                                           
           LDIR                   ; copy the line down          
           POP  BC                ; fetch original lines        
           DJNZ scr1              ; do 'full' screen            
                                                                
           LD   B,3               ; set star on top             
                                                                
           LD   A,R               ; better rnd than without     
rnd        ADD  A,0               ; add seed                    
           LD   C,A                                             
           RRCA                                                 
           RRCA                                                 
           RRCA                                                 
           XOR  31                                              
           ADD  A,C                                             
           SBC  A,255                                           
           LD   (rnd+1),A         ; next seed                   
           OR   128               ; second half of screen only  
           LD   C,A                                             
                                                                
           LD   E,rock-ASTEROID_MEM      ; pointer to rock-char        
           CALL disp              ; AT B,C char from DE         
                                                                
           LD   HL,asteroid_score+7        ; unvisible number            
tens       LD   (HL),10           ; reset number to "0"         
           DEC  HL                ; increase from prev. counter 
           DEC  (HL)              ; "increase" score            
           JR   Z,tens            ; do carry to next position   
                                                                
           LD   L,6               ; point to visible score      
           LD   BC,#0330          ; AT for score                
scdisp     LD   A,(HL)            ; fetch char                  
           ADD  A,A               ; x2                          
           ADD  A,A               ; x4                          
           ADD  A,(HL)            ; x5                          
           LD   E,A               ; index calculated            
           PUSH HL                ; save scorepointer           
           CALL disp              ; display character           
           POP  HL                ; get scorepointer            
           DEC  HL                ; next pointer                
           LD   A,C                               
           OUT (254),A            ; engine sound              
           SUB  8                 ; due to -8 sound on/off 6x                              
           LD   C,A                                             
           JR   NC,scdisp         ; do all pointers             
                                                                
                                                                
;          XOR  A                 ; not needed, use other keys  
           IN   A,(254)           ; read QWERT, set above       
           RRA                    ; test left key
           JR   NC,xposship       ; C holds -8                  
           LD   C,8               ; set for right               
           RRA                    ; test right key
xposship   LD   A,200             ; get old xpos                
           JR   C,okmove          ; false key only              
                                                                
           ADD  A,C               ; do move                     
           CP   127               ; test in range               
           JR   NC,okmove         ; valid move                  
           SUB  C                 ; undo move                   
                                                                
okmove     LD   (xposship+1),A    ; save new xpos               
           LD   C,A               ; set x for screen            
           LD   B,192-5           ; ypos of ship                
           XOR  A                                               
           LD   E,A               ; charindex of ship           
           DEC  A                 ; 255 for test on collision   
prship     CALL dispship          ; show ship                   
                                                                
           HALT                   ; some delay for gameplay     
           HALT                                                 
                                                                
           JR   scroll            ; Z from dispship = no coll.  
                                                                
; almost full copy of #22b0 from the ROM                        
                                                                
field      LD   A,B                                             
           AND  A                 ; reset needed from scroll    
           RRA                                                  
           SCF                                                  
           RRA                                                  
           AND  A                                               
           RRA                                                  
           XOR  B                                               
           AND  #F8                                             
           XOR  B                                               
           LD   H,A                                             
           LD   A,C                                             
           RLCA                                                 
           RLCA                                                 
           RLCA                                                 
           XOR  B                                               
           AND  #C7                                             
           XOR  B                                               
           RLCA                                                 
           RLCA                                                 
           LD   L,A                                             
           RET                                                  
                                                                
disp       XOR  A                 ; no collisiontest            
dispship   LD   (col+1),A         ; set collision yes/no        
           CALL field             ; screenaddress               
dloop      LD   D,ASTEROID_MEM/256    ; character highbyte          
           LD   A,(DE)            ; fetch char                  
           INC  E                 ; point to next               
           LD   D,(HL)            ; fetch screen                
           LD   (HL),A            ; set char                    
           OR   D                 ; something on screen         
           XOR  D                                               
           SUB  (HL)              ; take off new value          
col        AND  0                 ; the actual test             
           RET  NZ                ; collision ?                 
nohit      INC  H                 ; next row on screen          
           LD   A,H                                             
           AND  7                                               
           JR   NZ,dloop          ; fill char                   
           RET                    ; print done                  

           
           
           
           
           
           
           
           
           
           
           
                                               
; snake                                             
; 48K memory needed, can fit in 16K but needs rewriting.
; therefore start also somewhere in uppermemory            
                                                                
ORG  0x8500                                           
                                           
                                                                
snake_start     LD   A,(IX+3)          ; get winner colour             
           OUT  (254),A           ; indicate winner                              

;start of game                                                                 
w4enter    XOR  A                                               
           IN   A,(254)           ; read keyboard, all rows
           AND  16                ; start bit? 5 6 T Y G H B N    
           JR   NZ,w4enter        ; wait startkey (not enter!)  
           LD   (trcnt+1),A       ; reset tailcounter           
                                                                
snake_cls        LD   DE,23295          ; erase screen and attr       
snake_cls1       LD   (DE),A                                          
           SET  7,D                                             
           LD   (DE),A            ; clear track too             
           RES  7,D                                             
           DEC  DE                                              
           BIT  6,D               ; out of RAM                  
           JR   NZ,snake_cls1           ; only clear RAM-screen       
           LD   H,A                                             
           LD   L,A                                             
           LD   (init),HL         ; both start up               
                                                                
startxy    LD   BC,#081C          ; player 1                    
           LD   E,C               ; first "possible" move       
           EXX                    ; alternate reg per player                              
           LD   BC,#0808          ; player 2                    
           LD   E,C               ; idem                              
                                                                
gameloop   LD   IX,init           ; set IX to player 1          
           LD   SP,60000          ; clear SP from below         
gameloop2  PUSH BC                ; save xy                     
           INC  B                 ; undo pos 0                              
           INC  C                 ; undo pos 0                              
           LD   HL,22528-1        ; calc attr                   
ffield     INC  HL                                              
           DEC  C                 ; never first zero                              
           JR   NZ,ffield         ; x pos                              
           LD   C,32              ; y = 32 * x                              
           DJNZ ffield                                          
           LD   A,(HL)            ; fetch current position      
           LD   B,(IX+2)          ; color of snake              
           LD   (HL),B            ; set attr                    
           OR   A                                               
           JR   NZ,snake_start         ; tailbyte, already used      
           SET  7,H               ; trackermemory +32K further               
           LD   (HL),E            ; set index previous          
           LD   B,C               ; always 32                   
findtail   DEC  BC                                              
           LD   A,B                                             
           OR   C                                               
           JR   Z,snake_start          ; around on tail, failsave              
           LD   A,(HL)            ; pointer to previous field         
           SUB  33                ; pointer without shifting                
           LD   E,A               ; pointer to E                
           SBC  A,A               ; 0 or 255                    
           LD   D,A               ; set highbyte too            
           ADD  HL,DE             ; calculate previous field          
           LD   A,(HL)                                          
           OR   A                 ; also reset of carry                              
           JR   NZ,findtail       ; no end of tail passed       
           SBC  HL,DE             ; back to end of tail                
           POP  BC                ; get xy                               
           PUSH HL                ; save end for erase later              
                                                                
                                                                
; do move                                                       
           LD   D,254             ; upkey indicator             
           DEFB #DD               ; ld a,ixl                    
           LD   A,L                                             
           CP   init*256/256      ; lowbyte of init             
           JR   NZ,keybpl2        ; own routine per player      
                                                                
; read keys player 1                                            
keybpl1    LD   A,%11111011       ; Q-T                         
           IN   A,(254)                                         
           BIT  1,A                                             
           JR   Z,kup             ; do up                              
           LD   A,%11111101       ; A-G                         
           IN   A,(254)                                         
           RRA                                                  
           JR   NC,kleft          ; do left                              
           RRA                                                  
           JR   NC,kright         ; do right                              
           LD   A,%11111110       ; SH-V                        
           OUT  (254),A           ; clear winner, set yellow
           IN   A,(254)                                         
           BIT  1,A                                             
           JR   downtest          ; test at player2 saves 2       
                                                                
; read keys player 2                                            
keybpl2    LD   A,%11011111       ; Y-P                         
           IN   A,(254)                                         
           BIT  2,A                                             
           JR   Z,kup             ; do up                              
           LD   A,%10111111       ; H-Enter                     
           IN   A,(254)                                         
           BIT  2,A                                             
           JR   Z,kright          ; do right                              
           BIT  3,A                                             
           JR   Z,kleft           ; do left                              
           LD   A,%01111111       ; B-space                     
           IN   A,(254)                                         
           BIT  2,A                                             
downtest   JR   Z,kdown           ; do down                              
                                                                
nokey      INC  D                 ; nokey, 255                  
                                                                
; when rotated remains 255                                      
                                                                
kright     RLC  D                 ; rotate to right bit         
kleft      RLC  D                                               
kdown      RLC  D                                               
kup        LD   A,D                                             
                                                                
keyread    LD   D,A               ; save old read from below    
           RRA                    ; analyze read                
           LD   E,65              ; prev down                   
           JR   NC,up             ; do up                       
           RRA                                                  
           LD   E,1               ; prev up                     
           JR   NC,down           ; do down                     
           RRA                                                  
           LD   E,34              ; prev right                  
           JR   NC,left           ; do left                     
           RRA                                                  
           LD   E,32              ; prev left                   
           LD   A,(IX)            ; fetch old move              
           JR   C,keyread         ; false key                   
right      INC  C                                               
           DEFB 62                ; left hidden in LD A,N                              
left       DEC  C                                               
           DEFB 62                ; up hidden in LD A,N                              
up         DEC  B                                               
           DEFB 62                ; down hidden in LD A,N                              
down       INC  B                                               
           LD   A,B               ; out of screen               
           CP   24                                              
jrdead     JP   NC,snake_start         ; dead, below 1 byte saved                        
           LD   A,C                                             
           CP   32                                              
           JR   NC,jrdead         ; out of screen, dead          

; a valid move is possible
setmove    LD   A,D               ; fetch the valid move       
           LD   (IX),A            ; set as next old move           
                                                                
           EXX                    ; other player registers      
           DEFB #DD               ; ld a,ixl                    
           LD   A,L                                             
           CP   init*256/256                                    
           INC  IX                ; set IX to player 2          
           JP   Z,gameloop2       ; do both players             
                                                                
           HALT                   ; now some delay              
           HALT                   ; before erasing tails                              
           HALT                                                 
                                                                
trcnt      LD   A,0               ; step counter                
           INC  A                                               
           AND  31                                              
           LD   (trcnt+1),A                                     
           POP  HL                ; fetch first from stack      
           JR   Z,noerase                                       
                                                                
erase      XOR  A                                               
           LD   (HL),A            ; reset tracker               
           RES  7,H                                             
           LD   (HL),A            ; erase visible tail          
           POP  HL                ; fetch second                
           LD   (HL),A                                          
           RES  7,H                                             
           LD   (HL),A            ; erase tail 2                

noerase    JP   gameloop          ; playon, clear stack         
                                                                
init       DEFB 0,0               ; start movement              
           DEFB 36,45             ; color of snake              
           DEFB 36                ; winner/loser with previous                
                                                                
; init can be set here too, for data it would save 2 bytes      
; since program will clear it. But game is in range.



















ccc; -----------------------------------------------------------------------------
; Name:     fbird
; Author:   John McManus
; Started:  19th May 2016
; Finished: 
;
;
; This is an entry for the 256 bytes game competition #6 on the Z80 Assembly programming
; on the ZX Spectrum Facebook Group https://www.facebook.com/groups/z80asm/
org 0x8600

;***** Constants *****
K_ATTR_ADDR        equ             0x5800 ; 22528
K_ATTR_BUFFER      equ             0xAF00 ; 44800
K_SCORE_ATT        equ             45738  
K_GRAVITY		equ		0x00AFF
K_BIRDX     equ   0
K_BIRDY			equ		5
K_BLANK			equ		0
K_BIRD			equ		32
K_WALL			equ		24	
K_SCORE     equ 8
K_COLDB     equ   0xA028 ; 41000
K_XSTART equ 0
K_XGAP equ 8
K_ENDCOL equ 31 
; **** Variables *****
bird_x       DEFB     K_BIRDX ; x position will be constant
bird_y       DEFB     K_BIRDY ; x position will be constant
score_fbird        DEFB     0x01;
gravcount    DEFW     K_GRAVITY 
lastkey   DEFB     0
;flapdelay    defb     255
;********************************************************
  call builddbnew
mainloop     	
  call keypress ; if key pressed move bird up
  call chkgrav ; counts down gravity function. all updates are based
  ret z     ; end of game 
  jr mainloop ; program loop 
 
updateloop ; this does all the update called from delay function
  call applygravity
  ;call clrbuffer
  ;call drawcolumns
;  call updatescore
  call refreshScreen
  call mvcl

ret

; ***************** Subroutines ----re organised to reduce number of calls
;check for key press

keypress
  ld a , (lastkey)
  ld b, a
  xor a 
  IN A,(254)     ;
  and 31 
  cp b 
  ret z
;statechange
  ld (lastkey), a

flap 
  ld hl, bird_y
  dec (hl)
  ;dec (hl)
endflap
  ret

; decrements garvity count until zero (just a timer)
; trigger the rest of the game movements
chkgrav   LD  BC,(gravcount)
    DEC BC
    LD  A,C
    OR  B
    JR  NZ,  SAVGRAV
    ;reset gravcount
    LD BC, K_GRAVITY
    ;LD  (gravcount), BC
    call SAVGRAV
    call  updateloop
    RET
SAVGRAV
    LD  (gravcount), BC
    RET

;move the bird down - should really make if 23 stop game 
applygravity
  ;ld a,(bird_y)
 ; cp 22
  ;jp Z, endgrav 
  ld hl, bird_y
  inc (hl)
endgrav
 ; ret

clrbuffer
  ld de, K_ATTR_BUFFER+1
  ld hl, K_ATTR_BUFFER
  ld (hl), 0
  ld bc ,767
  ldir

  ;ld de, K_ATTR_BUFFER+1
  ;ld hl, K_ATTR_BUFFER
  ;ld (hl), K_WALL
  ;ld bc ,31
  
  ;ldir

  ;ld de, K_ATTR_BUFFER+705
  ;ld hl, K_ATTR_BUFFER+704
  ;ld (hl), K_WALL
  ;ld bc ,31
  ;ldir

;; put bird back on

  ld de ,(bird_x)
  ld a, K_BIRD
  call drawpoint
  ;ret



drawcolumns
	ld ix , K_COLDB
  ld b, 0
getnext
  ;d,e hold x,y for drawpoint routine

	ld d, (ix)
	inc ix
	ld e, (ix)
  ;ld d ,(ix)
	;push hl
  ld a, K_WALL ; holds the attrib colour for drawpoint 
	call drawpoint
	;pop hl
	inc ix
	ld a ,(ix)
	cp 255 ; looking for end of DB 
	jr NZ , getnext
  ret

mvcl
  ld hl, K_COLDB+1
mvnxt
  ld a, 255
  cp (hl)
  jr z , endmvcl
  dec (hl)
  cp (hl)
  jr Z , wrap
  jr nowrap
wrap
  ;ok we are on 0 check if the bird is on 
  ld b,1
  ld (hl), 31
  
  dec hl ; move to y position
  ld a,(bird_y)
  cp (hl)
  jr z, kill
  inc hl 
nowrap
  inc hl
  inc hl 
  jr mvnxt
endmvcl
ld a,1
  xor b
  jp nz , donemv

  ld hl, score_fbird 
  inc (hl)
 ;

donemv

  ret 
  
kill
  xor a 
  ret


;de = x,y
;a = attrib setting  
drawpoint 
  push af
  LD a, d 
  sra a
  sra a
  sra a
  add a, 0xAF 
  ld h,a
  ld a,d 
  and 7
  RRCA
  RRCA
  RRCA
  ADD a,e 
  LD L,A
  pop af
  ld (hl), a 
  ret 

 
builddbnew
  ;db format is y,x - 
; try and build from the bottom up
; b = length of screen
  
  ld c, 5 ; number of columns
  ld d, 0
  ;xor d 
  ;xor d 
  ;ld d, K_XGAP
  ld hl, K_COLDB
  
newcol
  ld b, 22 ; number of rows and y value
  ;ld a, 15 ; start the gap at 15

  ld a, 6
  add a,d 
  ld d,a 

;  halt
;  halt
;  halt
  LD A, R     
  ;halt
  ;ld  a,(Rand8+1)
  and %00001111
  or  %00001000
  ;inc d
  ;inc d
  ;inc d
  ;inc d
  ;inc d
  ;inc d
  
ldxy
  cp b
  jp z , skipgap 
  ld (hl), b
  inc hl
  ld (hl), d 
  inc hl
  jp nextrow 
 skipgap
  rr b
  ;dec b
  ;dec b
  ;dec b
 nextrow 
  djnz ldxy 
  ;if b wraps from z to 255 jump to col done
  ;jp P, coldone
ld (hl), b
  inc hl
  ld (hl), d 
  inc hl
  
    dec c 
    xor a
    cp c
    jp nz, newcol


  ld (hl), 255
  inc hl
   ld (hl), 255
 
ret

;updatescore
;  ld hl, K_ATTR_BUFFER+768-32
;  ld a,(score_fbird)
;  ld b, a 
;  ld b,10
;  ld b,1
;  ld a, K_BIRD
;more
;  ld (hl),K_SCORE
;  inc hl
;  djnz more

refreshScreen
  ld de, K_ATTR_ADDR
  ld hl, K_ATTR_BUFFER
  ld bc, 768
  ldir
  ret
;ret




















; +---+---+---+---+---+---+---+

; V2 now includes Score bar and pseudo-random restart position for the rocket.

; Z80 Assembly Programming On The ZX Spectrum - COMPO #6 - 256 byte game compo
; a Lander game in 256 bytes of machine code, including DATA.
; (C) by Lúcio Quintal (Islander LQ), 30-May-2016. This program is FREEWARE, but original author (me) should be acknowledged when using this code.
;
; Instructions:
; Your goal is to land your Falcon 9 rocket/ship safely on the landing platform (or sea drone as used by SpaceX).
; Press the SPACE key to make the ship descend. Enjoy your landings!
;
; Notes:
; When runing the game on an emulator please make sure "border" view is active since border colour changes to indicate success or failure of landing.
; Due to the limitation of 256 byte code length and no ROM access it wasn't possible to implement proper leveling: increase/decrease difficulty can be done in several ways, for e.g. by changing the starting altitude of the ship and/or the game speed.
;
; Engineering for this ship was based on the works for Falcon 9 rocket from SpaceX (R), so it also should be able to land on a sea drone, as proposed on the game.

ORG 0x8700        ; works as well on any other valid address if compiled to that address

LEFT      EQU 1   ; move left for any object
RIGHT     EQU 0   ; move right for any object
DOWNB     EQU 2   ; move down for ship
PLATSIZE  EQU 5   ; size/length of landing platform
B_BLUE      EQU 5   ; border color blue when landing on platform
B_RED       EQU 2   ; border color red when misses platform
YEL_SHIP  EQU 48  ; ship color in yellow
MAG_PLAT  EQU 24  ; platform color in magenta


;pointing IX to begining address of variables
ld IX,plx
ld de,32    ; DE will never change and will be used to move between lines, so D=0 and E=32


; *** BEGIN of  main cycle ***
main_loop:

;first call paints, second call clears, and so on
call draw_graphs

;loop main pause
        halt
        halt
        halt
        halt

;read_space_key:
       ld bc,32766         ;port for B, N, M, Symbol Shift, Space
       IN A,(C)
       rra                 ;Space key in bit 0: 0 if pressed
       jr c,cont_next      ;key is not pressed if bit 0 is 1
       ld (ix+4),DOWNB     ;(dir_ship) - make ship descend (set value to 4)
cont_next:                 ;carry on with main loop



; detect if ship landed on the platform -> has to hit inside the platform to be safe
; det_collision:
; only need to detect collision when ship is moving down
; while ply<20 we don't need to check for collision

       ld a,(ix+1)     ; (ply)
       ;cp 20          ; is the ship on its last line? If yes, we need to check if it landed on the platform
       cp 18           ; %00010010
       jr c,cont_main  ; if not then the ship keeps travelling down and collision detection is not yet necessary
       ld hl,(tmp)     ; recover value of HL for the last ship square that has been draw. It will be used for landing detection. Would use Push/Pop if not inside a Call/Return.
       add hl,de       ; DE equals 32 at this point and we want to check if the platform is just below the ship!
       ld a,(hl)       ; get color value at this location
       cp MAG_PLAT     ; if this square is magenta then the rigth landing gear of ship is on the landing platform
       ld b,B_RED        ; set border to RED to indicate a failed landing
       jr nz,missed    ; if not, the ship missed landing
       dec hl
       dec hl
       ld a,(hl)       ; now let's check if the left side landing gear is also on the platform
       cp MAG_PLAT     ; if this square is magenta then the rigth landing gear of ship is on the landing platform
       jr nz,missed    ; if not the ship missed landing
       ld b,B_BLUE       ; Otherwise, our ship is safe on the plataforma => let's make a celebration

       ; test score
       inc (ix+5)      ; increase (score)

missed:   ld a,b        ; recover intended colour into a
          out (254),a   ; set border to our color

         ;clear graphics / screen to BLACK color
         ld hl,16384 ; start of screen
         ld a,91     ; 91*256= 23296  = value of H at the end of the cycle
   cls   ld (hl),d   ; d is always 0
         inc hl
         cp h        ; check for end of screen: stop when HL = 23296
         jr nz,cls

            ; display score on line 0
            ld h,88         ; %01011000
            ld l,d          ;line 0 is at 22528. D is always 0
            ld a,(ix+5)     ; score begins at 0 and goes up to 31
            and %00011111   ; must fit in 1 line, so MAX score = 31
            ld b,a
            inc b          ; b will be always > 0 and score bar will restart after 32
score_loop  ld (hl),e      ; fill in green E=32
            inc hl          ; score resets only on a new LOAD (just to save CODE bytes)
            djnz score_loop


         ;ship restarts at its current X (plx) and is back to the top, with Y (ply) getting a "random" value
         ld a,(ix)      ; (plx)
         rra            ; divide a by 2
         and %00000111  ; keep A under 8 => new (ply) between 0 and 7
         ld (ix+1),a    ; (ply) gets new "random" value
         ld (ix+4),d   ; (dir_ship) put ship moving to the right (d=0). Ideally it should continue in the same direction it was before but that requires more code
         
         ; DEBUG...posso retirar seguinte???
         jr main_loop  ;continue next with main loop


;first call paints graphics, second call clears, and so on
cont_main: call draw_graphs


;update coordinates of the ship: it can be moving LEFT, RIGHT or DOWN
;atcoord_ship:
           ld a,(ix+4)    ; (dir_ship) holds the direction of movement: LEFT=1, RIGHT=0 or DOWN=2
           ;rra
           and a
           jr nz,movesq    ; if not moving right then continue checking
           ld a,(ix+0)    ; (plx)
           inc a          ; move ship to next column on its right
           cp 30          ; the limit on the right is column 29 since our ship takes 3 square wide
           jr nz,cont_right ; keep moving ship right if the right side end was not reached yet
           
           ld (ix+4),LEFT   ; (dir_ship) if we have reached the end of right side then start moving to the LEFT
           jr atcoordp      ; continue with updating landing platform coordinates

cont_right:  ld (ix+0),a    ; (plx) move to next position on the right
             jr atcoordp      ; continue with main loop

movesq:    rra
           jr nc,movdesc   ; here we get carry=1 if moving to the left since (dir_ship) will be 1
           ld a,(ix+0)     ; (plx) is the horizontal coordinate of the ship
           or a             ; have we reached left end?
           jr nz,cont_left  ; continue: keep moving to the left
           ld (ix+4),d     ; (dir_ship) make ship move to the right (D=0)
           jr atcoordp      ; continue with next block of code
cont_left: dec a            ; make (plx) to next place/value to the left
           ld (ix+0),a      ; (plx) gets its new value of next position of the ship to its left
           jr atcoordp      ; continue with next block of code

movdesc:   ;we reach this point when the ship is moving DOWN
           ld a,(ix+1)     ; (ply)
           inc a
           ;cp 21           ; if the ship stands on line 20 then we need to check if it's over the platform OR if it will miss safe landing.
           cp 19            ; %10011. (ply)=0 corresponds to line 2, so (ply)=18 corresponds to line 20
           jr nc,atcoordp  ; if the ship is already in line 20 it will not descend more.
cont_desc: ld (ix+1),a    ; (ply) continue with the ship moving down



;update platform coordinates
atcoordp:
           ld a,(ix+2)       ; (dir_plat) =0 if the platform moves to the right and equals 1 if it moves to the left.
           and a
           jr nz,mov_left_p  ; here we get Z if platform is moving to the right
           ld a,(ix+3)       ; (platx) holds current X position of the platform
           inc a
           cp 28             ; the limit on the right is column 27 since the platform takes 5 square wide
           jr c,cont_rightp  ; keep moving the platform right if the right side end was not reached yet
           ld (ix+2),LEFT    ; (dir_plat) indicate that the platform next moves to the left
           jr cont_cycle


mov_left_p:  ld a,(ix+3)      ; (platx) get X position of the platform
             or a
             jr nz,cont_esqp  ; if didn't reach left end then keep moving left
             ld (ix+2),d     ; (dir_plat) , otherwise start moving plataform to the right (D=0)
             jr cont_cycle
cont_esqp:   dec a
cont_rightp: ld (ix+3),a      ; (platx) : store next position of platform to its left


; continue main cycle
cont_cycle: jp main_loop

; *** END of  main cycle ***


; Next code either shows OR clears graphics alternatively
draw_graphs:
;DRAW SHIP
;to obtain starting address we simply make [(ply)*32 + (plx)] to 22528

         ;ld h,88
         ;ld l,64         ; line 2 is 64 bytes below 22528.
         ld hl,22592
         ld a,(ix+1)     ; (ply) holds vertical coordinate of the ship.
         and a
         jr z,scr_cont
         ld b,a
scr_sum: add hl,de       ; DE is defined as 32 at the begining and never changes
         djnz scr_sum
scr_cont:
         ld a,l
         add a,(ix)      ; (plx)
         ld l,a          ; HL now holds the starting address to draw the ship on the screen
         ld a,YEL_SHIP   ; XOR of (hl) with 24 will change alternatively BLACK <-> YELLOW on ship squares color

; the ship is a 3x3 block:
;  #
;  #
; # #
;line 1
       inc hl
       xor (hl)       ; 24 XOR (hl) makes it alternate between BLACK and YELLOW
       ld (hl),a
;line 1
       add hl,de
       ld (hl),a     ; line 2 of the ship is same as line 1
;line 3
       dec de        ; DE is now 31
       add hl,de
       ld (hl),a     ; char 1 of line 3 of the ship is same as lines 1 and 2
       inc hl
       ld (hl),d     ; char 2 of line 3 is always BLACK (d=0)
       inc hl
       ld (hl),a     ; char 3 of line 3 of the ship is same char 1
       inc de        ; DE is now back to 32
       ;keep the value of HL (to be used on the collision detection if necessary)
       ld (tmp),hl   ; would use Push/Pop if not inside a Call/Return


;DRAW PLATFORM
;clear_platf:
       ld hl,23264       ;  the platform moves on last line, which begins on address 23264
       ld a,(ix+3)       ; (platx)
       add a,l           ; the platform never leaves its line
       ld l,a
       ld b,PLATSIZE     ;  get platform SIZE (5 on this version of the game)
       ld a,(hl)
       xor MAG_PLAT       ; xor 24 will make (hl) alternate between black and magenta
drawplat:                ; paint/draw OR clear the platform, alternate
       ld (hl),a
       inc hl
       djnz drawplat
       ret


;VARIABLES
;IX points here
plx       defb 0       ; X coordinate of the ship (screen columns): 0 in the begining.
ply       defb 0       ; Y coordinate of the ship (screen lines): moves on line 2 in the begining.
dir_plat  defb 1       ; = 0 if platform is moving to the right, 1 if moving to the left: starts from right -> left.
platx     defb 27      ; X coordinate of the platform
dir_ship  defb 1       ; = 0 if the ship is moving to the right, 1 if moving to the left and 2 if moving down: starts from left -> right.
score_v2  defb 0       ; score will be kept between 1 and 32 to be presented in line 0
tmp       defw 0       ; 2 byte used for generic storage. Used to store value of HL after drawing the ship. Useful when ship is in the lower part of screen to check if it is above the platform.















org 0x8800


waitingMoveUpBit      EQU   0
moveUpBit             EQU   1
enemyBit              EQU   2
flashBit   	      EQU   7

clearBit              EQU   6

paper                 EQU   8

siloColourBit          EQU   3
rocketColourBit        EQU   4
landColourBit          EQU   5
tunnelColour1          EQU   0
playerFrontColour1     EQU   7
playerBehindColour1    EQU   6
bulletColour           EQU   5

landAttr              EQU   ((1<<landColourBit)+(1<<enemyBit))               ;green ink & paper
tunnelAttr            EQU   (tunnelColour1)               ;black ink & paper
rocketSiloAttr        EQU   ((1<<siloColourBit)+(1<<waitingMoveUpBit)+(1<<enemyBit))   ;blue
movingRocketAttr      EQU   ((1<<clearBit)+(1<<rocketColourBit)+(1<<moveUpBit)+(1<<enemyBit))   ;red
rocketAttr      	  EQU   ((1<<rocketColourBit)+(1<<enemyBit))   ;red

playerFrontAttr       EQU   ((1<<clearBit)+(playerFrontColour1*paper))
playerBehindAttr      EQU   ((1<<clearBit)+(playerBehindColour1*paper))
bulletAttr            EQU   ((1<<clearBit)+(bulletColour*paper))

playerStartX          EQU 10
playerStartY          EQU 10


bufferStart        EQU 49152
bufferLength       EQU 6144
bufferEndHi        EQU 216


startLandDir        EQU 2
startLandTimer      EQU 1
startRandomSeed     EQU 123
startLandHeight     EQU 7
scrollSpeed         EQU 2  ;speed of scroll compared to sprites

startDifficulty          EQU      17
difficultySpeed          EQU      64



scramble_start:
          xor   a                   ;clear buffer
          ld  (bufferStart),a
          call copyBuffer

;setup vars
           ld    hl,bufferStart+playerStartX+(256*playerStartY)   ;player start position
           push  hl
           exx
           ld  de,startLandDir+(startLandTimer*256)
           ld  bc,scrollSpeed+(startLandHeight*256)
           ld  h,startDifficulty                                      ;start difficulty (tunnel size)
           exx


mainLoop:



;copy buffer to attr map (buffer is 256 bytes wide)

           ld  hl,bufferStart
           ld  de,5800h
bufferCopyLoop:
           push    hl
           ld      bc,32
           ldir
           pop hl
           inc h
           bit 2,d      ;end of attr map? (d=05bh)
           jr  z,bufferCopyLoop


          ld   d,tunnelAttr                 ;load up the compare colours for the following section
          ld   e,movingRocketAttr

;scan the buffer and move stuff


logic:
         ld   hl,bufferStart             ;start
         ld   c,0                      ;have we found a bullet?
moveLoop:
         ld   a,(hl)
         or   a
         jr   z,moveJump

         ;check for bullet

         cp     bulletAttr
         jr     nz,noBullet
         inc    c                      ;found bullet, don't fire another
         ld     (hl),d
         inc    l
         bit    enemyBit,(hl)
         jr     z,noHit
         ld     (hl),d
         jr     noBullet
noHit:
         ld     (hl),a
         inc    l
noBullet:

         ;check for a moving rocket

         cp    e                         ;movingRocketColour
         jr    nz,noMoveUp
launch:
         dec   h                        ;move up
         bit   landColourBit,(hl)       ;hit land?
         jr    nz,rocketDead
         ld    (hl),e                   ;store in new position
rocketDead
	 inc h                          ;restore addr for scan
noMoveUp:

         ;check for a rocket silo

         cp    rocketSiloAttr
         jr    nz,noLaunch
         ld    a,r
varLaunchChance:
         and    31                       ;chance of launching a static rocket
         jr    z,launch                 ;launch it, logic will come back here to maybe launch another but that doesn't matter, it'll get out eventually
noLaunch:

         ;delete everything with the clearBit set

         bit   clearBit,(hl)
         jr    z,moveJump
         ld    (hl),d    ;tunnelColour

moveJump:
         inc   hl
         ld    a,h        ;compare with the end
         cp    bufferEndHi
         jr    nz,moveLoop
         push  bc                  ;save bullet state (launched?)
                                      ;a will be non zero here
         exx                         ;swap in the variables held in alternate regs

;set difficulty
          dec l                      ;increase difficulty
          dec l
          dec l
	  jr	nz,noIncrease
	  dec	h
	  ld   a,h
          ld   (varTunnelHeight+1),a
noIncrease:


varScrollSpeed:
          dec  c                      ;speed of scrolling
          call z,copyBuffer            ;to scroll, a should be non zero when calling here, which it should be from above


;time for the land to change direction?

varLandTimer:

          dec d                         ;time for land to change direction?

          jr  nz,landTimerNotZero
          ld  a,r                   ;time before next land change random 0-7
          and  7
          inc  a
          ld   d,a                  ;store land timer
          inc  e                    ;new land dir
landTimerNotZero:


;move the land up, down or flat

varLandDir:
          ld a,e                    ;get current land dir
          rrca
          jr  nc,landDone              ;if land direction is 1 or 3, it's flat
          rrca
          jr  c,landDown               ;if land direction is 2, it's going down
landUp:                                ;else it's going up
          bit   4,b                     ;has height gone too much? (max 16)
          jr nz,landDone
          inc b
          jr  landDone
landDown:
          dec b                             ;going down
          jr  nz,landDone                   ;hit floor?
          inc b                             ;go back up
landDone:



noScroll:      ;this could go below the next bit bit we need to preserve the alternate registers

          ld  a,b                      ;get the height into 'a' ready for the next bit befire we swap out the registers

          exx                          ;swap out the vars in the alternate regs

;draw the land

          ld  e,landAttr             ;d should still hold tunnel attr...
          ld  d,tunnelAttr          ;load up the compare colours


          ld b,a                       ;a should hold the height from above
          ld  hl,bufferStart+31+(256*23)  ;start at bottom right of attr map
ceilingLoop:
          ld  (hl),e                                ;fill
          dec h                    ;next line
          djnz    ceilingLoop          ;repeat
;do we draw a rocket?
          ld  a,r                       ;get the random number
varRocketChance:
          cp 32                         ;greater than this?
          jr nc,drawTunnel              ;no rocket

          ld  (hl),rocketSiloAttr       ;draw the silo

          dec h                     ;this makes the tunnel bigger at this point. Don't care for the moment
drawTunnel:
varTunnelHeight:                       ;draw the tunnel
          ld  b,15
tunnelLoop:
          ld  (hl),d                   ;fill (might go off the buffer but we don't care. Plenty of nothing below here)
          dec   h
          djnz    tunnelLoop
          ld   b,24
landLoop:
          ld (hl),e                    ;fill the top of the tunnel (this will zip off through memory as above but won't reach anything critical)
          dec  h
          djnz  landLoop



movePlayer:
           pop  de                                   ;retrieve bullet state
           pop hl                                     ;retrieve player position
           ld bc,0fdfeh
           in a,(c)
           bit 0,a                                    ;read 'a'
           jr  nz,noUp
           dec h
noUp:
           inc b                                      ;read 'z'
           in a,(c)
           bit 1,a
           jr nz,noDown
           inc h
noDown:
           bit 2,a                                    ;read 'x'
           jr nz,noLeft
           dec l
noLeft:
           bit 3,a                                    ;read 'c'
           jr nz,noRight
           inc l
noRight:
         push  hl                                    ;save player again
         bit   enemyBit,(hl)                          ;test for death
         jp    nz,scramble_start
         ld    (hl),playerFrontAttr                 ;draw the player
         bit   4,a                                    ;read 'v'
         jr    nz,noFire
         bit   1,e                                     ;is there already a bullet?
         jr    nz,noFire
         inc   l
         ld    (hl),bulletAttr                        ;put bullet in front of player
         dec   l
noFire:
         dec   l
         ld    (hl),playerBehindAttr

         jp mainLoop


copyBuffer:
           exx                      ;just is case we are using the alternate regs
          ld hl,bufferStart         ;assume normal forward copy
          ld de,bufferStart+1
          or a                      ;are we scrolling?
          jr z,noSwap
          ex de,hl                  ;swap so everything gets copied to the left
noSwap:
          ld bc,bufferLength
          ldir
          exx
          ld  c,scrollSpeed            ;reset scroll speed (assuming we're doing a scroll. If we're not, it doesn't matter)
          ret




          
          
          
; -----------------------------------------------------------------------------
; Name:     TRAPIT
; Author:   Mike Daley
; Started:  12th May 2016
; Finished: 19th May 2016
;
; The idea of the game is to move the red player square around the screen leaving a
; trail of black squares. The player and the ball are unable to move through black
; squares. The player must trap the ball so that it cannot move. When the ball cannot
; move any more the players green progress bar at the top of the screen is increased
; and the level is reset.
;
; If the player gets into a position where they are stuck and cannot trap the ball then
; pressing the Enter key will reset the level, loosing all their progress :) The aim of
; the game is to get the progress bar as long as possilbe.
;
; To move the player the Q, A, O, P keys are used and Enter resets the level.
; 
; Remember to be careful as the ball will pass through the players red square which can
; cause the ball to escape from the player just when you think you have it trapped.
;
; This game is very easy to play but hard to master :o)
;
; This is an entry for the 256 bytes game competition #6 on the Z80 Assembly programming
; on the ZX Spectrum Facebook Group https://www.facebook.com/groups/z80asm/
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; CONSTANTS
; -----------------------------------------------------------------------------
BITMAP_SCRN_ADDR        equ             0x4000
BITMAP_SCRN_SIZE        equ             0x1800
ATTR_SCRN_ADDR          equ             0x5800
ATTR_SCRN_SIZE          equ             0x300
ATTR_ROW_SIZE           equ             0x1f

BLACK                   equ             0x00
BLUE                    equ             0x01
RED                     equ             0x02
MAGENTA                 equ             0x03
GREEN                   equ             0x04
CYAN                    equ             0x05
YELLOW                  equ             0x06
WHITE                   equ             0x07
PAPER                   equ             0x08                        ; Multiply with inks to get paper colour
BRIGHT                  equ             0x40
FLASH                   equ             0x80                        ; e.g. ATTR = BLACK * PAPER + CYAN + BRIGHT

PLAYER_COLOUR           equ             RED * PAPER + WHITE + BRIGHT + FLASH
BALL_COLOUR             equ             YELLOW * PAPER + WHITE
PLAY_AREA_COLOUR        equ             BLUE * PAPER + BLACK
BORDER_COLOUR           equ             BLACK * PAPER               ; Must be Black on Black as that is what the attr memory is initialised too
PROGRESS_BAR_COLOUR     equ             GREEN * PAPER

UP_CELL                 equ             0xffe0                      ; - 32
DOWN_CELL               equ             0x0020                      ; + 32
LEFT_CELL               equ             0xffff                      ; -1 
RIGHT_CELL              equ             0x0001                      ; + 1

MAX_TRAPPED_COUNT       equ             0x03                        ; Numer of frames the ball has been unable to move. If the trapped
                                                                    ; count reaches this number then the ball is trapped and the level ends
PLAYING_AREA_DEPTH      equ             0x14                        ; How many rows to colour for the playing area

DYN_VAR_LEVELS_COMPLETE equ             0x00                        ; Stores the number of consecutive levels completed
DYN_VAR_TRAPPED_COUNT   equ             0x01                        ; Stores how many frames the ball has not been able to move

; -----------------------------------------------------------------------------
; MAIN CODE
; -----------------------------------------------------------------------------

                org     0x8900

; -----------------------------------------------------------------------------
; Initialiase the dynamic variables. They need to be reset each time the game
; is reset, so may as well set them to zero here and not used pre-assigned
; memory locations to save a 2 bytes
; -----------------------------------------------------------------------------
trap_init:
                ld      hl, dynamicVariables + DYN_VAR_LEVELS_COMPLETE
                ld      (hl), 0                                     ; Reset level count
                inc     hl
                ld      (hl), 0                                     ; Reset trap count

; -----------------------------------------------------------------------------
; Initiaise the screen by clearing the bitmap screen and attributes. Everything
; is set to 0 which is why the border colour left behind the player is black to save
; some bytes ;o)
; -----------------------------------------------------------------------------
startGame:
                ld      hl, BITMAP_SCRN_ADDR                        ; Point HL at the start of the bitmap file. This approach saves
                                                                    ; 1 byte over using LDIR
clearLoop: 
                ld      (hl), BORDER_COLOUR                         ; Reset contents of addr in HL to 0
                inc     hl                                          ; Move to the next address
                ld      a, 0x5b                                     ; Have we reached 0x5b00
                cp      h                                           
                jr      nz, clearLoop                               ; It not then loop

; -----------------------------------------------------------------------------
; Draw playing area by drawing 20 rows of BLUE PAPER attributes into attributes
; memory
; -----------------------------------------------------------------------------
drawPlayingArea:
                ld      a, PLAYING_AREA_DEPTH                                    
                ld      hl, ATTR_SCRN_ADDR + (3 * 32) + 1           ; Start on the third row to leave some space for the progress bar
drawRow:
                push    hl                                          ; Save HL so it can then be 
                pop     de                                          ; ... loaded into DE
                inc     de                                          ; ... and incremented ready for the LDIR
                ld      bc, ATTR_ROW_SIZE - 2                       ; Only draw 29 cells as there needs to be a black border around the screen
                ld      (hl), PLAY_AREA_COLOUR                      ; Drop the screen colour into attribute memory
                ldir                                                ; Draw a row of play area
                ld      c, 3                                        ; Add three
                add     hl, bc                                      ; ... to HL for the start of the next line
                dec     a                                           ; Dec A which is counting the lines drawn
                jr      nz, drawRow                                 ; If more rows are needed then loop - 21 bytes

; -----------------------------------------------------------------------------
; Draw the progress bar
; -----------------------------------------------------------------------------
drawProgressBar:
                ld      a, (dynamicVariables + DYN_VAR_LEVELS_COMPLETE) ; If the level count == 0...
                or      a                                           ; ... then don't draw the...
                jr      z, TrapMainLoop                                 ; ... progress bar

                ld      hl, ATTR_SCRN_ADDR + (1 * 32) + 1           ; Point HL to the start of the progress bar       
drawProgressBlock:
                ld      (hl), PROGRESS_BAR_COLOUR                   ; Paint the block
                inc     hl                                          ; Move to the right
                dec     a                                           ; Dec the level count
                jr      nz, drawProgressBlock                       ; If we are not at zero go again

                push    hl                                          ; Place an initial value on the stack
                                                                    ; to be used later when see if the ball has got trapped

; -----------------------------------------------------------------------------
; Main game loop
; -----------------------------------------------------------------------------
TrapMainLoop:                                                          
            ; -----------------------------------------------------------------------------
            ; Read the keyboard and update the players direction vector            
                ld      hl, playerVector                            ; We will use HL in a few places so just load it once here
                ld      c, 0xfe                                     ; Set up the port for the keyboard as this wont change
            
_checkRight:                                                        ; Move player right
                ld      b, 0xdf                                     ; Read keys YUIOP by setting B only as C is already set
                in      a, (c)          
                rra         
                jr      c, _checkLeft                               ; If P was not pressed check O
                ld      a, 0xff                                     ; If the player is already move left
                cp      (hl)                                        ; ... then don't let the player move right
                jr      z, _movePlayer
                ld      (hl), 0x01                                  ; P pressed so set the player vector to 0x0001
                inc     hl          
                ld      (hl), 0x00          
                jr      _movePlayer                                 ; Don't check for any more keys
            
_checkLeft:                                                         ; Move player left
                rra         
                jr      c, _checkUp         
                ld      a, 01                                       ; If the player is already moving right
                cp      (hl)                                        ; ... then don't let them move left
                jr      z, _movePlayer
                ld      (hl), 0xff                                  ; O pressed so set the player vector to 0xffff
                inc     hl          
                ld      (hl), 0xff          
                inc     c                                           ; Break the next IN so _checkUp will jump to _checkDown so we not need
                                                                    ; JR which saves 1 byte
            
_checkUp:                                                           ; Move player up
                ld      b, 0xfb                                     ; Read keys QWERT
                in      a, (c)          
                rra         
                jr      c, _checkDown                               ; If the player is already moving down
                ld      a, 0x20                                     ; ... then don't let them move up
                cp      (hl)
                jr      z, _movePlayer          
                ld      (hl), 0xe0                                  ; Q pressed so set the player vector to 0xfffe
                inc     hl          
                ld      (hl), 0xff          
                inc     b                                           ; Break the next IN so _checkEnter will be called which uses less bytes than JR

_checkDown:                                                         ; Move player down
                inc     b                                           ; INC B from 0xFB to 0xFD to read ASDFG
                inc     b           
                in      a, (c)          
                rra         
                jr      c, _checkEnter
                ld      a, 0xe0                                     ; If the player is already moving down
                cp      (hl)                                        ; ... then don't let them move up
                jr      z, _movePlayer          
                ld      (hl), 0x20                                  ; A pressed so set the player vectory to 0x0020
                inc     hl          
                ld      (hl), 0x00          

_checkEnter:         
                ld      b, 0xbf                                     ; Read keys HJKLEnter
                in      a, (c)          
                rra         
                jr      c, _movePlayer          
                jp      trap_init                                        ; Player wants to reset so init the game

            ; -----------------------------------------------------------------------------
            ; Update the players position based on the current player vector
_movePlayer:
                ld      hl, (playerAddr)                            ; Get the players location address             
                ld      (hl), BORDER_COLOUR                         ; Draw the border colour in the current location 
                ld      de, (playerVector)                          ; Get the players movement vector
                add     hl, de                                      ; Calculate the new player position address
                xor     a                                           ; Clear A to 0 which happens to be the BORDER_COLOUR saving 1 byte :o)
                cp      (hl)                                        ; Compare the new location with the border colour...
                jr      z, _drawplayer                              ; ... and if it is a border block then don't save HL
                ld      (playerAddr), hl                            ; New position is not a border block so save it
                
            ; -----------------------------------------------------------------------------
            ; Draw player 
_drawplayer:
                ld      hl, (playerAddr)                            ; Load the players position 
                ld      (hl), PLAYER_COLOUR                         ; and draw the player

            ; -----------------------------------------------------------------------------
            ; Move the ball
_moveBall:
                ld      de, xVector                                 ; We need to pass a pointer to the vector...
                ld      bc, (xVector)                               ; ... and the actual vector into the ball update routine
                call    updateBallWithVector                        ; Update the ball with the x vector

                ld      de, yVector
                ld      bc, (yVector)
                call    updateBallWithVector            

            ; -----------------------------------------------------------------------------
            ; Draw ball
_drawBall:
                ld      hl, (ballAddr)                              ; Draw the ball at the...
                ld      (hl), BALL_COLOUR                           ; ... current position 

            ; -----------------------------------------------------------------------------
            ; Sync screen and slow things down to 25 fps
_sync:
                halt                                    
                halt

            ; -----------------------------------------------------------------------------
            ; Erase ball
_eraseBall:
                ld      (hl), PLAY_AREA_COLOUR                      ; HL is already pointing to the balls location so erase it

            ; -----------------------------------------------------------------------------
            ; Has the ball been trapped    
                pop     de                                          ; Get the previous position 
                push    hl                                          ; Save the current position 
                or      a                                           ; Clear the carry flag
                sbc     hl, de                                      ; current pos - previous pos
                ld      hl, dynamicVariables + DYN_VAR_TRAPPED_COUNT; Do this now so we don't have to do it again in the
                                                                    ; _trapped branch :)
                jp      z, _trapped                                 ; If current pos == previous pos increment the trapped counter...
                ld      (hl), 0                                     ; ... else reset the trapped counter

                jp      TrapMainLoop                                    ; Round we go again :)

_trapped:
                inc     (hl)                                        ; Up the trapped count
                ld      a, (hl)                                     ; Check to see if the trapped count...
                cp      MAX_TRAPPED_COUNT                           ; ... is equal to MAX_TRAPPED_COUNT
                jp      nz, TrapMainLoop

                dec     hl                                          ; Point HL at the level pointer address
                inc     (hl)                                        ; Inc the level complete counter

                jp      startGame                                   ; Loop

; -----------------------------------------------------------------------------
; Update the balls position based on the vector provided
;
; DE = vector address
; BC = vector value
; -----------------------------------------------------------------------------
updateBallWithVector:
                ld      hl, (ballAddr)                              ; Get the balls current position address...
                add     hl, bc                                      ; ... and calculate the new position using the vector in BC
                cp      (hl)                                        ; A already holds the border colour at this point so see if..
                jr      nz, _saveBallPos                            ; ... the new position is a border block and is not save the new pos
    
                ld      hl, 0                                       ; The new position was a border block...
                or      a                                           ; Reset the carry flag
                sbc     hl, bc                                      ; ... so reverse the vector in BC
                    
                ex      de, hl                                      ; Need to save the new vector so switch DE and HL
                    
                ld      (hl), e                                     ; Save the new vector back into the vector addr
                inc     hl  
                ld      (hl), d 
    
_playClickLoop:
                ld      a, b                                        ; Not setting B for this loop saves us 2 bytes and woks fine :o)
                and     248                                         ; Make sure the border colour doesn't change
                out     (254), a                                    ; Push A to the port to get our high def 1 bit sond :oO
                djnz    _playClickLoop
                ret

_saveBallPos:        
                ld      (ballAddr), hl                              ; Save the new position in HL
                ret

; -----------------------------------------------------------------------------
; Variables
; -----------------------------------------------------------------------------
playerAddr:     dw      ATTR_SCRN_ADDR + (12 * 32) + 16
playerVector:   dw      UP_CELL

ballAddr:       dw      ATTR_SCRN_ADDR + (12 * 32) + 16
xVector:        dw      LEFT_CELL
yVector:        dw      DOWN_CELL

dynamicVariables:       ; Points to the address in memory where we will store some dynamic variables
                        ; db Stores the count of levels completed
                        ; db Stores the # frames the ball has not moved


                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
         
         org  0x8b00
         
snakeGREEN    equ  36+64
WALL     equ  0
         
snakeSTART    
         xor  a
         out  (254),a
         
         ld   a,22
SCENE    ld   hl,22528
         ld   de,22529
         ld   bc,32
         ld   (hl),l
         ldir 
AZ       inc  hl
         inc  de
         ld   (hl),snakeGREEN
         ld   c,30
         ldir 
         ld   (hl),b
         inc  hl
         inc  de
         ld   (hl),b
         dec  a
         jr   nz,AZ
         ld   c,31
         ldir 
         ld   hl,WORMBUF
         ld   de,89*256+111
         ld   (hl),e
         inc  hl
         ld   (hl),d
         inc  hl
         inc  de
         ld   (hl),e
         inc  hl
         ld   (hl),d
         ld   (TAIL+1),hl
         inc  hl
         push hl
         pop  de
         inc  de
         ld   bc,765
         ld   (hl),a
         ldir 
         ld   hl,2
         ld   (WLEN+1),hl
         
SEED     ld   de,0
         ld   h,e
         ld   l,253
         ld   a,d
         or   a
         sbc  hl,de
         sbc  a,0
         ld   e,a
         ld   d,0
         sbc  hl,de
         jr   nc,AB
AB       ld   (SEED+1),hl
         ld   a,r
         and  91
         ld   h,a
         ld   a,(hl)
         cp   snakeGREEN
         jr   nz,SEED
         
         ld   (hl),36
         ld   hl,GAME
GAME     push hl
WLEN     ld   hl,0
         dec  hl
         add  hl,hl
         ld   c,l
         ld   b,h
TAIL     ld   hl,0
         push hl
         ld   d,(hl)
         dec  hl
         ld   e,(hl)
         dec  hl
         ex   de,hl
         ex   (sp),hl
snakeMOVE     ld   a,(de)
         ld   (hl),a
         dec  hl
         dec  de
         dec  bc
         ld   a,b
         or   c
         jr   nz,snakeMOVE
         ld   d,(hl)
         dec  hl
         ld   e,(hl)
DIREC    ld   bc,-1
         ex   de,hl
         add  hl,bc
         
;vyber zasobnik pred restartom
         pop  bc
         
TESTEND  ld   a,(hl)
         or   a
         jr   z,JP_START
         cp   54
JP_START jp   z,snakeSTART
         cp   36
         jr   nz,SET_HEAD
         exx  
         ld   hl,(TAIL+1)
         dec  hl
         ld   e,(hl)
         inc  hl
         ld   d,(hl)
         inc  hl
         ld   (hl),e
         inc  hl
         ld   (hl),d
         ld   (TAIL+1),hl
         ld   hl,(WLEN+1)
         inc  hl
         ld   (WLEN+1),hl
         pop  hl
         ld   hl,SEED
         push hl
         exx  
         
;uloz znovu co si vybral stack
SET_HEAD push bc
         
         ex   de,hl
         ld   (hl),e
         inc  hl
         ld   (hl),d
         pop  hl
         ld   (hl),snakeGREEN
         ld   hl,(WLEN+1)
         ld   b,h
         ld   c,l
         ld   hl,(TAIL+1)
DRAW     ld   d,(hl)
         dec  hl
         ld   e,(hl)
         dec  hl
         ld   a,54
         ld   (de),a
         dec  bc
         ld   a,b
         or   c
         jr   nz,DRAW
         
         ld   b,8
WAIT     halt 
         djnz WAIT
KEYS     ld   bc,32766
         in   a,(c)
         rra  
         jr   c,KEYUP
         ld   hl,32
         jr   SETDIREC
KEYUP    rra  
         jr   c,KEYRIGHT
         ld   hl,-32
         jr   SETDIREC
KEYRIGHT rra  
         jr   c,KEYLEFT
         ld   hl,1
         jr   SETDIREC
KEYLEFT  rra  
         jr   c,JP_GAME
         ld   hl,-1
SETDIREC ld   (DIREC+1),hl
JP_GAME  pop  hl
         jp   (hl)
         
LEN      equ  $-snakeSTART
         
WORMBUF  defs 2*768













         
         org  0x9200
         
invadersSTART    ld   a,16
REinvadersSTART  ld   (DEFSPEED+1),a
         ld   (SPEED+1),a
         xor  a
         out  (254),a
         
invadersSCENE    ld   hl,22528
         ld   de,22529
         ld   bc,768
         ld   (hl),a
         ldir 
         
         ld   c,3
         ld   hl,22528+37
         ld   (EPOS+1),hl
         ld   de,32+8
         ld   a,9
ENEMIES  ld   b,8
LINE     ld   (hl),a
         inc  hl
         inc  hl
         inc  hl
         djnz LINE
         add  hl,de
         add  a,9
         dec  c
         jr   nz,ENEMIES
invadersKEYS     halt 
SHIP     ld   hl,23264+15
         ld   (hl),7
         ld   bc,32766
         in   a,(c)
         rra  
         rra  
         jr   c,invadersKEYRIGHT
FIRE     call BULLET
         jr   nz,invadersKEYRIGHT
         ld   (BULLET+1),hl
invadersKEYRIGHT rra  
         jr   c,invadersKEYLEFT
invadersRIGHT    ld   a,l
         inc  a
         jr   z,invadersSTARTMOV
         inc  hl
         jr   invadersMOVE
invadersKEYLEFT  rra  
         jr   c,invadersSTARTMOV
invadersLEFT     ld   a,l
         and  31
         jr   z,invadersSTARTMOV
         dec  hl
invadersMOVE     ld   (SHIP+1),hl
invadersSTARTMOV call BULLET
         ld   de,32
         jr   z,NOBULLET
         push hl
         ld   l,c
         ld   h,b
         ld   (hl),d
         sbc  hl,de
         ld   a,h
         cp   88
         jr   c,RSBULLET
         ld   a,(hl)
         or   a
         jr   z,NOHIT
         ld   (hl),d
RSBULLET ld   hl,0
         jr   STBULLET
NOHIT    
STBULLET ld   (BULLET+1),hl
         pop  hl
NOBULLET ld   (hl),63
         ld   hl,(BULLET+1)
         ld   (hl),d
EPOS     ld   hl,0
         ld   bc,128+23
SPEED    ld   a,16
         dec  a
         jr   nz,SETSPEED
DEFSPEED ld   a,16
SETSPEED ld   (SPEED+1),a
         
EMOVE    call z,ELEFT
         ld   hl,(BULLET+1)
         ld   (hl),63+64
         jp   invadersKEYS
         
ELEFT    ld   a,l
         and  31
         jr   nz,ELMOVE
         ld   hl,ERIGHT
SETDOWN  ld   (EMOVE+1),hl
         jr   EDOWN
         
ELMOVE   ld   e,l
         ld   d,h
         dec  de
         ldir 
         ld   hl,EPOS+1
         dec  (hl)
         ret  
         
ERIGHT   add  hl,bc
         ld   a,l
         dec  a
         and  31
         jr   nz,ERMOVE
         ld   hl,ELEFT
         jr   SETDOWN
ERMOVE   ld   e,l
         ld   d,h
         inc  de
         inc  bc
         inc  bc
         lddr 
         ld   hl,EPOS+1
         inc  (hl)
         ret  
         
EDOWN    ld   hl,(EPOS+1)
         add  hl,de
         ld   (EPOS+1),hl
         add  hl,bc
         push hl
         sbc  hl,de
         pop  de
         inc  bc
         ld   a,d
         cp   91
         jr   z,GAMEOVR
         lddr 
         ld   c,32
         lddr 
         ld   b,160
         xor  a
CHECK    or   (hl)
         ret  nz
         inc  hl
         djnz CHECK
LEVELOVR ld   a,(DEFSPEED+1)
         rra  
GAMEOVR  pop  hl
         jp   REinvadersSTART
         
BULLET   ld   bc,0
         ld   e,a
         ld   a,b
         or   c
         ld   a,e
         ret  
         
invLEN      equ  $-invadersSTART














                     ; infinite blocky jumper
                    ; by steve wetherill 2016

SCREEN      EQU     $4000 
ATTRIBS     EQU     $5800 
ATTRIBS_SIZE EQU    $300 

JUMP_STEPS  EQU     4 
LOWER_GROUND EQU    2 
UPPER_GROUND EQU    15 

SURFACE_COLOR EQU   $28 
SOLID_COLOR EQU     $20 
CLEAR_COLOR EQU     $00 
BADDY_COLOR EQU     $70 
DEAD_COLOR  EQU     $10 
WIN_COLOR   EQU     $20 

;             .ORG    $8000 
;             .ENT    $8000 

ORG	$9300

RESTART_POINT:      
            XOR     a 

                    ; clear attributes
            CALL    GET_LDIR_PARAMS 

            PUSH    hl ; points to attribs

            LD      (hl),a 
            LDIR    

            OUT     (254),a ; border black
            LD      (PLAYER_Y+1),a ; reset player y position

            LD      bc,$080c ; b' = segment width, c' = initial position of ground
            LD      l,a ; l' = old position of ground
            LD      h,b ; h' = counter for progress bar

            EXX     
            LD      ix,32 ; ixl = initial loop delay before player is active
                    ; ixh = jump

MAIN_LOOP:          
            HALT    
            HALT    
            HALT    
            HALT    

                    ; erase player @ old position
            XOR     a 
            CALL    GET_OR_DRAW_PLAYER 

            OR      ixl 
            JR      z,DELAY_START 
            DEC     ixl 
DELAY_START:        

SET_GROUND:         
                    ; setup the ground (incomin
            EXX     ; swap to alt reg set
            LD      e,-2 ; default baddy spawn y is off screen
            XOR     ixl 
            JR      nz,SET_GROUND_EXIT 

            DJNZ    SPAWN_BADDY ; loop to exit if not done

            RRC     h 
            JR      nc,NO_BUMP 

            EX      (sp),hl 
            INC     hl 
            EX      (sp),hl 

NO_BUMP:            
            LD      b,8 ; loop count (width of horizontal section)
            LD      a,l ; get restore point
            OR      a ; if z, no restore needed
            JR      z,NO_GAP 
            LD      c,l ; restore
            LD      l,0 ; flag no restore point
            JR      SET_GROUND_EXIT ; done
NO_GAP:             
            LD      a,r 
            AND     15 ; low 4 bits zero
            JR      nz,NO_GAP2 ; if not, no gap generated
            LD      l,c ; set restore point
            LD      c,a ; zero out height
            JR      SET_GROUND_EXIT ; done
NO_GAP2:            
            RRA     
            LD      a,c ; get current height
            JR      c,GO_DOWN ; carry set == go down
            CP      UPPER_GROUND ; check for top limit
            JR      z,SET_GROUND_EXIT ; if at limit, we're done

            INC     c ; otherwise move up
            JR      SET_GROUND_EXIT ; we're done
GO_DOWN:            
            CP      LOWER_GROUND ; check for bottom limit
            JR      z,SET_GROUND_EXIT ; if at limit, we're done
            DEC     c ; move down
            JR      SET_GROUND_EXIT 

SPAWN_BADDY:        
            LD      a,r 
            AND     15 
            JR      nz,SET_GROUND_EXIT 
            ADD     a,b 
            CP      4 

            JR      nz,SET_GROUND_EXIT 
            LD      e,c ; trigger baddy at this y coord

SET_GROUND_EXIT:    
            EXX     ; restore alt reg set

            EX      (sp),hl 
            LD      (hl),h 
            EX      (sp),hl 

                    ; scroll
            CALL    GET_LDIR_PARAMS 
            EX      de,hl 
            LDIR    

                    ; draw_line of incoming ground and enemies
            LD      hl,ATTRIBS+31 
            LD      de,32 
            LD      b,24 
DL0:                
            LD      a,b 
            EXX     
            CP      e ; e is spawn point for baddies
            JR      z,BADDY 
            INC     a 
            CP      e 
            JR      nz,NO_BADDY 
BADDY:              
            LD      a,BADDY_COLOR 
            JR      DL2 

STILL_ALIVE:        
            LD      a,$78 
            CALL    GET_OR_DRAW_PLAYER 
            JR      MAIN_LOOP 

NO_BADDY:           
            SUB     c 
            JR      nc,OPEN_SPACE 
            INC     a 
            JR      nz,NOT_SURFACE 
            LD      a,SURFACE_COLOR 
            DB      $c2 ; jp nz swallows ld a,solid_color
                    ;            JR      dl2

NOT_SURFACE:        
            LD      a,SOLID_COLOR 
            JR      DL2 

OPEN_SPACE:         
            LD      a,CLEAR_COLOR 

DL2:                
            EXX     
            LD      (hl),a 
            ADD     hl,de 
            DJNZ    DL0 

            CALL    MOVE_PLAYER 
            OR      a 
            JR      z,STILL_ALIVE 

                    ; b == 0 from the djnz above!
            LD      a,2 ; border red
            OUT     ($fe),a 
            LD      a,DEAD_COLOR 

            CALL    GET_OR_DRAW_PLAYER 
DEAD_LOOP:          
            HALT    
            DJNZ    DEAD_LOOP 
            POP     hl 
            JP      RESTART_POINT 

                    ; never returns!

                    ; ================
                    ; preserves:
                    ; bc
                    ; bc', de', hl'
MOVE_PLAYER:        
            LD      a,ixl 
            SRA     a 
            JR      nz,NORMAL_CONTINUE 

            CALL    GET_OR_DRAW_PLAYER 
                    ; relies on a == 0
            OR      c 
            RET     nz ; we've collided with something, game over

                    ; relies on d == 0 from get_player_position
            LD      e,32 
            ADD     hl,de 
                    ; relies on a == 0
            OR      (hl) 
            JR      z,NOT_GROUNDED 

CHECK_JUMP:         
            XOR     a 
            IN      a,($fe) 
            CPL     
            AND     $1f 
            RET     z 

            LD      ixh,4 ; start jump

NOT_GROUNDED:       
            LD      a,ixh 
            OR      a 
            LD      a,(PLAYER_Y+1) 
            JR      z,FALLING 
            SUB     ixh 
            DEC     ixh 
FALLING:            
            INC     a 
            CP      24 
            RET     z ; we fell off the bottom of the screen, game over

            LD      (PLAYER_Y+1),a 

NORMAL_CONTINUE:    
            XOR     a 
            RET     

GET_OR_DRAW_PLAYER: 
                    ; entry
                    ; a: color to write @ player location
                    ; carry set means don't draw player just return hl
                    ; preserves
                    ; c
                    ; bc', de', hl'

            LD      hl,ATTRIBS+4 
            LD      b,32 
PLAYER_Y:           
            LD      de,0 ; smc
LOOP:               
            ADD     hl,de 
            DJNZ    LOOP 

            LD      c,(hl) 
            LD      (hl),a 
            RET     


GET_LDIR_PARAMS:    
                    ; entry
                    ; none
                    ; returns
                    ; hl -> attribs
                    ; de -> attribs+1
                    ; bc = length of color file - 1
                    ; preserves
                    ; af
                    ; af', bc', de', hl'

            LD      hl,ATTRIBS 
            LD      de,ATTRIBS+1 
            LD      bc,ATTRIBS_SIZE-1 
            RET     


