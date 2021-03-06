	    ;; idt.s - Implements the asm portion of the IDT table.
	    ;; Copyright (C) 2016  Chris Smith

	    ;; This program is free software: you can redistribute it and/or modify
	    ;; it under the terms of the GNU General Public License as published by
	    ;; the Free Software Foundation, either version 3 of the License, or
	    ;; (at your option) any later version.

	    ;; This program is distributed in the hope that it will be useful,
	    ;; but WITHOUT ANY WARRANTY; without even the implied warranty of
	    ;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	    ;; GNU General Public License for more details.

	    ;; You should have received a copy of the GNU General Public License
	    ;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

BITS 64
DEFAULT REL

%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
    [GLOBAL isr%1]        ; %1 accesses the first parameter.
    isr%1:
        push 0
        push %1
        jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    [GLOBAL isr%1]
    isr%1:
        push %1
        jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 32
ISR_NOERRCODE 33
ISR_NOERRCODE 34
ISR_NOERRCODE 35
ISR_NOERRCODE 36
ISR_NOERRCODE 37
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47
ISR_NOERRCODE 255

[EXTERN isr_handler]

isr_common_stub:
    push rax

    xor rax, rax
    mov rax, [rsp+(4*8)]
    cmp rax, 0x8
    je .skip
    swapgs
.skip:
    xor rax, rax
    mov ax, fs
    push rax

    xor rax, rax
    mov ax, es
    push rax

    xor rax, rax
    mov ax, ds
    push rax

    mov rax, 0x10
    mov fs, ax
    mov ds, ax
    mov ss, ax
    mov es, ax

    push rdi ; 5
    push rsi
    push rdx
    push rbx
    push rcx
    push rbp ;10
    push r8
    push r9
    push r10
    push r11
    push r12 ;15
    push r13
    push r14
    push r15 ; 18
    mov rdi, rsp                ; move "pointer" from rsp to rdi (first parameter)
    mov rbx, rsp
    and rsp, 0xFFFFFFFFFFFFFFF0 ; align stack
    call [rel isr_handler]
    mov rsp, rbx
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11 ;5
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rcx ;10
    pop rbx
    pop rdx
    pop rsi
    pop rdi

    pop rax ;15
    mov ds, ax

    pop rax
    mov es, ax

    pop rax
    mov fs, ax

    mov ax, [rsp+(4*8)]
    cmp rax, 0x8
    je .skip2
    swapgs
.skip2:
    pop rax ;19
    add rsp, 16
    iretq

[GLOBAL idt_flush]
; flushes idt table change
;
; extern void idt_flush(void* idt_ptr)
idt_flush:
   mov rax, rdi                 ; Get the pointer to the IDT, passed as a parameter.
   lidt [rax]                   ; Load the IDT pointer.
   ret
