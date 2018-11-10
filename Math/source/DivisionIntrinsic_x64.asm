.CODE

;   uint64_t __fastcall _umulmod128@<rax>(uint64_t a@<rcx>, uint64_t b@<rdx>, 
;                                         uint64_t m@<r8>)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
_umulmod128 PROC
    mov rax, rdx
    mul rcx
    cmp rdx, r8
    jb _umulmod128_label0
	mov r9, rax
	mov rax, rdx
	xor rdx, rdx
	div r8
	mov rax, r9
_umulmod128_label0:
    div r8
	mov rax, rdx
	ret
_umulmod128 ENDP

;   uint64_t __fastcall _udiv128@<rax>(uint64_t dividend_l@<rcx>, uint64_t dividend_h@<rdx>, 
;                                      uint64_t divisor@<r8>)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
_udiv128 PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);
    ret                         ;   return rax;
_udiv128 ENDP

;   uint64_t __fastcall _udivmod128@<rax>(uint64_t dividend_l@<rcx>, uint64_t dividend_h@<rdx>, 
;                                         uint64_t divisor@<r8>, 
;                                         uint64_t* remainder@<r9>)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
;   r9  = remainder
_udivmod128 PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);   
    mov qword ptr[r9], rdx      ;   *remainder = rdx;
    ret                         ;   return rax;
_udivmod128 ENDP

;   uint64_t __fastcall _umod128@<rax>(uint64_t dividend_l@<rcx>, uint64_t dividend_h@<rdx>, 
;                                      uint64_t divisor@<r8>)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
_umod128 PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);
    mov rax, rdx                ;   rax = rdx(remainder)
    ret                         ;   return rax;
_umod128 ENDP

END
