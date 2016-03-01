.extern interrupt_handler

.global interrupt
.align 4
interrupt:
	pusha

    mov %ax, %ds
    push %eax

    mov %ax, 0x10 #Move back to PL0
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax

    call interrupt_handler

    pop %ebx #Back to original state (descriptors)
    mov %ds, %bx
    mov %es, %bx
    mov %fs, %bx
    mov %gs, %bx

    popa #Restore registers
    add %esp, 8 #Removes data from ISR (error code and INT number)
    iret

.global gdt_flush
gdt_flush:
    #mov %eax, (%esp,4)  # Get the pointer to the GDT, passed as a parameter.
    lgdt (%esp,4)        # Load the new GDT pointer

    mov %ax, 0x10      # 0x10 is the offset in the GDT to our data segment
    mov %ds, %ax        # Load all data segment selectors
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
    mov %ss, %ax

    jmp .flush   # 0x08 is the offset to our code segment: Far jump!
    .skip 0x08
.flush:
    ret
