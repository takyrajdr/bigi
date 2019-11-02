_main:
;bigi_cec1302.c,2 :: 		void main() {
SUB	SP, SP, #268
;bigi_cec1302.c,5 :: 		bigint_init(a);
ADD	R0, SP, #0
BL	_bigint_init+0
;bigi_cec1302.c,6 :: 		}
L_end_main:
L__main_end_loop:
B	L__main_end_loop
; end of _main
