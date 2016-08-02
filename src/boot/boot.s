;; boot.s - Initializes everything for project asiago
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

BITS 32

; List references
extern kernel_main
global _start
; Multiboot Header

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
    align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM

; Bootstrap Stack
section .bootstrap_stack
    align 4
        stack_bottom:
            times 8192 db 0
        stack_top:

; Note, do not modify eax / ebx, contain grub data.
_start:
	; Setup Stack
    mov esp, stack_top
    ; Push multiboot data
    push eax
    push ebx
	; Call kernel
    call kernel_main
    cli
.error_loop: ; The . means its local, like static in C.
    hlt
    jmp .error_loop
