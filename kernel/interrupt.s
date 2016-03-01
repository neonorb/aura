.extern interrupt_handler

.global interrupt
interrupt:
	pusha
	// TODO add detection for the interrupt number,
	//      this is by using different interrupt code
	//      for each interrupt and then call a common
	//      handler to compute it's end point (keyboard driver, etc.)
	//
	//      this isn't the issue currently as a simple `hlt` insturction
	//      at the top doesn't halt the os, hinting that the issue
	//      is in the IDT rather than interrupt code
/*
    mov %ax, %ds
    push %eax

    mov %ax, 0x10 #Move back to PL0
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
*/
    call interrupt_handler
/*
    pop %ebx #Back to original state (descriptors)
    mov %ds, %bx
    mov %es, %bx
    mov %fs, %bx
    mov %gs, %bx
*/
    popa #Restore registers
    //add %esp, 8 #Removes data from ISR (error code and INT number)
    iret

.global gdt_flush
gdt_flush:
    movl 4(%esp), %eax  # Get the pointer to the GDT, passed as a parameter.
    lgdt (%eax)        # Load the new GDT pointer

    mov 0x10, %ax      # 0x10 is the offset in the GDT to our data segment
    mov %ax, %ds         # Load all data segment selectors
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    jmp $0x08, $flush   # 0x08 is the offset to our code segment: Far jump!

flush:
    ret
