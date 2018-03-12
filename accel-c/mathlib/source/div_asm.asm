;/*********************************************************************
;* Filename:   div_asm.asm
;* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
;*********************************************************************/
.CODE

IFDEF _M_X64
;   Undecorate symbol name is
;   uint64_t __fastcall math_uintx_divmod_asm(uint64_t dividend_l, uint64_t dividend_h, 
;                                             uint64_t divisor, 
;                                             uint64_t* remainder)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
;   r9  = remainder
math_uintx_divmod_asm PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);   
    mov qword ptr[r9], rdx      ;   *remainder = rdx;
    ret                         ;   return rax;
math_uintx_divmod_asm ENDP

;   Undecorate symbol name is
;   uint64_t __fastcall math_uintx_div_asm(uint64_t dividend_l, uint64_t dividend_h, 
;                                          uint64_t divisor)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
math_uintx_div_asm PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);
    ret                         ;   return rax;
math_uintx_div_asm ENDP

;   Undecorate symbol name is
;   uint64_t __fastcall math_uintx_mod_asm(uint64_t dividend_l, uint64_t dividend_h, 
;                                          uint64_t divisor)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
math_uintx_mod_asm PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);   
    mov rax, rdx                ;   *remainder = rdx;
    ret                         ;   return rax;
math_uintx_mod_asm ENDP
ENDIF

END
