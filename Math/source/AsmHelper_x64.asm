.CODE

;   uint64_t __fastcall _uaddmod128@<rax>(uint64_t a@<rcx>, uint64_t b@<rdx>, 
;                                         uint64_t m@<r8>)
_uaddmod128 PROC
    cmp r8, 1                   ;   if (r8(m) != 1)
    jnz _uaddmod128_label0      ;       goto _uaddmod128_label0;
    xor rax, rax                ;   rax = 0;
    ret                         ;   return rax;
_uaddmod128_label0:             ;_uaddmod128_label0:
    mov rax, rdx                ;   rax = rdx(b);
    xor rdx, rdx                ;   rdx = 0;
    add rax, rcx                ;   rdx:rax(sum) = rax(b) + rcx(a);
    adc rdx, rdx                ;   
    div r8                      ;   rax(q), rdx(r) = rdx:rax(sum) / r8(m);
    mov rax, rdx                ;   rax = rdx(r);
    ret                         ;   return rax;
_uaddmod128 ENDP

;   uint64_t __fastcall _usubmod128@<rax>(uint64_t a@<rcx>, uint64_t b@<rdx>, 
;                                         uint64_t m@<r8>)
_usubmod128 PROC
    cmp rcx, rdx                ;   if (rcx(a) >= rdx(b)) {
    jb _usubmod128_label0       ;
    mov rax, rcx                ;       rdx:rax(diff) = rcx(a) - rdx(b);
    sub rax, rdx                ;       
    xor rdx, rdx                ;
    div r8                      ;       rax(q), rdx(r) = rdx:rax(diff) / r8(m);
    mov rax, rdx                ;       rax = rdx(r);
    ret                         ;       return rax;
_usubmod128_label0:             ;   } else {
    mov rax, rdx                ;       rdx:rax(diff) = rdx(b) - rax(a);
    sub rax, rcx                ;
    xor rdx, rdx                ;
    div r8                      ;       rax(q), rdx(r) = rdx:rax(diff) / r8(m);
    mov rax, r8                 ;       rax = r8(m);
    sub rax, rdx                ;       rax -= rdx(r);
    ret                         ;       return rax;
_usubmod128 ENDP                ;   }

;   uint64_t __fastcall _umulmod128@<rax>(uint64_t a@<rcx>, uint64_t b@<rdx>, 
;                                         uint64_t m@<r8>)
_umulmod128 PROC
    mov rax, rdx                ;   rax = rdx(b);
    mul rcx                     ;   rdx:rax = rax * rcx(a);
    cmp rdx, r8                 ;   if (rdx < r8(m))
    jb _umulmod128_label0       ;       goto _umulmod128_label0;
    mov r9, rax                 ;   r9 = rax;
    mov rax, rdx                ;   rdx:rax = 0:rdx;
    xor rdx, rdx                ;   
    div r8                      ;   rax, rdx = rdx:rax / r8(m);
    mov rax, r9                 ;   rdx:rax = rdx:r9;
_umulmod128_label0:             ;_umulmod128_label0:
    div r8                      ;   rax, rdx = rdx:rax / r8(m);
    mov rax, rdx                ;   rax = rdx;
    ret                         ;   return rax;
_umulmod128 ENDP

;   uint64_t __fastcall _udiv128@<rax>(uint64_t dividend_l@<rcx>, uint64_t dividend_h@<rdx>, 
;                                      uint64_t divisor@<r8>)
_udiv128 PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);
    ret                         ;   return rax;
_udiv128 ENDP

;   uint64_t __fastcall _udivmod128@<rax>(uint64_t dividend_l@<rcx>, uint64_t dividend_h@<rdx>, 
;                                         uint64_t divisor@<r8>, 
;                                         uint64_t* remainder@<r9>)
_udivmod128 PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);   
    mov qword ptr[r9], rdx      ;   *remainder = rdx;
    ret                         ;   return rax;
_udivmod128 ENDP

;   uint64_t __fastcall _umod128@<rax>(uint64_t dividend_l@<rcx>, uint64_t dividend_h@<rdx>, 
;                                      uint64_t divisor@<r8>)
_umod128 PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);
    mov rax, rdx                ;   rax = rdx(remainder)
    ret                         ;   return rax;
_umod128 ENDP

END
