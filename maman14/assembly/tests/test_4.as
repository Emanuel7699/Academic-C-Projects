;label already exsist.
.entry L3
.entry LENGTH
.extern L3
.extern W
MAIN: mov M1[r2][r7],W
 add r2,STR
LENGTH: jmp W
 prn #-5
mcro a_mc
mov M1[r3][r3],r3
 bne LOOP
mcroend
 sub r1, r4
 inc K
a_mc
END: stop
W: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
M1: .mat [2][2] 1,2,3,4 